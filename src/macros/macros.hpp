//
// Created by César B. on 4/8/2024.
//

#pragma once

#include <sstream>

#define make_string(...) (std::stringstream() << __VA_ARGS__).str()
