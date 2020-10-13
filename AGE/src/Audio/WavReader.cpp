#include <stdexcept>
#include <fstream>
#include <iostream>

#include "WavReader.hpp"

namespace age {

struct ChunkHeader {
    uint32_t id;
    uint32_t size;
};

struct Header : public ChunkHeader {
    uint32_t format;
};

struct FmtSubchunk : public ChunkHeader {
    uint16_t format;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

void WavReader::read(const std::string& path) {
    std::ifstream file;
    file.open(path, std::ios::binary);

    Header header;
    file.read((char*)&header, sizeof(Header));

    // read format
    FmtSubchunk fmtChunk;
    file.read((char*)&fmtChunk, sizeof(FmtSubchunk));
    if (fmtChunk.id != 0x20746d66) {
        throw std::runtime_error("[WavReader]: unsupported or corrupted wav file");
    }

    m_freq = fmtChunk.sampleRate;

    if (fmtChunk.numChannels != 1) {
        std::cout << "Warning: file is not mono. Some features will not work\n";
    }

    // this will get unique id for each possible combination
    uint32_t id = fmtChunk.numChannels + fmtChunk.bitsPerSample;
    switch (id) {
    case 9:  m_format = AL_FORMAT_MONO8; break;
    case 17: m_format = AL_FORMAT_MONO16; break;
    case 10: m_format = AL_FORMAT_STEREO8; break;
    case 18: m_format = AL_FORMAT_STEREO16; break;
    default:
        throw std::runtime_error("[WavReader]: unsupported or corrupted wav file");
    }

    // find data chunk
    while (1) {
        ChunkHeader chunk;
        // read chunk header
        file.read((char*)&chunk, sizeof(chunk));
        // read chunk contents
        m_buf.resize(chunk.size);
        file.read(m_buf.data(), chunk.size);
        // quit loop if chunk.id is data
        if (chunk.id == 0x61746164) {
            m_size = chunk.size;
            return;
        }

        if (!file) {
            throw std::runtime_error("failed to read wav");
        }
    }
}

} // namespace age
