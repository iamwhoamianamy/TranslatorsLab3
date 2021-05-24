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
   sa.LL1("tokens.txt", la);
   
  /* LLParse(tokenFile);

   ofstream postfix;
   postfix.open("postfix.txt");
   for (int i = 0; i < toPostfixFile.size(); i++)
      postfix << toPostfixFile[i] << " ";
   postfix.close();*/

}