#pragma once

#include <gear/Race/RaceInfo.hpp>
#include <gear/Player/PlayerManager.hpp>

extern "C"
{
    [[gnu::noinline]]
    bool can_enable_user_interface()
    {
        gear::RaceInfo* raceInfo = gear::GetRaceInfo();
        gear::PlayerManager* playerManager = gear::GetPlayerManager();

        return raceInfo->mRaceRule != gear::ERaceRule::Battle 
            && raceInfo->mRaceRule != gear::ERaceRule::TheaterMenu
            && playerManager->mLocalPlayerAmount == 1;
    }
}