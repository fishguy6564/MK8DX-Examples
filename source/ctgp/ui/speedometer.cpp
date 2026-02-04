#include <stdio.h>
#include <base/pointers.hpp>
#include <prim/seadSafeString.hpp>

#include <gear/UI/UIUtil.hpp>
#include <gear/UI/UIControl.hpp>

#include "game/game_utils.hpp"
// #include "ctgp/ui/ui_utils.hpp"
#include "ctgp/ui/speedometer.hpp"

nn::ui2d::Pane* ctgp::ui::Speedometer::pSOMText = nullptr;

void set_som_value()
{
    sead::WFixedSafeString<5> speedStr;

    s32 playerIndex = Game::Utils::GetMainPlayerIndex();
    object::KartVehicleMove* kart = Game::Utils::GetMyKartVehicle(playerIndex);

    int32_t speed = (int)(abs(kart->mSpeed * 10.0f));

    gear::UIUtil::getFullNumberStr(&speedStr, speed, 0);
    gear::UIControl::setMessage(ctgp::ui::Speedometer::pSOMText, speedStr.cstr());
}

void prepare_som_pane(gear::UIControl* control)
{
    ctgp::ui::Speedometer::pSOMText = control->findPane("T_SOM_00");
}

HOOK_DEFINE_INLINE(SetSOMValue)
{
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        set_som_value();
    }
};

HOOK_DEFINE_INLINE(PrepareSOMPane)
{
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        prepare_som_pane(reinterpret_cast<gear::UIControl*>(ctx->X[20]));
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