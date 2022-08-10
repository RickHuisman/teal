#pragma once

#include <cstdint>

enum class Opcode : std::uint8_t {
    Return,
    Constant,
    Add,
    Subtract,
    Multiply,
    Equal,
    Greater,
    Less,
    Divide,
    Negate,
    Not,
    Jump,
    JumpIfFalse,
    Loop,
    Pop,
    DefineGlobal,
    GetGlobal,
    SetGlobal,
    Print,
    Closure,
    Call,
};
