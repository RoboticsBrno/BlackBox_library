#pragma once

#if BB_HW_VER == 0x0100
#include "pinout/v1_0.hpp"
#else
#error "Invalid BB_HW_VER"
#endif