#include "main.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup
static GlobalNamespace::LevelCollectionNavigationController *levelCollectionNavigationController;
static GlobalNamespace::LevelFilteringNavigationController *filteringNavigationController;
static UnityEngine::UI::Button *button;

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

MAKE_HOOK_MATCH(SoloFreePlayFlowCoordinatorHook, &HMUI::FlowCoordinator::PresentFlowCoordinator, void, HMUI::FlowCoordinator *self, HMUI::FlowCoordinator *flowCoordinator, System::Action *finishedCallback, HMUI::ViewController::AnimationDirection animationDirection, bool immediately, bool replaceTopViewController)
{
    // Base Call
    SoloFreePlayFlowCoordinatorHook(self, flowCoordinator, finishedCallback, animationDirection, immediately, replaceTopViewController);

    // Save Controller for later use in button onClick
    if (il2cpp_utils::try_cast<GlobalNamespace::SoloFreePlayFlowCoordinator>(flowCoordinator))
    {
        auto *ActuallySoloFreePlayFlowCoordinator = (GlobalNamespace::SoloFreePlayFlowCoordinator *)flowCoordinator;
        levelCollectionNavigationController = ActuallySoloFreePlayFlowCoordinator->dyn_levelSelectionNavigationController()->dyn__levelCollectionNavigationController();
        filteringNavigationController = ActuallySoloFreePlayFlowCoordinator->dyn_levelSelectionNavigationController()->dyn__levelFilteringNavigationController();
        button->get_gameObject()->set_active(true);
        getLogger().info("Activated Random Song Button");
    }
    else
    {
        button->get_gameObject()->set_active(false);
        getLogger().info("Disabled Random Song Button");
    }
}

MAKE_HOOK_MATCH(GamePlaySetUpHook, &GlobalNamespace::GameplaySetupViewController::DidActivate, void, GlobalNamespace::GameplaySetupViewController *self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    // Base Call
    GamePlaySetUpHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation)
    {
        // Create Randomselection Button
        button = QuestUI::BeatSaberUI::CreateUIButton(self->get_transform(), "", UnityEngine::Vector2(56.0f, 0), UnityEngine::Vector2(11.0f, 11.0f), []() {
            // Only do Things, if we also have the necessary References
            if(levelCollectionNavigationController && filteringNavigationController)
            {
                // Get Array of all beatmaps that the user currently sees
                System::Array* allmapsArray;
                
                auto* levelPack = reinterpret_cast<GlobalNamespace::IAnnotatedBeatmapLevelCollection*>(levelCollectionNavigationController->dyn__levelPack());
                if(levelPack) {
                    allmapsArray = reinterpret_cast<System::Array*>(levelPack->get_beatmapLevelCollection()->get_beatmapLevels());
                    getLogger().info("Acquired Maps using CollectionNavigationView");
                }
                else {
                    allmapsArray = reinterpret_cast<System::Array*>(filteringNavigationController->dyn__levelSearchViewController()->dyn__beatmapLevelPackCollection()->get_beatmapLevelCollection()->get_beatmapLevels());
                    getLogger().info("Acquired Maps using SearchView");
                }

                // Calculate Upper Bound for rand
                int max = allmapsArray->System_Collections_ICollection_get_Count();
                getLogger().info("Acquired BeatMapCount");

                if(max > 0)
                {
                    // Select a random level from 0 to max (exclusive)
                    levelCollectionNavigationController->SelectLevel(reinterpret_cast<GlobalNamespace::IPreviewBeatmapLevel*>(allmapsArray->System_Collections_IList_get_Item(rand() % max)));
                    getLogger().info("Selected level");
                }
                else {
                    getLogger().info("No level found to select");
                }
            } });

        // Set Icon of the Button and scale it to fit the Button
        auto *image = QuestUI::BeatSaberUI::CreateImage(button->get_transform(), QuestUI::BeatSaberUI::Base64ToSprite(diceIcon));
        image->get_rectTransform()->set_localScale({0.65f, 0.65f, 1.0f});

        getLogger().info("Created Random Song Button");
    }
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo &info)
{
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load()
{
    il2cpp_functions::Init();

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), SoloFreePlayFlowCoordinatorHook);
    INSTALL_HOOK(getLogger(), GamePlaySetUpHook);
    getLogger().info("Installed all hooks!");
}