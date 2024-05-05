#include "AppWindow.h"
#include "AppWindow.h"
#include "AppWindow.h"


#include "AppWindow.h"
#include "Logging.h"
#include "Adapter.h"
#include "Device.h"


AppWindow::AppWindow()
{
}

bool AppWindow::Init(int width, int height, const std::string& name)
{
	m_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	
	if (!m_window) {
		LOG_ERROR("Window creation failed! ");
		return false;
	}
	else LOG_INFO("GLFW window initialized");
	
	m_width = width;
	m_height = height;
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, onWindowResize);
	glfwSetKeyCallback(m_window, onKey);
	return true;
}

void AppWindow::ToggleFullScreen()
{
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videomode = glfwGetVideoMode(monitor);
	if (mode == Fullscreen) {
		mode = Windowed;
		m_width = videomode->width / 2;
		m_height = videomode->height / 2;
		glfwSetWindowMonitor(m_window, NULL, m_width / 2, m_height / 2, m_width, m_height, videomode->refreshRate);
	}
	else if (mode == Windowed)
	{
		m_width = videomode->width;
		m_height = videomode->height;
		mode = Fullscreen;
		glfwSetWindowMonitor(m_window, monitor, 0, 0, m_width, m_height, videomode->refreshRate);
	}
}

void AppWindow::SetCursorMode(CursorMode mode)
{
	switch (mode)
	{
	case Visible:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case Hidden:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	case HiddenRaw:
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported()) glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		break;
	default:
		break;
	}
}

void AppWindow::InitSwapChain()
{
	CreateSwapChain();
}

void AppWindow::OnResize(int width, int height)
{
	m_width = width;
	m_height = height;
	CreateSwapChain();
	ResizeEvent e(width, height);
	if (m_resizeHandler != nullptr)
	{
		m_resizeHandler(e);
	}

}

void AppWindow::OnKey(int key, int scancode, int action, int mods)
{
	KeyboardEvent e(key, scancode, action, mods);
	if (key == GLFW_KEY_F11) { // F11 is used as fullscreen toggle and is not propagated further
		if (action == GLFW_PRESS) ToggleFullScreen();
	}
	else if (m_keyboardHandler != nullptr)
	{
		m_keyboardHandler(e);
	}
}

AppWindow::~AppWindow()
{
	if (m_surface) wgpuSurfaceRelease(m_surface);
	if (m_swapChain) wgpuSwapChainRelease(m_swapChain);
	if (m_window) glfwDestroyWindow(m_window);
	glfwTerminate();
}

void AppWindow::CreateSwapChain()
{
	WGPUSwapChainDescriptor chainDesc = {};
	chainDesc.nextInChain = nullptr;
	chainDesc.width = m_width;
	chainDesc.height = m_height;

#ifdef WEBGPU_BACKEND_DAWN
	WGPUTextureFormat chainFormat = WGPUTextureFormat_BGRA8Unorm;
#else
	m_chainFormat = wgpuSurfaceGetPreferredFormat(m_surface, Adapter::Get());
#endif // WEBGPU_BACKEND_DAWN

	chainDesc.format = m_chainFormat;
	chainDesc.usage = WGPUTextureUsage_RenderAttachment;
	chainDesc.presentMode = WGPUPresentMode_Fifo;
	m_swapChain = wgpuDeviceCreateSwapChain(Device::Get(), m_surface, &chainDesc);
	LOG_INFO("Created swap chain " + std::to_string(m_width) + "x" + std::to_string(m_height));
}
