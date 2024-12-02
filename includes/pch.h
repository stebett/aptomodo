#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

// STL containers
#include <array>
#include <vector>
#include <valarray>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <bitset>


// STL strings and streams
#include <format>
#include <iostream>
#include <fstream>
#include <string>

// STL random stuff
#include <numeric>
#include <filesystem>
#include <chrono>
#include <random>

// Vendors
#include <toml++/toml.hpp>
// #include <boost/container_hash/hash.hpp>
#include <raylib.h>
#include <raymath.h>
#include <entt/entity/registry.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <imgui_impl_raylib.h>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <LDtkLoader/Entity.hpp>
#include <box2d/box2d.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>