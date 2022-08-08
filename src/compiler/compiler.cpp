#include <syntax/lexer.h>

#include <utility>
#include "compiler.h"
#include "bytecode.h"
#include "syntax/parser.h"

std::unique_ptr<FunctionObj> compile(const std::string& source) {
  auto lexer = std::make_unique<Lexer>(source);
  auto tokens = lexer->Lex();

  auto parser = std::make_unique<Parser>(tokens);
  auto expressions = parser->Parse();

  auto compiler = std::make_unique<Compiler>();

  for (auto expr: expressions) {
    expr->Compile(compiler.get()); // TODO: Use raw pointer?
  }

  return compiler->end_compiler();
}

void Compiler::begin_scope() {
  current.scope_depth += 1;
}

void Compiler::end_scope() {
  current.scope_depth -= 1;

  // Pop locals.
  // TODO:
}

std::unique_ptr<FunctionObj> Compiler::end_compiler() {
  // TODO: Fix.
  current.function->bytecode->DisassembleBytecode();

  std::unique_ptr<FunctionObj> ret;
  std::swap(ret, current.function);
  return ret;
}

int Compiler::emit_jump(Opcode opcode) { // TODO: Int?
  emit(opcode);
  emit_byte(0xff);
  emit_byte(0xff);
  return current.function->bytecode->Code().size() - 2;
}

void Compiler::emit_loop(int loop_start) { // TODO: Int?
  emit(Opcode::Loop);

  auto sub = current.function->bytecode->Code().size() - loop_start + 2;

  // TODO: Works?
  auto lo = ((sub >> 8) & 0xff);
  auto hi = (sub & 0xff);

  emit_byte(lo);
  emit_byte(hi);
}

void Compiler::patch_jump(int offset) { // TODO: Int?
  // -2 to adjust for the bytecode for the jump offset itself.
  auto jump = current.function->bytecode->Code().size() - offset - 2;

  current.function->bytecode->Code()[offset] = ((jump >> 8) & 0xff);
  current.function->bytecode->Code()[offset + 1] = (jump & 0xff);
}

void Compiler::emit_constant(Value value) {
  auto index = current.function->bytecode->AddConstant(value);
  emit(Opcode::Constant);
  emit_byte(index);
}

void Compiler::emit(Opcode opcode) {
  current.function->bytecode->Write(opcode);
}

void Compiler::emit_byte(std::uint8_t byte) {
  current.function->bytecode->WriteByte(byte);
}

void Compiler::add_local(Identifier name) {
  if (current.locals.size() == UINT8_MAX) {
    // TODO: Throw error if too many locals.
  }

  Local local;;
  local.name = name;
  local.depth = -1;
  local.initialized = false;

  current.locals.push_back(local);
}

void Compiler::declare_variable(Identifier *name) {
  if (current.scope_depth == 0) return;

  for (const auto& local : current.locals) {
    if (local.depth != -1 && local.depth < current.scope_depth) {
      break;
    }

//    if (name == local.name) {
    // TODO: Throw error.
//    }
  }

  add_local(*name); // TODO: Dereference?
}

void Compiler::mark_initialized() {
  if (current.scope_depth == 0) return;
  current.locals[current.locals.size() - 1].depth =
      current.scope_depth;
}

void Compiler::define_variable(Identifier *name) {
  if (current.scope_depth > 0) {
    mark_initialized();
    return;
  }

  // TODO:
//  auto global = current_chunk()->add_constant(name);
//  emitBytes(Opcode::define_global, global);
}
