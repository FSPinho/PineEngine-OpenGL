cmake_minimum_required(VERSION 4.2)
project(PineEngine)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

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
        src/PineEngine/rendering/GeometryBuffer/GeometryBuffer.h
        src/PineEngine/rendering/GeometryBuffer/GeometryBuffer.cpp
        src/PineEngine/rendering/ShaderSet/ShaderSet.h
        src/PineEngine/rendering/ShaderSet/ShaderSet.cpp

        src/PineEngine/application/Application/Application.h
        src/PineEngine/application/Application/Application.cpp
        src/PineEngine/application/Scene/Scene.h
        src/PineEngine/application/Scene/Scene.cpp
        src/PineEngine/application/Object/Object.h
        src/PineEngine/application/Object/Object.cpp
        src/PineEngine/application/Camera/Camera.h
        src/PineEngine/application/Camera/Camera.cpp
        src/PineEngine/application/Light/Light.h
        src/PineEngine/editor/CameraController/CameraController.h
        src/PineEngine/editor/CameraController/CameraController.cpp

        src/PineEngine/editor/Editor/Editor.h
        src/PineEngine/editor/Editor/Editor.cpp

        src/PineEngine/Util/Log/Log.h
        src/PineEngine/Util/SerialID/SerialID.h
        src/PineEngine/Util/SerialID/SerialID.cpp
        src/PineEngine/Util/Time/Time.h
        src/PineEngine/util/Timer/Timer.h
        src/PineEngine/util/Timer/Timer.cpp
        src/PineEngine/Util/File/File.h
        src/PineEngine/Util/File/File.cpp
        src/PineEngine/Util/FileWatch/FileWatch.h
        src/PineEngine/Util/FileWatch/FileWatch.cpp
        src/PineEngine/Util/Transform/Transform.h
        src/PineEngine/Util/Transform/Transform.cpp
        src/PineEngine/Util/Path/Path.h
        src/PineEngine/Util/Path/Path.cpp
        src/PineEngine/Util/Resource/Resource.h
        src/PineEngine/Util/Resource/Resource.cpp
        src/PineEngine/Util/ResourceManager/ResourceManager.h
        src/PineEngine/Util/ResourceManager/ResourceManager.cpp
        src/PineEngine/Util/ResourceHandler/ResourceHandler.h
        src/PineEngine/Util/GeometryLoader/GeometryLoader.h
        src/PineEngine/Util/GeometryLoader/GeometryLoader.cpp
        src/PineEngine/util/OcclusionMap/OcclusionMap.h
        src/PineEngine/util/Subdivider/Subdivider.h
)
target_link_libraries(
        PineEngine PUBLIC
        glad::glad
        glfw
        assimp::assimp
)
target_include_directories(
        PineEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_options(
        PineEngine PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/FI${CMAKE_SOURCE_DIR}/src/PineEngine/config/Config.h>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-include${CMAKE_SOURCE_DIR}/src/PineEngine/config/Config.h>
        $<$<CXX_COMPILER_ID:MSVC>:/FI${CMAKE_SOURCE_DIR}/src/PineEngine/util/Log/Log.h>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-include${CMAKE_SOURCE_DIR}/src/PineEngine/util/Log/Log.h>
)

# --------------------
# Examples

add_executable(Examples_Basic examples/Basic/main.cpp)
target_link_libraries(
        Examples_Basic PRIVATE
        PineEngine
)

# --------------------
# Tests

add_executable(Tests_ResourceManager tests/test_ResourceManager.cpp)
add_executable(Tests_Path tests/test_Path.cpp)
add_executable(Tests_OcclusionMap tests/test_OcclusionMap.cpp)
target_link_libraries(Tests_ResourceManager PRIVATE PineEngine Catch2::Catch2WithMain)
target_link_libraries(Tests_Path PRIVATE PineEngine Catch2::Catch2WithMain)
target_link_libraries(Tests_OcclusionMap PRIVATE PineEngine Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(Tests_ResourceManager)
catch_discover_tests(Tests_Path)
catch_discover_tests(Tests_OcclusionMap)