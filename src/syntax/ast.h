#pragma once

#include <utility>
#include <vector>
#include <string>
#include <optional>

#include "../compiler/compiler.h"
#include "../compiler/bytecode.h"

using Identifier = std::string;

class Expr {
public:
  Expr() = default;

  virtual void Compile(Compiler *compiler) = 0;
};

enum class UnaryOperator {
  Negate,
  Not,
};

struct UnaryExpr : Expr {
  UnaryOperator op;
  Expr *expr;

  UnaryExpr(UnaryOperator op, Expr *expr) : Expr(), op(op), expr(expr) {}

  void Compile(Compiler *compiler) override;
};

enum class BinaryOperator {
  Subtract,
  Add,
  Divide,
  Multiply,
  Equal,
  BangEqual,
  GreaterThan,
  GreaterThanEqual,
  LessThan,
  LessThanEqual,
};

struct BinaryExpr : Expr {
  Expr *left;
  BinaryOperator op;
  Expr *right;

  BinaryExpr(Expr *left, BinaryOperator op, Expr *right)
      : Expr(), left(left), op(op), right(right) {}

  void Compile(Compiler *compiler) override;
};

struct LetAssignExpr : Expr {
  Identifier ident;
  Expr *initializer;

  explicit LetAssignExpr(std::string ident, Expr *expr)
      : Expr(), ident(ident), initializer(expr) {}

  void Compile(Compiler *compiler);
};

struct LetSetExpr : Expr {
  Identifier ident;
  Expr *expr;

  explicit LetSetExpr(std::string str, Expr *ex)
      : Expr(), ident(str), expr(ex) {}

  void Compile(Compiler *compiler) override;
};

struct LetGetExpr : Expr {
  Identifier ident;

  explicit LetGetExpr(std::string str)
      : Expr(), ident(std::move(str)) {}

  void Compile(Compiler *compiler);
};

struct IfElseExpr : Expr {
  Expr *cond;
  Expr *then;
  std::optional<Expr *> else_;

  explicit IfElseExpr(Expr *cond, Expr *then, std::optional<Expr *> else_)
      : Expr(), cond(cond), then(then), else_(else_) {}

  void Compile(Compiler *compiler) override;
};

struct BlockExpr : Expr {
  std::vector<Expr *> exprs;

  explicit BlockExpr(std::vector<Expr *> exprs)
      : Expr(), exprs(std::move(exprs)) {}

  void Compile(Compiler *compiler) override;
};

struct FunExpr : Expr {
  Identifier name;
  std::vector<Identifier> params;
  std::vector<Expr *> exprs;
  // TODO: Params.

  explicit FunExpr(std::string name, std::vector<Identifier> params, std::vector<Expr *> exprs)
      : Expr(), name(std::move(name)), params(std::move(params)), exprs(std::move(exprs)) {}

  void Compile(Compiler *compiler) override;
};

struct PrintExpr : Expr {
  Expr *expr;

  explicit PrintExpr(Expr *ex) : Expr() { expr = ex; }

  void Compile(Compiler *compiler) override;
};

struct LiteralExpr : Expr {
  Value value;

  explicit LiteralExpr(Value v) : value(v) {};

  void Compile(Compiler *compiler) override;
};

struct CallExpr : Expr {
  Expr *callee;
  std::vector<Expr *> args;

  CallExpr(Expr *callee, std::vector<Expr *> args)
      : Expr(), callee(callee), args(std::move(args)) {}

  void Compile(Compiler *compiler) override;
};

struct ReturnExpr : Expr {
  std::optional<Expr *> expr;

  ReturnExpr(std::optional<Expr *> expr)
      : Expr(), expr(expr) {}

  void Compile(Compiler *compiler) override;
};