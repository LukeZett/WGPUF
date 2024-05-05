#pragma once
#include "webgpu/webgpu.h"


class Device
{
public:
    
    static bool Init(WGPUAdapter adapter);

    static void Release() { wgpuDeviceRelease(s_instance.m_device); }

    static const WGPUDevice Get() { return s_instance.m_device; }

    static const WGPUQueue GetQueue() { return s_instance.m_deviceQueue; }

    static void CreateEncoder();

    static WGPUCommandEncoder GetEncoder() { return s_instance.m_encoder; }
    
    static void ReleaseEncoder() { wgpuCommandEncoderRelease(s_instance.m_encoder); }

    static void SetVertexRequiredLimits(uint16_t maxVertexAttribs, uint16_t maxVertexBuffers, uint64_t maxBufferSize, uint16_t maxStride);
    static void SetInterShaderStageLimits(uint32_t maxInterStageShaderComponents, uint32_t maxInterStageShaderVariables);
    static void SetBindGroupsLimits(uint16_t maxBindGroups, uint16_t maxUniformBuffersPerShader, uint64_t maxUniformSize);
    static void SetTextureLimits(uint16_t height, uint16_t width, uint16_t depth, uint16_t maxTextures, uint16_t textureArrayLayers);



    ~Device() {};
private:
    
    Device() { DefaultLimits(); };

    WGPUDevice requestDevice(WGPUAdapter adapter, WGPUDeviceDescriptor const* descriptor);

    void DefaultLimits();

    static Device s_instance;

    WGPURequiredLimits m_limits = {};
    WGPUDevice m_device = nullptr;
    WGPUQueue m_deviceQueue = nullptr;
    WGPUCommandEncoder m_encoder = nullptr;
};
