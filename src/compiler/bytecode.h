#pragma once

#include <utility>
#include <vector>
#include <string>
#include "../vm/opcode.h"

//struct Value;

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

struct FunctionObj {
  std::string name;
  std::unique_ptr<Bytecode> bytecode;
  std::uint8_t arity = 0;

  FunctionObj(std::string name,
              std::unique_ptr<Bytecode> bytecode,
              std::uint8_t arity) : name(std::move(name)),
                                    bytecode(std::move(bytecode)),
                                    arity(arity) {};
};

enum ValueType {
  Number,
  Bool,
  Closure,
};

class Value {
 public:
  virtual ~Value() = default;

  [[nodiscard]] ValueType Type() const { return type_; }

 protected:
  explicit Value(ValueType type) : type_(type) {}

 private:
  ValueType type_;
};

struct NumberValue : public Value {
  float number;

  explicit NumberValue(float number)
      : Value(ValueType::Number), number(number) {}
};

struct BoolValue : public Value {
  bool bool_;

  explicit BoolValue(bool bool_) : Value(ValueType::Bool), bool_(bool_) {}
};

struct ClosureValue : public Value {
  FunctionObj *function;

  explicit ClosureValue(FunctionObj *function)
      : Value(ValueType::Closure), function(function) {}
};

//struct Value {
//  ValueType type;
//  double numberValue;
//  bool boolValue;
//  std::unique_ptr<ClosureObj> closureValue;
//
//  Value(double val) : numberValue(val), type(ValueType::Double) {
//  }
//
//  Value(bool val) : boolValue(val), type(ValueType::Bool) {
//  }
//
//  Value(std::unique_ptr<ClosureObj> clos) : closureValue(std::move(clos)), type(ValueType::Closure) {
//  }
//};
//
//Value operator+(const Value &a, const Value &b);
//
//Value operator-(const Value &a, const Value &b);
//
//Value operator*(const Value &a, const Value &b);
//
//Value operator/(const Value &a, const Value &b);
//
//Value operator==(const Value &a, const Value &b);
//
//Value operator<(const Value &a, const Value &b);
//
//Value operator>(const Value &a, const Value &b);
//
//Value operator-(const Value &a);
//
//Value operator!(const Value &a);
//
//std::ostream &operator<<(std::ostream &os, const Value &value);