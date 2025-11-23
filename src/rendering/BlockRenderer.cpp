#include "BlockRenderer.h"
#include <cstring>

namespace clonemine {

BlockRenderer::BlockRenderer(VkDevice device, VkPhysicalDevice physicalDevice)
    : m_device(device), m_physicalDevice(physicalDevice) {
}

BlockRenderer::~BlockRenderer() {
    for (auto& [pos, mesh] : m_chunkMeshes) {
        destroyChunkMesh(mesh);
    }
}

void BlockRenderer::buildChunkMesh(const glm::ivec3& chunkPos, const std::vector<BlockType>& blocks, ChunkMesh& mesh) {
    mesh.vertices.clear();
    mesh.indices.clear();
    
    constexpr int CHUNK_SIZE = 16;
    constexpr int CHUNK_HEIGHT = 256;
    
    // Iterate through all blocks in chunk
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_HEIGHT; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int idx = x + z * CHUNK_SIZE + y * CHUNK_SIZE * CHUNK_SIZE;
                BlockType type = blocks[idx];
                
                if (type == BlockType::AIR) continue;
                
                glm::vec3 blockPos(
                    chunkPos.x * CHUNK_SIZE + x,
                    y,
                    chunkPos.z * CHUNK_SIZE + z
                );
                
                // Check each face (0=front, 1=back, 2=left, 3=right, 4=top, 5=bottom)
                for (int face = 0; face < 6; ++face) {
                    if (shouldRenderFace(blocks, x, y, z, face)) {
                        // Simple lighting based on face direction and height
                        float light = (face == 4) ? 1.0f : 0.7f + (y / 256.0f) * 0.3f;
                        addFace(mesh, blockPos, face, type, light);
                    }
                }
            }
        }
    }
    
    // Create Vulkan buffers
    if (!mesh.vertices.empty()) {
        createVertexBuffer(mesh);
        createIndexBuffer(mesh);
    }
    
    mesh.needsRebuild = false;
}

bool BlockRenderer::shouldRenderFace(const std::vector<BlockType>& blocks, int x, int y, int z, int face) {
    constexpr int CHUNK_SIZE = 16;
    constexpr int CHUNK_HEIGHT = 256;
    
    int nx = x, ny = y, nz = z;
    
    // Calculate neighbor position based on face
    switch (face) {
        case 0: ++nz; break; // front
        case 1: --nz; break; // back
        case 2: --nx; break; // left
        case 3: ++nx; break; // right
        case 4: ++ny; break; // top
        case 5: --ny; break; // bottom
    }
    
    // Check bounds
    if (nx < 0 || nx >= CHUNK_SIZE || 
        ny < 0 || ny >= CHUNK_HEIGHT || 
        nz < 0 || nz >= CHUNK_SIZE) {
        return true; // Render faces at chunk boundaries
    }
    
    int neighborIdx = nx + nz * CHUNK_SIZE + ny * CHUNK_SIZE * CHUNK_SIZE;
    BlockType neighbor = blocks[neighborIdx];
    
    // Don't render face if neighbor is opaque
    return !isSolid(neighbor) || isTransparent(neighbor);
}

void BlockRenderer::addFace(ChunkMesh& mesh, const glm::vec3& pos, int face, BlockType type, float light) {
    uint32_t baseIdx = static_cast<uint32_t>(mesh.vertices.size());
    
    // Face vertices (positions relative to block)
    glm::vec3 verts[4];
    glm::vec3 normal;
    
    switch (face) {
        case 0: // Front (+Z)
            verts[0] = {0, 0, 1}; verts[1] = {1, 0, 1};
            verts[2] = {1, 1, 1}; verts[3] = {0, 1, 1};
            normal = {0, 0, 1};
            break;
        case 1: // Back (-Z)
            verts[0] = {1, 0, 0}; verts[1] = {0, 0, 0};
            verts[2] = {0, 1, 0}; verts[3] = {1, 1, 0};
            normal = {0, 0, -1};
            break;
        case 2: // Left (-X)
            verts[0] = {0, 0, 0}; verts[1] = {0, 0, 1};
            verts[2] = {0, 1, 1}; verts[3] = {0, 1, 0};
            normal = {-1, 0, 0};
            break;
        case 3: // Right (+X)
            verts[0] = {1, 0, 1}; verts[1] = {1, 0, 0};
            verts[2] = {1, 1, 0}; verts[3] = {1, 1, 1};
            normal = {1, 0, 0};
            break;
        case 4: // Top (+Y)
            verts[0] = {0, 1, 1}; verts[1] = {1, 1, 1};
            verts[2] = {1, 1, 0}; verts[3] = {0, 1, 0};
            normal = {0, 1, 0};
            break;
        case 5: // Bottom (-Y)
            verts[0] = {0, 0, 0}; verts[1] = {1, 0, 0};
            verts[2] = {1, 0, 1}; verts[3] = {0, 0, 1};
            normal = {0, -1, 0};
            break;
    }
    
    glm::vec2 texCoords = getTextureCoords(type, face);
    float texSize = 1.0f / BLOCKS_PER_ROW;
    
    // Add 4 vertices for the face
    for (int i = 0; i < 4; ++i) {
        BlockVertex vertex;
        vertex.position = pos + verts[i];
        vertex.normal = normal;
        vertex.lightLevel = light;
        
        // Texture coordinates
        float u = texCoords.x + ((i == 1 || i == 2) ? texSize : 0.0f);
        float v = texCoords.y + ((i >= 2) ? texSize : 0.0f);
        vertex.texCoord = {u, v};
        
        mesh.vertices.push_back(vertex);
    }
    
    // Add indices for two triangles
    mesh.indices.push_back(baseIdx);
    mesh.indices.push_back(baseIdx + 1);
    mesh.indices.push_back(baseIdx + 2);
    
    mesh.indices.push_back(baseIdx);
    mesh.indices.push_back(baseIdx + 2);
    mesh.indices.push_back(baseIdx + 3);
}

