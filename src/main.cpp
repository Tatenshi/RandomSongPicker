#include "main.hpp"

MAKE_HOOK_MATCH(ActivateFlowCoordinatorHook, &HMUI::FlowCoordinator::Activate, void, HMUI::FlowCoordinator *self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    // Base Call
    ActivateFlowCoordinatorHook(self, firstActivation, addedToHierarchy, screenSystemEnabling);
	
    // Button only enabled in Solo and Party
    if (il2cpp_utils::try_cast<GlobalNamespace::SoloFreePlayFlowCoordinator>(self) || il2cpp_utils::try_cast<GlobalNamespace::PartyFreePlayFlowCoordinator>(self) || il2cpp_utils::try_cast<GlobalNamespace::MultiplayerLevelSelectionFlowCoordinator>(self))
    {
        // Save Controller for later use in button onClick
        auto *LevelSelectionFlowCoordinatorInstance = (GlobalNamespace::LevelSelectionFlowCoordinator *)self;
        RandomSongImpl::levelCollectionNavigationController = LevelSelectionFlowCoordinatorInstance->levelSelectionNavigationController->levelCollectionNavigationController;
        RandomSongImpl::filteringNavigationController = LevelSelectionFlowCoordinatorInstance->levelSelectionNavigationController->levelFilteringNavigationController;
    }
    else
    {
        // We dont want to hold the old NavigationController references, so that they can be freed
        RandomSongImpl::levelCollectionNavigationController.emplace(nullptr);
        RandomSongImpl::filteringNavigationController.emplace(nullptr);
    }
}

MAKE_HOOK_MATCH(LevelSelectionNavigationControllerDidActivate, &GlobalNamespace::LevelSelectionNavigationController::DidActivate, void, GlobalNamespace::LevelSelectionNavigationController *self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    // Base Call
    LevelSelectionNavigationControllerDidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation)
    {
        // Create Randomselection Button
        button = QuestUI::BeatSaberUI::CreateUIButton(self->get_transform(), "", UnityEngine::Vector2(-70, 30), UnityEngine::Vector2(9.0f, 9.0f), RandomSongImpl::selectRandomSong);

        // Set Icon of the Button and scale it to fit the Button
        auto *image = QuestUI::BeatSaberUI::CreateImage(button->get_transform(), QuestUI::BeatSaberUI::Base64ToSprite(diceIcon));
        image->get_rectTransform()->set_localScale({0.65f, 0.65f, 1.0f});

        getLogger().info("Created Random Song Button");
        return;
    }

    if(button) 
    {
        button->get_gameObject()->SetActive(true);
        getLogger().info("Activated Random Song Button");
    }
    else
    {
        getLogger().info("Wanted to activated Random Song Button, but button was null");
    }
}

MAKE_HOOK_MATCH(LevelSelectionNavigationControllerDidDeactivate, &GlobalNamespace::LevelSelectionNavigationController::DidDeactivate, void, GlobalNamespace::LevelSelectionNavigationController *self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    // Base Call
    LevelSelectionNavigationControllerDidDeactivate(self, removedFromHierarchy, screenSystemDisabling);

    if(button) 
    {
        button->get_gameObject()->SetActive(false);
        getLogger().info("Activated Random Song Button");
    }
    else
    {
        getLogger().info("Wanted to activated Random Song Button, but button was null");
    }
}

MAKE_HOOK_MATCH(FixedUpdateHook, &GlobalNamespace::OculusVRHelper::FixedUpdate, void, GlobalNamespace::OculusVRHelper* self){
    FixedUpdateHook(self);

    // 0 Means nothing is assigned, and we dont need to do anything
    int useButtonValue = getModConfig().UseButton.GetValue();
    if(useButtonValue > 0){
        // Determine if we need the Right or Left Controller (Right is 2 Left is One)
        // Definition from: GlobalNamespace::OVRInput::Controller::RTouch
        int controllerIndex = useButtonValue > 2 ? 1 : 2;

        // Here we correct the Index for direct Usage as Input for OVRInput.Get
        // After this line the Primary Button A/X (1/3 in Config) is 0 and the Secondary Button (2/4 in Config) is 1
        // Source: https://developer.oculus.com/documentation/unity/unity-ovrinput/
        useButtonValue = ((useButtonValue - 1) % 2) + 1;

        bool buttonPressed = GlobalNamespace::OVRInput::Get(useButtonValue, controllerIndex);
        if(buttonPressed){
            if(!pressedEventAllreadyRun) {
                RandomSongImpl::selectRandomSong();
                pressedEventAllreadyRun = true;
            }
        }
        else {
            pressedEventAllreadyRun = false;
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
    // Init things
    getModConfig().Init(modInfo);
    il2cpp_functions::Init();
    QuestUI::Init();

    // Regsiter our Settings
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);

    // Install Hooks
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), ActivateFlowCoordinatorHook);
    INSTALL_HOOK(getLogger(), FixedUpdateHook);
    INSTALL_HOOK(getLogger(), LevelSelectionNavigationControllerDidActivate);
    INSTALL_HOOK(getLogger(), LevelSelectionNavigationControllerDidDeactivate);
    getLogger().info("Installed all hooks!");
}
