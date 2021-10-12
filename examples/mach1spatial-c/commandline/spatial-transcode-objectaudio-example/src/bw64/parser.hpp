/**
 * @file parser.hpp
 *
 * Collection of parser functions, which construct chunk objects from istreams.
 */
#pragma once
#include "chunks.hpp"
#include "utils.hpp"

namespace bw64 {

  ///@brief Parse ExtraData from input stream
  inline std::shared_ptr<ExtraData> parseExtraData(std::istream& stream) {
    uint16_t validBitsPerSample;
    uint32_t dwChannelMask;
    uint16_t subFormat;
    char subFormatString[14];
    utils::readValue(stream, validBitsPerSample);
    utils::readValue(stream, dwChannelMask);
    utils::readValue(stream, subFormat);
    utils::readValue(stream, subFormatString);
    return std::make_shared<ExtraData>(validBitsPerSample, dwChannelMask,
                                       subFormat,
                                       std::string(subFormatString, 14));
  }

  /// @brief Parse FormatInfoChunk from input stream
  inline std::shared_ptr<FormatInfoChunk> parseFormatInfoChunk(
      std::istream& stream, uint32_t id, uint64_t size) {
    if (id != utils::fourCC("fmt ")) {
      std::stringstream errorString;
      errorString << "chunkId != 'fmt '";
      throw std::runtime_error(errorString.str());
    }
    if (size != 16 && size != 18 && size != 40) {
      throw std::runtime_error("illegal 'fmt ' chunk size");
    }

    uint16_t formatTag;
    uint16_t channelCount;
    uint32_t sampleRate;
    uint32_t bytesPerSecond;
    uint16_t blockAlignment;
    uint16_t bitsPerSample;
    uint16_t cbSize;
    std::shared_ptr<ExtraData> extraData;

    utils::readValue(stream, formatTag);
    utils::readValue(stream, channelCount);
    utils::readValue(stream, sampleRate);
    utils::readValue(stream, bytesPerSecond);
    utils::readValue(stream, blockAlignment);
    utils::readValue(stream, bitsPerSample);

    if (size > 16) {
      utils::readValue(stream, cbSize);
    } else {
      cbSize = 0;
    }
    if (size > 18 && cbSize > 0) {
      extraData = parseExtraData(stream);
    }

    if (cbSize != 0 && cbSize != 22) {
      throw std::runtime_error("unsupported cbSize");
    }

    if (formatTag != 1 && formatTag != 0xfffe) {
      std::stringstream errorString;
      errorString << "format unsupported: " << formatTag;
      throw std::runtime_error(errorString.str());
    }
    if (formatTag == 0xfffe) {
      if (!extraData) {
        throw std::runtime_error(
            "missing extra data for WAVE_FORMAT_EXTENSIBLE");
      } else {
        if (extraData->subFormat() != 1) {
          throw std::runtime_error("subformat unsupported");
        }
      }
    }

    auto formatInfoChunk = std::make_shared<FormatInfoChunk>(
        channelCount, sampleRate, bitsPerSample, extraData);

    if (formatInfoChunk->blockAlignment() != blockAlignment) {
      std::stringstream errorString;
      errorString << "sanity check failed. 'blockAlignment' is "
                  << blockAlignment << " but should be "
                  << formatInfoChunk->blockAlignment();
      throw std::runtime_error(errorString.str());
    }
    if (formatInfoChunk->bytesPerSecond() != bytesPerSecond) {
      std::stringstream errorString;
      errorString << "sanity check failed. 'bytesPerSecond' is "
                  << bytesPerSecond << " but should be "
                  << formatInfoChunk->bytesPerSecond();
      throw std::runtime_error(errorString.str());
    }

    return formatInfoChunk;
  }

  ///@brief Parse AxmlChunk from input stream
  inline std::shared_ptr<AxmlChunk> parseAxmlChunk(std::istream& stream,
                                                   uint32_t id, uint64_t size) {
    if (id != utils::fourCC("axml")) {
      std::stringstream errorString;
      errorString << "chunkId != 'axml'";
      throw std::runtime_error(errorString.str());
    }
    std::string data;
    data.resize(size);
    stream.read(&data[0], size);
    return std::make_shared<AxmlChunk>(data);
  }

