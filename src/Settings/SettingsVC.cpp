#include "Settings/SettingsVC.hpp"

std::vector<std::string_view> buttonOptions = {
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
        auto* container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());

        // Create actual Dropdown
        BSML::Lite::CreateDropdown(container->get_transform(), "ControllerButton Binding", buttonOptions[getModConfig().UseButton.GetValue()], buttonOptions, [](StringW value) {

            // Find Index of selected Element
            int index = std::find(buttonOptions.begin(), buttonOptions.end(), value) - buttonOptions.begin();

            // And Save it
            getModConfig().UseButton.SetValue(index);
            
            getLogger().info("Changed ButtonConfig"); });
    }
}
