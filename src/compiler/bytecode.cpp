#include "bytecode.h"
#include <iomanip>
#include <iostream>

void Bytecode::Write(Opcode opcode) {
  code_.push_back(static_cast<std::uint8_t>(opcode)); // TODO: Works?
}

void Bytecode::WriteByte(std::uint8_t byte) {
  code_.push_back(byte);
}

std::uint8_t Bytecode::AddConstant(std::unique_ptr<Value> value) {
  constants_.push_back(std::move(value));
  return constants_.size() - 1;
}

Value *Bytecode::ReadConstant(std::uint8_t index) {
  return constants_[index].get(); // TODO: Get?
}

std::vector<std::uint8_t> Bytecode::Code() {
  return code_;
}

void Bytecode::DisassembleBytecode() {
  std::cout << "== TODO ==\n";

  for (auto offset = 0; offset < code_.size();) {
    offset = DisassembleInstruction(offset);
  }
}

int Bytecode::SimpleInstruction(const std::string &name, int offset) {
  std::cout << name << std::endl;
  return offset + 1;
}

int Bytecode::ConstantInstruction(const std::string &name, int offset) {
  auto constant = code_[offset + 1];
  std::cout << std::setw(16)
            << std::left
            << std::setfill(' ')
            << name
            << toascii(constant) // TODO: toascii?
            << ' ';

  std::cout << "'" << *constants_[constant] << "'" << std::endl;
  return offset + 2;
}

int Bytecode::JumpInstruction(const std::string &name, int sign, int offset) {
  uint16_t jump = (uint16_t) code_[offset + 1] << 8;
  jump |= code_[offset + 2];

  // Name.
  std::cout << std::setw(16)
            << std::left
            << std::setfill(' ')
            << name
            << ' ';

  // Offset.
  std::cout << std::setw(4)
            << std::setfill('0')
            << std::right
            << offset
            << " -> ";
  // TODO:
  std::cout << offset + 3 + sign * jump
            << std::endl;
  return offset + 3;
}

int Bytecode::DisassembleInstruction(int offset) {
  std::cout << std::setw(4)
            << std::setfill('0')
            << std::right
            << offset;

  // TODO: Print line.
  std::cout << "   | ";

  auto instruction = code_[offset];
  switch (static_cast<Opcode>(instruction)) {
    case Opcode::Return:return SimpleInstruction("OP_RETURN", offset);
    case Opcode::Constant:return ConstantInstruction("OP_CONSTANT", offset);
    case Opcode::Add:return SimpleInstruction("OP_ADD", offset);
    case Opcode::Subtract:return SimpleInstruction("OP_SUBTRACT", offset);
    case Opcode::Multiply:return SimpleInstruction("OP_MULTIPLY", offset);
    case Opcode::Divide:return SimpleInstruction("OP_DIVIDE", offset);
    case Opcode::Equal:return SimpleInstruction("OP_EQUAL", offset);
    case Opcode::Greater:return SimpleInstruction("OP_GREATER", offset);
    case Opcode::Less:return SimpleInstruction("OP_LESS", offset);
    case Opcode::Negate:return SimpleInstruction("OP_NEGATE", offset);
    case Opcode::Not:return SimpleInstruction("OP_NOT", offset);
    case Opcode::Jump:return JumpInstruction("OP_JUMP", 1, offset);
    case Opcode::JumpIfFalse:
      return JumpInstruction("OP_JUMP_IF_FALSE",
                             1,
                             offset);
    case Opcode::Loop:return JumpInstruction("OP_LOOP", -1, offset);
    case Opcode::Pop:return SimpleInstruction("OP_POP", offset);
    case Opcode::Print:return SimpleInstruction("OP_PRINT", offset);
  }
}

//// TODO: Move to value.cpp.
Value operator+(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number + b.number);
  }
  throw std::exception();
}

Value operator-(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number - b.number);
  }
  throw std::exception();
}

Value operator/(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number / b.number);
  }
  throw std::exception();
}

Value operator*(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number * b.number);
  }
  throw std::exception();
}

Value operator-(const Value &a) {
  if (a.Type == ValueType::Number) {
    return Value(-a.number);
  }
  throw std::exception();
}

Value operator!(const Value &a) {
  if (a.Type == ValueType::Bool) {
    return Value(!a.bool_);
  }
  throw std::exception();
}

Value operator==(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number == b.number);
  }
  if (a.Type == ValueType::Bool && b.Type == ValueType::Bool) {
    return Value(a.bool_ == b.bool_);
  }
  return Value(false);
}

Value operator<(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number < b.number);
  }
  throw std::exception();
}

Value operator>(const Value &a, const Value &b) {
  if (a.Type == ValueType::Number && b.Type == ValueType::Number) {
    return Value(a.number > b.number);
  }
  throw std::exception();
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
  switch (value.Type) {
    case Number:os << value.number;
      break;
    case Bool:
      if (value.bool_) {
        os << "true";
      } else {
        os << "false";
      }
      break;
    case Closure:os << "CLOSURE";
      break;
  }

  return os;
}
