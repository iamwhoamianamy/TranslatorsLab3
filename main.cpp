#include <iostream>
#include "SyntaxAnalyzer.h"
#include "CodeGenerator.h"

using namespace std;

int main()
{
   LexicalAnalyzer la = LexicalAnalyzer();
   la.MakeTokens("prog.txt", "tokens.txt");
   la.PrintAllTables("tables");

   SyntaxlAnalyzer sa = SyntaxlAnalyzer();
   sa.readParseTable("parsingTable.txt");
   if (sa.LL1("tokens.txt", "postfix.txt", "postfixSimple.txt", la))
   {
      la.PrintAllTables("tables");
      CodeGenerator cg = CodeGenerator();
      cg.CodeToAsm("code.asm", "postfixSimple.txt", la);
      la.PrintAllTables("tables");
   }
}