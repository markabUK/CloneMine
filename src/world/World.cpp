#include "world/World.h"
#include "world/Chunk.h"

namespace clonemine {

World::World() {
    generateInitialChunks();
}

void World::update(float deltaTime) {
    // World update logic (physics, block updates, etc.)
    (void)deltaTime; // Suppress unused parameter warning
}

void World::setBlock(int x, int y, int z, BlockType type) {
    int chunkX = x / Chunk::SIZE;
    int chunkZ = z / Chunk::SIZE;
    int localX = x % Chunk::SIZE;
    int localZ = z % Chunk::SIZE;
    
    if (localX < 0) localX += Chunk::SIZE;
    if (localZ < 0) localZ += Chunk::SIZE;
    
    if (auto* chunk = getChunk(chunkX, chunkZ)) {
        chunk->setBlock(localX, y, localZ, type);
    }
}

Block World::getBlock(int x, int y, int z) const {
    int chunkX = x / Chunk::SIZE;
    int chunkZ = z / Chunk::SIZE;
    int localX = x % Chunk::SIZE;
    int localZ = z % Chunk::SIZE;
    
    if (localX < 0) localX += Chunk::SIZE;
    if (localZ < 0) localZ += Chunk::SIZE;
    
    if (auto* chunk = getChunk(chunkX, chunkZ)) {
        return chunk->getBlock(localX, y, localZ);
    }
    return Block{BlockType::Air};
}

void World::generateInitialChunks() {
    // Generate a 4x4 grid of chunks around origin
    for (int x = -2; x < 2; ++x) {
        for (int z = -2; z < 2; ++z) {
            auto chunk = std::make_unique<Chunk>(x, z);
            uint64_t key = chunkKey(x, z);
            m_chunkMap[key] = m_chunks.size();
            m_chunks.push_back(std::move(chunk));
        }
    }
}

Chunk* World::getChunk(int chunkX, int chunkZ) const {
    uint64_t key = chunkKey(chunkX, chunkZ);
    auto it = m_chunkMap.find(key);
    if (it != m_chunkMap.end()) {
        return m_chunks[it->second].get();
    }
    return nullptr;
}

} // namespace clonemine
