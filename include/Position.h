#pragma once

#include "pch.h"

struct Position final {
    size_t line;
    size_t column;

    Position() : line { 1 }, column { 1 } {}
};
