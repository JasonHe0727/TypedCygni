#include <iostream>
#include <locale>
#include "StringBuilder.h"
#include "Test.h"

using namespace std;

int main(int argc, char *argv[])
{
	locale loc("");
	locale::global(loc);

	// TestStringBuilder();
	// TestLexer();
	// TestType();
	// TestParser();
	// TestScope();
	// TestTypeChecker();
	TestCompiler();
	cin.get();
	return 0;
}
