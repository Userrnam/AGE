#include "OpenAL.hpp"

#include "Sound.hpp"

namespace age {

void Sound::create() {
    alGenBuffers(1, &m_alBuffer);
    ERROR_CHECK("Sound::create");
}

void Sound::destroy() {
    alDeleteBuffers(1, &m_alBuffer);
    ERROR_CHECK("Sound::destroy");
}

void Sound::load(const std::string& file) {
    ALsizei size,freq;
    ALenum  format;
    ALvoid  *data;

    alutLoadWAVFile((ALbyte*)file.c_str(), &format,&data, &size, &freq);
    alBufferData(m_alBuffer, format, data, size, freq);
    alutUnloadWAV(format,data,size,freq);

    ERROR_CHECK("Sound::load");
}



} // namespace age
