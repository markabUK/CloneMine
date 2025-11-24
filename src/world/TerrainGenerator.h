#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <memory>

namespace clonemine {

enum class Biome {
    FOREST,
    PLAINS,
    MOUNTAINS,
    SWAMP,
    DESERT,
    UNDERGROUND,
    VOLCANIC
};

class TerrainGenerator {
public:
    explicit TerrainGenerator(uint32_t seed);
    
    // Generate terrain height at world position
    float getHeight(float x, float z) const;
    
    // Get biome at position
    Biome getBiome(float x, float z) const;
    
    // Get block type at specific position
    uint8_t getBlockType(int x, int y, int z) const;
    
    // Check if structure should spawn at location
    bool shouldSpawnDungeon(int x, int z) const;
    bool shouldSpawnTown(int x, int z) const;
    
private:
    // Multi-layered noise functions
    float continentalNoise(float x, float z) const;
    float mountainNoise(float x, float z) const;
    float hillNoise(float x, float z) const;
    float detailNoise(float x, float z) const;
    
    // Perlin noise implementation
    float perlin(float x, float y, float scale) const;
    float fade(float t) const;
    float lerp(float t, float a, float b) const;
    float grad(int hash, float x, float y) const;
    
    uint32_t m_seed;
    std::vector<int> m_permutation;
    
    // Terrain parameters
    static constexpr float CONTINENTAL_SCALE = 0.0001f;
    static constexpr float MOUNTAIN_SCALE = 0.001f;
    static constexpr float HILL_SCALE = 0.01f;
    static constexpr float DETAIL_SCALE = 0.1f;
    
    static constexpr float SEA_LEVEL = 64.0f;
    static constexpr float MAX_HEIGHT = 256.0f;
};

} // namespace clonemine
