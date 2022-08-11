#include "logger.hpp"

ModInfo modInfo;

// Loads the config from disk using our modInfo, then returns it for use
Configuration &getConfig()
{
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}