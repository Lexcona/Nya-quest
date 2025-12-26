#include "TaskRunner.hpp"
#include "Events.hpp"
#include "NyaConfig.hpp"

#include "GlobalNamespace/OVRInput.hpp"

DEFINE_TYPE(Nya, TaskRunner);

void Nya::TaskRunner::FixedUpdate() {
    static bool pressedEventAllreadyRun = false;

    // // 0 Means nothing is assigned, and we dont need to do anything
    int useButtonValue = getNyaConfig().UseButton.GetValue();
    if (useButtonValue > 0) {
        // TODO: Maybe we should access it differently to avoid performance issues?
        GlobalNamespace::OVRInput::Update();
        // Determine if we need the Right or Left Controller (Right is 2 Left is One)
        // Definition from: GlobalNamespace::OVRInput::Controller::RTouch
        int controllerIndex = useButtonValue > 2 ? 1 : 2;

        // Here we correct the Index for direct Usage as Input for OVRInput.Get
        // After this line the Primary Button A/X (1/3 in Config) is 0 and the Secondary Button (2/4 in Config) is 1
        // Source: https://developer.oculus.com/documentation/unity/unity-ovrinput/
        useButtonValue = ((useButtonValue - 1) % 2) + 1;

        bool buttonPressed =
            GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput_Button(useButtonValue), controllerIndex);

        if (buttonPressed) {
            if (!pressedEventAllreadyRun) {
                if (Nya::GlobalEvents::onControllerNya.size() > 0) {
                    Nya::GlobalEvents::onControllerNya.invoke();
                    pressedEventAllreadyRun = true;
                }
            }
        } else {
            pressedEventAllreadyRun = false;
        }
    }
}