#pragma once

#include "paper2_scotland2/shared/logger.hpp"
#include "config-utils/shared/config-utils.hpp"

// Static Variables
inline modloader::ModInfo modInfo = {MOD_ID, VERSION, 0}; // Stores the ID and version of our mod, and is sent to the modloader upon startup

Paper::ConstLoggerContext<17UL> getLogger();