#include "logger.hpp"

// Returns a logger, useful for printing debug messages
Paper::ConstLoggerContext<17UL> getLogger() {
    static constexpr auto fastContext = Paper::ConstLoggerContext("RandomSongPicker");
    return fastContext;
}