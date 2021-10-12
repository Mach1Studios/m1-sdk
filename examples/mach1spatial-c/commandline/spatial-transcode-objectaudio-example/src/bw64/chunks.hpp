/// @file chunks.hpp
#pragma once
#include <cstring>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>
#include "utils.hpp"

namespace bw64 {

  /**
   * @brief Data structure to cache position and size of chunks
   *
   * After opening a file and some basic checks, the file will be scanned
   * for chunks. The data (chunkId, chunkSize, position in the file) of the
   * found chunks will be saved in ChunkHeader objects.
   */
  struct ChunkHeader {
    ChunkHeader(uint32_t id = 0, uint64_t size = 0, uint64_t position = 0)
        : id(id), size(size), position(position) {}
    uint32_t id;
    uint64_t size;
    uint64_t position;
  };

  /**
   * @brief RIFF chunk base class
   */
  class Chunk {
   public:
    virtual ~Chunk() = default;

    /// @brief Get FourCC id
    virtual uint32_t id() const = 0;
    /// @brief Get the size of the chunk
    virtual uint64_t size() const = 0;

    /// @brief Write the chunk to a stream
    virtual void write(std::ostream& stream) const = 0;

   protected:
    Chunk() = default;
  };

  /**
   * @brief Class representation of a custom chunk
   *
   * This class can be used to copy unknown chunks from an input file to an
   * output file.
   */
  class UnknownChunk : public Chunk {
   public:
    UnknownChunk(uint32_t id) { chunkId_ = id; }
    UnknownChunk(std::istream& stream, uint32_t id, uint64_t size) {
      chunkId_ = id;
      data_.resize(size);
      stream.read(&data_[0], size);
    }

    uint32_t id() const override { return chunkId_; }
    uint64_t size() const override { return data_.size(); }

    void write(std::ostream& stream) const override {
      std::copy(data_.begin(), data_.end(),
                std::ostreambuf_iterator<char>(stream));
    }

   private:
    uint32_t chunkId_;
    std::vector<char> data_;
  };

  /**
   * @brief Class representation of the ExtraData of a FormatInfoChunk
   */
  class ExtraData {
   public:
    /// @brief ExtraData constructor
    ExtraData(uint16_t validBitsPerSample, uint32_t dwChannelMask,
              uint16_t subFormat, std::string subFormatString)
        : validBitsPerSample_(validBitsPerSample),
          dwChannelMask_(dwChannelMask),
          subFormat_(subFormat),
          subFormatString_(subFormatString) {}

    /// @brief ValidBitsPerSample getter
    uint16_t validBitsPerSample() const { return validBitsPerSample_; }
    /// @brief DwChannelMask getter
    uint32_t dwChannelMask() const { return dwChannelMask_; }
    /// @brief SubFormat getter
    uint16_t subFormat() const { return subFormat_; }
    /// @brief SubFormatString getter
    std::string subFormatString() const { return subFormatString_; }

   private:
    uint16_t validBitsPerSample_;
    uint32_t dwChannelMask_;
    uint16_t subFormat_;
    std::string subFormatString_;
  };

  /**
   * @brief Class representation of a FormatInfoChunk
   */
  class FormatInfoChunk : public Chunk {
   public:
    /**
     * @brief Simple FormatInfoChunk constructor
     *
     * @param channels number of channels
     * @param sampleRate sample rate of the audio data
     * @param bitDepth bit depth used in file
     * @param extraData custom ExtraData (optional, nullptr if not custom)
     */
    FormatInfoChunk(uint16_t channels, uint32_t sampleRate, uint32_t bitDepth,
                    std::shared_ptr<ExtraData> extraData = nullptr) {
      formatTag_ = 1;
      channelCount_ = channels;
      sampleRate_ = sampleRate;
      bitsPerSample_ = bitDepth;
      extraData_ = extraData;

      // validation
      if (channelCount_ < 1) {
        throw std::runtime_error("channelCount < 1");
      }
      if (sampleRate_ < 1) {
        throw std::runtime_error("sampleRate < 1");
      }
      if (bitsPerSample_ != 16u && bitsPerSample_ != 24u &&
          bitsPerSample_ != 32u) {
        std::stringstream errorString;
        errorString << "bitDepth not supported: " << bitsPerSample_;
        throw std::runtime_error(errorString.str());
      }
    }

    uint32_t id() const override { return utils::fourCC("fmt "); }
    uint64_t size() const override { return 16u; }

    /// @brief FormatTag getter
    uint16_t formatTag() const { return formatTag_; }
    /// @brief ChannelCount getter
    uint16_t channelCount() const { return channelCount_; }
    /// @brief SampleRate getter
    uint32_t sampleRate() const { return sampleRate_; }
    /// @brief BytesPerSecond getter
    uint32_t bytesPerSecond() const { return sampleRate() * blockAlignment(); }
    /// @brief BlockAlignment getter
    uint16_t blockAlignment() const {
      return channelCount() * bitsPerSample() / 8;
    }
    /// @brief BitsPerSample getter
    uint16_t bitsPerSample() const { return bitsPerSample_; }

