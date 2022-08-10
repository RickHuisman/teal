#pragma once

#include <string>
#include <vector>
#include <optional>
#include "bytecode.h"

// TODO: Import from ast.h.
using Identifier = std::string;

struct Local {
  Identifier name;
  std::uint8_t depth;
  bool initialized;
  std::uint8_t slot;
};

struct CompilerInstance {
  CompilerInstance(FunctionType function_type)
      : function(std::make_unique<FunctionObj>(FunctionObj("todo",
                                                           new Bytecode(),
                                                           0))),
        function_type(function_type) {};

  std::unique_ptr<FunctionObj> function;
  FunctionType function_type;
  std::vector<Local> locals;
  std::uint8_t scope_depth = 0; // TODO: uint8_t
  std::optional<CompilerInstance *> enclosing;
};

struct Compiler {
  CompilerInstance current;
  // TODO: Errors.

  Compiler() : current(CompilerInstance(FunctionType::Script)) {}

  void begin_scope();
  void end_scope();
  void SetInstance();
  std::unique_ptr<FunctionObj> end_compiler();
  int emit_jump(Opcode opcode);
  void emit_loop(int loop_start);
  void patch_jump(int offset);
  void emit_constant(std::unique_ptr<Value> value);
  void emit(Opcode opcode);
  void emit_byte(uint8_t byte);
  void add_local(Identifier name);
  void declare_variable(Identifier *name);
  void mark_initialized();
  void define_variable(Identifier *name);
};

std::unique_ptr<FunctionObj> compile(const std::string &source);