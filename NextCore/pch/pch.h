#pragma once

// STL Includes
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include <string_view>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

// Fix compiler error from including dsound.h in NextAPI/SimpleSound.h
#include <mmsystem.h>

#include <NextAPI/SimpleSound.h>
#include <NextAPI/SimpleSprite.h>
#include <NextAPI/app.h>
