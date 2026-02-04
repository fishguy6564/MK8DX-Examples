#pragma once

#include <cstdint>
#include <gear/Network/NetworkUtil.hpp>
#include <object/Kart/KartVehicleMove.hpp>

namespace Game::Utils
{
    int32_t GetMainPlayerIndex();
    object::KartVehicleMove* GetMyKartVehicle(int32_t playerIndex);
}