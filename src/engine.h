#pragma once
#include <fcitx/inputmethodengine.h>
#include <fcitx/instance.h>
#include <string>
#include "parser.h"
#include <fcitx/addonfactory.h>

class HasantaEngine : public fcitx::InputMethodEngineV2 {
public:
    HasantaEngine(fcitx::Instance* instance);

    void keyEvent(const fcitx::InputMethodEntry& entry,
                  fcitx::KeyEvent& keyEvent) override;

    void activate(const fcitx::InputMethodEntry& entry,
                  fcitx::InputContextEvent& event) override;

    void deactivate(const fcitx::InputMethodEntry& entry,
                    fcitx::InputContextEvent& event) override;

private:
    fcitx::Instance* instance_;
    std::string buffer_; // Holds raw English input keystrokes

    // Helper functions to handle engine mutations
    void processPhonetics(fcitx::KeyEvent& keyEvent);
    void commitText(fcitx::InputContext* ic, const std::string& text);
    std::string parseBufferToBengali();
};

// Factory macro instantiation placeholder
class HasantaEngineFactory : public fcitx::AddonFactory {
public:
    fcitx::AddonInstance* create(fcitx::AddonManager* manager) override;
};
