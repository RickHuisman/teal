#pragma once

#include <vector>
#include <string>
#include "../vm/opcode.h"

struct Value;

class Bytecode {
  std::vector<std::uint8_t> code_;
  std::vector<Value> constants_;

 public:
  void Write(Opcode opcode);
  void WriteByte(std::uint8_t byte);
  std::uint8_t AddConstant(Value value);
  std::vector<std::uint8_t> Code();
  std::vector<Value> Constants();
  Value ReadConstant(uint8_t index);
  void DisassembleBytecode();
  int DisassembleInstruction(int offset);
  int ConstantInstruction(std::string name, int offset);
  int SimpleInstruction(std::string name, int offset);
  int JumpInstruction(std::string name, int sign, int offset);
};

// TODO: Move?
enum class FunctionType {
  Closure,
  Function,
  Script,
};

enum class ObjType {
  Closure,
  Function,
  String,
};

struct Obj {
  ObjType type;
  struct Obj *next;
};

struct ObjFunction {
  std::string name;
  Bytecode bytecode;
  std::uint8_t arity = 0;
};

struct ObjClosure {
  ObjFunction function;

  ObjClosure(ObjFunction function) : function(function) {}
};

enum ValueType {
  Double,
  Bool,
  Closure,
};

struct Value {
  ValueType type;
  double numberValue;
  bool boolValue;
  ObjClosure *closureValue; // TODO: Pointer?

  Value(double val) : numberValue(val), type(ValueType::Double) {
  }

  Value(bool val) : boolValue(val), type(ValueType::Bool) {
  }

  Value(ObjClosure clos) : closureValue(&clos), type(ValueType::Closure) {
  }
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