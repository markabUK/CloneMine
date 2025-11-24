#include "ChunkManager.h"
#include <cmath>
#include <algorithm>
#include <fstream>

namespace clonemine {

// Chunk implementation
Chunk::Chunk(const glm::ivec3& position)
    : m_position(position)
    , m_blocks(BLOCKS_PER_CHUNK)
{
    // Initialize all blocks to air
    for (auto& block : m_blocks) {
        block.type = BlockType::AIR;
        block.light = 0;
    }
}

Block& Chunk::getBlock(int x, int y, int z) {
    return m_blocks[getIndex(x, y, z)];
}

const Block& Chunk::getBlock(int x, int y, int z) const {
    return m_blocks[getIndex(x, y, z)];
}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
    m_blocks[getIndex(x, y, z)].type = type;
    m_dirty = true;
}

// ChunkManager implementation
ChunkManager::ChunkManager() {
    // Start 4 worker threads for chunk generation
    for (int i = 0; i < 4; ++i) {
        m_generationThreads.emplace_back(&ChunkManager::generationWorker, this);
    }
}

ChunkManager::~ChunkManager() {
    m_running = false;
    for (auto& thread : m_generationThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ChunkManager::update(const glm::vec3& playerPos, float deltaTime) {
    // Load chunks around player
    loadChunksAroundPlayer(playerPos);
    
    // Unload distant chunks
    unloadDistantChunks(playerPos);
}

Chunk* ChunkManager::getChunk(const glm::ivec3& chunkPos) {
    std::lock_guard<std::mutex> lock(m_chunkMutex);
    auto it = m_chunks.find(chunkPos);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    return nullptr;
}

Chunk* ChunkManager::getOrCreateChunk(const glm::ivec3& chunkPos) {
    std::lock_guard<std::mutex> lock(m_chunkMutex);
    auto it = m_chunks.find(chunkPos);
    if (it != m_chunks.end()) {
        return it->second.get();
    }
    
    // Create new chunk
    auto chunk = std::make_unique<Chunk>(chunkPos);
    Chunk* chunkPtr = chunk.get();
    m_chunks[chunkPos] = std::move(chunk);
    
    // Add to generation queue
    m_generationQueue.push(chunkPos);
    
    return chunkPtr;
}

Block* ChunkManager::getBlock(const glm::ivec3& worldPos) {
    glm::ivec3 chunkPos = worldToChunk(worldPos);
    glm::ivec3 localPos = worldToLocal(worldPos);
    
    Chunk* chunk = getChunk(chunkPos);
    if (!chunk) {
        return nullptr;
    }
    
    return &chunk->getBlock(localPos.x, localPos.y, localPos.z);
}

void ChunkManager::setBlock(const glm::ivec3& worldPos, BlockType type) {
    glm::ivec3 chunkPos = worldToChunk(worldPos);
    glm::ivec3 localPos = worldToLocal(worldPos);
    
    Chunk* chunk = getOrCreateChunk(chunkPos);
    chunk->setBlock(localPos.x, localPos.y, localPos.z, type);
}

glm::ivec3 ChunkManager::worldToChunk(const glm::ivec3& worldPos) {
    return glm::ivec3(
        std::floor(static_cast<float>(worldPos.x) / CHUNK_SIZE_X),
        std::floor(static_cast<float>(worldPos.y) / CHUNK_SIZE_Y),
        std::floor(static_cast<float>(worldPos.z) / CHUNK_SIZE_Z)
    );
}

glm::ivec3 ChunkManager::worldToLocal(const glm::ivec3& worldPos) {
    glm::ivec3 chunkPos = worldToChunk(worldPos);
    return glm::ivec3(
        worldPos.x - chunkPos.x * CHUNK_SIZE_X,
        worldPos.y - chunkPos.y * CHUNK_SIZE_Y,
        worldPos.z - chunkPos.z * CHUNK_SIZE_Z
    );
}

std::vector<Chunk*> ChunkManager::getLoadedChunks() {
    std::lock_guard<std::mutex> lock(m_chunkMutex);
    std::vector<Chunk*> chunks;
    chunks.reserve(m_chunks.size());
    
    for (auto& pair : m_chunks) {
        if (pair.second->isGenerated()) {
            chunks.push_back(pair.second.get());
        }
    }
    
    return chunks;
}

void ChunkManager::loadChunksAroundPlayer(const glm::vec3& playerPos) {
    glm::ivec3 playerChunk = worldToChunk(glm::ivec3(playerPos));
    
    // Load chunks within load distance
    for (int x = -m_loadDistance; x <= m_loadDistance; ++x) {
        for (int z = -m_loadDistance; z <= m_loadDistance; ++z) {
            glm::ivec3 chunkPos = playerChunk + glm::ivec3(x, 0, z);
            
            // Check distance
            float dist = std::sqrt(x * x + z * z);
            if (dist <= m_loadDistance) {
                getOrCreateChunk(chunkPos);
            }
        }
    }
}

void ChunkManager::unloadDistantChunks(const glm::vec3& playerPos) {
    glm::ivec3 playerChunk = worldToChunk(glm::ivec3(playerPos));
    
    std::lock_guard<std::mutex> lock(m_chunkMutex);
    
    // Find chunks to unload
    std::vector<glm::ivec3> toUnload;
    for (const auto& pair : m_chunks) {
        glm::ivec3 delta = pair.first - playerChunk;
        float dist = std::sqrt(delta.x * delta.x + delta.z * delta.z);
        
        if (dist > m_unloadDistance) {
            toUnload.push_back(pair.first);
        }
    }
    
    // Unload chunks
    for (const auto& pos : toUnload) {
        m_chunks.erase(pos);
    }
}

void ChunkManager::generationWorker() {
    while (m_running) {
        glm::ivec3 chunkPos;
        bool hasWork = false;
        
        {
            std::lock_guard<std::mutex> lock(m_chunkMutex);
            if (!m_generationQueue.empty()) {
                chunkPos = m_generationQueue.front();
                m_generationQueue.pop();
                hasWork = true;
            }
        }
        
        if (hasWork) {
            Chunk* chunk = getChunk(chunkPos);
            if (chunk && !chunk->isGenerated()) {
                generateChunk(chunk);
                chunk->setGenerated(true);
                chunk->setDirty(true);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void ChunkManager::generateChunk(Chunk* chunk) {
    const glm::ivec3& chunkPos = chunk->getPosition();
    
    // Simple terrain generation (heightmap based)
    for (int x = 0; x < CHUNK_SIZE_X; ++x) {
        for (int z = 0; z < CHUNK_SIZE_Z; ++z) {
            int worldX = chunkPos.x * CHUNK_SIZE_X + x;
            int worldZ = chunkPos.z * CHUNK_SIZE_Z + z;
            
            // Simple height calculation (can be replaced with noise)
            int height = 64 + static_cast<int>(std::sin(worldX * 0.1f) * 10 + std::cos(worldZ * 0.1f) * 10);
            
            for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
                BlockType type = BlockType::AIR;
                
                if (y < height - 5) {
                    type = BlockType::STONE;
                } else if (y < height - 1) {
                    type = BlockType::DIRT;
                } else if (y == height - 1) {
                    type = BlockType::GRASS;
                } else if (y < 60) {
                    type = BlockType::WATER;
                }
                
                chunk->setBlock(x, y, z, type);
            }
        }
    }
}

void ChunkManager::saveChunk(const glm::ivec3& chunkPos, const std::string& saveDir) {
    Chunk* chunk = getChunk(chunkPos);
    if (!chunk) return;
    
    std::string filename = saveDir + "/chunk_" + 
                          std::to_string(chunkPos.x) + "_" +
                          std::to_string(chunkPos.y) + "_" +
                          std::to_string(chunkPos.z) + ".dat";
    
    std::ofstream file(filename, std::ios::binary);
    if (file) {
        const auto& blocks = chunk->getBlocks();
        file.write(reinterpret_cast<const char*>(blocks.data()), 
                   blocks.size() * sizeof(Block));
    }
}

bool ChunkManager::loadChunk(const glm::ivec3& chunkPos, const std::string& saveDir) {
    std::string filename = saveDir + "/chunk_" + 
                          std::to_string(chunkPos.x) + "_" +
                          std::to_string(chunkPos.y) + "_" +
                          std::to_string(chunkPos.z) + ".dat";
    
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;
    
    Chunk* chunk = getOrCreateChunk(chunkPos);
    file.read(reinterpret_cast<char*>(&chunk->getBlock(0, 0, 0)), 
              BLOCKS_PER_CHUNK * sizeof(Block));
    
    chunk->setGenerated(true);
    chunk->setDirty(true);
    
    return true;
}

} // namespace clonemine
