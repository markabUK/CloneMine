#include "SaveSystem.h"
#include <fstream>
#include <zlib.h>

namespace clonemine {

bool SaveSystem::save(int slot, const GameState& state) {
    std::vector<uint8_t> data;
    
    // Serialize all game state
    serializePlayer(state.player, data);
    serializeInventory(state.player.inventory, data);
    serializeEquipment(state.player.equipment, data);
    serializeQuests(state.quests, data);
    
    for (const auto& chunk : state.world.modifiedChunks) {
        serializeChunk(chunk, data);
    }
    
    for (const auto& chest : state.world.chests) {
        serializeChest(chest, data);
    }
    
    for (const auto& npc : state.world.npcs) {
        serializeNPC(npc, data);
    }
    
    // Compress
    std::vector<uint8_t> compressed = compressData(data);
    
    // Write to file
    std::string filename = getSaveFilePath(slot);
    return writeToFile(filename, compressed);
}

bool SaveSystem::load(int slot, GameState& state) {
    std::string filename = getSaveFilePath(slot);
    std::vector<uint8_t> compressed = readFromFile(filename);
    std::vector<uint8_t> data = decompressData(compressed);
    
    size_t offset = 0;
    deserializePlayer(data, offset, state.player);
    deserializeInventory(data, offset, state.player.inventory);
    deserializeEquipment(data, offset, state.player.equipment);
    deserializeQuests(data, offset, state.quests);
    deserializeChunks(data, offset, state.world);
    deserializeChests(data, offset, state.world.chests);
    deserializeNPCs(data, offset, state.world.npcs);
    
    return true;
}

std::string SaveSystem::getSaveFilePath(int slot) {
    return "saves/slot" + std::to_string(slot) + ".dat";
}

void SaveSystem::serializePlayer(const Player& player, std::vector<uint8_t>& data) {
    // Write player data to byte vector
}

void SaveSystem::serializeInventory(const Inventory& inv, std::vector<uint8_t>& data) {
    // Write inventory data
}

void SaveSystem::serializeEquipment(const Equipment& equip, std::vector<uint8_t>& data) {
    // Write equipment data
}

void SaveSystem::serializeQuests(const QuestList& quests, std::vector<uint8_t>& data) {
    // Write quest data
}

void SaveSystem::serializeChunk(const Chunk& chunk, std::vector<uint8_t>& data) {
    // Write chunk data
}

void SaveSystem::serializeChest(const Chest& chest, std::vector<uint8_t>& data) {
    // Write chest data
}

void SaveSystem::serializeNPC(const NPC& npc, std::vector<uint8_t>& data) {
    // Write NPC data
}

std::vector<uint8_t> SaveSystem::compressData(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> compressed(compressBound(data.size()));
    uLongf compressedSize = compressed.size();
    compress(compressed.data(), &compressedSize, data.data(), data.size());
    compressed.resize(compressedSize);
    return compressed;
}

std::vector<uint8_t> SaveSystem::decompressData(const std::vector<uint8_t>& compressed) {
    std::vector<uint8_t> data(compressed.size() * 10); // Estimate
    uLongf dataSize = data.size();
    uncompress(data.data(), &dataSize, compressed.data(), compressed.size());
    data.resize(dataSize);
    return data;
}

bool SaveSystem::writeToFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

std::vector<uint8_t> SaveSystem::readFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    size_t size = file.tellg();
    file.seekg(0);
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    return data;
}

} // namespace clonemine
