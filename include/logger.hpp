#pragma once

#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "scotland2/shared/loader.hpp"
#include "paper/shared/logger.hpp"

// Static Variables
inline modloader::ModInfo modInfo = {MOD_ID, VERSION, 0}; // Stores the ID and version of our mod, and is sent to the modloader upon startup

Paper::ConstLoggerContext<17UL> getLogger();