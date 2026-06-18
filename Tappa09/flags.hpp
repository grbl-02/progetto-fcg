#pragma once
#include "config.hpp"

struct Flags {
    bool chest_3_opened = false;
    bool chest_5_opened = false;
    bool chest_6_opened = false;
    bool room_3_gauntlet_triggered = false;
    bool room_3_gauntlet_cleared = false;
    bool room_5_gauntlet_triggered = false;
    bool room_5_gauntlet_cleared = false;
    bool room_6_gauntlet_triggered = false;
    bool room_6_gauntlet_cleared = false;
    
    bool chest_3_fell = false;
    bool chest_5_fell = false;
    bool chest_6_fell = false;
};