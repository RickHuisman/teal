#pragma once

#include <string>
#include <utility>
#include <optional>

enum class TokenType {
  LeftParen,
  RightParen,
  LeftBrace,
  RightBrace,

  Comma,
  Dot,
  Minus,
  Plus,
  Slash,
  Star,

  Bang,
  BangEqual,
  Equal,
  EqualEqual,
  Greater,
  GreaterEqual,
  Less,
  LessEqual,

  Identifier,
  Number,

  Let,
  Print,
  True,
  False,
  If,
  Else,
  Def,
  Do,
  End,
  Return,

  Line,
  Error,
  Eof,
};

struct Token {
  TokenType type;
  std::string source;

  Token(TokenType type, std::string source)
      : type(type), source(std::move(source)) {}
};

bool operator==(const Token &a, const Token &b);

class Lexer {
  int current;
  std::string source;

  std::optional <Token> ScanToken();
  static TokenType IdentifierType(const std::string &identifier);
  Token Identifier();
  Token Number();
  Token MakeToken(TokenType type);
  void SkipWhitespace();
  char Match(char expected);
  char Advance();
  char PeekNext();
  char Peek();
  bool Check(char c);
  bool IsAtEnd();

public:
  explicit Lexer(std::string source) : source(std::move(source)), current(0) {}

  std::vector <Token> Lex();
};
