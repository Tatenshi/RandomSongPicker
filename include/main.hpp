#pragma once

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

// Includes for the functionality of the mod
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/LevelCollectionNavigationController.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"
#include "GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionsViewController.hpp"
#include "GlobalNamespace/IBeatmapLevelCollection.hpp"
#include "GlobalNamespace/IBeatmapLevelPack.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/IAnnotatedBeatmapLevelCollection.hpp"
#include "GlobalNamespace/LevelSearchViewController.hpp"
#include "GlobalNamespace/LevelSearchViewController_BeatmapLevelPackCollection.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Vector2.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "System/Array.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include <random>

// Icons
#include "icons.hpp"

// Define these functions here so that we can easily read configuration and log information from other files
Configuration& getConfig();
Logger& getLogger();

// Static Variables
static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
// TODO Look into deleting pointers when exiting instead of SafePtr, to avoid Memory Leak
static SafePtr<GlobalNamespace::LevelCollectionNavigationController> levelCollectionNavigationController;
static SafePtr<GlobalNamespace::LevelFilteringNavigationController> filteringNavigationController;
static UnityEngine::UI::Button *button;
static bool pressedEventAllreadyRun = false;

void selectRandomSong();