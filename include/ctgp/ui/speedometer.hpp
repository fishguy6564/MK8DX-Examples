#pragma once

#include <nn/ui2d/Layout.h>

namespace ctgp::ui
{
    class Speedometer
    {
    public:
        static constexpr int MaxPlayers = 4;
        static nn::ui2d::Pane* pSOMText[MaxPlayers];

        static void initialize();
    };
}