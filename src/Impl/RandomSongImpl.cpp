#include "Impl/RandomSongImpl.hpp"

namespace RandomSongImpl {

    SafePtrUnity<GlobalNamespace::LevelCollectionNavigationController> levelCollectionNavigationController;
    SafePtrUnity<GlobalNamespace::LevelFilteringNavigationController> filteringNavigationController;
    
    void selectRandomSong()
    {
        // Only do Things, if we also have the necessary References
        if (levelCollectionNavigationController && filteringNavigationController)
        {
            // Get Array of all beatmaps that the user currently sees
            ArrayW<GlobalNamespace::IPreviewBeatmapLevel *> allmapsArray;

            auto *levelPack = levelCollectionNavigationController->levelPack;
            if (levelPack)
            {
                allmapsArray = il2cpp_utils::cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection>(levelPack)->get_beatmapLevelCollection()->get_beatmapLevels();
                getLogger().info("Acquired Maps using CollectionNavigationView");
            }
            else
            {
                allmapsArray = filteringNavigationController->levelSearchViewController->beatmapLevelPackCollection->get_beatmapLevelCollection()->get_beatmapLevels();
                getLogger().info("Acquired Maps using SearchView");
            }

            // Calculate Upper Bound for rand
            int max = allmapsArray->Length();
            getLogger().info("Acquired BeatMapCount");

            if (max > 0)
            {
                // Select a random level from 0 to max (exclusive)
                levelCollectionNavigationController->SelectLevel(allmapsArray->get(rand() % max));
                getLogger().info("Selected level");
            }
            else
            {
                getLogger().info("No level found to select");
            }
        }
    }
}