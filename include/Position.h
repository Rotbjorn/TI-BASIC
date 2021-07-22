#pragma once

#include <cstddef>

//0.4% C


struct Position final {
    size_t line;
    size_t column;

    Position() : line { 1 }, column { 1 } {}
};
