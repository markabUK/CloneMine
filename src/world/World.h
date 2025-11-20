#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

namespace clonemine {

class Chunk;
struct Block;
enum class BlockType : uint8_t;

class World {
public:
    World();
    ~World() = default;

    // Delete copy operations
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    void update(float deltaTime);
    void setBlock(int x, int y, int z, BlockType type);
    [[nodiscard]] Block getBlock(int x, int y, int z) const;
    
    [[nodiscard]] const std::vector<std::unique_ptr<Chunk>>& getChunks() const noexcept {
        return m_chunks;
    }

private:
    void generateInitialChunks();
    [[nodiscard]] Chunk* getChunk(int chunkX, int chunkZ) const;

    std::vector<std::unique_ptr<Chunk>> m_chunks;
    std::unordered_map<uint64_t, size_t> m_chunkMap;
    
    [[nodiscard]] static uint64_t chunkKey(int x, int z) noexcept {
        return (static_cast<uint64_t>(x) << 32) | static_cast<uint64_t>(z);
    }
};

} // namespace clonemine
