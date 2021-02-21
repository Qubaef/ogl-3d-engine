#pragma once

// Shader paths
constexpr auto SHADER_PATH_VERTEX = "Shaders/VertexShader.vert";
constexpr auto SHADER_PATH_FRAGMENT = "Shaders/FragmentShader.frag";

constexpr auto WATER_SHADER_PATH_VERTEX = "Shaders/WaterVertexShader.vert";

constexpr auto LIGHT_SHADER_PATH_VERTEX = "Shaders/LightingVertexShader.vert";
constexpr auto LIGHT_SHADER_PATH_FRAGMENT = "Shaders/LightingFragmentShader.frag";

constexpr auto SKYBOX_SHADER_PATH_VERTEX = "Shaders/SkyBoxVertexShader.vert";
constexpr auto SKYBOX_SHADER_PATH_FRAGMENT = "Shaders/SkyBoxFragmentShader.frag";

// Engine parameters
constexpr auto SCREEN_W = 1280;
constexpr auto SCREEN_H = 720;
constexpr auto CAMERA_FOV = 45;
constexpr auto CAMERA_NEAR_CLIPPING = 5.f;
constexpr auto CAMERA_FAR_CLIPPING = 100000.f;

constexpr auto SECTOR_DENSITY = 1000;
constexpr auto SECTOR_SIZE = 1000;
constexpr auto TILE_SIZE = (SECTOR_SIZE - 1) / (SECTOR_DENSITY - 1);



// Changing the following, requires modification in the fragment shader
constexpr auto MAX_POINT_LIGHTS_COUNT = 16;