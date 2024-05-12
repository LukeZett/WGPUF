#pragma once


#include "GLFW/glfw3.h"
#include <iostream>
#include "webgpu/webgpu.h"
#include "webgpu/wgpu.h"
#include "glfw3webgpu.h"
#include "Events.h"
#include <functional>

#define RESIZE_CALLBACK(method, object) (std::function<void(ResizeEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define KEYBOARD_CALLBACK(method, object) (std::function<void(KeyboardEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define FILEDROP_CALLBACK(method, object) (std::function<void(FileDropEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define MOUSEMOVE_CALLBACK(method, object) (std::function<void(MouseMoveEvent&)>)std::bind(&method, &object, std::placeholders::_1)
#define MOUSEBUTTON_CALLBACK(method, object) (std::function<void(MouseButtonEvent&)>)std::bind(&method, &object, std::placeholders::_1)

enum WindowMode
{
	Fullscreen,
	WindowedFullScreen,
	Windowed
};

enum DepthTexFormat {
	Depth24Plus = WGPUTextureFormat_Depth24Plus,
	Depth24Stencil8 = WGPUTextureFormat_Depth24PlusStencil8,
	Depth32 = WGPUTextureFormat_Depth32Float,
	Depth16UNorm = WGPUTextureFormat_Depth16Unorm,
	Depth32Stencil8 = WGPUTextureFormat_Depth32FloatStencil8
};

enum CursorMode
{
	Visible,
	Hidden,
	HiddenRaw
};

class AppWindow
{
public:
	/**
	* @brief Create unintialized window instance
	*/
	AppWindow();

	/**
	* @brief Initialize window with glfw
	*/
	bool Init(int width, int height, const std::string& name);

	void ToggleFullScreen();

	void SetCursorMode(CursorMode mode);

	void UseDepth(DepthTexFormat format = Depth24Plus);
	
	DepthTexFormat GetDepthFormat() const { return static_cast<DepthTexFormat>(m_depthTexDesc.format); };

	WGPUTextureView GetDepthView() const { return m_depthTexView; }

	/**
	* @brief returns window's close flag
	*/
	inline bool ShouldClose() { return glfwWindowShouldClose(m_window); }

	/**
	* @brief Process events
	*/
	inline void PollEvents() { return glfwPollEvents(); }

	/**
	* @brief initialize surface for drawing
	* @param instance initialized webgpu instance
	*/
	inline void InitSurface(WGPUInstance instance) { m_surface = glfwGetWGPUSurface(instance, m_window); }

	inline WGPUSurface GetSurface() const { return m_surface; }

	/**
	* @brief Fetch texture view from swap chain 
	*/
	inline WGPUTextureView GetNextTexView() { return wgpuSwapChainGetCurrentTextureView(m_swapChain); }

	/**
	* @brief Show frame in window
	*/
	inline void PresentFrame() { wgpuSwapChainPresent(m_swapChain); }

	/**
	* @brief Get swap chain texture format
	*/
	inline const WGPUTextureFormat& GetTextureFormat() const { return m_chainFormat; }

	inline std::tuple<int, int> GetWindowSize() const { return { m_width, m_height }; }

	inline float GetAspectRatio() const { return m_width / (float)m_height; }

	inline void SetResizeCallback(std::function<void(ResizeEvent&)> callback) { m_resizeHandler = callback; }

	inline void SetKeyCallback(std::function<void(KeyboardEvent&)> callback) { m_keyboardHandler = callback; }

	inline void SetFileDropCallback(std::function<void(FileDropEvent&)> callback) { m_fileDropHandler = callback; }

	inline void SetMouseMoveCallback(std::function<void(MouseMoveEvent&)> callback) { m_mouseMoveHandler = callback; }

	inline void SetMouseClickCallback(std::function<void(MouseButtonEvent&)> callback) { m_mouseClickHandler = callback; }

	/**
	* @brief initialize swap chain
	*/
	void InitSwapChain();

#pragma region eventHandlers
	
	void OnResize(int width, int height);

	void OnKey(int key, int scancode, int action, int mods);

	void OnFileDrop(int count, const char** paths);

	void OnMouseMove(double xpos, double ypos);

	void OnMouseClick(int key, int action, int mods);

#pragma endregion

	~AppWindow();

private: // functions
	void CreateSwapChain();

private: // members
	WindowMode mode = Windowed;
	WGPUTextureFormat m_chainFormat = WGPUTextureFormat_BGRA8Unorm;
	GLFWwindow* m_window = nullptr;
	WGPUSurface m_surface = nullptr;
	WGPUSwapChain m_swapChain = nullptr;
	int m_width = 0;
	int m_height = 0;

	WGPUTextureDescriptor m_depthTexDesc = {};
	WGPUTexture m_depthTexture = nullptr;
	WGPUTextureViewDescriptor m_depthTexViewDesc = {};
	WGPUTextureView m_depthTexView = nullptr;

	std::function<void(FileDropEvent&)> m_fileDropHandler = nullptr;
	std::function<void(ResizeEvent&)> m_resizeHandler = nullptr;
	std::function<void(KeyboardEvent&)> m_keyboardHandler = nullptr;
	std::function<void(MouseButtonEvent&)> m_mouseClickHandler = nullptr;
	std::function<void(MouseMoveEvent&)> m_mouseMoveHandler = nullptr;
};

//helper callbacks for GLFW

inline void onWindowResize(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	app->OnResize(width, height);
}

inline void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	app->OnKey(key, scancode, action, mods);
}

inline void onFileDrop(GLFWwindow* window, int count, const char** paths)
{
	auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	app->OnFileDrop(count, paths);
}

inline void onMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	app->OnMouseMove(xpos, ypos);
}

inline void onMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	auto app = reinterpret_cast<AppWindow*>(glfwGetWindowUserPointer(window));
	app->OnMouseClick(button, action, mods);
}


