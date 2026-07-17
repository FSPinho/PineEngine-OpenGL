cmake_minimum_required(VERSION 4.2)
project(PineEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_library(
        PineEngine

        src/PineEngine/Config/Config.h

        src/PineEngine/platform/Input/Input.h
        src/PineEngine/platform/Platform/Platform.h
        src/PineEngine/platform/Platform/Platform.cpp
        src/PineEngine/platform/InputManager/InputManager.h
        src/PineEngine/platform/InputManager/InputManager.cpp

        src/PineEngine/rendering/Renderer/Renderer.h
        src/PineEngine/rendering/Renderer/Renderer.cpp
        src/PineEngine/rendering/RendererBackend/RendererBackend.h
        src/PineEngine/rendering/RendererBackend/RendererBackend.cpp
        src/PineEngine/rendering/RendererComponent/RendererComponent.h
        src/PineEngine/rendering/RendererComponent/RendererComponent.cpp
        src/PineEngine/rendering/FrameClearer/FrameClearer.h
        src/PineEngine/rendering/FrameClearer/FrameClearer.cpp
        src/PineEngine/rendering/FrameSwapper/FrameSwapper.h
        src/PineEngine/rendering/FrameSwapper/FrameSwapper.cpp
        src/PineEngine/rendering/GeometryBuffer/GeometryBuffer.h
        src/PineEngine/rendering/GeometryBuffer/GeometryBuffer.cpp
        src/PineEngine/rendering/ShaderSet/ShaderSet.h
        src/PineEngine/rendering/ShaderSet/ShaderSet.cpp
        src/PineEngine/rendering/Timer/Timer.h
        src/PineEngine/rendering/Timer/Timer.cpp

        src/PineEngine/Util/Log/Log.h
        src/PineEngine/Util/SerialID/SerialID.h
        src/PineEngine/Util/SerialID/SerialID.cpp
        src/PineEngine/Util/Time/Time.h
        src/PineEngine/Util/File/File.h
        src/PineEngine/Util/File/File.cpp
        src/PineEngine/Util/FileWatch/FileWatch.h
        src/PineEngine/Util/FileWatch/FileWatch.cpp
)
target_link_libraries(
        PineEngine PUBLIC
        glad::glad
        glfw
)
target_include_directories(
        PineEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_options(
        PineEngine PRIVATE
        "/FI${CMAKE_SOURCE_DIR}/src/PineEngine/Config/Config.h"
        "/FI${CMAKE_SOURCE_DIR}/src/PineEngine/Util/Log/Log.h"
)

add_executable(Examples_Basic examples/Basic/main.cpp)
target_link_libraries(
        Examples_Basic PRIVATE
        PineEngine
)