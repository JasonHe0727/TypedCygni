#include "Lexer.h"

Lexer::Lexer(string path)
{
	this->stream = new wifstream(path);
	this->builder = new StringBuilder();
	this->line = 1;
	this->column = 1;
}

Lexer::~Lexer()
{
	this->stream->close();
	delete this->stream;
	delete this->builder;
}

vector<Token> Lexer::ReadAll()
{
	vector<Token> tokens;
	wstring prefix = L"=+-*/%><!{}()[].,;:";

	SkipSpaces();

	while (!IsEof())
	{
		wchar_t c = (wchar_t) Peek();
		int currentLine = line;
		int currentColumn = column;

		if (isdigit(c))
		{
			Token t = Digits();
			SetPosition(currentLine, currentColumn, t);
			tokens.push_back(t);
		}
		else if (c == L'"')
		{
			Token t = Text();
			SetPosition(currentLine, currentColumn, t);
			tokens.push_back(t);
		}
		else if (isalpha(c) || c == '_')
		{
			Token t = Identifier();
			SetPosition(currentLine, currentColumn, t);
			tokens.push_back(t);
		}
		else if (prefix.find(c) != wstring::npos)
		{
			Token t = Operator();
			SetPosition(currentLine, currentColumn, t);
			tokens.push_back(t);
		}
		else
		{
			throw LexicalException(line, column,
					L"unrecognizable token");
		}

		SkipSpaces();
	}
	tokens.push_back(Token(line, column, TokenKind::Eof, L"<EOF>"));
	return tokens;
}

inline int Lexer::Peek()
{
	return (int) stream->peek();
}

inline wchar_t Lexer::Read()
{
	return (wchar_t) stream->get();
}

inline bool Lexer::IsEof()
{
	return Peek() == -1;
}

void Lexer::Consume()
{
	wchar_t c = this->Read();
	if (c == L'\n')
	{
		this->builder->Append(c);
		this->line++;
		this->column = 1;
	}
	else
	{
		this->builder->Append(c);
		this->column++;
	}
}

void Lexer::Move()
{
	wchar_t c = this->Read();
	if (c == L'\n')
	{
		this->line++;
		this->column = 1;
	}
	else
	{
		this->column++;
	}
}

Token Lexer::Digits()
{
	builder->Clear();
	Consume();
	while (!IsEof() && isdigit(Peek()))
	{
		Consume();
	}

	if (Peek() == L'.')
	{
		return Decimals();
	}
	else if (tolower(Peek()) == L'e')
	{
		return Exponent();
	}
	else
	{
		return Token(TokenKind::Integer, builder->ToString());
	}
}

Token Lexer::Decimals()
{
	Consume();
	if (IsEof() || !isdigit(Peek()))
	{
		throw LexicalException(line, column,
				L"error occurs when reading decimals.");
	}
	else
	{
		while (!IsEof() && isdigit(Peek()))
		{
			Consume();
		}

		if (IsEof())
		{
			return Token(TokenKind::Float, builder->ToString());
		}
		else if (tolower(Peek()) == L'e')
		{
			return Exponent();
		}
		else
		{
			return Token(TokenKind::Float, builder->ToString());
		}
	}
}

Token Lexer::Exponent()
{
	Consume();
	if (Peek() == L'+' || Peek() == L'-')
	{
		Consume();
	}

	if (IsEof() || !isdigit(Peek()))
	{
		throw LexicalException(line, column,
				L"error occurs when reading exponent.");
	}
	else
	{
		while (!IsEof() && isdigit(Peek()))
		{
			Consume();
		}
		return Token(TokenKind::Float, builder->ToString());
	}
}

Token Lexer::Text()
{
	Move();
	builder->Clear();
	bool done = false;
	while (!IsEof() && !done)
	{
		if (Peek() == L'"')
		{
			Move();
			done = true;
		}
		else if (Peek() == L'\\')
		{
			EscapedChar();
		}
		else
		{
			Consume();
		}
	}
	return Token(TokenKind::String, builder->ToString());
}

Token Lexer::Identifier()
{
	builder->Clear();
	Consume();
	while (!IsEof() && (isalnum(Peek()) || Peek() == '_'))
	{
		Consume();
	}
	return Token(TokenKind::Name, builder->ToString());
}

