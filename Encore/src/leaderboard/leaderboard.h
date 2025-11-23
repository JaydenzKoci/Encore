#pragma once

#include <string>
#include <filesystem>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

struct ScoreData {
    int score = 0;
    int stars = 0;
    int difficulty = 0;
    int instrument = 0;
    int perfectHits = 0;
    int goodHits = 0;
    int misses = 0;
    bool hasScore = false;
};

class LeaderboardManager {
public:
    static std::string GenerateSongID(const std::string& title, const std::string& artist);
    static void SaveScore(const std::string& playerUUID, const std::string& songID, 
                         int score, int stars, int difficulty, int instrument,
                         int perfectHits, int goodHits, int misses);
    static ScoreData GetHighestScore(const std::string& playerUUID, const std::string& songID);
    static std::filesystem::path GetLeaderboardPath();
private:
    static std::string SanitizeForID(const std::string& input);
};
