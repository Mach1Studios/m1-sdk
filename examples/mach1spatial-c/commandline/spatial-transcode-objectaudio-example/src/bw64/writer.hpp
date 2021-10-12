/// @file writer.hpp
#pragma once
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>
#include "chunks.hpp"
#include "utils.hpp"

namespace bw64 {

  const uint32_t MAX_NUMBER_OF_UIDS = 1024;

  /**
   * @brief BW64 Writer class
   *
   * Normally, you will create an instance of this class using
   * bw64::writeFile().
   *
   * This is a
   * [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization)
   * class, meaning that the file will be openend and initialized (required
   * headers etc.) on construction, and closed and finalized (writing chunk
   * sizes etc.) on destruction.
   */
  class Bw64Writer {
   public:
    /**
     * @brief Open a new BW64 file for writing
     *
     * Opens a new BW64 file for writing, initializes everything up to the
     * `data` chunk. Afterwards, you may write interleaved audio samples to this
     * file.
     *
     * @warning If the file already exists it will be overwritten.
     *
     * If you need any chunks to appear *before* the data chunk, include them in
     * the `additionalChunks`. They will be written directly after opening the
     * file.
     *
     * @note For convenience, you might consider using the `writeFile` helper
     * function.
     */
    Bw64Writer(const char* filename, uint16_t channels, uint32_t sampleRate,
               uint16_t bitDepth,
               std::vector<std::shared_ptr<Chunk>> additionalChunks) {
      fileStream_.open(filename, std::fstream::out | std::fstream::binary);
      if (!fileStream_.is_open()) {
        std::stringstream errorString;
        errorString << "Could not open file: " << filename;
        throw std::runtime_error(errorString.str());
      }
      writeRiffHeader();
      writeChunkPlaceholder(utils::fourCC("JUNK"), 28u);
      auto formatChunk =
          std::make_shared<FormatInfoChunk>(channels, sampleRate, bitDepth);
      writeChunk(formatChunk);

      for (auto chunk : additionalChunks) {
        writeChunk(chunk);
      }
      if (!chnaChunk()) {
        writeChunkPlaceholder(utils::fourCC("chna"),
                              MAX_NUMBER_OF_UIDS * 40 + 4);
      }
      auto dataChunk = std::make_shared<DataChunk>();
      writeChunk(dataChunk);
    }

    /**
     * @brief Finalize file
     *
     * This destructor will write all yet-to-be-written chunks to the file
     * and will also finalize all required information, i.e. the final chunk
     * sizes etc.
     */
    ~Bw64Writer() {
      finalizeDataChunk();
      for (auto chunk : postDataChunks_) {
        writeChunk(chunk);
      }
      finalizeRiffChunk();
      fileStream_.close();
    }

    /// @brief Get format tag
    uint16_t formatTag() const { return formatChunk()->formatTag(); };
    /// @brief Get number of channels
    uint16_t channels() const { return formatChunk()->channelCount(); };
    /// @brief Get sample rate
    uint32_t sampleRate() const { return formatChunk()->sampleRate(); };
    /// @brief Get bit depth
    uint16_t bitDepth() const { return formatChunk()->bitsPerSample(); };
    /// @brief Get number of frames
    uint64_t framesWritten() const {
      return dataChunk()->size() / formatChunk()->blockAlignment();
    }

    template <typename ChunkType>
    std::vector<std::shared_ptr<ChunkType>> chunksWithId(
        const std::vector<Chunk>& chunks, uint32_t chunkId) const {
      std::vector<char> foundChunks;
      auto chunk =
          std::copy_if(chunks.begin(), chunks.end(), foundChunks.begin(),
                       [chunkId](const std::shared_ptr<Chunk> chunk) {
                         return chunk->id() == chunkId;
                       });
      return foundChunks;
    }

    template <typename ChunkType>
    std::shared_ptr<ChunkType> chunk(
        const std::vector<std::shared_ptr<Chunk>>& chunks,
        uint32_t chunkId) const {
      auto chunk = std::find_if(chunks.begin(), chunks.end(),
                                [chunkId](const std::shared_ptr<Chunk> chunk) {
                                  return chunk->id() == chunkId;
                                });
      if (chunk != chunks.end()) {
        return std::static_pointer_cast<ChunkType>(*chunk);
      } else {
        return nullptr;
      }
    }

    std::shared_ptr<DataSize64Chunk> ds64Chunk() const {
      return chunk<DataSize64Chunk>(chunks_, utils::fourCC("ds64"));
    }
    std::shared_ptr<FormatInfoChunk> formatChunk() const {
      return chunk<FormatInfoChunk>(chunks_, utils::fourCC("fmt "));
    }
    std::shared_ptr<DataChunk> dataChunk() const {
      return chunk<DataChunk>(chunks_, utils::fourCC("data"));
    }
    std::shared_ptr<ChnaChunk> chnaChunk() const {
      return chunk<ChnaChunk>(chunks_, utils::fourCC("chna"));
    }
    std::shared_ptr<AxmlChunk> axmlChunk() const {
      return chunk<AxmlChunk>(chunks_, utils::fourCC("axml"));
    }

    /// @brief Check if file is bigger than 4GB and therefore a BW64 file
    bool isBw64File() {
      if (riffChunkSize() > UINT32_MAX) {
        return true;
      }
      if (dataChunk()->size() > UINT32_MAX) {
        return true;
      }
      return false;
    }

    void setChnaChunk(std::shared_ptr<ChnaChunk> chunk) {
      if (chunk->numUids() > 1024) {
        // TODO: make pre data chunk chna chunk a JUNK chunk and add chnaChunk
        // to postDataChunks_?
        throw std::runtime_error("number of trackUids is > 1024");
      }
      auto last_position = fileStream_.tellp();
      overwriteChunk(utils::fourCC("chna"), chunk);
      fileStream_.seekp(last_position);
    }

