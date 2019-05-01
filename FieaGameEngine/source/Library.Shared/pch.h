#pragma once

// \file pch.h
// \brief Precompiled header for the library

// Standard
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <tuple>
#include <thread>
#include <memory>
#include <future>
#include <mutex>

// glm
#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

// gsl
#include <gsl/gsl>

// json
#include <json/json.h>