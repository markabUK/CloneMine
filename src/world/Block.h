#pragma once

#include <cstdint>

namespace clonemine {

enum class BlockType : uint8_t {
    Air = 0,
    Grass,
    Dirt,
    Stone,
    Wood,
    Leaves,
    Sand,
    Water
};

struct Block {
    BlockType type{BlockType::Air};
    
    [[nodiscard]] bool isSolid() const noexcept {
        return type != BlockType::Air && type != BlockType::Water;
    }
    
    [[nodiscard]] bool isTransparent() const noexcept {
        return type == BlockType::Air || type == BlockType::Water || type == BlockType::Leaves;
    }
};

} // namespace clonemine
