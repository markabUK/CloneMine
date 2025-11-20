#pragma once

#include "world/Block.h"
#include <array>
#include <memory>

namespace clonemine {

class Chunk {
public:
    static constexpr int SIZE = 16;
    static constexpr int HEIGHT = 256;

    Chunk(int x, int z);
    ~Chunk() = default;

    // Delete copy operations
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
    
    // Allow move operations
    Chunk(Chunk&&) noexcept = default;
    Chunk& operator=(Chunk&&) noexcept = default;

    void setBlock(int x, int y, int z, BlockType type);
    [[nodiscard]] Block getBlock(int x, int y, int z) const;
    
    [[nodiscard]] int getX() const noexcept { return m_x; }
    [[nodiscard]] int getZ() const noexcept { return m_z; }
    
    void generate();

private:
    [[nodiscard]] size_t getIndex(int x, int y, int z) const noexcept {
        return static_cast<size_t>(x + y * SIZE + z * SIZE * HEIGHT);
    }

    int m_x;
    int m_z;
    std::unique_ptr<std::array<Block, SIZE * HEIGHT * SIZE>> m_blocks;
};

} // namespace clonemine
