#include "lexer.h"
#include <vector>

std::vector<Token> Lexer::Lex() {
  std::vector<Token> tokens;

  while (!IsAtEnd()) {
    auto token = ScanToken();
    if (token.has_value()) {
      tokens.push_back(token.value());
    }
  }

  return tokens;
}

std::optional<Token> Lexer::ScanToken() {
  SkipWhitespace();

  if (IsAtEnd()) return MakeToken(TokenType::Eof);

  auto c = Advance();

  if (isalpha(c)) return Identifier();
  if (isdigit(c)) return Number();

  switch (c) {
    case '(':
      return MakeToken(TokenType::LeftParen);
    case ')':
      return MakeToken(TokenType::RightParen);
    case '{':
      return MakeToken(TokenType::LeftBrace);
    case '}':
      return MakeToken(TokenType::RightBrace);
    case ';':
      return MakeToken(TokenType::Semicolon);
    case ',':
      return MakeToken(TokenType::Comma);
    case '.':
      return MakeToken(TokenType::Dot);
    case '-':
      return MakeToken(TokenType::Minus);
    case '+':
      return MakeToken(TokenType::Plus);
    case '/': {
      // Ignore comments.
      if (Check('/')) {
        // TODO: Cleanup.
        while (Peek() != '\n' && !IsAtEnd()) {
          Advance();
        }

        return std::nullopt;
      }

      return MakeToken(TokenType::Slash);
    }
    case '*':
      return MakeToken(TokenType::Star);
    case '!':
      return MakeToken(Match('=') ? TokenType::BangEqual : TokenType::Bang);
    case '=':
      return MakeToken(Match('=') ? TokenType::EqualEqual : TokenType::Equal);
    case '<':
      return MakeToken(Match('=') ? TokenType::LessEqual : TokenType::Less);
    case '>':
      return MakeToken(Match('=') ? TokenType::GreaterEqual : TokenType::Greater);
    case '"':
      return String();
  }

  return MakeToken(TokenType::Error);
}

TokenType Lexer::IdentifierType(const std::string &source) {
  if (source == "let") return TokenType::Let;
  if (source == "print") return TokenType::Print;
  if (source == "true") return TokenType::True;
  if (source == "false") return TokenType::False;
  if (source == "if") return TokenType::If;
  if (source == "else") return TokenType::Else;
  if (source == "fun") return TokenType::Fun;
  if (source == "return") return TokenType::Return;

  return TokenType::Identifier;
}

Token Lexer::Identifier() {
  int start = current - 1;
  while (isalpha(Peek()) || isdigit(Peek())) Advance();

  std::string identifier = source.substr(start, current - start);
  TokenType ident_type = IdentifierType(identifier);

  Token token(ident_type, identifier);
  return token;
}

Token Lexer::Number() {
  int start = current - 1;
  while (isdigit(Peek())) Advance();

  // Look for a fractional part.
  if (Peek() == '.' && isdigit(PeekNext())) {
    // Consume the ".".
    Advance();

    while (isdigit(Peek())) Advance();
  }

  auto token_source = source.substr(start, current - start);
  Token token(TokenType::Number, token_source);
  return token;
}

Token Lexer::String() {
  int start = current - 1;

  while (Peek() != '"' && !IsAtEnd()) {
    Advance();
  }

  // The closing quote.
  Advance();

  auto token_source = source.substr(start, current - start);
  Token token(TokenType::String, token_source);
  return token;
}

Token Lexer::MakeToken(TokenType token_type) {
  Token token(token_type, std::string(1, source[current - 1]));
  return token;
}

void Lexer::SkipWhitespace() {
  for (;;) {
    if (isspace(Peek())) {
      Advance();
    } else return;
  }
}

char Lexer::Match(char expected) {
  if (Peek() != expected) return false;

  Advance();
  return true;
}

char Lexer::Advance() {
  current += 1;
  return source[current - 1];
}

char Lexer::PeekNext() {
  return IsAtEnd() ? '\0' : source[current + 1];
}

char Lexer::Peek() {
  return IsAtEnd() ? '\0' : source[current];
}

bool Lexer::Check(char c) {
  if (!IsAtEnd()) {
    return Peek() == c;
  }

  return false;
}

bool Lexer::IsAtEnd() {
  return source[current] == '\0';
}
