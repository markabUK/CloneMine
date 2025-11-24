#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

namespace clonemine {

enum class BlockType : uint8_t {
    AIR = 0,
    STONE,
    DIRT,
    GRASS,
    WOOD,
    LEAVES,
    SAND,
    WATER,
    GLASS,
    COBBLESTONE,
    BRICK,
    OBSIDIAN,
    GOLD_ORE,
    DIAMOND_ORE,
    EMERALD_ORE,
    ICE,
    LAVA
};

struct BlockVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    float lightLevel;
};

struct ChunkMesh {
    std::vector<BlockVertex> vertices;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory indexMemory = VK_NULL_HANDLE;
    bool needsRebuild = true;
};

class BlockRenderer {
public:
    BlockRenderer(VkDevice device, VkPhysicalDevice physicalDevice);
    ~BlockRenderer();

    // Build mesh for a chunk (16x256x16 blocks)
    void buildChunkMesh(const glm::ivec3& chunkPos, const std::vector<BlockType>& blocks, ChunkMesh& mesh);
    
    // Render all visible chunks
    void render(VkCommandBuffer cmd, const glm::mat4& viewProj, const glm::vec3& cameraPos);
    
    // Update a chunk's mesh
    void updateChunk(const glm::ivec3& chunkPos, const std::vector<BlockType>& blocks);
    
    // Face culling: only render visible faces
    bool shouldRenderFace(const std::vector<BlockType>& blocks, int x, int y, int z, int face);
    
    // Greedy meshing optimization
    void greedyMesh(const std::vector<BlockType>& blocks, ChunkMesh& mesh);
    
    // Get texture coordinates for block type
    glm::vec2 getTextureCoords(BlockType type, int face);
    
    // Check if block is transparent
    static bool isTransparent(BlockType type);
    
    // Check if block is solid
    static bool isSolid(BlockType type);

private:
    void createVertexBuffer(ChunkMesh& mesh);
    void createIndexBuffer(ChunkMesh& mesh);
    void destroyChunkMesh(ChunkMesh& mesh);
    
    // Add a face to the mesh
    void addFace(ChunkMesh& mesh, const glm::vec3& pos, int face, BlockType type, float light);
    
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    std::unordered_map<glm::ivec3, ChunkMesh> m_chunkMeshes;
    
    // Atlas: 16x16 block textures in 256x256 atlas
    static constexpr int ATLAS_SIZE = 256;
    static constexpr int BLOCK_TEX_SIZE = 16;
    static constexpr int BLOCKS_PER_ROW = ATLAS_SIZE / BLOCK_TEX_SIZE;
};

} // namespace clonemine
