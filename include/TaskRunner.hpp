#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(RandomSongPicker, TaskRunner, UnityEngine::MonoBehaviour) {
    DECLARE_INSTANCE_METHOD(void, FixedUpdate);
};