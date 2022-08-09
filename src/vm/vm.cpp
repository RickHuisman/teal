#include "vm.h"
#include <iostream>
#include <sstream>
#include <compiler/compiler.h>

void VM::interpret(const std::string& source) {
  // TODO: Use interpret_with_output.
  output = &std::cout;

  auto fun = compile(source);
  auto closure = ClosureValue(fun.get());
  push(std::make_unique<Value>(closure));
  CallValue(0);
  run();
}

void VM::interpret_with_output(const std::string &source, std::stringstream *buffer) {
  auto fun = compile(source);
  output = buffer;

  auto closure = ClosureValue(fun.get());
  push(std::make_unique<Value>(closure));
  CallValue(0);
  run();
}

void VM::run() {
  while (!frames.empty()) {
    auto instruction = static_cast<Opcode>(read_byte());
    switch (instruction) {
      case Opcode::Return:
        return_();
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
      case Opcode::DefineGlobal:
        DefineGlobal();
        break;
      case Opcode::GetGlobal:
        GetGlobal();
        break;
    }
  }
}

void VM::return_() {
  auto result = pop();
  frames.pop_back();

  auto stack_start = frames.back().stack_start;

//  stack.erase(stack.begin() + stack_start, stack.end());
//  stack.resize(stack.begin(), stack.begin() + stack_start); // TODO: Truncate?

  push(std::move(result));
}

void VM::constant() {
  push(std::move(read_constant()));
}

void VM::add() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a + *b));
}

void VM::subtract() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a - *b));
}

void VM::multiply() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a * *b));
}

void VM::divide() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a / *b));
}

void VM::negate() {
  auto a = pop().release();
  push(std::make_unique<Value>(-*a));
}

void VM::not_() {
  auto a = pop().release();
  push(std::make_unique<Value>(!*a));
}

void VM::equal() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a == *b));
}

void VM::greater() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a > *b));
}

void VM::less() {
  auto b = pop().release();
  auto a = pop().release();
  push(std::make_unique<Value>(*a < *b));
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
  auto popped = pop().release();
  *output << *popped << std::endl;
}

void VM::DefineGlobal() {
  auto value = pop().release();
  auto varName = ReadString();
  globals.insert(std::make_pair(varName, value));
}

void VM::GetGlobal() {
  auto name = ReadString();

  if(globals.find(name) == globals.end()) {
    throw std::exception();
  } else {
    auto value = std::move(globals.at(name));

    if (value->Type == ValueType::Number) {
      push(std::make_unique<Value>(value->number));
    } else if (value->Type == ValueType::Bool) {
      push(std::make_unique<Value>(value->bool_));
    } else if (value->Type == ValueType::String) {
      push(std::make_unique<Value>(value->string_));
    }
  }
}

void VM::CallValue(std::uint8_t arity) {
  auto frame_start = stack.size() - (arity + 1);
  auto callee = &stack[frame_start];

  if (callee->get()->Type == ValueType::Closure) {
    Call(*callee->get()->closure, arity);
  } else {
    throw std::exception();
  }
}

void VM::Call(ClosureValue closure, std::uint8_t arity) {
  if (arity != closure.function->arity) {
    throw std::exception();
  }

  auto frame_start = stack.size() - (arity + 1);
  frames.emplace_back(std::make_unique<ClosureValue>(closure), frame_start);
}

void VM::push(std::unique_ptr<Value> value) {
  stack.push_back(std::move(value));
}

std::unique_ptr<Value> VM::pop() {
  auto popped = std::move(stack.back());
  stack.pop_back();
  return popped; // TODO: Pop back?
}

std::unique_ptr<Value> VM::read_constant() {
  auto index = read_byte();
  auto constant = current_bytecode()->ReadConstant(index);

  // TODO:
  if (constant->Type == ValueType::Number) {
    return std::make_unique<Value>(constant->number);
  } else if (constant->Type == ValueType::Bool) {
    return std::make_unique<Value>(constant->bool_);
  } else if (constant->Type == ValueType::String) {
    return std::make_unique<Value>(constant->string_);
  }

  throw std::exception();

//  auto foobar = std::make_unique<Value>(*constant);
//
//  return foobar; // TODO:
}

std::string VM::ReadString() {
  auto val = read_constant();
  if (val->Type != ValueType::String) throw std::exception();
  return val->string_;
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
