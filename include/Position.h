#pragma once

#include <cstddef>

struct Position final {
    size_t line;
    size_t column;

    Position() : line { 1 }, column { 1 } {}
};
