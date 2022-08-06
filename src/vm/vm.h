#pragma once

#include "../compiler/bytecode.h"

struct CallFrame {
  ObjClosure closure;
  std::uint8_t ip;
  std::uint8_t stack_size;
};

class VM {
  std::vector<Value> stack;
  std::vector<CallFrame> frames;
  // TODO: HashMap of globals.
  std::ostream *output;

  std::uint8_t read_byte();
  std::uint16_t read_short();
  CallFrame current_frame();

 public:
  explicit VM() {
    // TODO: Init stack & frames with fixed size.
  }

  void interpret(std::string source); // TODO: Return interpret result.
  void interpret_with_output(std::string source,
                             std::stringstream *buffer); // TODO: Return interpret result.
  void run();
  Value read_constant();
  void constant();
  void push(Value value);
  Value pop();
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
};