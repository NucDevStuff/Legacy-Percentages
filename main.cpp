#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

bool running = false;

CCLabelBMFont* percentageText;

class PlayThread {
public:
    void RunPercentage(float);
};

void PlayThread::RunPercentage(float) {
	GameManager* gm = GameManager::sharedState();

	int decimalPlaces = Mod::get()->getSettingValue<int64_t>("dp");;

	if (gm->m_playLayer != nullptr && !PlayLayer::get()->m_player1->m_isPlatformer) {
        PlayLayer* playLayer = PlayLayer::get();

		PlayerObject* plr = playLayer->m_player1;

        if (playLayer->m_endPortal) {
            float levelLength = playLayer->m_endPortal->getPositionX();

			float percentage = (plr->getPositionX() / levelLength) * 100;
			percentage = floorf(percentage * pow(10, decimalPlaces)) / pow(10, decimalPlaces);

			if (percentage > 100) {
				percentage = 100.00000000f;
			}
			
			int offset = 1;

			if (decimalPlaces == 0) {
				offset = 0;
			}

			std::string percentageString = std::to_string(percentage);
			percentageString = percentageString.substr(0, decimalPlaces + percentageString.find(".") + offset) + "%";

			percentageText->setString(percentageString.c_str());
		}
	}
}

class $modify(PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}

		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize();

		percentageText = CCLabelBMFont::create("here%", "bigFont.fnt");
		percentageText->setPosition({size.width / 2, size.height - 8});
		percentageText->setScale(0.5f);

		this->addChild(percentageText);

		return true;
	}
};

class $modify(MenuLayer) {
    bool init() {
        MenuLayer::init();

        if (!running) {
            running = true;

            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(PlayThread::RunPercentage), CCDirector::sharedDirector(), 0, false);
        }

        return true;
    }
};
