#ifndef QUEST_SERVICE_H
#define QUEST_SERVICE_H

#include <memory>
#include <sstream>
#include "../Interfaces/IQuestService.h"
#include "../Interfaces/IQuestRepository.h"
#include "../Interfaces/IProgressRepository.h"
#include "../Interfaces/IInputValidator.h"

namespace CloneMine {
namespace Quest {

class QuestService : public IQuestService {
private:
    std::shared_ptr<IQuestRepository> questRepo;
    std::shared_ptr<IProgressRepository> progressRepo;
    std::shared_ptr<IInputValidator> validator;
    
public:
    QuestService(std::shared_ptr<IQuestRepository> questRepo,
                std::shared_ptr<IProgressRepository> progressRepo,
                std::shared_ptr<IInputValidator> validator)
        : questRepo(questRepo), progressRepo(progressRepo), validator(validator) {}
    
    std::string ListQuests(const std::string& typeFilter) override {
        std::vector<Quest> quests;
        
        if (typeFilter == "ALL") {
            quests = questRepo->GetAllQuests();
        } else {
            QuestType type = StringToQuestType(typeFilter);
            quests = questRepo->GetQuestsByType(type);
        }
        
        std::ostringstream oss;
        oss << "QUESTS " << quests.size();
        for (const auto& quest : quests) {
            oss << "\n" << quest.ToString();
        }
        return oss.str();
    }
    
    std::string AcceptQuest(const std::string& playerId, const std::string& questId) override {
        auto [validPlayer, playerError] = validator->ValidatePlayerId(playerId);
        if (!validPlayer) return "ERROR " + playerError;
        
        auto [validQuest, questError] = validator->ValidateQuestId(questId);
        if (!validQuest) return "ERROR " + questError;
        
        auto quest = questRepo->GetQuest(questId);
        if (!quest) return "ERROR Quest not found";
        
        auto existing = progressRepo->GetProgress(playerId, questId);
        if (existing) return "ERROR Quest already accepted";
        
        PlayerQuestProgress progress(playerId, questId, 0.0, "IN_PROGRESS");
        progressRepo->SaveProgress(progress);
        
        return "SUCCESS Quest accepted";
    }
    
    std::string UpdateProgress(const std::string& playerId, const std::string& questId, double progress) override {
        auto [validPlayer, playerError] = validator->ValidatePlayerId(playerId);
        if (!validPlayer) return "ERROR " + playerError;
        
        auto [validQuest, questError] = validator->ValidateQuestId(questId);
        if (!validQuest) return "ERROR " + questError;
        
        auto [validProgress, progressError] = validator->ValidateProgress(progress);
        if (!validProgress) return "ERROR " + progressError;
        
        auto existing = progressRepo->GetProgress(playerId, questId);
        if (!existing) return "ERROR Quest not accepted";
        
        if (existing->status == "COMPLETED") return "ERROR Quest already completed";
        
        PlayerQuestProgress updated(playerId, questId, progress, "IN_PROGRESS");
        progressRepo->SaveProgress(updated);
        
        return "SUCCESS Quest progress updated to " + std::to_string(progress) + "%";
    }
    
    std::string CompleteQuest(const std::string& playerId, const std::string& questId) override {
        auto [validPlayer, playerError] = validator->ValidatePlayerId(playerId);
        if (!validPlayer) return "ERROR " + playerError;
        
        auto [validQuest, questError] = validator->ValidateQuestId(questId);
        if (!validQuest) return "ERROR " + questError;
        
        auto quest = questRepo->GetQuest(questId);
        if (!quest) return "ERROR Quest not found";
        
        auto existing = progressRepo->GetProgress(playerId, questId);
        if (!existing) return "ERROR Quest not accepted";
        
        if (existing->status == "COMPLETED") return "ERROR Quest already completed";
        
        PlayerQuestProgress completed(playerId, questId, 100.0, "COMPLETED");
        progressRepo->SaveProgress(completed);
        
        return "SUCCESS Quest completed! Reward: " + std::to_string(quest->reward);
    }
    
    std::string GetPlayerProgress(const std::string& playerId) override {
        auto [valid, error] = validator->ValidatePlayerId(playerId);
        if (!valid) return "ERROR " + error;
        
        auto progressList = progressRepo->GetPlayerProgress(playerId);
        
        std::ostringstream oss;
        oss << "PROGRESS " << progressList.size();
        for (const auto& prog : progressList) {
            oss << "\n" << prog.ToString();
        }
        return oss.str();
    }
};

} // namespace Quest
} // namespace CloneMine

#endif
