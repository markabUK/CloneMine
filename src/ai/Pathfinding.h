#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <glm/glm.hpp>
#include <functional>

namespace clonemine {

struct PathNode {
    glm::ivec3 position;
    float gCost;  // Cost from start
    float hCost;  // Heuristic cost to end
    float fCost() const { return gCost + hCost; }
    PathNode* parent = nullptr;
    
    PathNode(const glm::ivec3& pos) : position(pos), gCost(0.0f), hCost(0.0f) {}
};

struct PathNodeHash {
    std::size_t operator()(const glm::ivec3& v) const {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1) ^ (std::hash<int>()(v.z) << 2);
    }
};

class Pathfinding {
public:
    using CostFunction = std::function<float(const glm::ivec3&, const glm::ivec3&)>;
    using TraversableFunction = std::function<bool(const glm::ivec3&)>;
    
    struct PathResult {
        std::vector<glm::ivec3> path;
        bool success = false;
        float totalCost = 0.0f;
    };
    
    static PathResult findPath(const glm::ivec3& start, const glm::ivec3& goal,
                              TraversableFunction isTraversable,
                              CostFunction getCost,
                              int maxIterations = 1000) {
        PathResult result;
        
        if (start == goal) {
            result.path.push_back(start);
            result.success = true;
            return result;
        }
        
        auto comp = [](PathNode* a, PathNode* b) { return a->fCost() > b->fCost(); };
        std::priority_queue<PathNode*, std::vector<PathNode*>, decltype(comp)> openSet(comp);
        std::unordered_map<glm::ivec3, PathNode*, PathNodeHash> allNodes;
        std::unordered_map<glm::ivec3, bool, PathNodeHash> closedSet;
        
        PathNode* startNode = new PathNode(start);
        startNode->hCost = heuristic(start, goal);
        openSet.push(startNode);
        allNodes[start] = startNode;
        
        int iterations = 0;
        while (!openSet.empty() && iterations < maxIterations) {
            iterations++;
            
            PathNode* current = openSet.top();
            openSet.pop();
            
            if (current->position == goal) {
                // Reconstruct path
                result.success = true;
                result.totalCost = current->gCost;
                PathNode* node = current;
                while (node != nullptr) {
                    result.path.push_back(node->position);
                    node = node->parent;
                }
                std::reverse(result.path.begin(), result.path.end());
                
                // Cleanup
                for (auto& pair : allNodes) {
                    delete pair.second;
                }
                return result;
            }
            
            closedSet[current->position] = true;
            
            // Check neighbors (6 directions + up/down)
            std::vector<glm::ivec3> neighbors = getNeighbors(current->position);
            
            for (const auto& neighborPos : neighbors) {
                if (closedSet.count(neighborPos) > 0) continue;
                if (!isTraversable(neighborPos)) continue;
                
                float tentativeGCost = current->gCost + getCost(current->position, neighborPos);
                
                PathNode* neighbor = nullptr;
                if (allNodes.count(neighborPos) == 0) {
                    neighbor = new PathNode(neighborPos);
                    neighbor->hCost = heuristic(neighborPos, goal);
                    allNodes[neighborPos] = neighbor;
                } else {
                    neighbor = allNodes[neighborPos];
                }
                
                if (tentativeGCost < neighbor->gCost || neighbor->parent == nullptr) {
                    neighbor->gCost = tentativeGCost;
                    neighbor->parent = current;
                    openSet.push(neighbor);
                }
            }
        }
        
        // Cleanup on failure
        for (auto& pair : allNodes) {
            delete pair.second;
        }
        
        result.success = false;
        return result;
    }
    
private:
    static float heuristic(const glm::ivec3& a, const glm::ivec3& b) {
        // Manhattan distance in 3D
        return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
    }
    
    static std::vector<glm::ivec3> getNeighbors(const glm::ivec3& pos) {
        return {
            {pos.x + 1, pos.y, pos.z},
            {pos.x - 1, pos.y, pos.z},
            {pos.x, pos.y, pos.z + 1},
            {pos.x, pos.y, pos.z - 1},
            {pos.x, pos.y + 1, pos.z},  // Up
            {pos.x, pos.y - 1, pos.z},  // Down
            {pos.x, pos.y + 2, pos.z},  // Jump (2 blocks up)
        };
    }
};

} // namespace clonemine
