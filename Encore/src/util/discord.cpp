//
// Created by maria on 16/12/2024.
//

#include "discord.h"

#ifdef DISCORD_ENABLED
#include "discord-rpc/core.h"
#endif

#include <array>
#include <ctime>
#include <iostream>
#include <chrono>

#ifdef DISCORD_ENABLED
discord::Core* core{};
#endif

void Encore::Discord::Initialize() {
#ifdef DISCORD_ENABLED
    auto result = discord::Core::Create(1216298119457804379, DiscordCreateFlags_Default, &core);
    if (!core) {
        std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result)
                  << ")\n";
        Initialized = false;
        return;
    }
    const auto p0 = std::chrono::system_clock::now();
    startTime = std::chrono::duration_cast<std::chrono::seconds>(p0.time_since_epoch()).count();
    Initialized = true;
#else
    std::cout << "Discord SDK not available on this platform\n";
    Initialized = false;
#endif
}



Encore::Discord::~Discord() {
    if (!Initialized)
        return;
    Initialized = false;
}
void Encore::Discord::Update() {
#ifdef DISCORD_ENABLED
    if (core) {
        core->RunCallbacks();
    }
#endif
}

void Encore::Discord::DiscordUpdatePresence(const std::string &title, const std::string &details, int players) {
#ifdef DISCORD_ENABLED
    if (!Initialized || !core)
        return;
    discord::Activity activity{};
    if (players == 1) {
        activity.SetState("Playing solo");
    }
    else {
        activity.SetState("In a band");
    }
    activity.SetDetails((details).c_str());
    activity.SetName("encore");
    activity.GetAssets().SetLargeImage("encore");
    activity.SetType(discord::ActivityType::Playing);
    activity.GetTimestamps().SetStart(startTime);
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                  << " updating activity!\n";
    });
#endif
}

std::array<std::string, 11> AssetNames = {
    "pad_drums",    "pad_bass",       "pad_guitar",    "pad_keys",
    "pad_vocals",   "classic_drums",  "classic_bass",  "classic_guitar",
    "classic_keys", "classic_vocals", "classic_vocals"
};

std::array<std::string, 11> PartNames = {
    "Pad Drums",    "Pad Bass",       "Pad_guitar",    "Pad keys",
    "Pad vocals",   "Classic Drums",  "Classic Bass",  "Classic Guitar",
    "Classic Keys", "Classic Vocals", "Classic Vocals"
};

void Encore::Discord::DiscordUpdatePresenceSong(
    const std::string &title, const std::string &details, int instrument, int length,
    const std::string &songTitle, const std::string &songArtist
) {
#ifdef DISCORD_ENABLED
    if (!Initialized || !core)
        return;
    discord::Activity activity{};
    activity.SetDetails((details).c_str());
    if (length == 1) {
        activity.SetState("Playing solo");
    }
    else {
        activity.SetState("In a band");
    }

    activity.SetName("encore");
    activity.GetAssets().SetLargeImage("encore");
    
    activity.GetAssets().SetSmallImage(AssetNames[instrument].c_str());
    activity.GetAssets().SetSmallText(PartNames[instrument].c_str());
    activity.SetType(discord::ActivityType::Playing);
    activity.GetTimestamps().SetStart(startTime);
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                  << " updating activity!\n";
    });
#endif
}
