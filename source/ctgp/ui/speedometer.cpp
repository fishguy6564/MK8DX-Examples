#include <stdio.h>
#include <base/pointers.hpp>
#include <prim/seadSafeString.hpp>

#include <gear/UI/UIUtil.hpp>
#include <gear/UI/UIControl.hpp>

#include "game/game_utils.hpp"
// #include "ctgp/ui/ui_utils.hpp"
#include "ctgp/ui/speedometer.hpp"

nn::ui2d::Pane* ctgp::ui::Speedometer::pSOMText[ctgp::ui::Speedometer::MaxPlayers] = {};

/*
 * Offset of the per-instance player index byte within Control_RaceLapCoin,
 * found via disassembly: onInit_ reads this byte to index a per-player
 * resource array right before the point where our hook installs.
 */
static constexpr ptrdiff_t PlayerIndexOffset = 0xD3;

void set_som_value(int32_t playerIndex)
{
    if (playerIndex < 0 || playerIndex >= ctgp::ui::Speedometer::MaxPlayers) return;

    nn::ui2d::Pane* pane = ctgp::ui::Speedometer::pSOMText[playerIndex];
    if (!pane) return;

    sead::WFixedSafeString<5> speedStr;

    object::KartVehicleMove* kart = Game::Utils::GetMyKartVehicle(playerIndex);

    int32_t speed = (int)(abs(kart->mSpeed * 10.0f));

    gear::UIUtil::getFullNumberStr(&speedStr, speed, 0);
    gear::UIControl::setMessage(pane, speedStr.cstr());
}

void prepare_som_pane(gear::UIControl* control, int32_t playerIndex)
{
    if (playerIndex < 0 || playerIndex >= ctgp::ui::Speedometer::MaxPlayers) return;
    ctgp::ui::Speedometer::pSOMText[playerIndex] = control->findPane("T_SOM_00");
}

HOOK_DEFINE_INLINE(SetSOMValue)
{
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        // At this hook point (Control_RaceLapCoin::onCalc_ + 0x10), X0 is "this".
        uint8_t playerIndex = *reinterpret_cast<uint8_t*>(ctx->X[0] + PlayerIndexOffset);
        set_som_value(playerIndex);
    }
};

HOOK_DEFINE_INLINE(PrepareSOMPane)
{
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        // At this hook point (Control_RaceLapCoin::onInit_ + 0xD0), X20 holds
        // "this" + 0x30 (the embedded gear::UIControl base subobject).
        gear::UIControl* control = reinterpret_cast<gear::UIControl*>(ctx->X[20]);
        uintptr_t self = ctx->X[20] - 0x30;
        uint8_t playerIndex = *reinterpret_cast<uint8_t*>(self + PlayerIndexOffset);
        prepare_som_pane(control, playerIndex);
    }
};

namespace ctgp::ui
{
    void Speedometer::initialize()
    {
        uintptr_t addr;
        auto pointers = base::Pointers::getInstance();

        addr = pointers->get("_ZN2ui19Control_RaceLapCoin7onInit_Ev", 0xD0u);
        PrepareSOMPane::InstallAtOffset(addr);

        addr = pointers->get("_ZN2ui19Control_RaceLapCoin7onCalc_Ev", 0x10u);
        SetSOMValue::InstallAtOffset(addr);
    }
}
