#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>

// File containing constant variables
#include "Constants.h"

// Custom exceptions
#include "InitializationException.h"

// Library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;