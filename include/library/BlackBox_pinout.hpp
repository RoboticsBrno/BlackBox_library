/**
 * @file BlackBox_pinout.hpp
 * @author Tomáš Rohlínek (haberturdeur)
 * \~czech @brief Soubor řídící více verzí pinoutu desky BlackBox 
 * \~english @brief File managing different versions of pinout
 */

#pragma once

#if (BB_HW_VER == 0x0100)
#include "library/pinout/v1_0.hpp"
#elif (BB_HW_VER == 0x0101)
#include "library/pinout/v1_1.hpp"
#else
#error "Invalid BB_HW_VER"
#endif