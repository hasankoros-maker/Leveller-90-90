#include <Geode/Geode.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iterator>
#include <string>

using namespace geode::prelude;

namespace {
    bool g_botMode = false;
    bool g_botHolding = false;
    float g_lastJumpX = -10000.f;
    int g_framesSinceJump = 0;

    CCMenuItemSpriteExtra* makeButton(char const* text, CCObject* target, SEL_MenuHandler callback) {
        auto label = CCLabelBMFont::create(text, "goldFont.fnt");
        label->setScale(.42f);
        auto button = CCMenuItemSpriteExtra::create(label, target, callback);
        button->setContentSize({54.f, 24.f});
        return button;
    }

    std::string lower(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        return value;
    }

    // The bot is deliberately opt-in and limited to a small, explicit test list.
    // Names are matched case-insensitively because level-name capitalization varies.
    bool isSupportedBotLevel(GJGameLevel* level) {
        if (!level) return false;
        auto name = lower(level->m_levelName.c_str());
        static constexpr const char* selected[] = {
            "detah corridor", "death corridor", "ton 618",
            "acheron", "slaughterhouse", "silent clubstep",
            "tidal wave", "avernus"
        };
        return std::any_of(std::begin(selected), std::end(selected), [&](auto item) {
            return name.find(item) != std::string::npos;
        });
    }

    void openLevel(GJGameLevel* level, bool botMode) {
        Mod::get()->setSavedValue("bot-mode", botMode);
        Mod::get()->setSavedValue("selected-level-id", level ? level->m_levelID : 0);
        g_botMode = botMode;
        g_botHolding = false;
        g_lastJumpX = -10000.f;
        g_framesSinceJump = 0;
    }

    bool isHazard(GameObject* object) {
        if (!object) return false;
        auto type = object->getType();
        return type == GameObjectType::Hazard || type == GameObjectType::AnimatedHazard;
    }

    bool isSolid(GameObject* object) {
        if (!object) return false;
        auto type = object->getType();
        return type == GameObjectType::Solid || type == GameObjectType::Slope ||
               type == GameObjectType::Breakable;
    }
}

class $modify(LevellerLevelPage, LevelPage) {
    bool init(GJGameLevel* level) {
        if (!LevelPage::init(level))
            return false;

        if (!Mod::get()->getSettingValue<bool>("enabled"))
            return true;

        auto menu = CCMenu::create();
        menu->setID("leveller-menu");
        menu->setPosition({0.f, 0.f});
        menu->setContentSize({116.f, 24.f});
        menu->setAnchorPoint({0.f, 0.f});

        auto normal = makeButton("PLAY", this, menu_selector(LevellerLevelPage::onNormalPlay));
        normal->setPosition({27.f, 12.f});
        menu->addChild(normal);

        auto bot = makeButton("BOT", this, menu_selector(LevellerLevelPage::onBotPlay));
        bot->setPosition({89.f, 12.f});
        menu->addChild(bot);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        menu->setPosition({winSize.width / 2.f + 82.f, winSize.height / 2.f - 86.f});
        this->addChild(menu, 20);
        return true;
    }

    void onNormalPlay(CCObject*) {
        openLevel(this->m_level, false);
        this->LevelPage::onPlay(this);
    }

    void onBotPlay(CCObject*) {
        if (!isSupportedBotLevel(this->m_level)) {
            FLAlertLayer::create(
                "Leveller BOT",
                "Bu deneysel bot yalnızca seçili zor level listesinde çalışır.\n"
                "Desteklenenler arasında Detah/Death Corridor ve Ton 618 bulunur.",
                "OK"
            )->show();
            return;
        }
        if (Mod::get()->getSettingValue<bool>("normal-mode-only") &&
            this->m_level && this->m_level->m_levelLength <= 0.f) {
            FLAlertLayer::create("Leveller BOT", "Level normal modda doğrulanmamış görünüyor.", "OK")->show();
            return;
        }

        openLevel(this->m_level, true);
        FLAlertLayer::create(
            "Leveller BOT",
            "Level analiz ediliyor. Bu sürüm deneysel bir offline heuristik bottur;\n"
            "her leveli bitirme garantisi yoktur.",
            "PLAY"
        )->show();
        this->LevelPage::onPlay(this);
    }
};

class $modify(LevellerPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;
        g_botMode = Mod::get()->getSavedValue<bool>("bot-mode", false) &&
                    isSupportedBotLevel(level);
        g_botHolding = false;
        g_lastJumpX = -10000.f;
        g_framesSinceJump = 0;
        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);
        if (!g_botMode || !Mod::get()->getSettingValue<bool>("bot-enabled"))
            return;
        if (!m_player1 || m_player1->m_isDead || m_hasCompletedLevel || m_isPracticeMode)
            return;

        ++g_framesSinceJump;
        auto player = m_player1;
        const float px = player->getPositionX();
        const float py = player->getPositionY();
        const float lookAhead = 180.f + 50.f *
            static_cast<float>(Mod::get()->getSettingValue<int64_t>("analysis-strength"));

        bool imminentHazard = false;
        bool blockingStep = false;
        float nearest = 100000.f;
        if (m_objects) {
            for (auto object : CCArrayExt<GameObject*>(m_objects)) {
                if (!object) continue;
                const float dx = object->getPositionX() - px;
                if (dx < 0.f || dx > lookAhead) continue;
                const float dy = std::fabs(object->getPositionY() - py);
                if (isHazard(object) && dy < 90.f) {
                    nearest = std::min(nearest, dx);
                    imminentHazard = true;
                }
                if (isSolid(object) && dx < 75.f && dy < 80.f)
                    blockingStep = true;
            }
        }

        // Conservative cube/robot/spider-style tap: press for one frame, then release.
        // The cooldown prevents repeatedly triggering on the same saw/spike cluster.
        const float trigger = 42.f + 5.f *
            static_cast<float>(Mod::get()->getSettingValue<int64_t>("analysis-strength"));
        const bool shouldJump = player->m_isOnGround &&
            ((imminentHazard && nearest <= trigger) || blockingStep);
        if (shouldJump && g_framesSinceJump > 3 && px - g_lastJumpX > 28.f) {
            this->handleButton(true, 1, true);
            g_botHolding = true;
            g_lastJumpX = px;
            g_framesSinceJump = 0;
        } else if (g_botHolding) {
            this->handleButton(false, 1, true);
            g_botHolding = false;
        }
    }

    void onQuit() {
        g_botMode = false;
        g_botHolding = false;
        PlayLayer::onQuit();
    }
};

$on_mod(Loaded) {
    log::info("Leveller loaded for Geometry Dash 2.2.144; offline bot prototype enabled");
}
