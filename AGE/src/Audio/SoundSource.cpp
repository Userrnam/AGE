#include "OpenAL.hpp"

#include "SoundSource.hpp"


namespace age {

void SoundSource::create() {
    alGenSources(1, &m_alSource);
    ERROR_CHECK("SoundSource::create");
}

void SoundSource::destroy() {
    alDeleteSources(1, &m_alSource);
    ERROR_CHECK("SoundSource::destroy");
}

void SoundSource::play() {
    alSourcePlay(m_alSource);
    ERROR_CHECK("SoundSource::play");
}

void SoundSource::stop() {
    alSourceStop(m_alSource);
    ERROR_CHECK("SoundSource::stop");
}

void SoundSource::pause() {
    alSourcePause(m_alSource);
    ERROR_CHECK("SoundSource::pause");
}

void SoundSource::rewind() {
    alSourceRewind(m_alSource);
    ERROR_CHECK("SoundSource::rewind");
}

void SoundSource::setSound(const Sound& sound) {
    alSourcei(m_alSource, AL_BUFFER, sound.m_alBuffer);
    ERROR_CHECK("SoundSource::setSound");
}

void SoundSource::setPosition(const Vector3f& position) {
    alSource3f(m_alSource, AL_POSITION, position.x, position.y, position.z);
    ERROR_CHECK("SoundSource::setPosition");
}

void SoundSource::setVelocity(const Vector3f& velocity) {
    alSource3f(m_alSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    ERROR_CHECK("SoundSource::setVelocity");
}

void SoundSource::setPitch(float pitch) {
    alSourcef(m_alSource, AL_PITCH, pitch);
    ERROR_CHECK("SoundSource::setPitch");
}

void SoundSource::setGain(float gain) {
    alSourcef(m_alSource, AL_GAIN, gain);
    ERROR_CHECK("SoundSource::setGain");
}

void SoundSource::setLooping(bool b) {
    alSourcei(m_alSource, AL_LOOPING, b);
    ERROR_CHECK("SoundSource::setLooping");
}

SoundSourceState SoundSource::getState() {
    ALint sourceState;
    alGetSourcei(m_alSource, AL_SOURCE_STATE, &sourceState);

    switch (sourceState) {
    case AL_INITIAL: return SoundSourceState::INITIAL;
    case AL_PLAYING: return SoundSourceState::PLAYING;
    case AL_PAUSED:  return SoundSourceState::PAUSED;
    case AL_STOPPED: return SoundSourceState::STOPPED;
    }
}

} // namespace age
