#include "logger.hpp"

// Returns a logger, useful for printing debug messages
Paper::ConstLoggerContext<17UL> getLogger() {
    static auto fastContext = Paper::Logger::WithContext<"RandomSongPicker">();
    return fastContext;
}