    /// @brief ExtraData getter
    const std::shared_ptr<ExtraData> extraData() const { return extraData_; }

    void write(std::ostream& stream) const override {
      utils::writeValue(stream, formatTag());
      utils::writeValue(stream, channelCount());
      utils::writeValue(stream, sampleRate());
      utils::writeValue(stream, bytesPerSecond());
      utils::writeValue(stream, blockAlignment());
      utils::writeValue(stream, bitsPerSample());
      if (extraData()) {
        utils::writeValue(stream, extraData()->validBitsPerSample());
        utils::writeValue(stream, extraData()->dwChannelMask());
        utils::writeValue(stream, extraData()->subFormat());
        utils::writeValue(stream, extraData()->subFormatString());
      }
    }

   private:
    uint16_t formatTag_;
    uint16_t channelCount_;
    uint32_t sampleRate_;
    uint16_t bitsPerSample_;
    std::shared_ptr<ExtraData> extraData_;
  };

  /**
   * @brief Class representation of a DataChunk
   */
  class DataChunk : public Chunk {
   public:
    DataChunk() { size_ = 0; }

    uint32_t id() const override { return utils::fourCC("data"); }
    uint64_t size() const override { return size_; }

    void setSize(uint64_t size) { size_ = size; }

    /**
     * @brief Not to be used write chunk to stream
     *
     * @warning As the data chunk is usually not written in one piece the
     * override for this function is not used. Using this method will throw an
     * exception.
     */
    void write(std::ostream& /* stream */) const override {
      throw std::logic_error(
          "DataChunk::write method is not implemented. Use Bw64Writer::write "
          "instead.");
    }

   private:
    uint64_t size_;
  };

  /**
   * @brief Class representation of a DataChunk
   */
  class JunkChunk : public Chunk {
   public:
    JunkChunk() { data_.resize(28, '\0'); }

    uint32_t id() const override { return utils::fourCC("JUNK"); }
    uint64_t size() const override { return data_.size(); }

    void write(std::ostream& stream) const override {
      std::copy(data_.begin(), data_.end(),
                std::ostreambuf_iterator<char>(stream));
    }

   private:
    std::vector<char> data_;
  };

  /**
   * @brief Class representation of an AxmlChunk
   */
  class AxmlChunk : public Chunk {
   public:
    static uint32_t Id() { return utils::fourCC("axml"); }

    AxmlChunk(const std::string& axml) {
      std::copy(axml.begin(), axml.end(), std::back_inserter(data_));
    }

    uint32_t id() const override { return AxmlChunk::Id(); }
    uint64_t size() const override { return data_.size(); }

    /*
     * @brief Write the AxmlChunk to a stream
     */
    void write(std::ostream& stream) const override {
      std::copy(data_.begin(), data_.end(),
                std::ostreambuf_iterator<char>(stream));
    }

   private:
    std::vector<char> data_;
  };

  /**
   * @brief Class representation of an AudioId field
   */
  class AudioId {
   public:
    AudioId(uint16_t trackIndex, const std::string& uid,
            const std::string trackRef, const std::string& packRef) {
      if (uid.size() > 12) {
        std::stringstream errorString;
        errorString << "uid \'" << uid << "\' is too long (" << uid.size()
                    << " > " << 12;
        throw std::runtime_error("uid is too long ");
      }
      if (trackRef.size() > 14) {
        std::stringstream errorString;
        errorString << "trackRef \'" << trackRef << "\' is too long ("
                    << trackRef.size() << " > " << 14;
        throw std::runtime_error("uid is too long ");
      }
      if (packRef.size() > 11) {
        std::stringstream errorString;
        errorString << "packRef \'" << packRef << "\' is too long ("
                    << packRef.size() << " > " << 11;
        throw std::runtime_error("packRef is too long ");
      }

      // init arrays with whitspaces
      std::memset(uid_, ' ', 12);
      std::memset(trackRef_, ' ', 14);
      std::memset(packRef_, ' ', 11);

      // save values
      trackIndex_ = trackIndex;
      std::copy(uid.begin(), uid.end(), uid_);
      std::copy(trackRef.begin(), trackRef.end(), trackRef_);
      std::copy(packRef.begin(), packRef.end(), packRef_);
    }

    /*
     * @brief TrackIndex getter
     *
     * @returns Track index of the AudioId
     */
    uint16_t trackIndex() const { return trackIndex_; };
    /*
     * @brief audioTrackUID getter
     *
     * @returns audioTrackUID of the AudioId
     */
    std::string uid() const { return std::string(uid_, 12); }
    /*
     * @brief audioTrackFormatID getter
     *
     * @returns audioTrackFormatID of the AudioId
     */
    std::string trackRef() const { return std::string(trackRef_, 14); }
    /*
     * @brief audioPackFormatID getter
     *
     * @returns audioPackFormatID of the AudioId
     */
    std::string packRef() const { return std::string(packRef_, 11); }

