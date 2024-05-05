#pragma once
#include "webgpu/webgpu.h"
#include "Device.h"
#include <iostream>
#include <fstream>
#include <filesystem>

class RenderPipeline;

class Shader
{
	friend RenderPipeline;
public:
	Shader(Shader&& other) noexcept {
		this->m_shaderModule = other.m_shaderModule;
		other.m_shaderModule = nullptr;
	}

	inline const WGPUShaderModule Get() const { return m_shaderModule; };
private:
	Shader(const Shader&) = delete;

	Shader();
	Shader(std::filesystem::path source);
	~Shader();


	std::string LoadFromPath(std::filesystem::path path);
	WGPUShaderModule m_shaderModule = nullptr;
};