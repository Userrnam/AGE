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

#define ERROR_CHECK(name) \
    {\
        ALCenum error = alGetError(); \
        if (error != AL_NO_ERROR) { \
            throw std::runtime_error(name ": " + std::to_string(error)); \
        } \
    }
