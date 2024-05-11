#include "WGPUFramework.h"
#include "WGPUFramework.h"
#include "WGPUFramework.h"

bool WGPUFramework::i_Init(bool headless) {
	WGPUInstanceDescriptor desc = {};
	desc.nextInChain = nullptr;
	m_instance = wgpuCreateInstance(&desc);
	if (!m_instance)
	{
		LOG_ERROR("Could not initialize WebGPU!");
		return false;
	}

	if (!headless) {
		if (!glfwInit()) {
			LOG_ERROR("Could not initialize GLFW!");
			return false;
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}
	else {
		return InitAdapterDevice(nullptr);
	}
	return true;
}

bool WGPUFramework::i_CreateWindow(const std::string& name, uint32_t width, uint32_t height)
{
	if (!m_window.Init(width, height, name)) return false;
	m_window.InitSurface(m_instance);
	if (!InitAdapterDevice(m_window.GetSurface())) return false;
	m_window.InitSwapChain();
	return true;
}

WGPURenderPassEncoder WGPUFramework::i_BeginScreenPass()
{
	m_screenTexView = m_window.GetNextTexView();
	if (!m_screenTexView)
	{
		LOG_ERROR("Cannot get next texture view");
		return nullptr;
	}

	Device::CreateEncoder();
	auto encoder = Device::GetEncoder();

	WGPURenderPassDescriptor renderPassDesc = {};
	WGPURenderPassColorAttachment renderPassColorAttachment = {};

	renderPassColorAttachment.view = m_screenTexView;
	renderPassColorAttachment.resolveTarget = nullptr;
	renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
	renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
	renderPassColorAttachment.clearValue = m_clearColor;

	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttachment;

	if (m_window.GetDepthView() != nullptr)
	{
		WGPURenderPassDepthStencilAttachment depthStencilAttachment;
		depthStencilAttachment.view = m_window.GetDepthView();
		depthStencilAttachment.depthClearValue = 1.0f;
		depthStencilAttachment.depthLoadOp = WGPULoadOp_Clear;
		depthStencilAttachment.depthStoreOp = WGPUStoreOp_Store;
		depthStencilAttachment.depthReadOnly = false;

		depthStencilAttachment.stencilClearValue = 0;
		depthStencilAttachment.stencilLoadOp = WGPULoadOp_Clear;
		depthStencilAttachment.stencilStoreOp = WGPUStoreOp_Store;
		depthStencilAttachment.stencilReadOnly = true;

		renderPassDesc.depthStencilAttachment = &depthStencilAttachment;
	}
	else
	{
		renderPassDesc.depthStencilAttachment = nullptr;
	}
	
	renderPassDesc.timestampWriteCount = 0;
	renderPassDesc.timestampWrites = nullptr;
	renderPassDesc.nextInChain = nullptr;

	m_screenPass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);
	return m_screenPass;
}

void WGPUFramework::i_FinishScreenPass()
{
	wgpuRenderPassEncoderEnd(m_screenPass);
	wgpuRenderPassEncoderRelease(m_screenPass);
	wgpuTextureViewRelease(m_screenTexView);

	WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
	cmdBufferDescriptor.nextInChain = nullptr;
	cmdBufferDescriptor.label = "Command buffer";
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(Device::GetEncoder(), &cmdBufferDescriptor);
	Device::ReleaseEncoder();
	wgpuQueueSubmit(Device::GetQueue(), 1, &command);
	wgpuCommandBufferRelease(command);

	m_window.PresentFrame();
}

bool WGPUFramework::InitAdapterDevice(const WGPUSurface& surface)
{
	if (!Adapter::Init(m_instance, surface)) {
		LOG_ERROR("Could not initialize WGPU Adapter!");
		return false;
	}

	if (!Device::Init(Adapter::Get())) {
		LOG_ERROR("Could not initialize WGPU Device!");
		return false;
	}
	return true;
}
