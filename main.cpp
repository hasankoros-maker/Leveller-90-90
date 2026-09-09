#include <Geode/Geode.hpp>
#include <Geode/modify/LevelPage.hpp>

using namespace geode::prelude;

namespace {
    // A compact button that uses only built-in Geometry Dash assets.
    CCMenuItemSpriteExtra* makeLevellerButton(CCObject* target, SEL_MenuHandler callback) {
        auto label = CCLabelBMFont::create("LEVELLER", "goldFont.fnt");
        label->setScale(.42f);
        auto button = CCMenuItemSpriteExtra::create(label, target, callback);
        button->setContentSize({54.f, 24.f});
        return button;
    }
}

class $modify(LevellerLevelPage, LevelPage) {
    bool init(GJGameLevel* level) {
        if (!LevelPage::init(level))
            return false;

        if (!Mod::get()->getSettingValue<bool>("enabled"))
            return true;

        // Keep this button in its own menu so the original LevelPage layout remains intact.
        auto menu = CCMenu::create();
        menu->setID("leveller-menu");
        menu->setPosition({0.f, 0.f});
        menu->setContentSize({54.f, 24.f});
        menu->setAnchorPoint({0.f, 0.f});

        auto button = makeLevellerButton(this, menu_selector(LevellerLevelPage::onLeveller));
        button->setPosition({27.f, 12.f});
        menu->addChild(button);

        // The play button is normally on the right side of LevelPage. Put Leveller beside it.
        // This position is intentionally relative to the standard page and can be adjusted per
        // platform if a future Geode layout change moves the original play control.
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        menu->setPosition({winSize.width / 2.f + 112.f, winSize.height / 2.f - 86.f});
        this->addChild(menu, 20);
        return true;
    }

    void onLeveller(CCObject*) {
        // LevelPage::onPlay enters the game's ordinary Normal Mode flow. It does not grant
        // progress, alter attempts, or bypass verification. Automation can be added later as
        // a separate, version-tested PlayLayer input module.
        this->LevelPage::onPlay(this);
    }
};

$on_mod(Loaded) {
    log::info("Leveller loaded for Geometry Dash 2.2.x");
}
