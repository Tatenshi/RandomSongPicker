// TODO Move Headers to hpp File
#include "main.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/LevelCollectionNavigationController.hpp"
#include "GlobalNamespace/IBeatmapLevelCollection.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/IAnnotatedBeatmapLevelCollection.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector2.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "System/Array.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TextOverflowModes.hpp"
#include <iostream>
#include <random>

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
static GlobalNamespace::LevelCollectionNavigationController *navigationController;

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

MAKE_HOOK_MATCH(NavigationHook, &GlobalNamespace::LevelCollectionNavigationController::DidActivate, void, GlobalNamespace::LevelCollectionNavigationController *self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    // Base Call
    NavigationHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if(firstActivation)
    {
        // Save Controller for later use
        // TODO Check if we need to do this everytime (custom songs -> ost -> custom songs (is this still firstActivation?))
        navigationController = self;
        getLogger().info("Setted NavigationController");
    }
}

MAKE_HOOK_MATCH(GamePlaySetUpHook, &GlobalNamespace::GameplaySetupViewController::DidActivate, void, GlobalNamespace::GameplaySetupViewController *self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    // Base Call
    GamePlaySetUpHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation)
    {
        // Create Randomselection Button
        // TODO Find better position? Where can we place it?
        // TODO Use GetLogger just once
        UnityEngine::UI::Button* button = QuestUI::BeatSaberUI::CreateUIButton(self->get_transform(), "Random\r\nSong!", UnityEngine::Vector2(-55.0f, 0), UnityEngine::Vector2(15.0f, 13.0f),[]() {
            // Get Array of all beatmaps in this Collection
            auto* levelPack = reinterpret_cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>(navigationController->dyn__levelPack());
            getLogger().info("Acquired LevelPack");
            auto* levelCollection = levelPack->get_beatmapLevelCollection();
            getLogger().info("Acquired LevelCollection");
            auto* allmaps = levelCollection->get_beatmapLevels();
            getLogger().info("Acquired AllMaps");
            System::Array* allmapsArray = reinterpret_cast<System::Array*>(allmaps);
            getLogger().info("Acquired allmapsList");

            // Calculate Upper Bound for rand
            int max = allmapsArray->System_Collections_ICollection_get_Count();
            getLogger().info("Acquired max");

            // Select a random level form 0 to max (exclusive)
            navigationController->SelectLevel(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(allmapsArray->System_Collections_IList_get_Item(rand() % max)));
            getLogger().info("Selected Level");
        });

        // Correct Alignment, so that the text is inside of the Button
        // TODO Check if still necessary
        TMPro::TextMeshProUGUI* textMesh = button->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
        if (textMesh)
        {
            textMesh->set_alignment(TMPro::TextAlignmentOptions::Left);
            textMesh->set_overflowMode(TMPro::TextOverflowModes::Linked);
        }
        else {
             getLogger().info("Button had no TextMesh");
        }
    }
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), NavigationHook);
    INSTALL_HOOK(getLogger(), GamePlaySetUpHook);
    getLogger().info("Installed all hooks!");
}