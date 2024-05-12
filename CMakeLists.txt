cmake_minimum_required(VERSION 3.16)
project(WGPUframework VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(WGPUframework
	"utilities/Logging.cpp"
    "utilities/Logging.h"
    "utilities/Timer.h"
    "Application/AppWindow.h"
    "Application/AppWindow.cpp"
    "Application/Events.h"
    "Application/Device.h"
    "Application/Device.cpp"
    "Application/Adapter.h"
    "Application/Adapter.cpp"
    "Application/WGPUFramework.h"
    "Application/WGPUFramework.cpp"
    "Pipeline/RenderPipeline.h"
    "Pipeline/RenderPipelineBuilder.h"
    "Pipeline/RenderPipelineBuilder.cpp"
    "Pipeline/Shader.h"
    "Pipeline/Shader.cpp"
    "Pipeline/RenderPipeline.cpp"
    "GraphicObjects/Buffer.h"
    "GraphicObjects/Buffer.cpp"
    "GraphicObjects/BufferLayout.h"
    "GraphicObjects/Textures/Texture2D.h"
    "GraphicObjects/Textures/Texture2D.cpp"
    "vendor/headerOnly/stb_image.h"
    "vendor/headerOnly/tiny_obj_loader.h"
    "Mixins/Uncopyable.h"
    "Mixins/Movable.h"
    "utilities/Object3D.h"
    "utilities/Object3D.cpp"
    "GraphicObjects/Textures/Texture.h"
    "GraphicObjects/Textures/Texture3D.h"
    "Pipeline/BindGroup.h"
    "Cameras/Camera.h"
    "Cameras/TurnTableCamera.h"
    
)

target_include_directories(WGPUframework PUBLIC utilities)
target_include_directories(WGPUframework PUBLIC Application)
target_include_directories(WGPUframework PUBLIC Pipeline)
target_include_directories(WGPUframework PUBLIC GraphicObjects)
target_include_directories(WGPUframework PUBLIC vendor/headerOnly)
target_include_directories(WGPUframework PUBLIC Mixins)


add_subdirectory(vendor/webgpu)
add_subdirectory(vendor/glfw3webgpu)
add_subdirectory(vendor/glm)
target_link_libraries(WGPUframework PUBLIC webgpu glfw3webgpu)
target_link_libraries(WGPUframework PUBLIC glm)


if (NOT EMSCRIPTEN)
	add_subdirectory(vendor/glfw)
	target_link_libraries(WGPUframework PUBLIC glfw)
endif()

if(MSVC)
    target_compile_options(WGPUframework PRIVATE /W4)
else()
    target_compile_options(WGPUframework PRIVATE -Wall -Wextra -pedantic)
endif()

if(EMSCRIPTEN)
    target_link_options(WGPUframework PRIVATE
        -sUSE_GLFW=3
        -sUSE_WEBGPU
        -sASYNCIFY
        -sALLOW_MEMORY_GROWTH
    )
    set_target_properties(WGPUframework PROPERTIES SUFFIX ".html")
endif()


target_copy_webgpu_binaries(WGPUframework)