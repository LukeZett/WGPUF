#include "Buffer.h"
#include "Buffer.h"
#include "Buffer.h"
#include "Device.h"

Buffer::Buffer(WGPUBufferUsageFlags usage, BufferType type, bool instanced) : layout(instanced)
{
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "base buffer";
	bufferDesc.usage = usage | type | WGPUBufferUsage_CopyDst;
	bufferDesc.mappedAtCreation = false;
}

Buffer::Buffer(uint64_t allocationSize, WGPUBufferUsageFlags usage, BufferType type, bool instanced) : layout(instanced)
{
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "base buffer";
	bufferDesc.usage = usage | type | WGPUBufferUsage_CopyDst;
	bufferDesc.mappedAtCreation = false;
	Allocate(allocationSize);
}

void Buffer::Allocate(uint64_t allocationSize)
{
	bufferDesc.size = allocationSize;
	if (buffer != nullptr)
	{
		wgpuBufferDestroy(buffer);
		wgpuBufferRelease(buffer);
	}
	buffer = wgpuDeviceCreateBuffer(Device::Get(), &bufferDesc);
}

Buffer::~Buffer()
{
	wgpuBufferDestroy(buffer);
	wgpuBufferRelease(buffer);
}