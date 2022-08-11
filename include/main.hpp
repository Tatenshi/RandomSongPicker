#pragma once

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
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Vector2.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "System/Array.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include <vector>
#include <random>

// Icons
#include "Impl/icons.hpp"
#include "Impl/RandomSongImpl.hpp"
#include "Settings/config.hpp"
#include "Settings/SettingsVC.hpp"

static UnityEngine::UI::Button *button;
static bool pressedEventAllreadyRun = false;