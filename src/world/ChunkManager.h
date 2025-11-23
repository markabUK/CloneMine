#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>

namespace clonemine {

// Block type definition
enum class BlockType : uint8_t {
    AIR = 0,
    STONE,
    DIRT,
    GRASS,
    WOOD,
    SAND,
    GRAVEL,
    WATER,
    LAVA,
    GLASS,
    OBSIDIAN,
    GOLD_ORE,
    DIAMOND_ORE,
    IRON_ORE,
    COAL_ORE
};

// Single block in the world
struct Block {
    BlockType type = BlockType::AIR;
    uint8_t light = 0;
    
    bool isSolid() const {
        return type != BlockType::AIR && type != BlockType::WATER;
    }
    
    bool isTransparent() const {
        return type == BlockType::AIR || type == BlockType::WATER || type == BlockType::GLASS;
    }
};

// Chunk dimensions
constexpr int CHUNK_SIZE_X = 16;
constexpr int CHUNK_SIZE_Y = 256;
constexpr int CHUNK_SIZE_Z = 16;
constexpr int BLOCKS_PER_CHUNK = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;

// Single chunk
class Chunk {
public:
    Chunk(const glm::ivec3& position);
    ~Chunk() = default;
    
    // Block access
    Block& getBlock(int x, int y, int z);
    const Block& getBlock(int x, int y, int z) const;
    
    void setBlock(int x, int y, int z, BlockType type);
    
    // Position and state
    const glm::ivec3& getPosition() const { return m_position; }
    bool isGenerated() const { return m_generated; }
    void setGenerated(bool generated) { m_generated = generated; }
    bool isDirty() const { return m_dirty; }
    void setDirty(bool dirty) { m_dirty = dirty; }
    
    // Get blocks for rendering
    const std::vector<Block>& getBlocks() const { return m_blocks; }
    
private:
    glm::ivec3 m_position;
    std::vector<Block> m_blocks;
    bool m_generated = false;
    bool m_dirty = false;
    
    inline int getIndex(int x, int y, int z) const {
        return x + z * CHUNK_SIZE_X + y * CHUNK_SIZE_X * CHUNK_SIZE_Z;
    }
};

// Hash function for chunk positions
struct ChunkPosHash {
    std::size_t operator()(const glm::ivec3& pos) const {
        return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1) ^ (std::hash<int>()(pos.z) << 2);
    }
};

// Chunk manager - handles all chunks in the world
class ChunkManager {
public:
    ChunkManager();
    ~ChunkManager();
    
    // Chunk lifecycle
    void update(const glm::vec3& playerPos, float deltaTime);
    Chunk* getChunk(const glm::ivec3& chunkPos);
    Chunk* getOrCreateChunk(const glm::ivec3& chunkPos);
    
    // Block access (world coordinates)
    Block* getBlock(const glm::ivec3& worldPos);
    void setBlock(const glm::ivec3& worldPos, BlockType type);
    
    // Chunk <-> World coordinate conversion
    static glm::ivec3 worldToChunk(const glm::ivec3& worldPos);
    static glm::ivec3 worldToLocal(const glm::ivec3& worldPos);
    
    // Configuration
    void setRenderDistance(int distance) { m_renderDistance = distance; }
    int getRenderDistance() const { return m_renderDistance; }
    
    // Get all loaded chunks for rendering
    std::vector<Chunk*> getLoadedChunks();
    
    // Save/Load
    void saveChunk(const glm::ivec3& chunkPos, const std::string& saveDir);
    bool loadChunk(const glm::ivec3& chunkPos, const std::string& saveDir);
    
private:
    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, ChunkPosHash> m_chunks;
    std::mutex m_chunkMutex;
    
    // Generation queue
    std::queue<glm::ivec3> m_generationQueue;
    std::vector<std::thread> m_generationThreads;
    bool m_running = true;
    
    // Settings
    int m_renderDistance = 12;  // 12 chunks = 192 blocks
    int m_loadDistance = 16;    // 16 chunks
    int m_unloadDistance = 20;  // 20 chunks
    
    // Worker thread for chunk generation
    void generationWorker();
    void generateChunk(Chunk* chunk);
    
    // Chunk loading/unloading based on player position
    void loadChunksAroundPlayer(const glm::vec3& playerPos);
    void unloadDistantChunks(const glm::vec3& playerPos);
};

} // namespace clonemine
