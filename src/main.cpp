#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

using namespace geode::prelude;

static std::string idiomaGlobal = "en";

class $modify(DuckyDeath, PlayLayer) {
    struct Fields {
        bool yaMostrado = false;
    };

    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2))
            return false;

        idiomaGlobal = Mod::get()->getSavedValue<std::string>("idioma", "en");
        return true;
    }

    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        PlayLayer::destroyPlayer(player, obj);

        if (m_fields->yaMostrado) return;
        if (!Mod::get()->getSavedValue("MessageOfDeathEnabled", true)) return;

        if (this->m_isPracticeMode) return;
        if (this->m_attempts <= 1) return;
        if (this->m_level && this->getCurrentPercent() > this->m_level->m_normalPercent) return;

        m_fields->yaMostrado = true;

        std::map<std::string, std::vector<std::string>> mensajes = {
            {"es", {
                "skill issue",
                "te falta gaming",
                "eso no fue un error... fue arte",
                "el spike te estaba esperando",
                "ese click fue opcional verdad?",
                "el timing se fue de vacaciones",
                "eso dolio mas que tarea sorpresa",
                "bro piensa que esto es facil",
                "respira... y vuelve a fallar",
                "casi... pero no",
                "eso conto como intento?",
                "el nivel: 1 - tu: 0",
                "lag mental detectado",
                "modo manco activado",
                "eso fue speedrun de morir",
                "nuevo record: decepcion",
                "faltaron manos ahi",
                "eso fue planificado... seguro",
                "pro gamer moment",
                "ese spike te hizo bully",
                "te ganaron los pixeles",
                "intenta no morir challenge (imposible)",
                "casi inicio el juego"
            }},
            {"en", {
                "skill issue",
                "bro forgot how to click",
                "that spike had a personal vendetta",
                "that click was optional right?",
                "timing.exe stopped working",
                "this was not the plan",
                "almost... not really",
                "new record: disappointment",
                "pro gamer moment",
                "you vs spike, spike wins",
                "mental lag detected",
                "hands not found",
                "speedrun death any%",
                "that was tragic",
                "you tried... kinda",
                "press space to not die (failed)",
                "this level is laughing at you",
                "the spike predicted your move",
                "that was painful to watch",
                "retry simulator",
                "you blinked and died",
                "mission failed successfully",
                "67"
            }}
        };

        auto it = mensajes.find(idiomaGlobal);
        if (it == mensajes.end()) {
            idiomaGlobal = "en";
            it = mensajes.find("en");
        }

        auto& lista = it->second;
        if (lista.empty()) {
            m_fields->yaMostrado = false;
            return;
        }

        auto index = std::rand() % static_cast<int>(lista.size());
        auto texto = lista[index];

        auto label = CCLabelBMFont::create(texto.c_str(), "goldFont.fnt");
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        label->setPosition({winSize.width / 2, winSize.height / 2});
        label->setScale(0.6f);
        this->addChild(label, 100);

        label->runAction(CCSequence::create(
            CCFadeIn::create(0.2f),
            CCDelayTime::create(0.9f),
            CCFadeOut::create(0.3f),
            CCCallFuncN::create(this, callfuncN_selector(DuckyDeath::quitarLabel)),
            nullptr
        ));
    }

    void quitarLabel(CCNode* node) {
        if (node) node->removeFromParent();
        m_fields->yaMostrado = false;
    }
};

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        idiomaGlobal = Mod::get()->getSavedValue<std::string>("idioma", "en");

        auto sprite = CCSprite::create("buscar.png"_spr);
        if (!sprite)
            return true;

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::abrirMenu)
        );

        btn->setPosition({0.f, -80.f});
        btn->setScale(0.5f);

        if (auto bottom = this->getChildByID("bottom-menu")) {
            bottom->addChild(btn);
            bottom->updateLayout();
        }

        return true;
    }

    void abrirMenu(CCObject*) {
        geode::createQuickPopup(
            "Ducky Settings",
            "Choose an option",
            "Language",
            "Toggle Message",
            [](FLAlertLayer*, bool btn2) {
                if (btn2) {
                    bool enabled = Mod::get()->getSavedValue("MessageOfDeathEnabled", true);
                    enabled = !enabled;
                    Mod::get()->setSavedValue("MessageOfDeathEnabled", enabled);

                    geode::createQuickPopup(
                        "Info",
                        enabled ? "Message enabled" : "Message disabled",
                        "OK",
                        "Que me importa",
                        [](FLAlertLayer*, bool) {}
                    );
                } else {
                    geode::createQuickPopup(
                        "Language",
                        "Choose your language",
                        "ES",
                        "EN",
                        [](FLAlertLayer*, bool btn2) {
                            idiomaGlobal = btn2 ? "en" : "es";
                            Mod::get()->setSavedValue("idioma", idiomaGlobal);
                        }
                    );
                }
            }
        );
    }
};