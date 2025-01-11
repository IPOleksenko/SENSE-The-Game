#include <objects/soundtrack.hpp>
#include <assets/assets.hpp>


Soundtrack::Soundtrack() :
    m_waveform(SDL_Incbin(SOUND_WIND_WAV))
{

}