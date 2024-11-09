#pragma once

#include <iostream>
#include <filesystem>

#include <functional>
#include <string>
#include <chrono>
#include <optional>
#include <windows.h>
#include <minwindef.h>
#include <queue>
#include <condition_variable>
#include <memory>

#include <mutex>
#include <stack>
#include <stdexcept>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "memory/wrapper.hpp"
#include "memory/segment.hpp"
#include "memory/module.hpp"
#include "memory/signature.hpp"
#include "memory/scanner.hpp"