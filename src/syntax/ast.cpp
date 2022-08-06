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
  std::cout << "let set\n";
}

void LetGetExpr::Compile(Compiler *compiler) {
  std::cout << "let get\n";
}

void IfElseExpr::Compile(Compiler *compiler) {
}

void BlockExpr::Compile(Compiler *compiler) {
  std::cout << "block expr\n";

  compiler->begin_scope();
  for (auto expr: exprs) {
    expr->Compile(compiler);
  }
  compiler->end_scope();
}

void FunExpr::Compile(Compiler *compiler) {
}

void PrintExpr::Compile(Compiler *compiler) {
  expr->Compile(compiler);
  compiler->emit(Opcode::Print);
}

void LiteralExpr::Compile(Compiler *compiler) {
  compiler->emit_constant(value);
}

void CallExpr::Compile(Compiler *compiler) {
}

void ReturnExpr::Compile(Compiler *compiler) {
}