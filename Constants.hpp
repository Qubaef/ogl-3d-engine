#pragma once

// Shader paths
constexpr auto SHADER_PATH_VERTEX = "Shaders/VertexShader.vert";
constexpr auto SHADER_PATH_FRAGMENT = "Shaders/FragmentShader.frag";
constexpr auto LIGHT_SHADER_PATH_VERTEX = "Shaders/LightingVertexShader.vert";
constexpr auto LIGHT_SHADER_PATH_FRAGMENT = "Shaders/LightingFragmentShader.frag";

// Exceptions messages
constexpr auto EXCEPTION_MSG_INIT = "Error while initializing program";

// Engine parameters
constexpr auto SCREEN_NAME = "Custom engine";
constexpr auto SCREEN_W = 1280;
constexpr auto SCREEN_H = 720;
constexpr auto CAMERA_FOV = 45;

constexpr auto SECTOR_DENSITY = 64;
constexpr auto SECTOR_SIZE = 128;

// changing the following, requires modification in the fragment shader
constexpr auto MAX_POINT_LIGHTS_COUNT = 16;