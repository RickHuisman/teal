#include "parser.h"
#include <iostream>

std::vector<Expr *> Parser::Parse() {
  std::vector<Expr *> exprs;

  while (HasNext() && !Check(TokenType::Eof)) {
    exprs.push_back(Declaration());
  }

  return exprs;
}

Expr *Parser::Declaration() {
  if (Match(TokenType::Let)) return DeclareLet();
  if (Match(TokenType::If)) return ParseIf();
  if (Match(TokenType::LeftBrace)) return ParseBlock();
  if (Match(TokenType::Print)) return ParsePrint();
  if (Match(TokenType::Fun)) return ParseFun();
  if (Match(TokenType::Return)) return ParseReturn();

  return ExprStatement();
}

Expr *Parser::DeclareLet() {
  auto ident = Consume(TokenType::Identifier, "Expect identifier.");
  Consume(TokenType::Equal, "Expect '=' after identifier.");

  // TODO Initializer var with null e.g. let x;.
  auto initializer = ExprStatement();
  return new LetAssignExpr(ident.source, initializer);
}

Expr *Parser::ParseIf() {
  auto condition = Expression();
  auto then = Declaration();

  std::optional<Expr *> else_;
  if (Match(TokenType::Else)) {
    else_ = Declaration();
  }

  return new IfElseExpr(condition, then, else_);
}

Expr *Parser::ParseBlock() {
  std::vector<Expr *> exprs;
  while (!Check(TokenType::RightBrace) && !Check(TokenType::Eof)) {
    exprs.push_back(Declaration());
  }

  Consume(TokenType::RightBrace, "Expect '}' after block.");

  return new BlockExpr(exprs);
}

Expr *Parser::ParsePrint() {
  auto expr = ExprStatement();
  return new PrintExpr(expr);
}

Expr *Parser::ParseFun() {
  auto ident = Consume(TokenType::Identifier, "TODO");

  // Parameters.
  Consume(TokenType::LeftParen, "TODO");
  std::vector<std::string> params;
  while (!Check(TokenType::RightParen) &&
      !Check(TokenType::Eof)) {
    auto param = Consume(TokenType::Identifier, "TODO");
    params.push_back(param.source);

    if (!Match(TokenType::Comma)) break;
  }

  Consume(TokenType::RightParen, "TODO");

  // Body.
  Consume(TokenType::LeftBrace, "TODO");

  std::vector<Expr *> exprs;
  while (!Match(TokenType::RightBrace)) {
    exprs.push_back(Declaration());
  }

  return new FunExpr(ident.source, params, exprs);
}

Expr *Parser::ParseReturn() {
  std::optional<Expr *> expr;
  if (!Match(TokenType::Semicolon)) {
    expr = ExprStatement();
  }

  return new ReturnExpr(expr);
}

Expr *Parser::ExprStatement() {
  auto expr = Expression();
  Consume(TokenType::Semicolon, "Expect ';' after expression.");
  return expr;
}

Expr *Parser::Expression() { return ParsePrecedence(Precedence::Assignment); }

Expr *Parser::ParsePrecedence(int precedence) {
  auto token = Consume();
  auto prefix = rules[token.type].prefix;

  if (prefix == nullptr) {
    std::cout << "Unexpected token '" << token.source << "'";
    std::exit(EXIT_FAILURE); // TODO: Error handling.
  }

  auto left = (this->*prefix)(token);

  while (precedence <= rules[PeekType()].precedence) {
    token = Consume();
    auto infix = rules[token.type].infix;
    left = (this->*infix)(left, token);
  }

  return left;
}

Expr *Parser::Unary(Token token) {
  auto expr = Expression();

  UnaryOperator op;
  if (token.type == TokenType::Minus) op = UnaryOperator::Negate;
  if (token.type == TokenType::Bang) op = UnaryOperator::Not;

  return new UnaryExpr(op, expr);
}

Expr *Parser::Binary(Expr *left, Token token) {
  Expr *right = ParsePrecedence(rules[token.type].precedence + 1);

  BinaryOperator op;
  if (token.type == TokenType::Minus) op = BinaryOperator::Subtract;
  if (token.type == TokenType::Plus) op = BinaryOperator::Add;
  if (token.type == TokenType::Slash) op = BinaryOperator::Divide;
  if (token.type == TokenType::Star) op = BinaryOperator::Multiply;
  if (token.type == TokenType::EqualEqual) op = BinaryOperator::Equal;
  if (token.type == TokenType::BangEqual) op = BinaryOperator::BangEqual;
  if (token.type == TokenType::Greater) op = BinaryOperator::GreaterThan;
  if (token.type == TokenType::GreaterEqual)
    op = BinaryOperator::GreaterThanEqual;
  if (token.type == TokenType::Less) op = BinaryOperator::LessThan;
  if (token.type == TokenType::LessEqual)
    op = BinaryOperator::LessThanEqual;

  return new BinaryExpr(left, op, right);
}

Expr *Parser::ParseNumber(Token token) {
  auto number = std::stod(token.source);
  return new LiteralExpr(std::make_unique<Value>((float(number))));
}

Expr *Parser::Call(Expr *left, Token token) {
  std::vector<Expr *> args;
  while (!Check(TokenType::RightParen) &&
      !Check(TokenType::Eof)) {
    args.push_back(Expression());

    if (!Match(TokenType::Comma)) break;
  }

  Consume(TokenType::RightParen, "TODO");

  return new CallExpr(left, args);
}

Expr *Parser::Literal(Token token) {
  if (token.type == TokenType::True)
    return new LiteralExpr(std::make_unique<Value>(true));
  if (token.type == TokenType::False)
    return new LiteralExpr(std::make_unique<Value>(false));
  throw std::exception();
}

Expr *Parser::ParseIdent(Token token) {
  auto ident = token.source;

  // Set let.
  if (Match(TokenType::Equal)) {
    auto expr = Expression();
    return new LetSetExpr(ident, expr);
  }

  // Get let.
  return new LetGetExpr(ident);
}

Token Parser::Consume(TokenType type, const std::string &message) {
  if (Check(type)) {
    return Consume();
  }

  std::cout << message;
  std::exit(EXIT_FAILURE);
}

Token Parser::Consume() {
  auto popped = tokens.back();
  tokens.pop_back();
  return popped;
}

bool Parser::Match(TokenType type) {
  if (!Check(type)) {
    return false;
  }

  Consume();
  return true;
}

bool Parser::Check(TokenType type) { return PeekType() == type; }

TokenType Parser::PeekType() {
  return HasNext() ? tokens.back().type : TokenType::Eof;
}

bool Parser::HasNext() { return !tokens.empty(); }