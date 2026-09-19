#include <Geode/Geode.hpp>
#include <Geode/modify/LevelPage.hpp>

using namespace geode::prelude;

namespace {
    CCMenuItemSpriteExtra* makeButton(char const* text, CCObject* target, SEL_MenuHandler callback) {
        auto label = CCLabelBMFont::create(text, "goldFont.fnt");
        label->setScale(.42f);
        auto button = CCMenuItemSpriteExtra::create(label, target, callback);
        button->setContentSize({54.f, 24.f});
        return button;
    }

    void openLevel(GJGameLevel* level, bool botMode) {
        Mod::get()->setSavedValue("bot-mode", botMode);
        Mod::get()->setSavedValue("selected-level-id", level ? level->m_levelID : 0);
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

        // The BOT control is deliberately beside PLAY, as requested.
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
        if (Mod::get()->getSettingValue<bool>("normal-mode-only") &&
            this->m_level && this->m_level->m_levelLength <= 0.f) {
            FLAlertLayer::create("Leveller", "BOT is limited to normal, verified gameplay.", "OK")->show();
            return;
        }

        openLevel(this->m_level, true);
        this->LevelPage::onPlay(this);
    }
};

$on_mod(Loaded) {
    log::info("Leveller loaded for Geometry Dash 2.2.144");
}
