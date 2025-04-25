#include "Impl/RandomSongImpl.hpp"

namespace RandomSongImpl {

    SafePtrUnity<GlobalNamespace::LevelCollectionNavigationController> levelCollectionNavigationController;
    SafePtrUnity<GlobalNamespace::LevelFilteringNavigationController> filteringNavigationController;

    std::mt19937 generator(time(nullptr));
    
    void selectRandomSong()
    {
        // We cant do anything, when songs arent loaded yet
        // if(!RuntimeSongLoader::API::HasLoadedSongs())
        // {
        //     return;
        // }

        // Only do Things, if we also have the necessary References
        if (levelCollectionNavigationController && filteringNavigationController)
        {
            // Get Array of all beatmaps that the user currently sees
            ArrayW<GlobalNamespace::BeatmapLevel*> allmapsArray;

            GlobalNamespace::BeatmapLevelPack *levelPack = levelCollectionNavigationController->_levelPack;
            if (levelPack)
            {
                allmapsArray = levelPack->____beatmapLevels;
                getLogger().info("Acquired Maps using CollectionNavigationView");
            }
            else
            {
                allmapsArray = filteringNavigationController->_levelSearchViewController->_beatmapLevelPack->_beatmapLevels;
                getLogger().info("Acquired Maps using SearchView");
            }

            // Calculate Upper Bound for rand
            int max = allmapsArray.size() - 1;
            getLogger().info("Acquired BeatMapCount: %i", max);

            if (max > 0)
            {
                // Select a random level from 0 to max (exclusive)
                std::uniform_int_distribution<int> distribution(0, max);
                int selectedLevel = distribution(generator);
                auto map = allmapsArray[selectedLevel];
                // Select the found map. In combination with RecentlyPlayed this Controller is null and would nullref, so we just do nothing?
                if(levelCollectionNavigationController->_levelCollectionViewController)
                {
                    levelCollectionNavigationController->SelectLevel(map);
                }
                getLogger().info("Selected level: %i", selectedLevel);
            }
            else
            {
                getLogger().info("No level found to select");
            }
        }
    }
}