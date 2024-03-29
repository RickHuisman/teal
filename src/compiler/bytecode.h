#pragma once

#include <utility>
#include <vector>
#include <string>
#include "../vm/opcode.h"

struct Value;

class Bytecode {
  std::vector<std::uint8_t> code_;
  std::vector<std::unique_ptr<Value>> constants_;

 public:
  void Write(Opcode opcode);
  void WriteByte(std::uint8_t byte);
  std::uint8_t AddConstant(std::unique_ptr<Value> value);
  std::vector<std::uint8_t> Code();
  Value *ReadConstant(uint8_t index);
  void DisassembleBytecode();
  int DisassembleInstruction(int offset);
  int ConstantInstruction(const std::string &name, int offset);
  static int SimpleInstruction(const std::string &name, int offset);
  int JumpInstruction(const std::string &name, int sign, int offset);
  int byte_instruction(const std::string &name, int offset);
};

// TODO: Move?
enum class FunctionType {
  Closure,
  Function,
  Script,
};

struct FunctionObj {
  std::string name;
  Bytecode *bytecode;
  std::uint8_t arity = 0;

  FunctionObj(std::string name,
              Bytecode *bytecode,
              std::uint8_t arity) : name(std::move(name)),
                                    bytecode(bytecode),
                                    arity(arity) {};
};

enum ValueType {
  Number,
  Bool,
  String,
  Closure,
};

struct ClosureValue {
  FunctionObj *function;

  explicit ClosureValue(FunctionObj *function) : function(function) {}
};

struct Value {
  ValueType Type;

  float number;
  bool bool_;
  std::string string_;
  std::unique_ptr<ClosureValue> closure;

  explicit Value(float n)
      : Type(ValueType::Number), number(n), closure(nullptr) {}

  explicit Value(bool b)
      : Type(ValueType::Bool), bool_(b), closure(nullptr) {}

  explicit Value(std::string s)
      : Type(ValueType::String), string_(std::move(s)), closure(nullptr) {}

  explicit Value(ClosureValue closure)
      : Type(ValueType::Closure),
        closure(std::make_unique<ClosureValue>(closure)),
        number(0) {}

  explicit Value(FunctionObj *function)
      : Type(ValueType::Closure),
        closure(std::make_unique<ClosureValue>(function)),
        number(0) {}
};

Value operator+(const Value &a, const Value &b);

Value operator-(const Value &a, const Value &b);

Value operator*(const Value &a, const Value &b);

Value operator/(const Value &a, const Value &b);

Value operator==(const Value &a, const Value &b);

Value operator<(const Value &a, const Value &b);

Value operator>(const Value &a, const Value &b);

Value operator-(const Value &a);

Value operator!(const Value &a);

std::ostream &operator<<(std::ostream &os, const Value &value);