    void setAxmlChunk(std::shared_ptr<Chunk> chunk) {
      postDataChunks_.push_back(chunk);
    }

    /// @brief Get the chunk size for header
    uint32_t chunkSizeForHeader(uint32_t id) {
      if (chunkHeader(id).size >= UINT32_MAX) {
        return UINT32_MAX;
      } else {
        return static_cast<uint32_t>(chunkHeader(id).size);
      }
    }

    /// @brief Calculate riff chunk size
    uint64_t riffChunkSize() {
      auto last_position = fileStream_.tellp();
      fileStream_.seekp(0, std::ios::end);
      uint64_t endPos = fileStream_.tellp();
      fileStream_.seekp(last_position);
      return endPos - 8u;
    }

    /// @brief Write RIFF header
    void writeRiffHeader() {
      uint32_t RiffId = utils::fourCC("RIFF");
      uint32_t fileSize = UINT32_MAX;
      uint32_t WaveId = utils::fourCC("WAVE");
      utils::writeValue(fileStream_, RiffId);
      utils::writeValue(fileStream_, fileSize);
      utils::writeValue(fileStream_, WaveId);
    }

    /// @brief Update RIFF header
    void finalizeRiffChunk() {
      auto last_position = fileStream_.tellp();
      fileStream_.seekp(0);
      if (isBw64File()) {
        utils::writeValue(fileStream_, utils::fourCC("BW64"));
        utils::writeValue(fileStream_, INT32_MAX);
        overwriteJunkWithDs64Chunk();
      } else {
        utils::writeValue(fileStream_, utils::fourCC("RIFF"));
        uint32_t fileSize = static_cast<uint32_t>(riffChunkSize());
        utils::writeValue(fileStream_, fileSize);
      }
      fileStream_.seekp(last_position);
    }

    void overwriteJunkWithDs64Chunk() {
      auto ds64Chunk = std::make_shared<DataSize64Chunk>();
      ds64Chunk->bw64Size(riffChunkSize());
      ds64Chunk->dataSize(dataChunk()->size());
      // TODO: add other chunks which are bigger than 4GB
      overwriteChunk(utils::fourCC("JUNK"), ds64Chunk);
    }

    void finalizeDataChunk() {
      if (dataChunk()->size() % 2 == 1) {
        utils::writeValue(fileStream_, '\0');
      }
      auto last_position = fileStream_.tellp();
      seekChunk(utils::fourCC("data"));
      utils::writeValue(fileStream_, utils::fourCC("data"));
      utils::writeValue(fileStream_, chunkSizeForHeader(utils::fourCC("data")));
      fileStream_.seekp(last_position);
    }

    /// @brief Write chunk template
    template <typename ChunkType>
    void writeChunk(std::shared_ptr<ChunkType> chunk) {
      if (chunk) {
        uint64_t position = fileStream_.tellp();
        chunkHeaders_.push_back(
            ChunkHeader(chunk->id(), chunk->size(), position));
        utils::writeChunk<ChunkType>(fileStream_, chunk,
                                     chunkSizeForHeader(chunk->id()));
        chunks_.push_back(chunk);
      }
    }

    void writeChunkPlaceholder(uint32_t id, uint32_t size) {
      uint64_t position = fileStream_.tellp();
      chunkHeaders_.push_back(ChunkHeader(id, size, position));
      utils::writeChunkPlaceholder(fileStream_, id, size);
    }

    /// @brief Overwrite chunk template
    template <typename ChunkType>
    void overwriteChunk(uint32_t id, std::shared_ptr<ChunkType> chunk) {
      auto last_position = fileStream_.tellp();
      seekChunk(id);
      utils::writeChunk<ChunkType>(fileStream_, chunk, chunkSizeForHeader(id));
      fileStream_.seekp(last_position);
    }

    void seekChunk(uint32_t id) {
      auto header = chunkHeader(id);
      fileStream_.clear();
      fileStream_.seekp(header.position);
    }

    ChunkHeader& chunkHeader(uint32_t id) {
      auto foundHeader = std::find_if(
          chunkHeaders_.begin(), chunkHeaders_.end(),
          [id](const ChunkHeader header) { return header.id == id; });
      if (foundHeader != chunkHeaders_.end()) {
        return *foundHeader;
      }
      std::stringstream errorMsg;
      errorMsg << "no chunk with id '" << utils::fourCCToStr(id) << "' found";
      throw std::runtime_error(errorMsg.str());
    }

    /**
     * @brief Write frames to dataChunk
     *
     * @param[out] inBuffer Buffer to write the samples to
     * @param[in]  frames   Number of frames to write
     *
     * @returns number of frames written
     */
    template <typename T,
              typename = std::enable_if<std::is_floating_point<T>::value>>
    uint64_t write(T* inBuffer, uint64_t frames) {
      uint64_t bytesWritten = frames * formatChunk()->blockAlignment();
      rawDataBuffer_.resize(bytesWritten);
      utils::encodePcmSamples(inBuffer, &rawDataBuffer_[0],
                              frames * formatChunk()->channelCount(),
                              formatChunk()->bitsPerSample());
      fileStream_.write(&rawDataBuffer_[0], bytesWritten);
      dataChunk()->setSize(dataChunk()->size() + bytesWritten);
      chunkHeader(utils::fourCC("data")).size = dataChunk()->size();
      return frames;
    }

   private:
    std::ofstream fileStream_;
    std::vector<char> rawDataBuffer_;
    std::vector<std::shared_ptr<Chunk>> chunks_;
    std::vector<ChunkHeader> chunkHeaders_;
    std::vector<std::shared_ptr<Chunk>> postDataChunks_;
  };

}  // namespace bw64
