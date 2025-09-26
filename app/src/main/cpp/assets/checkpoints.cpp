#include "assets/checkpoints.hpp"
#include "../utils/utils/localization.hpp"
#include <map>



std::string getCheckpointText(const CheckPoint& checkPoint) {
    static bool initialized = false;
    if (!initialized) {
        LocalizationManager::instance().init();
        initialized = true;
    }

    static const std::map<CheckPoint, std::pair<std::string, std::string>> labels = {
            {CheckPoint::LOADING_TEXT, {"LOADING_TEXT", "Loading..."}},
            {CheckPoint::ENDLESS_MODE_TEXT, {"ENDLESS_MODE", "ENDLESS MODE"}},
            { CheckPoint::IDLE, {"IDLE", std::string(
R"(
Instructions:
\
Keyboard:
    * Press A (or the Left Arrow key) and D (or the Right Arrow key) alternately to move.
    * Press the Spacebar to enable or disable Endless Mode (only before you start moving).
    * Press the Escape key to exit the game.
)"
#if !defined(__ANDROID__)
R"(
    * Press F to switch between windowed and fullscreen mode.
)"
#endif
R"(\
Gamepad:
    * Press Left or Right on the Directional Pad (or the X / B buttons) alternately to move.
    * Press the A button to enable or disable Endless Mode (only before you start moving).
    * Press the Start button to exit the game.
)"
#if !defined(__ANDROID__)
R"(
    * Press the Y button to switch between windowed and fullscreen mode.
)"
#endif
R"(\
Touchscreen:
    * Tap the left and right sides of the screen alternately to move.
    * Hold two fingers on the screen for two seconds to enable or disable Endless Mode (only before you start moving).
)"
R"(\
* Cross the center mark to begin moving.
* After crossing the center, maintain your balance — do not allow the pointer to touch the red zone, or you will lose.
)"
    )}
},
        {CheckPoint::A_START, {"A_START", "You opened your eyes, but did you see anything new?"}},
        {CheckPoint::B_START, {"B_START", "Every day is like the one before, yet you search for differences"}},
        {CheckPoint::C_START, {"C_START", "People chase dreams, but who said dreams hold value?"}},
        {CheckPoint::D_START, {"D_START", "How many questions have you asked, and how many answers have you received?"}},
        {CheckPoint::E_START, {"E_START", "The world moves in circles, but where is its beginning and where is its end?"}},
        {CheckPoint::F_START, {"F_START", "You strive to find a purpose, but what is it even for?"}},
        {CheckPoint::G_START, {"G_START", "Stones lie on the ground for millennia, yet you live for only a moment."}},
        {CheckPoint::H_START, {"H_START", "What matters more: your thoughts or the sound of the wind?"}},
        {CheckPoint::I_START, {"I_START", "The history of the world is full of heroes, but no one remembers them."}},
        {CheckPoint::J_START, {"J_START", "If something disappears tomorrow, what changes today?"}},
        {CheckPoint::K_START, {"K_START", "The sun rises every day, but not for you."}},
        {CheckPoint::L_START, {"L_START", "Your heart beats, but who cares?"}},
        {CheckPoint::M_START, {"M_START", "Everything you build will one day turn to dust."}},
        {CheckPoint::N_START, {"N_START", "You search for truth, but in this world, there is no law of truth."}},
        {CheckPoint::O_START, {"O_START", "You search for gods, but there are none."}},
        {CheckPoint::P_START, {"P_START", "Joy and pain alternate, but both eventually fade."}},
        {CheckPoint::Q_START, {"Q_START", "You want to be needed, but by whom?"}},
        {CheckPoint::R_START, {"R_START", "The stars shine, but not to show you the way."}},
        {CheckPoint::S_START, {"S_START", "Eternity is a word that both frightens and frees."}},
        {CheckPoint::T_START, {"T_START", "In this chaos, you seek meaning, but chaos demands no explanation."}},
        {CheckPoint::FINAL_START, {"FINAL_START", "THE UNIVERSE DOESN'T MAKE SENSE."}},
    };

    const auto it = labels.find(checkPoint);
    if (it != labels.end()) {
        std::string localized = LocalizationManager::instance().getText(it->second.first);
        return localized.empty() ? it->second.second : localized;
    }
    return {};
}
