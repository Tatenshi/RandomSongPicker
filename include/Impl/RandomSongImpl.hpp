#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"

#include "GlobalNamespace/LevelCollectionNavigationController.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"
#include "GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionsViewController.hpp"
#include "GlobalNamespace/LevelSearchViewController.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapLevelPack.hpp"

#include <random>
#include <shared_mutex>

#include "logger.hpp"

namespace RandomSongImpl {

    extern SafePtrUnity<GlobalNamespace::LevelCollectionNavigationController> levelCollectionNavigationController;
    extern SafePtrUnity<GlobalNamespace::LevelFilteringNavigationController> filteringNavigationController;

    void selectRandomSong();
}