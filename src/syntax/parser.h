#pragma once

#include <vector>
#include <map>
#include "lexer.h"
#include "ast.h"

enum Precedence : int {
  None,
  Assignment,  // =
  Record,      // ,
  Logical,     // and or
  Not,        // not
  Is,          // is
  Equality,    // == !=
  Comparison,  // < > <= >=
  Range,       // .. ...
  Term,        // + -
  Factor,      // * / %
  Prefix,      // any operator in prefix position
  Call         // infix () []
};

class Parser {
  std::vector<Token> tokens;

  Expr *Declaration();
  Expr *DeclareLet();
  Expr *ParseIf();
  Expr *ParseBlock();
  Expr *ParseNumber(Token token);  // TODO: Rename.
  Expr *Literal(Token token);

  Token Consume();
  Token Consume(TokenType token_type, const std::string &message);
  bool Match(TokenType token_type);
  bool Check(TokenType token_type);
  TokenType PeekType();
  bool HasNext();
  Expr *ExprStatement();
  Expr *Expression();
  Expr *ParsePrecedence(int precedence);

  Expr *ParseIdent(Token token);
  Expr *ParsePrint();
  Expr *ParseReturn();

  Expr *Unary(Token token);
  Expr *Binary(Expr *left, Token token);
  Expr *Call(Expr *left, Token token);

  typedef Expr *(Parser::*PrefixParseFn)(Token token);
  typedef Expr *(Parser::*InfixParseFn)(Expr *left, Token token);

  struct Parselet {  // TODO: Rename?
    PrefixParseFn prefix;
    InfixParseFn infix;
    int precedence;
  };

  std::map<TokenType, Parselet> rules{
      {TokenType::Minus,        {&Parser::Unary,      &Parser::Binary, Precedence::Term}},
      {TokenType::Plus,         {nullptr,             &Parser::Binary, Precedence::Term}},
      {TokenType::Slash,        {nullptr,             &Parser::Binary, Precedence::Factor}},
      {TokenType::Star,         {nullptr,             &Parser::Binary, Precedence::Factor}},
      {TokenType::Equal,        {nullptr,             &Parser::Binary, Precedence::None}},
      {TokenType::EqualEqual,   {nullptr,             &Parser::Binary, Precedence::Equality}},
      {TokenType::Bang,         {&Parser::Unary,      nullptr,         Precedence::Term}},
      {TokenType::BangEqual,    {nullptr,             &Parser::Binary, Precedence::Comparison}},
      {TokenType::Greater,      {nullptr,             &Parser::Binary, Precedence::Comparison}},
      {TokenType::GreaterEqual, {nullptr,             &Parser::Binary, Precedence::Comparison}},
      {TokenType::Less,         {nullptr,             &Parser::Binary, Precedence::Comparison}},
      {TokenType::LessEqual,    {nullptr,             &Parser::Binary, Precedence::Comparison}},
      {TokenType::Semicolon,    {nullptr,             nullptr,         Precedence::None}},
      {TokenType::Identifier,   {&Parser::ParseIdent, nullptr,         Precedence::None}},
      {TokenType::True,         {&Parser::Literal,    nullptr,         Precedence::None}},
      {TokenType::False,        {&Parser::Literal,    nullptr,         Precedence::None}},
      {TokenType::Number,       {&Parser::ParseNumber, nullptr, Precedence::None}},
      {TokenType::LeftParen,    {nullptr,             &Parser::Call,   Precedence::Call}},
  };

public:
  std::vector<Expr *> Parse();

  Parser(std::vector<Token> tks) {
    std::reverse(tks.begin(), tks.end());

    tokens = tks;
  }

  Expr *ParseFun();
};
