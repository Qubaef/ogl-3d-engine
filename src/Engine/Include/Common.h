#pragma once

// OpenGl universal headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_SWIZZLE		// used for glm .xyz components
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

// Tracy Profiler
#include "../../../tracy/Tracy.hpp"
#include "../../../tracy/TracyOpenGL.hpp"

// Dear ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Publicly available logger
#include "../Logger/Logger.h"
