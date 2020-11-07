#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <stdexcept>
#include <string>

#define ec(e) case e: return #e

inline std::string alErrorToString(ALCenum err) {
    switch (err) {
        ec(ALC_NO_ERROR);
        ec(ALC_INVALID_CONTEXT);
        ec(ALC_INVALID_DEVICE);
        ec(ALC_INVALID_ENUM);
        ec(ALC_INVALID_VALUE);
        ec(ALC_OUT_OF_MEMORY);
    default:
        return "Unknown error code";
    }
}

inline void ERROR_CHECK(const std::string& name) {
    ALCenum error = alGetError();
    if (error != AL_NO_ERROR) {
        throw std::runtime_error(name + ": " + alErrorToString(error));
    }
}