glm::vec2 BlockRenderer::getTextureCoords(BlockType type, int face) {
    // Map block types to texture atlas positions
    int texX = 0, texY = 0;
    
    switch (type) {
        case BlockType::STONE:
            texX = 0; texY = 0;
            break;
        case BlockType::DIRT:
            texX = 1; texY = 0;
            break;
        case BlockType::GRASS:
            // Different textures for top, sides, bottom
            if (face == 4) { texX = 2; texY = 0; } // Top
            else if (face == 5) { texX = 1; texY = 0; } // Bottom (dirt)
            else { texX = 3; texY = 0; } // Sides
            break;
        case BlockType::WOOD:
            texX = 4; texY = 0;
            break;
        case BlockType::LEAVES:
            texX = 5; texY = 0;
            break;
        case BlockType::SAND:
            texX = 6; texY = 0;
            break;
        case BlockType::WATER:
            texX = 7; texY = 0;
            break;
        case BlockType::GLASS:
            texX = 8; texY = 0;
            break;
        default:
            texX = 0; texY = 0;
            break;
    }
    
    float texSize = 1.0f / BLOCKS_PER_ROW;
    return {texX * texSize, texY * texSize};
}

bool BlockRenderer::isTransparent(BlockType type) {
    return type == BlockType::AIR || 
           type == BlockType::WATER || 
           type == BlockType::GLASS ||
           type == BlockType::LEAVES;
}

bool BlockRenderer::isSolid(BlockType type) {
    return type != BlockType::AIR && type != BlockType::WATER;
}

void BlockRenderer::createVertexBuffer(ChunkMesh& mesh) {
    // Placeholder - actual Vulkan buffer creation would go here
    (void)mesh;
}

void BlockRenderer::createIndexBuffer(ChunkMesh& mesh) {
    // Placeholder - actual Vulkan buffer creation would go here
    (void)mesh;
}

void BlockRenderer::destroyChunkMesh(ChunkMesh& mesh) {
    if (mesh.vertexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device, mesh.vertexBuffer, nullptr);
        mesh.vertexBuffer = VK_NULL_HANDLE;
    }
    if (mesh.indexBuffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(m_device, mesh.indexBuffer, nullptr);
        mesh.indexBuffer = VK_NULL_HANDLE;
    }
    if (mesh.vertexMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, mesh.vertexMemory, nullptr);
        mesh.vertexMemory = VK_NULL_HANDLE;
    }
    if (mesh.indexMemory != VK_NULL_HANDLE) {
        vkFreeMemory(m_device, mesh.indexMemory, nullptr);
        mesh.indexMemory = VK_NULL_HANDLE;
    }
}

void BlockRenderer::updateChunk(const glm::ivec3& chunkPos, const std::vector<BlockType>& blocks) {
    auto it = m_chunkMeshes.find(chunkPos);
    if (it == m_chunkMeshes.end()) {
        ChunkMesh& mesh = m_chunkMeshes[chunkPos];
        buildChunkMesh(chunkPos, blocks, mesh);
    } else {
        destroyChunkMesh(it->second);
        buildChunkMesh(chunkPos, blocks, it->second);
    }
}

void BlockRenderer::render(VkCommandBuffer cmd, const glm::mat4& viewProj, const glm::vec3& cameraPos) {
    // Render all chunks within view distance
    (void)cmd;
    (void)viewProj;
    (void)cameraPos;
    
    // Actual rendering code would bind pipeline, descriptor sets, and draw each chunk
}

void BlockRenderer::greedyMesh(const std::vector<BlockType>& blocks, ChunkMesh& mesh) {
    // Greedy meshing optimization - combines adjacent same-type faces
    // Implementation would scan for runs of identical faces and merge them
    (void)blocks;
    (void)mesh;
}

} // namespace clonemine
