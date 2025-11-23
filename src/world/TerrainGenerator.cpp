#include "TerrainGenerator.h"
#include <cmath>
#include <algorithm>

namespace clonemine {

TerrainGenerator::TerrainGenerator(uint32_t seed) : m_seed(seed) {
    // Initialize permutation table for Perlin noise
    m_permutation.resize(512);
    for (int i = 0; i < 256; i++) {
        m_permutation[i] = i;
    }
    
    // Shuffle using seed
    std::mt19937 rng(seed);
    std::shuffle(m_permutation.begin(), m_permutation.begin() + 256, rng);
    
    // Duplicate for overflow
    for (int i = 0; i < 256; i++) {
        m_permutation[256 + i] = m_permutation[i];
    }
}

float TerrainGenerator::getHeight(float x, float z) const {
    float continental = continentalNoise(x, z);
    float mountain = mountainNoise(x, z);
    float hill = hillNoise(x, z);
    float detail = detailNoise(x, z);
    
    // Combine layers with weights
    float height = SEA_LEVEL;
    height += continental * 30.0f;  // Continental variation
    height += mountain * 100.0f;     // Mountain peaks
    height += hill * 20.0f;          // Hills and valleys
    height += detail * 5.0f;         // Fine detail
    
    return std::clamp(height, 0.0f, MAX_HEIGHT);
}

Biome TerrainGenerator::getBiome(float x, float z) const {
    float temperature = perlin(x, z, 0.0005f);
    float moisture = perlin(x + 1000.0f, z + 1000.0f, 0.0005f);
    float height = getHeight(x, z);
    
    // Underground biome
    if (height < SEA_LEVEL) {
        return Biome::UNDERGROUND;
    }
    
    // Volcanic in hot, high areas
    if (temperature > 0.7f && height > 150.0f) {
        return Biome::VOLCANIC;
    }
    
    // Mountain biome for high elevations
    if (height > 120.0f) {
        return Biome::MOUNTAINS;
    }
    
    // Swamp in wet, low areas
    if (moisture > 0.6f && height < 80.0f) {
        return Biome::SWAMP;
    }
    
    // Desert in hot, dry areas
    if (temperature > 0.5f && moisture < 0.3f) {
        return Biome::DESERT;
    }
    
    // Forest in moderate wet areas
    if (moisture > 0.4f) {
        return Biome::FOREST;
    }
    
    // Default to plains
    return Biome::PLAINS;
}

uint8_t TerrainGenerator::getBlockType(int x, int y, int z) const {
    float height = getHeight(static_cast<float>(x), static_cast<float>(z));
    Biome biome = getBiome(static_cast<float>(x), static_cast<float>(z));
    
    // Air above surface
    if (y > height) {
        return 0; // AIR
    }
    
    // Bedrock at bottom
    if (y == 0) {
        return 7; // BEDROCK
    }
    
    // Surface layer based on biome
    if (y == static_cast<int>(height)) {
        switch (biome) {
            case Biome::DESERT:
                return 12; // SAND
            case Biome::FOREST:
            case Biome::PLAINS:
                return 2; // GRASS
            case Biome::MOUNTAINS:
                return y > 140 ? 16 : 1; // SNOW or STONE
            case Biome::SWAMP:
                return 3; // DIRT
            case Biome::VOLCANIC:
                return 10; // OBSIDIAN
            default:
                return 2; // GRASS
        }
    }
    
    // Subsurface layers
    if (y > height - 4) {
        return 3; // DIRT
    }
    
    // Stone layer with ore generation
    float oreChance = perlin(static_cast<float>(x), static_cast<float>(y * 10), 0.1f);
    if (oreChance > 0.95f && y < 32) {
        return 14; // DIAMOND_ORE
    } else if (oreChance > 0.90f && y < 64) {
        return 15; // GOLD_ORE
    } else if (oreChance > 0.85f) {
        return 13; // IRON_ORE
    }
    
    return 1; // STONE
}

bool TerrainGenerator::shouldSpawnDungeon(int x, int z) const {
    // Use structured grid with randomization
    int gridX = x / 500;
    int gridZ = z / 500;
    
    std::mt19937 rng(m_seed + gridX * 73856093 + gridZ * 19349663);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    float offsetX = dist(rng) * 500.0f;
    float offsetZ = dist(rng) * 500.0f;
    
    int dungeonX = gridX * 500 + static_cast<int>(offsetX);
    int dungeonZ = gridZ * 500 + static_cast<int>(offsetZ);
    
    // Check if close to dungeon location
    return (x == dungeonX && z == dungeonZ);
}

bool TerrainGenerator::shouldSpawnTown(int x, int z) const {
    // Towns spawn less frequently
    int gridX = x / 2000;
    int gridZ = z / 2000;
    
    std::mt19937 rng(m_seed + gridX * 73856093 + gridZ * 19349663);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // Only spawn if on relatively flat terrain
    float height = getHeight(static_cast<float>(x), static_cast<float>(z));
    if (height < 60.0f || height > 100.0f) {
        return false;
    }
    
    float offsetX = dist(rng) * 2000.0f;
    float offsetZ = dist(rng) * 2000.0f;
    
    int townX = gridX * 2000 + static_cast<int>(offsetX);
    int townZ = gridZ * 2000 + static_cast<int>(offsetZ);
    
    return (x == townX && z == townZ);
}

// Private noise functions
float TerrainGenerator::continentalNoise(float x, float z) const {
    return perlin(x, z, CONTINENTAL_SCALE);
}

float TerrainGenerator::mountainNoise(float x, float z) const {
    float noise = perlin(x, z, MOUNTAIN_SCALE);
    // Amplify peaks
    return noise * noise * (noise > 0.0f ? 1.0f : -1.0f);
}

float TerrainGenerator::hillNoise(float x, float z) const {
    return perlin(x, z, HILL_SCALE);
}

float TerrainGenerator::detailNoise(float x, float z) const {
    return perlin(x, z, DETAIL_SCALE);
}

float TerrainGenerator::perlin(float x, float y, float scale) const {
    x *= scale;
    y *= scale;
    
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    
    x -= std::floor(x);
    y -= std::floor(y);
    
    float u = fade(x);
    float v = fade(y);
    
    int A = m_permutation[X] + Y;
    int AA = m_permutation[A];
    int AB = m_permutation[A + 1];
    int B = m_permutation[X + 1] + Y;
    int BA = m_permutation[B];
    int BB = m_permutation[B + 1];
    
    float result = lerp(v,
        lerp(u, grad(m_permutation[AA], x, y),
                grad(m_permutation[BA], x - 1, y)),
        lerp(u, grad(m_permutation[AB], x, y - 1),
                grad(m_permutation[BB], x - 1, y - 1)));
    
    return result;
}

float TerrainGenerator::fade(float t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float TerrainGenerator::lerp(float t, float a, float b) const {
    return a + t * (b - a);
}

float TerrainGenerator::grad(int hash, float x, float y) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

} // namespace clonemine
