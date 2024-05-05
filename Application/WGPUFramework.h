#pragma once
#include "Uncopyable.h"

#include "webgpu/webgpu.h"
#include "Logging.h"
#include "AppWindow.h"
#include "Adapter.h"
#include "Device.h"
#include "RenderPipelineBuilder.h"
#include "Textures/Texture2D.h"




class WGPUFramework : Uncopyable
{
public:
	static bool Init(bool headless = false) {
		return s_instance.i_Init(headless);
	}

	static bool CreateWindow(const std::string& name, uint32_t width = 720, uint32_t height = 480) {
		return s_instance.i_CreateWindow(name, width, height);
	}

	static AppWindow& Window() {
		return s_instance.m_window;
	}

	static WGPURenderPassEncoder BeginScreenPass() {
		return s_instance.i_BeginScreenPass();
	}

	static void FinishScreenPass() {
		s_instance.i_FinishScreenPass();
	}

	static void SetClearColor(float red, float green, float blue) {
		s_instance.m_clearColor = { red, green, blue, 1 };
	}

	static RenderPipelineBuilder CreateRenderPipelineBuilder() {
		return RenderPipelineBuilder(s_instance.m_window.GetTextureFormat());
	}


	~WGPUFramework() {
		wgpuInstanceRelease(m_instance);
	}


private:
	static WGPUFramework s_instance;
	
	WGPUInstance m_instance = nullptr;
	AppWindow m_window;
	WGPURenderPassEncoder m_screenPass = nullptr;
	WGPUTextureView m_screenTexView = nullptr;
	WGPUColor m_clearColor = { 0.0, 0.0, 0.0, 1.0 };
	
	bool i_Init(bool headless);
	bool i_CreateWindow(const std::string& name, uint32_t width, uint32_t height);
	WGPURenderPassEncoder i_BeginScreenPass();
	void i_FinishScreenPass();
	
	bool InitAdapterDevice(const WGPUSurface& surface);

	WGPUFramework() {};
};

typedef WGPUFramework WGF;
