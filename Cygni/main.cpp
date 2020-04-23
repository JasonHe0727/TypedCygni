#include "Expression.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Visitor.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;


void ViewTokens(const std::vector<cygni::Token> &tokens)
{
	for (const cygni::Token &token : tokens)
	{
		cout << cygni::Enum<cygni::Tag>::ToString(token.tag) << ": ";
		cout << token.text << endl;
	}
}

void CompileProgram(std::string path, std::string outputJsonPath)
{
	/* pass 1: read code in text format */
	std::string code{ cygni::ReadText(path) };
	std::u32string utf32Code = cygni::UTF8ToUTF32(code);
	cout << cygni::UTF32ToUTF8(utf32Code) << endl;
	cout << "Successfully load file!" << endl;

	/* pass 2: tokenize code */
	cygni::Lexer lexer(utf32Code);
	auto tokens = lexer.ReadAll();
	cout << "Complete lexical analysis!" << endl;
	ViewTokens(tokens);

	/* pass 3: parse the code and create an abstract syntax tree */
	auto document = std::make_shared<cygni::SourceDocument>(path, path);
	cygni::Parser parser(tokens, document);
	auto program = parser.ParseProgram();
	cout << "Complete Syntax Analysis!" << endl;

	/* pass 4: check and infer types of each node */
	cygni::TypeChecker typeChecker(program);
	cygni::ScopePtr scope = std::make_shared<cygni::Scope>();
	typeChecker.VisitProgram(scope);
	cout << "Complete Type Checking!" << endl;

	/* pass 5: collect local variables */
	cygni::LocalVariableCollector localVariableCollector;
	localVariableCollector.VisitProgram(program);
	cout << "Complete Local Variable Collection!" << endl;

	/* pass 6: locate variables */
	cygni::VariableLocator variableLocator;
	variableLocator.VisitProgram(program);
	cout << "Complete Local Variable Locatoring!" << endl;

	/* pass 7: collect constants */
	cygni::ConstantCollector constantCollector;
	constantCollector.VisitProgram(program);
	cout << "Complete Constant Collection!" << endl;

	/* pass 8: convert the abstract syntax tree to json format */
	cygni::AstToJsonSerialization astToJson;
	auto jsonObj = astToJson.VisitProgram(program);
	auto jsonText = jsonObj.dump();
	cygni::WriteText(outputJsonPath, jsonText);

	cout << "Output AST in JSON format!" << endl;
}

int main(int argc, char **argv)
{
	cout << "Hello Cygni!" << endl;
	try
	{
		CompileProgram("sample_code/factorial.cyg", "sample_code/factorial.json");
	}
	catch (cygni::ArgumentException &ex)
	{
		cout << cygni::UTF32ToUTF8(ex.message) << endl;
	}
	catch (cygni::NotImplementedException &ex)
	{
		cout << cygni::UTF32ToUTF8(ex.message) << endl;
	}
	catch (cygni::LexicalException &ex)
	{
		cout << "Syntax Error: (" << (ex.line + 1) << ", " << (ex.column + 1)
			<< ") " << cygni::UTF32ToUTF8(ex.message) << endl;
	}
	catch (cygni::ParserException &ex)
	{
		cout << "Syntax Error: (" << (ex.line + 1) << ", " << (ex.column + 1)
			<< ") " << cygni::UTF32ToUTF8(ex.message) << endl;
	}
	catch (cygni::TypeException &ex)
	{
		cout << "Type Error: (" << (ex.location.startLine + 1) << ", "
			<< (ex.location.startCol + 1) << ") " << cygni::UTF32ToUTF8(ex.message)
			<< endl;
	}
	catch (cygni::FileNotFoundException& ex)
	{
		cout << ex.message << endl;
	}
	cout << "Press any key to close..." << endl;
	cin.get();
	// cygni::Lexer lexer(U"var a = '\\x0058'");
	// try {
	// 	auto tokens = lexer.ReadAll();
	// 	for (const cygni::Token& token : tokens) {
	// 		cout << cygni::Enum<cygni::Tag>::ToString(token.tag) << ": ";
	// 		cout << token.text << endl;
	// 	}
	// } catch (cygni::LexicalException& ex) {
	// 	cout << "(" << ex.line << ", " << ex.column << "): ";
	// 	cout << ex.message << endl;
	// }
	// cout << "Press any key to close..." << endl;
	// cin.get();
	return 0;
}