Token Lexer::Operator()
{
	wchar_t first = (wchar_t) Peek();
	Move();
	int second = Peek();
	if (first == L'>' && second == L'=')
	{
		Move();
		return Token(TokenKind::GreaterThanOrEqual, L">=");
	}
	else if (first == L'<' && second == L'=')
	{
		Move();
		return Token(TokenKind::LessThanOrEqual, L"<=");
	}
	else if (first == L'=' && second == L'=')
	{
		Move();
		return Token(TokenKind::Equal, L"==");
	}
	else if (first == L'!' && second == L'=')
	{
		Move();
		return Token(TokenKind::NotEqual, L"!=");
	}
	else
	{
		switch(first)
		{
			case L'=':
				return Token(TokenKind::Assign, L"=");
			case L'+':
				return Token(TokenKind::Add, L"+");
			case L'-':
				return Token(TokenKind::Subtract, L"-");
			case L'*':
				return Token(TokenKind::Multiply, L"*");
			case L'/':
				return Token(TokenKind::Divide, L"/");
			case L'%':
				return Token(TokenKind::Modulo, L"%");

			case L'>':
				return Token(TokenKind::GreaterThan, L">");
			case L'<':
				return Token(TokenKind::LessThan, L"<");
			case L'{':
				return Token(TokenKind::LeftBrace, L"{");
			case L'}':
				return Token(TokenKind::RightBrace, L"}");
			case L'[':
				return Token(TokenKind::LeftBracket, L"[");
			case L']':
				return Token(TokenKind::RightBracket, L"]");
			case L'(':
				return Token(TokenKind::LeftParenthesis, L"(");
			case L')':
				return Token(TokenKind::RightParenthesis, L")");
			case L'.':
				return Token(TokenKind::Dot, L".");
			case L',':
				return Token(TokenKind::Comma, L",");
			case L';':
				return Token(TokenKind::Semicolon, L";");
			case L':':
				return Token(TokenKind::Colon, L":");
			default:
				throw LexicalException(line, column,
						L"error operator format");
		}
	}
}

void Lexer::EscapedChar()
{
	Move();
	if (IsEof())
	{
		throw LexicalException(line, column, L"error string format");
	}
	else
	{
		wchar_t c = (wchar_t) Peek();
		Move();
		switch(c)
		{
			case L'a':
				builder->Append(L'\a');
				return;
			case L'b':
				builder->Append(L'\b');
				return;
			case L'f':
				builder->Append(L'\f');
				return;
			case L'n':
				builder->Append(L'\n');
				return;
			case L'r':
				builder->Append(L'\r');
				return;
			case L't':
				builder->Append(L'\t');
				return;
			case L'v':
				builder->Append(L'\v');
				return;
			case L'\\':
				builder->Append(L'\\');
				return;
			case L'"':
				builder->Append(L'"');
				return;
			case L'?':
				builder->Append(L'\?');
				return;
			case L'0':
				builder->Append(L'\0');
				return;
			default:
				throw LexicalException(line, column,
						L"unsupported escaped char");
		}
	}
}

void Lexer::SkipSpaces()
{
	while (!IsEof() && isspace(Peek()))
	{
		Move();
	}
}


void Lexer::SetPosition(int currentLine, int currentColumn, Token& token)
{
	token.line = currentLine;
	token.column = currentColumn;

	if (token.kind == TokenKind::Name)
	{
		if (token.text == L"and")
		{
			token.kind = TokenKind::And;
		}
		else if (token.text == L"or")
		{
			token.kind = TokenKind::Or;
		}
		else if (token.text == L"not")
		{
			token.kind = TokenKind::Not;
		}
		else if (token.text == L"true")
		{
			token.kind = TokenKind::True;
		}
		else if (token.text == L"false")
		{
			token.kind = TokenKind::False;
		}
		else if (token.text == L"var")
		{
			token.kind = TokenKind::Var;
		}
		else if (token.text == L"def")
		{
			token.kind = TokenKind::Define;
		}
		else if (token.text == L"if")
		{
			token.kind = TokenKind::If;
		}
		else if (token.text == L"else")
		{
			token.kind = TokenKind::Else;
		}
		else if (token.text == L"while")
		{
			token.kind = TokenKind::While;
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
}
