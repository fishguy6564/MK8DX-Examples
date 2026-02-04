#include <stdio.h>
#include <base/pointers.hpp>

#include "game/game_utils.hpp"
#include "ctgp/ui/ui_utils.hpp"
#include "ctgp/ui/racetimer.hpp"

extern "C"
{
    [[gnu::noinline]]
    bool can_enable_race_timer()
    {
        gear::RaceInfo* raceInfo = gear::GetRaceInfo();
        return can_enable_user_interface() || raceInfo->mRaceRule == gear::ERaceRule::TimeAttack;
    }
}

HOOK_DEFINE_INLINE(GetTimePlayerIndex)
{
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        ctx->W[9] = Game::Utils::GetMainPlayerIndex();
    }
};

HOOK_DEFINE_INLINE(EnableRaceTimer)
{
    static void Callback(exl::hook::nx64::InlineCtx* ctx)
    {
        if (can_enable_race_timer())
        {
            ctx->W[8] = 2;
        }
        else
        {
            ctx->W[8] = 0;
        }
    }
};

namespace ctgp::ui
{
    void RaceTimer::initialize()
    {
        uintptr_t addr;
        exl::patch::RandomAccessPatcher patcher;

        auto pointers = base::Pointers::getInstance();

        addr = pointers->get("_ZN2ui9Page_Race9onCreate_Ev", 0x3D0u);
        EnableRaceTimer::InstallAtOffset(addr);

        addr = pointers->get("_ZN2ui17Control_RaceTimer5onIn_Ev", 0x78u);
        patcher.Write(addr, 0xD503201F);
        GetTimePlayerIndex::InstallAtOffset(addr);
    }
}