#include <syntax/lexer.h>

#include <utility>
#include "compiler.h"
#include "bytecode.h"
#include "syntax/parser.h"
#include "syntax/morph.h"

std::unique_ptr<FunctionObj> compile(const std::string& source) {
  auto lexer = std::make_unique<Lexer>(source);
  auto tokens = lexer->Lex();
  auto morphed = morph(tokens);

  auto parser = std::make_unique<Parser>(morphed);
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

  // TODO: Pop locals.
}

void Compiler::SetInstance() {
  // Take old compiler instance.
  auto current_copy = new CompilerInstance(FunctionType::Function);
  std::swap(*current_copy, current);

  // Set new compiler instance.
  current = CompilerInstance(FunctionType::Function);

  // Add old compiler instance.
  current.enclosing = std::make_optional(current_copy);
}

std::unique_ptr<FunctionObj> Compiler::end_compiler() {
  emit(Opcode::Return);

  auto fun_copy = *current.function;
  fun_copy.bytecode->DisassembleBytecode();

  if (current.enclosing.has_value()) {
    current = std::move(*current.enclosing.value());
  }

  return std::make_unique<FunctionObj>(fun_copy);
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

void Compiler::emit_constant(std::unique_ptr<Value> value) {
  auto index = current.function->bytecode->AddConstant(std::move(value));
  emit(Opcode::Constant);
  emit_byte(index);
}

void Compiler::emit(Opcode opcode) {
  current.function->bytecode->Write(opcode);
}

void Compiler::emit_byte(std::uint8_t byte) {
  current.function->bytecode->WriteByte(byte);
}

void Compiler::add_local(std::string name) {
  if (current.locals.size() == UINT8_MAX) {
    throw std::exception();
    // TODO: Throw error if too many locals.
  }

  Local local;;
  local.name = std::move(name);
  local.depth = -1;
  local.initialized = false;

  current.locals.push_back(local);
}

void Compiler::declare_variable(std::string *name) {
  if (current.scope_depth == 0) return;

  for (const auto& local : current.locals) {
    if (local.depth < current.scope_depth) {
      break;
    }

    if (*name == local.name) throw std::exception();
  }

  add_local(*name);
}

void Compiler::mark_initialized() {
  if (current.scope_depth == 0) return;
  current.locals[current.locals.size() - 1].depth =
      current.scope_depth;
  current.locals[current.locals.size() - 1].initialized = true;
}

void Compiler::define_variable(std::string *name) {
  if (current.scope_depth > 0) {
    mark_initialized();
    return;
  }

  // TODO: Works?
  emit(Opcode::DefineGlobal);
  auto val = std::make_unique<Value>(*name);
  auto global = current.function->bytecode->AddConstant(std::move(val));
  emit_byte(global);
}
