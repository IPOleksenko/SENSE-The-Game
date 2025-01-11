#pragma once

#include <utils/waveform.hpp>

class Soundtrack {
public:
    Soundtrack();
    ~Soundtrack() = default;

//    [[nodiscard]] Waveform& getAudio() const;

private:
    Waveform m_waveform;
};