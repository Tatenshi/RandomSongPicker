#include "Settings/SettingsVC.hpp"

std::vector<std::string> buttonOptions = {
    "None",
    "A",
    "B",
    "X",
    "Y"
};

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(firstActivation) {
        // Make Touchable
        self->get_gameObject()->AddComponent<HMUI::Touchable*>();

        // Create Container
        auto* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());

        // Create StringWrapper Vector with Content of buttonOptions
        // We cant create it directly, as StringW is not designed for static use
        std::vector<StringW> buttonOptionsWrapper = {};
        for (auto option : buttonOptions) {
            buttonOptionsWrapper.emplace_back(option);
        }

        // Create actual Dropdown
        QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), "ControllerButton Binding", buttonOptionsWrapper[getModConfig().UseButton.GetValue()], buttonOptionsWrapper, [](auto value) {

            // Find Index of selected Element
            int index = std::find(buttonOptions.begin(), buttonOptions.end(), value) - buttonOptions.begin();

            // And Save it
            getModConfig().UseButton.SetValue(index);
            
            getLogger().info("Changed ButtonConfig"); });
    }
}
