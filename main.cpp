#include <iostream>
#include "SyntaxAnalyzer.h"

using namespace std;

int main()
{
   LexicalAnalyzer la = LexicalAnalyzer();

   la.MakeTokens("prog.txt", "tokens.txt");
   la.PrintAllTables("tables");

   SyntaxlAnalyzer sa = SyntaxlAnalyzer();
   sa.readParseTable("parsingTable.txt");
   sa.LL1("tokens.txt", "postfix.txt", "postfixSimple.txt", la);

   la.PrintAllTables("tables");
}