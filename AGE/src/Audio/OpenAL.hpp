#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/alut.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
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
