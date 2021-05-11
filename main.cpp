#include <iostream>
#include "SyntaxAnalyzer.h"

using namespace std;

int main()
{
   LexicalAnalyzer la = LexicalAnalyzer();

   la.MakeTokens("test_1.txt", "tokens.txt");
   //la.PrintAllTables("tables");

   SyntaxlAnalyzer sa = SyntaxlAnalyzer();
   sa.readParseTable();
   
}