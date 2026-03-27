#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

using namespace geode::prelude;

static std::string idiomaGlobal = "en";

class IdiomaAlert : public FLAlertLayerProtocol {
public:
    void show() {
        FLAlertLayer::create(
            this,
            "Language",
            "Choose your language",
            "Español",
            "English"
        )->show();
    }

    void FLAlert_Clicked(FLAlertLayer*, bool btn2) override {
        idiomaGlobal = btn2 ? "en" : "es";
        Mod::get()->setSavedValue("idioma", idiomaGlobal);
        delete this;
    }
};

class $modify(DuckyDeath, PlayLayer) {
    struct Fields {
        bool yaMostrado = false;
    };

    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2)) return false;

        idiomaGlobal = Mod::get()->getSavedValue<std::string>("idioma", "en");
        return true;
    }

    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        PlayLayer::destroyPlayer(player, obj);

        if (m_fields->yaMostrado) return;
        m_fields->yaMostrado = true;

        std::map<std::string, std::vector<std::string>> mensajes = {
            {"es", {
                "skill issue",
                "casi lo logras",
                "vas bien",
                "xD",
                "¡No te rindas!",
                "La gravedad te odia",
                "Un click tarde...",
                "Ese triple spike no era para tanto",
                "Respira... y dale otra vez",
                "Estas calentando",
                "Fue el lag, ¿verdad?",
                "¡Tu puedes, Ducky!",
                "Casi 100%",
                "Pura practica",
                "¡No rompas el mouse!",
                "Un intento mas...",
                "Checkmate",
                "A la proxima sale"
            }},
            {"en", {
                "skill issue",
                "almost",
                "you're doing good",
                "lol",
                "Don't give up!",
                "Gravity is your enemy",
                "One frame late...",
                "That triple spike though",
                "Take a breath",
                "You're just warming up",
                "It was lag, definitely",
                "You got this, Ducky!",
                "Basically 100%",
                "Practice makes perfect",
                "Don't smash the mouse!",
                "One more attempt...",
                "Focus!",
                "Next time for sure"
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

        int index = std::rand() % static_cast<int>(lista.size());
        std::string texto = lista[index];

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
        if (!MenuLayer::init()) return false;

        idiomaGlobal = Mod::get()->getSavedValue<std::string>("idioma", "en");

        auto sprite = CCSprite::create("buscar.png"_spr);
        if (!sprite) return true;

        auto btn = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(MyMenuLayer::abrirMenu)
        );

        btn->setPosition({0.f, -100.f});
        btn->setScale(0.5f);

        if (auto bottom = this->getChildByID("bottom-menu")) {
            bottom->addChild(btn);
        }

        return true;
    }

    void abrirMenu(CCObject*) {
        auto* alert = new IdiomaAlert();
        alert->show();
    }
};