#include "main.hpp"

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

MAKE_HOOK_MATCH(PresentFlowCoordinatorHook, &HMUI::FlowCoordinator::PresentFlowCoordinator, void, HMUI::FlowCoordinator *self, HMUI::FlowCoordinator *flowCoordinator, System::Action *finishedCallback, HMUI::ViewController::AnimationDirection animationDirection, bool immediately, bool replaceTopViewController)
{
    // Base Call
    PresentFlowCoordinatorHook(self, flowCoordinator, finishedCallback, animationDirection, immediately, replaceTopViewController);

    // Button only enabled in Solo and Party
    if (il2cpp_utils::try_cast<GlobalNamespace::SoloFreePlayFlowCoordinator>(flowCoordinator) || il2cpp_utils::try_cast<GlobalNamespace::PartyFreePlayFlowCoordinator>(flowCoordinator))
    {
        // Save Controller for later use in button onClick
        auto *LevelSelectionFlowCoordinatorInstance = (GlobalNamespace::LevelSelectionFlowCoordinator *)flowCoordinator;
        levelCollectionNavigationController.emplace(LevelSelectionFlowCoordinatorInstance->levelSelectionNavigationController->levelCollectionNavigationController);
        filteringNavigationController.emplace(LevelSelectionFlowCoordinatorInstance->levelSelectionNavigationController->levelFilteringNavigationController);
        
        if(button) {
            button->get_gameObject()->set_active(true);
            getLogger().info("Activated Random Song Button");
        }
        else {
            getLogger().info("Wanted to activated Random Song Button, but button was null");
        }
    }
    else
    {
        if(button) {
            button->get_gameObject()->set_active(false);
            getLogger().info("Disabled Random Song Button");
        }
        else {
            getLogger().info("Wanted to disable Random Song Button, but button was null");
        }
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
            selectRandomSong();
            });

        // Set Icon of the Button and scale it to fit the Button
        auto *image = QuestUI::BeatSaberUI::CreateImage(button->get_transform(), QuestUI::BeatSaberUI::Base64ToSprite(diceIcon));
        image->get_rectTransform()->set_localScale({0.65f, 0.65f, 1.0f});

        getLogger().info("Created Random Song Button");
    }
}

MAKE_HOOK_MATCH(FixedUpdateHook, &GlobalNamespace::OculusVRHelper::FixedUpdate, void, GlobalNamespace::OculusVRHelper* self){
    FixedUpdateHook(self);

    // TODO Make configurable over config utils (all Buttons available)
    bool AButtonPressed = GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::One, GlobalNamespace::OVRInput::Controller::RTouch);
    if(AButtonPressed){
        if(!pressedEventAllreadyRun) {
            selectRandomSong();
            pressedEventAllreadyRun = true;
        }
    }
    else {
        pressedEventAllreadyRun = false;
    }
}

// TODO Extract to own Class & File
void selectRandomSong() {
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
    INSTALL_HOOK(getLogger(), PresentFlowCoordinatorHook);
    INSTALL_HOOK(getLogger(), GamePlaySetUpHook);
    INSTALL_HOOK(getLogger(), FixedUpdateHook);
    getLogger().info("Installed all hooks!");
}
