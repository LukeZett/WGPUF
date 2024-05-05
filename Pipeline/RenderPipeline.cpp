#include "RenderPipeline.h"
#include "Device.h"

void RenderPipeline::Init(WGPURenderPipelineDescriptor& descriptor)
{
	m_pipeline = wgpuDeviceCreateRenderPipeline(Device::Get(), &descriptor);
}
