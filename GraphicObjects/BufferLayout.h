#pragma once

#include <vector>
#include "webgpu/webgpu.h"
#include "Logging.h"

struct BufferElement
{
	WGPUVertexFormat format;
	uint16_t elementSize = 0;
	uint16_t elementoffset = 0;
	BufferElement(WGPUVertexFormat format, uint16_t elementSize = 0, uint16_t elementoffset = 0)
	{
		this->format = format;
		this->elementSize = elementSize;
		this->elementoffset = elementoffset;
	}
};


class BufferLayout
{
public:
	inline BufferLayout(bool instanced);

	inline void getVertexBufferLayout(WGPUVertexBufferLayout& vertexBufferLayout, WGPUVertexAttribute*& vertexAttributes, uint16_t shaderLocOffset = 0)
	{
		int location = 0;
		vertexAttributes = new WGPUVertexAttribute[elements.size()];

		for (size_t i = 0; i < elements.size(); i++)
		{
			vertexAttributes[i].format = elements[i].format;
			vertexAttributes[i].shaderLocation = i + shaderLocOffset;
			vertexAttributes[i].offset = elements[i].elementoffset;
		}

		vertexBufferLayout.attributeCount = elements.size();
		vertexBufferLayout.attributes = vertexAttributes;
		vertexBufferLayout.stepMode = mode;
		vertexBufferLayout.arrayStride = stride;
	}

	template<typename T>
	inline void AddElement(int count) { LOG_ERROR("Unsupported vertex attribute type, skipping"); }

	template<>
	inline void AddElement<int>(int count)
	{
		i_AddElement<WGPUVertexFormat_Sint32, float>(count);
	}

	template<>
	inline void AddElement<float>(int count)
	{
		i_AddElement<WGPUVertexFormat_Float32, float>(count);
	}

	~BufferLayout() = default;

private:

	template<WGPUVertexFormat Format, typename T>
	inline void i_AddElement(int count) {
		if (count > 4)
		{
			LOG_ERROR("Unsupported vertex attribute count, skipping");
			return;
		}
		// (Format + count - 1) is for choosing the correct vector type (defined by count)
		BufferElement element((WGPUVertexFormat)(Format + count - 1), sizeof(T) * count, stride);
		elements.push_back(element);
		stride += element.elementSize;
	}

	std::vector<BufferElement> elements;
	uint32_t stride = 0;
	WGPUVertexStepMode mode;
};

inline BufferLayout::BufferLayout(bool instanced)
{
	if (!instanced) mode = WGPUVertexStepMode_Vertex;
	else mode = WGPUVertexStepMode_Instance;
}


