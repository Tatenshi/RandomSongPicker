#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(UseButton, int, "Assigned Random Button", 0);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(UseButton)
    )
)