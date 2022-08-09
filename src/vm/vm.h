#pragma once

#include "../compiler/bytecode.h"

struct CallFrame {
  std::unique_ptr<ClosureValue> closure;
  std::uint8_t ip;
  std::uint8_t stack_start;

  CallFrame(std::unique_ptr<ClosureValue> closure, std::uint8_t stack_start)
      : closure(std::move(closure)), ip(0), stack_start(0) {}
};

class VM {
  std::vector<std::unique_ptr<Value>> stack;
  std::vector<CallFrame> frames;
  // TODO: HashMap of globals.
  std::ostream *output;

  std::uint8_t read_byte();
  std::uint16_t read_short();
  CallFrame *current_frame();

 public:
  // TODO: Init stack & frames with fixed size.

  void interpret(const std::string &source); // TODO: Return interpret result.
  void interpret_with_output(std::string source,
                             std::stringstream *buffer); // TODO: Return interpret result.
  void run();
  std::unique_ptr<Value> read_constant();
  Bytecode *current_bytecode();
  void constant();
  void push(std::unique_ptr<Value> value);
  std::unique_ptr<Value> pop();
  void return_();
  void add();
  void subtract();
  void multiply();
  void divide();
  void negate();
  void not_();
  void equal();
  void greater();
  void less();
  void jump();
  void jump_if_false();
  void loop();
  void print();
  template<class Func> void binary(Func func);
  void CallValue(std::uint8_t arity);
  void Call(ClosureValue closure, std::uint8_t arity);
};
