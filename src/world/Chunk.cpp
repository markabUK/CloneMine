#include "world/Chunk.h"
#include <cmath>

namespace clonemine {

Chunk::Chunk(int x, int z)
    : m_x(x)
    , m_z(z)
    , m_blocks(std::make_unique<std::array<Block, SIZE * HEIGHT * SIZE>>())
{
    generate();
}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
    if (x >= 0 && x < SIZE && y >= 0 && y < HEIGHT && z >= 0 && z < SIZE) {
        (*m_blocks)[getIndex(x, y, z)].type = type;
    }
}

Block Chunk::getBlock(int x, int y, int z) const {
    if (x >= 0 && x < SIZE && y >= 0 && y < HEIGHT && z >= 0 && z < SIZE) {
        return (*m_blocks)[getIndex(x, y, z)];
    }
    return Block{BlockType::Air};
}

void Chunk::generate() {
    // Simple terrain generation
    for (int x = 0; x < SIZE; ++x) {
        for (int z = 0; z < SIZE; ++z) {
            // Simple height map using sine wave
            int worldX = m_x * SIZE + x;
            int worldZ = m_z * SIZE + z;
            int height = 64 + static_cast<int>(8 * std::sin(worldX * 0.1) + 8 * std::cos(worldZ * 0.1));
            
            for (int y = 0; y < HEIGHT; ++y) {
                if (y < height - 4) {
                    setBlock(x, y, z, BlockType::Stone);
                } else if (y < height - 1) {
                    setBlock(x, y, z, BlockType::Dirt);
                } else if (y < height) {
                    setBlock(x, y, z, BlockType::Grass);
                } else {
                    setBlock(x, y, z, BlockType::Air);
                }
            }
        }
    }
}

} // namespace clonemine
