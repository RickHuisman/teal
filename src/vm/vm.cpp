#include "vm.h"
#include <iostream>
#include <sstream>
#include <compiler/compiler.h>

void VM::interpret(const std::string& source) {
  // TODO: Use interpret_with_output.
  // interpret_with_output(source, dynamic_cast<std::stringstream *>(&std::cout));
  output = &std::cout;

  auto fun = compile(source);
  auto closureVal = new ClosureValue(fun.get());
  stack.push_back(*closureVal);
  CallValue(0);
  run();
}

void VM::interpret_with_output(std::string source, std::stringstream *buffer) {
  auto fun = compile(source);
  output = buffer;
  run();
}

void VM::run() {
  while (!frames.empty()) {
    auto instruction = static_cast<Opcode>(read_byte());
    switch (instruction) {
      case Opcode::Return:
        std::cout << "return_\n"; // TODO:
        break;
      case Opcode::Constant:
        constant();
        break;
      case Opcode::Add:
        add();
        break;
      case Opcode::Subtract:
        subtract();
        break;
      case Opcode::Multiply:
        multiply();
        break;
      case Opcode::Divide:
        divide();
        break;
      case Opcode::Negate:
        negate();
        break;
      case Opcode::Not:
        not_();
        break;
      case Opcode::Equal:
        equal();
        break;
      case Opcode::Greater:
        greater();
        break;
      case Opcode::Less:
        less();
        break;
      case Opcode::Jump:
        jump();
        break;
      case Opcode::JumpIfFalse:
        jump_if_false();
        break;
      case Opcode::Loop:
        loop();
        break;
      case Opcode::Pop:
        pop();
        break;
      case Opcode::Print:
        print();
        break;
    }
  }
}

void VM::constant() {
  push(read_constant());
}

void VM::add() {
  auto b = pop();
  auto a = pop();
//  push(a + b);
}

void VM::subtract() {
  auto b = pop();
  auto a = pop();
//  push(a - b);
}

void VM::multiply() {
  auto b = pop();
  auto a = pop();
//  push(a * b);
}

void VM::divide() {
  auto b = pop();
  auto a = pop();
//  push(a / b);
}

void VM::negate() {
  auto a = pop();
//  push(-a);
}

void VM::not_() {
  auto a = pop();
//  push(!a);
}

void VM::equal() {
  auto b = pop();
  auto a = pop();
//  push(a == b);
}

void VM::greater() {
  auto b = pop();
  auto a = pop();
//  push(a > b);
}

void VM::less() {
  auto b = pop();
  auto a = pop();
//  push(a < b);
}

void VM::jump() {
  auto offset = read_short();
  // TODO: frame_mut()?.ip_mut() += offset as usize;
}

void VM::jump_if_false() {
  // TODO:
}

void VM::loop() {
  // TODO:
}

void VM::print() {
  auto popped = pop();
  std::cout << "PRINT!" << std::endl;
//  std::cout << popped/**/;
//  *output << popped << std::endl;
}

void VM::CallValue(std::uint8_t arity) {
  auto frame_start = stack.size() - (arity + 1);
  Value *callee = &stack[frame_start];

  if (callee->Type() == ValueType::Closure) {
    auto *clos = static_cast<ClosureValue*>(callee);
    Call(clos, arity);
  } else {
    throw std::exception();
//      _ => return Err(RuntimeError::InvalidCallee),
  }
}

void VM::Call(ClosureValue *closure, std::uint8_t arity) {
  if (arity != closure->function->arity) {
    throw std::exception();
//    return Err(RuntimeError::IncorrectArity);
  }

  auto frame_start = stack.size() - (arity + 1);
//  auto newFrame = new CallFrame(closure, frame_start);
//  frames.push_back(newFrame);
}

void VM::push(Value value) {
  stack.push_back(value);
}

Value VM::pop() {
  auto popped = stack.back();
  stack.pop_back();
  return popped; // TODO: Pop back?
}

Value VM::read_constant() {
  auto index = read_byte();
  return current_bytecode()->ReadConstant(index);
}

std::uint8_t VM::read_byte() {
  auto index = current_frame()->ip;
  auto byte = current_bytecode()->Code()[index];
  // current_frame().ip += 1;
  frames.back().ip += 1; // TODO: Use current_frame() function.
  return byte;
}

// TODO: Cleanup.
std::uint16_t VM::read_short() {
  frames.back().ip += 2; // TODO: Use current_frame() function.
  // *frame_mut()?.ip_mut() += 2;

  auto lo_index = frames.back().ip - 2;
  auto hi_index = frames.back().ip - 1;

  auto lo = current_bytecode()->Code()[lo_index];
  auto hi = current_bytecode()->Code()[hi_index];
  return ((lo << 8) | hi);
}

CallFrame *VM::current_frame() {
  return &frames.back();
}

Bytecode *VM::current_bytecode() {
  return current_frame()->closure->function->bytecode.get();
}
