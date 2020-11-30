#include "OpenAL.hpp"
#include "WavReader.hpp"

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
    WavReader reader;
    reader.read(file);

    alBufferData(m_alBuffer, reader.getFormat(), reader.getData(), reader.getSize(), reader.getFreq());

    ERROR_CHECK("Sound::load");
}

} // namespace age
