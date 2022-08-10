#include "ast.h"
#include <iostream>

#include <utility>

void UnaryExpr::Compile(Compiler *compiler) {
  expr->Compile(compiler);

  if (op == UnaryOperator::Negate) compiler->emit(Opcode::Negate);
  if (op == UnaryOperator::Not) compiler->emit(Opcode::Not);
}

void BinaryExpr::Compile(Compiler *compiler) {
  left->Compile(compiler);
  right->Compile(compiler);

  if (op == BinaryOperator::Subtract) compiler->emit(Opcode::Subtract);
  if (op == BinaryOperator::Add) compiler->emit(Opcode::Add);
  if (op == BinaryOperator::Divide) compiler->emit(Opcode::Divide);
  if (op == BinaryOperator::Multiply) compiler->emit(Opcode::Multiply);
  if (op == BinaryOperator::Equal) compiler->emit(Opcode::Equal);
  if (op == BinaryOperator::BangEqual) {
    compiler->emit(Opcode::Equal);
    compiler->emit(Opcode::Not);
  }
  if (op == BinaryOperator::GreaterThan) compiler->emit(Opcode::Greater);
  if (op == BinaryOperator::GreaterThanEqual) {
    compiler->emit(Opcode::Less);
    compiler->emit(Opcode::Not);
  }
  if (op == BinaryOperator::LessThan) compiler->emit(Opcode::Less);
  if (op == BinaryOperator::LessThanEqual) {
    compiler->emit(Opcode::Greater);
    compiler->emit(Opcode::Not);
  }
}

void LetAssignExpr::Compile(Compiler *compiler) {
  compiler->declare_variable(&ident);

  // Compile initializer.
  initializer->Compile(compiler);

  compiler->define_variable(&ident);
}

void LetSetExpr::Compile(Compiler *compiler) {
  expr->Compile(compiler);

  // Global variable
  compiler->emit(Opcode::SetGlobal);
  auto stringVal = std::make_unique<Value>(ident);
  auto constant_id = compiler->current.function->bytecode->AddConstant(
      std::move(stringVal)
  );
  compiler->emit_byte(constant_id);
}

void LetGetExpr::Compile(Compiler *compiler) {
  compiler->emit(Opcode::GetGlobal);

  auto stringVal = std::make_unique<Value>(ident);
  auto constant_id = compiler->current.function->bytecode->AddConstant(
      std::move(stringVal)
  );
  compiler->emit_byte(constant_id);
}

void IfElseExpr::Compile(Compiler *compiler) {
}

void BlockExpr::Compile(Compiler *compiler) {
  compiler->begin_scope();
  for (auto expr : exprs) {
    expr->Compile(compiler);
  }
  compiler->end_scope();
}

void compile_closure(Compiler *compiler,
                     Identifier ident,
                     std::vector<Identifier> params,
                     std::vector<Expr *> exprs) {
  compiler->begin_scope();

  auto arity = params.size();

  // Compile arguments.
  for (auto &arg : params) {
    compiler->declare_variable(&arg);
    compiler->define_variable(&arg);
  }

  // Compile body.
  for (auto expr : exprs) {
    expr->Compile(compiler);
  }

  // Create the function object.
  auto fun = compiler->end_compiler();
  fun->name = ident;
  fun->arity = arity;

  compiler->emit(Opcode::Closure);

  auto fun_copy = new FunctionObj(*fun);
  auto constant_id = compiler->current.function->bytecode->AddConstant(
      std::make_unique<Value>(fun_copy)
  );
  compiler->emit_byte(constant_id);
}

void FunExpr::Compile(Compiler *compiler) {
  compiler->SetInstance();

  auto ident = name;

  compile_closure(compiler, name, params, exprs);

  compiler->define_variable(&ident); // TODO: pointer exists?
}

void PrintExpr::Compile(Compiler *compiler) {
  expr->Compile(compiler);
  compiler->emit(Opcode::Print);
}

void LiteralExpr::Compile(Compiler *compiler) {
  compiler->emit_constant(std::move(value));
}

void CallExpr::Compile(Compiler *compiler) {
  auto arity = args.size();

  callee->Compile(compiler);

  for (auto arg : args) {
    arg->Compile(compiler);
  }

  compiler->emit(Opcode::Call);
  compiler->emit_byte(arity);
}

void ReturnExpr::Compile(Compiler *compiler) {
}