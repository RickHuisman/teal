#pragma once

#include <string>
#include <vector>
#include <optional>
#include "bytecode.h"

// TODO: Import from ast.h.
using Identifier = std::string;

struct Local {
  std::string name;
  uint8_t depth; // TODO: uint8_t?
  bool initialized;
  uint8_t slot; // TODO: uint8_t
};

struct CompilerInstance {
  CompilerInstance(FunctionType function_type)
      : function(ObjFunction()), function_type(function_type) {};

  ObjFunction function;
  FunctionType function_type;
  std::vector<Local> locals;
  std::uint8_t scope_depth = 0; // TODO: uint8_t
  std::optional<CompilerInstance *> enclosing;
};

class Compiler {
  CompilerInstance current;
  // TODO: Errors.

 public:
  Compiler() : current(CompilerInstance(FunctionType::Script)) {}

  void begin_scope();
  void end_scope();
  ObjFunction end_compiler();
  int emit_jump(Opcode opcode);
  void emit_loop(int loop_start);
  void patch_jump(int offset);
  void emit_constant(Value value);
  void emit(Opcode opcode);
  void emit_byte(uint8_t byte);
  void add_local(Identifier name);
  void declare_variable(Identifier *name);
  void mark_initialized();
  void define_variable(Identifier *name);
  Bytecode CurrentBytecode();
};

ObjFunction compile(std::string source);