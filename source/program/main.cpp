#include <stdio.h>
#include <format>

#include "lib.hpp"
#include "ctgp/ui/racetimer.hpp"
#include "ctgp/ui/speedometer.hpp"
#include "base/pointers.hpp"

HOOK_DEFINE_TRAMPOLINE(LoggerPoC)
{
    static void Callback()
    {
        Logging.Log("turbo-exlaunch! :^)");

        Orig();
    }
};

namespace nn::oe
{
    void Initialize();
};

extern "C" void exl_main(void *x0, void *x1)
{
    exl::hook::Initialize();

    LoggerPoC::InstallAtFuncPtr(nn::oe::Initialize);

    Logging.Log("Patching!");

    // The following is required to run the byte pattern searcher.
    base::Pointers::create();

    ctgp::ui::RaceTimer::initialize();
    ctgp::ui::Speedometer::initialize();
}