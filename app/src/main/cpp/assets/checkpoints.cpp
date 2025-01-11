#include <assets/checkpoints.hpp>
#include <map>



std::string getCheckpointText(const CheckPoint& checkPoint) {
    static const std::map<CheckPoint, std::string> labels = {
        {
            CheckPoint::IDLE,
            R"(
Instructions:
    *Press A (or LEFT) and D (or RIGHT) alternately to move.
    *Pass the middle mark to start moving.
    *Keep your balance—don't let the pointer hit the red zone, or you'll lose!

Fullscreen: Press F to toggle fullscreen mode.)"
        },
        {
            CheckPoint::A_START,
            "You opened your eyes, but did you see anything new?",
        },
        {
            CheckPoint::B_START,
            "Every day is like the one before, yet you search for differences",
        },
        {
            CheckPoint::C_START,
            "People chase dreams, but who said dreams hold value?",
        },
        {
            CheckPoint::D_START,
            "How many questions have you asked, and how many answers have you received?",
        },
        {
            CheckPoint::E_START,
            "The world moves in circles, but where is its beginning and where is its end?",
        },
        {
            CheckPoint::F_START,
            "You strive to find a purpose, but what is it even for?",
        },
        {
            CheckPoint::G_START,
            "Stones lie on the ground for millennia, yet you live for only a moment.",
        },
        {
            CheckPoint::H_START,
            "What matters more: your thoughts or the sound of the wind?",
        },
        {
            CheckPoint::I_START,
            "The history of the world is full of heroes, but no one remembers them.",
        },
        {
            CheckPoint::J_START,
            "If something disappears tomorrow, what changes today?",
        },
        {
            CheckPoint::K_START,
            "The sun rises every day, but not for you.",
        },
        {
            CheckPoint::L_START,
            "Your heart beats, but who cares?",
        },
        {
            CheckPoint::M_START,
            "Everything you build will one day turn to dust.",
        },
        {
            CheckPoint::N_START,
            "You search for truth, but in this world, there is no law of truth.",
        },
        {
            CheckPoint::O_START,
            "Who created this world if even they had no plan?",
        },
        {
            CheckPoint::P_START,
            "Joy and pain alternate, but both eventually fade.",
        },
        {
            CheckPoint::Q_START,
            "You want to be needed, but by whom?",
        },
        {
            CheckPoint::R_START,
            "The stars shine, but not to show you the way.",
        },
        {
            CheckPoint::S_START,
            "Eternity is a word that both frightens and frees.",
        },
        {
            CheckPoint::T_START,
            "In this chaos, you seek meaning, but chaos demands no explanation.",
        },
        {
            CheckPoint::FINAL_START,
            "THE UNIVERSE DOESN'T MAKE SENSE.",
        },
    };

    const auto& it = labels.find(checkPoint);

    if(it != labels.end()) {
        return it->second;
    } else {
        return {};
    }
}