  ///@brief Parse AudioId from input stream
  inline AudioId parseAudioId(std::istream& stream) {
    uint16_t trackIndex;
    char uid[12];
    char trackRef[14];
    char packRef[11];

    utils::readValue(stream, trackIndex);
    utils::readValue(stream, uid);
    utils::readValue(stream, trackRef);
    utils::readValue(stream, packRef);
    stream.seekg(1, std::ios::cur);  // skip padding

    return AudioId(trackIndex, std::string(uid, 12), std::string(trackRef, 14),
                   std::string(packRef, 11));
  }

  ///@brief Parse ChnaChunk from input stream
  inline std::shared_ptr<ChnaChunk> parseChnaChunk(std::istream& stream,
                                                   uint32_t id, uint64_t size) {
    if (id != utils::fourCC("chna")) {
      std::stringstream errorString;
      errorString << "chunkId != 'chna'";
      throw std::runtime_error(errorString.str());
    }
    if (size < 4) {
      throw std::runtime_error("illegal chna chunk size");
    }

    uint16_t numUids;
    uint16_t numTracks;
    utils::readValue(stream, numTracks);
    utils::readValue(stream, numUids);
    auto chnaChunk = std::make_shared<ChnaChunk>();
    for (int i = 0; i < numUids; ++i) {
      auto audioId = parseAudioId(stream);
      chnaChunk->addAudioId(audioId);
    }

    if (chnaChunk->numUids() != numUids) {
      std::stringstream errorString;
      errorString << "numUids != '" << chnaChunk->numUids() << "'";
      throw std::runtime_error(errorString.str());
    }
    if (chnaChunk->numTracks() != numTracks) {
      std::stringstream errorString;
      errorString << "numTracks != '" << chnaChunk->numTracks() << "'";
      throw std::runtime_error(errorString.str());
    }
    return chnaChunk;
  }

  /// @brief Construct DataSize64Chunk from input stream
  inline std::shared_ptr<DataSize64Chunk> parseDataSize64Chunk(
      std::istream& stream, uint32_t id, uint64_t size) {
    if (id != utils::fourCC("ds64")) {
      std::stringstream errorString;
      errorString << "chunkId != 'ds64'";
      throw std::runtime_error(errorString.str());
    }
    if (size < 28u) {
      throw std::runtime_error("illegal ds64 chunk size");
    }
    uint32_t tableLength;
    uint64_t bw64Size;
    uint64_t dataSize;
    uint64_t dummySize;
    utils::readValue(stream, bw64Size);
    utils::readValue(stream, dataSize);
    utils::readValue(stream, dummySize);
    utils::readValue(stream, tableLength);

    std::map<uint32_t, uint64_t> table;
    for (uint32_t i = 0; i < tableLength; ++i) {
      uint32_t id;
      uint64_t size;
      utils::readValue(stream, id);
      utils::readValue(stream, size);
      table[id] = size;
    }
    // skip junk data
    stream.seekg(size - 28u - (12u * tableLength), std::ios::cur);

    return std::make_shared<DataSize64Chunk>(bw64Size, dataSize, table);
  }

  inline std::shared_ptr<DataChunk> parseDataChunk(std::istream& /* stream */,
                                                   uint32_t id, uint64_t size) {
    if (id != utils::fourCC("data")) {
      std::stringstream errorString;
      errorString << "chunkId != 'data'";
      throw std::runtime_error(errorString.str());
    }
    auto dataChunk = std::make_shared<DataChunk>();
    dataChunk->setSize(size);
    return dataChunk;
  }

  inline std::shared_ptr<Chunk> parseChunk(std::istream& stream,
                                           ChunkHeader header) {
    stream.clear();
    stream.seekg(header.position + 8u);
    if (header.id == utils::fourCC("ds64")) {
      return parseDataSize64Chunk(stream, header.id, header.size);
    } else if (header.id == utils::fourCC("fmt ")) {
      return parseFormatInfoChunk(stream, header.id, header.size);
    } else if (header.id == utils::fourCC("axml")) {
      return parseAxmlChunk(stream, header.id, header.size);
    } else if (header.id == utils::fourCC("chna")) {
      return parseChnaChunk(stream, header.id, header.size);
    } else if (header.id == utils::fourCC("data")) {
      return parseDataChunk(stream, header.id, header.size);
    } else {
      return std::make_shared<UnknownChunk>(stream, header.id, header.size);
    }
  }

}  // namespace bw64
