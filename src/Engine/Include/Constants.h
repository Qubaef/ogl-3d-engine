#pragma once

// Engine parameters
constexpr auto SECTOR_DENSITY = 1000;
constexpr auto SECTOR_SIZE = 1000;
constexpr auto TILE_SIZE = (SECTOR_SIZE - 1) / (SECTOR_DENSITY - 1);

// Changing the following, requires modification in the fragment shader
constexpr auto MAX_POINT_LIGHTS_COUNT = 16;