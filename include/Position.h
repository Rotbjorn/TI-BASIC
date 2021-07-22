#pragma once

#include <cstddef>

//0.4% C gillar du små kukar??? det gör inte jag


struct Position final {
    size_t line;
    size_t column;

    Position() : line { 1 }, column { 1 } {}
};
