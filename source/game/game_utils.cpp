#include <gear/Framework/FrameworkUtil.hpp>
#include <object/Kart/KartVehicle.hpp>

#include "game/game_utils.hpp"



namespace Game::Utils
{
    object::KartVehicleMove* GetMyKartVehicle(int32_t playerIndex)
    {
        gear::FrameworkGameScene* currentGamescene = gear::FrameworkUtil::getCurrentGameScene();
        return currentGamescene->mObjectEngine->mKartDirector->mKartUnitHolders[playerIndex]->mKartVehicle->mKartVehicleMove;
    }

    int32_t GetMainPlayerIndex()
    {
        int32_t index = gear::NetworkUtil::getMyKartIndex();

        // Sometimes "getMyKartIndex" returns -1.
        // This is to account for that.
        if (index < 0) return 0;
        return index;
    }
}