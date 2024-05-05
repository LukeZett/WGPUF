#include "Adapter.h"
#include "Logging.h"

bool Adapter::Init(WGPUInstance instance, WGPUSurface surface)
{
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.nextInChain = nullptr;
    adapterOpts.compatibleSurface = surface;
    s_instance.m_adapter = s_instance.requestAdapter(instance, &adapterOpts);
    return s_instance.m_adapter != nullptr;
}

WGPUAdapter Adapter::requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options) {

    struct UserData {
        WGPUAdapter adapter = nullptr;
        bool requestEnded = false;
    };
    UserData userData;

    auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData) {
        UserData& userData = *reinterpret_cast<UserData*>(pUserData);
        if (status == WGPURequestAdapterStatus_Success) {
            LOG_INFO("Obtained WGPU adapter");
            userData.adapter = adapter;
        }
        else {
            LOG_ERROR("Could not get WebGPU adapter: " + std::string(message));
        }
        userData.requestEnded = true;
        };


    wgpuInstanceRequestAdapter(
        instance,
        options,
        onAdapterRequestEnded,
        (void*)&userData
    );
    return userData.adapter;
}