    /*
     * @brief Write the AudioId to a stream
     */
    void write(std::ostream& stream) const {
      utils::writeValue(stream, trackIndex());
      utils::writeValue(stream, uid_);
      utils::writeValue(stream, trackRef_);
      utils::writeValue(stream, packRef_);
      utils::writeValue(stream, ' ');  // padding
    }

    bool operator==(const AudioId& rhs) const {
      return trackIndex() == rhs.trackIndex() && uid() == rhs.uid() &&
             trackRef() == rhs.trackRef() && packRef() == rhs.packRef();
    }
    bool operator!=(const AudioId& rhs) const { return !(*this == rhs); }

   private:
    uint16_t trackIndex_;
    char uid_[12];
    char trackRef_[14];
    char packRef_[11];
  };

  /**
   * @brief Class representation of an ChnaChunk
   */
  class ChnaChunk : public Chunk {
   public:
    ChnaChunk(){};
    ChnaChunk(std::initializer_list<AudioId> audioIds) : audioIds_(audioIds){};
    ChnaChunk(std::vector<AudioId> audioIds) : audioIds_(audioIds){};

    uint32_t id() const override { return utils::fourCC("chna"); }
    uint64_t size() const override {
      return sizeof(numTracks()) + sizeof(numUids()) + numUids() * 40;
    }

    /// @brief NumTracks getter
    uint16_t numTracks() const {
      std::set<uint16_t> trackIndices;
      for (auto audioId : audioIds()) {
        trackIndices.insert(audioId.trackIndex());
      }
      return static_cast<uint16_t>(trackIndices.size());
    }
    /// @brief NumUids getter
    uint16_t numUids() const { return static_cast<uint16_t>(audioIds_.size()); }
    /// @brief AudioIds getter
    std::vector<AudioId> audioIds() const { return audioIds_; }
    /// @brief Add AudioId to AudioId table
    void addAudioId(AudioId id) { audioIds_.push_back(id); }

    void write(std::ostream& stream) const override {
      utils::writeValue(stream, numTracks());
      utils::writeValue(stream, numUids());
      for (auto audioId : audioIds()) {
        audioId.write(stream);
      }
    }

   private:
    std::vector<AudioId> audioIds_;
  };

  /**
   * @brief Class representation of a DataSize64 chunk
   */
  class DataSize64Chunk : public Chunk {
   public:
    /// @brief DataSize64Chunk constructor
    DataSize64Chunk(
        uint64_t bw64Size = 0, uint64_t dataSize = 0,
        std::map<uint32_t, uint64_t> table = std::map<uint32_t, uint64_t>())
        : bw64Size_(bw64Size), dataSize_(dataSize), table_(table) {
      dummySize_ = 0;
    }

    uint32_t id() const override { return utils::fourCC("ds64"); }
    uint64_t size() const override {
      return sizeof(bw64Size()) + sizeof(dataSize()) + sizeof(dummySize()) +
             sizeof(tableLength()) + table_.size() * 12;
    }
    /// @brief Bw64Size getter
    uint64_t bw64Size() const { return bw64Size_; }
    /// @brief DataSize getter
    uint64_t dataSize() const { return dataSize_; }
    /// @brief DummySize getter
    uint64_t dummySize() const { return dummySize_; }
    /// @brief TableLength getter
    uint32_t tableLength() const {
      return static_cast<uint32_t>(table_.size());
    }

    /// @brief Bw64Size setter
    void bw64Size(uint64_t size) { bw64Size_ = size; }
    /// @brief DataSize setter
    void dataSize(uint64_t size) { dataSize_ = size; }
    /// @brief DummySize setter
    void dummySize(uint64_t size) { dummySize_ = size; }

    /// @brief Get table
    const std::map<uint32_t, uint64_t>& table() const { return table_; }

    /// @brief Has chunkSize for id
    bool hasChunkSize(uint32_t id) const { return table_.count(id) != 0; }
    /// @brief Get chunkSize for id
    uint64_t getChunkSize(uint32_t id) const { return table_.at(id); }
    /// @brief Set or add a ChunkSize
    void setChunkSize(uint32_t id, uint64_t size) {
      if (id == utils::fourCC("bw64")) {  //
        bw64Size_ = size;
      } else if (id == utils::fourCC("data")) {
        dataSize_ = size;
      } else {
        table_[id] = size;
      }
    }

    /// @brief Remove a ChunkSize from table
    void removeChunkSize(uint32_t id) { table_.erase(id); }
    /// @brief Clear ChunkSize table
    void clearChunkSizeTable() { table_.clear(); }

    void write(std::ostream& stream) const override {
      utils::writeValue(stream, bw64Size());
      utils::writeValue(stream, dataSize());
      utils::writeValue(stream, dummySize());
      utils::writeValue(stream, tableLength());
      for (auto& entry : table()) {
        utils::writeValue(stream, entry.first);  // chunkId
        utils::writeValue(stream, entry.second);  // chunkSize
      }
    }

   private:
    uint64_t bw64Size_;
    uint64_t dataSize_;
    uint64_t dummySize_;
    std::map<uint32_t, uint64_t> table_;
  };

}  // namespace bw64
