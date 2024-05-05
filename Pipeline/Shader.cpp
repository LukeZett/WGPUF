#include "Shader.h"

Shader::Shader()
{
	WGPUShaderModuleDescriptor shaderDesc{};

#ifdef WEBGPU_BACKEND_WGPU
	shaderDesc.hintCount = 0;
	shaderDesc.hints = nullptr;
#endif

	WGPUShaderModuleWGSLDescriptor shaderCodeDesc{};
	// Set the chained struct's header
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
	shaderCodeDesc.code =
		"@vertex																			  "
		"fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {"
		"	var p = vec2f(0.0, 0.0);														  "
		"	if (in_vertex_index == 0u) {													  "
		"		p = vec2f(-0.5, -0.5);														  "
		"	}																				  "
		"	else if (in_vertex_index == 1u) {												  "
		"		p = vec2f(0.5, -0.5);														  "
		"	}																				  "
		"	else {																			  "
		"		p = vec2f(0.0, 0.5);														  "
		"	}																				  "
		"	return vec4f(p, 0.0, 1.0);														  "
		"}																					  "
		"																					  "
		"@fragment																			  "
		"fn fs_main() -> @location(0) vec4f {												  "
		"	return vec4f(0.8, 0.2, 1.0, 1.0);												  "
		"}																					  ";
	// Connect the chain
	shaderDesc.nextInChain = &shaderCodeDesc.chain;

	m_shaderModule = wgpuDeviceCreateShaderModule(Device::Get(), &shaderDesc);
}


Shader::Shader(std::filesystem::path source)
{
	WGPUShaderModuleDescriptor shaderDesc{};

#ifdef WEBGPU_BACKEND_WGPU
	shaderDesc.hintCount = 0;
	shaderDesc.hints = nullptr;
#endif

	WGPUShaderModuleWGSLDescriptor shaderCodeDesc{};
	// Set the chained struct's header
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;

	std::ifstream ifs(source);
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	shaderCodeDesc.code = content.c_str();
	// Connect the chain
	shaderDesc.nextInChain = &shaderCodeDesc.chain;

	m_shaderModule = wgpuDeviceCreateShaderModule(Device::Get(), &shaderDesc);
}


Shader::~Shader()
{
	if (m_shaderModule == nullptr) return;
	wgpuShaderModuleRelease(m_shaderModule);
}


inline std::string Shader::LoadFromPath(std::filesystem::path path)
{
	std::ifstream ifs(path);
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return content;
}