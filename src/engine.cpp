#include "engine.h"
#include <fcitx/inputcontext.h>
#include "parser.h"
#include <fcitx-utils/key.h>
#include <fcitx/inputpanel.h>
#include <fcitx/text.h>
#include <fcitx/addonmanager.h>

HasantaParser parser;
HasantaEngine::HasantaEngine(fcitx::Instance* instance) : instance_(instance) {}

void HasantaEngine::activate(const fcitx::InputMethodEntry&, fcitx::InputContextEvent&) {
    buffer_.clear();
}

void HasantaEngine::deactivate(const fcitx::InputMethodEntry&, fcitx::InputContextEvent&) {
    buffer_.clear();
}

void HasantaEngine::keyEvent(const fcitx::InputMethodEntry&, fcitx::KeyEvent& keyEvent) {
    // 1. Ignore key releases and raw modifier keys (like pressing Shift alone)
    if (keyEvent.isRelease() || keyEvent.key().isModifier()) {
        return;
    }

    // 2. Ignore ANY key combination that includes Ctrl, Alt, or Super (Fixes Ctrl+A, etc.)
    if (keyEvent.key().states() & (static_cast<int>(fcitx::KeyState::Ctrl) | static_cast<int>(fcitx::KeyState::Alt) | static_cast<int>(fcitx::KeyState::Super))) {
        return;
    }

    auto keySym = keyEvent.key().sym();

    // Handle backspace to delete from buffer safely
    if (keySym == FcitxKey_BackSpace && !buffer_.empty()) {
        buffer_.pop_back();
        processPhonetics(keyEvent);
        keyEvent.filterAndAccept();
        return;
    }

    // Handle Escape to cancel the current preedit box
    if (keySym == FcitxKey_Escape && !buffer_.empty()) {
        buffer_.clear();
        auto* ic = keyEvent.inputContext();
        ic->inputPanel().reset();
        ic->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
        // Notice we DO NOT call filterAndAccept() here.
        // We want the Escape key to actually reach the application after we clear our box.
        return;
    }

    // Handle space or enter to commit the text sequence
    if ((keySym == FcitxKey_space || keySym == FcitxKey_Return) && !buffer_.empty()) {
        auto* ic = keyEvent.inputContext();
        std::string bnText = parseBufferToBengali();

        commitText(ic, bnText);
        if (keySym == FcitxKey_space) {
            commitText(ic, " "); // Pass the physical space through
        }

        buffer_.clear();
        ic->inputPanel().reset();
        ic->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
        keyEvent.filterAndAccept();
        return;
    }

    // Intercept alphabet keys for phonetic building (Both lowercase and uppercase)
        if ((keySym >= FcitxKey_a && keySym <= FcitxKey_z) ||
            (keySym >= FcitxKey_A && keySym <= FcitxKey_Z)) {

            buffer_ += static_cast<char>(keySym);
            processPhonetics(keyEvent);
            keyEvent.filterAndAccept();
            return;
        }
}

void HasantaEngine::processPhonetics(fcitx::KeyEvent& keyEvent) {
    auto* ic = keyEvent.inputContext();
    if (buffer_.empty()) {
        ic->inputPanel().reset();
    } else {
        std::string translated = parseBufferToBengali();
        // Set the UI text under the cursor
        ic->inputPanel().setPreedit(fcitx::Text(translated));
    }
    ic->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
}

void HasantaEngine::commitText(fcitx::InputContext* ic, const std::string& text) {
    ic->commitString(text);
}

// Temporary layout lookup stub (We'll replace this with the complete state map)
std::string HasantaEngine::parseBufferToBengali() {
    if (buffer_.empty()) return "";
    return parser.translate(buffer_);
}

fcitx::AddonInstance* HasantaEngineFactory::create(fcitx::AddonManager* manager) {
    return new HasantaEngine(manager->instance());
}

FCITX_ADDON_FACTORY(HasantaEngineFactory)
