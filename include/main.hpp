#pragma once

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

// Includes for the functionality of the mod
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"
#include "GlobalNamespace/SoloFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/PartyFreePlayFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/Vector2.hpp"
#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Image.hpp"
#include "bsml/shared/BSML.hpp"
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