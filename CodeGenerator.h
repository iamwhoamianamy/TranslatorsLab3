#pragma once
#include "LexicalAnalyzer.h"
#include <stack>

class CodeGenerator
{
public:
	CodeGenerator()
	{
	};

	void CodeToAsm(const string& asmFile, const string& postfixFile, LexicalAnalyzer& la)
	{
		ifstream fpostfix(postfixFile);
		ofstream fasm(asmFile);
		stack<string> Stack;
		stringstream asmCode;
		string line, temp;
      vector<string> variableVector;
		int index = -1;

		fasm << ".386\n.model FLAT, C\n\n";

		while(getline(fpostfix, line))
		{
			vector<string> postfix;
			stringstream ss(line);

			while (ss >> temp)
				postfix.push_back(temp);

			if (postfix.size() == 3 && la.GetSymbolType(postfix[0], index) == SymbolType::Letter && postfix[2] == "=" &&
				la.GetSymbolType(postfix[1], index) != SymbolType::Letter && la.GetSymbolType(postfix[1], index) != SymbolType::Operator)
			{
				index = la.var_table.GetIndexByName(postfix[0]);
				int a = stoi(postfix[1]);
				la.var_table.SetValue(index, a);
				la.const_table.SetIsSet(la.const_table.GetIndexByName(postfix[1]), 1);
			}
			else
				for (int i = 0; i < postfix.size(); i++)
				{
               if (postfix[i].find("m") != string::npos)
               {
                  if (postfix[i].find(":") != string::npos)
                  {
                     asmCode << postfix[i] << endl;
                     continue;
                  }
                  else
                  {
                     asmCode << "\t" << "jmp " << postfix[i] << endl;
                     continue;
                  }
               }

               if (postfix[i] == "CJF" || postfix[i] == "UJ") { continue; }
					
               if (la.GetSymbolType(postfix[i], index) != SymbolType::Operator)
               {
                  Stack.push(postfix[i]);
                  bool added = false;
                  for (int j = 0; !added && j < (int)variableVector.size(); j++)
                  {
                     if (variableVector[j] == postfix[i])
                        added = true;
                  }
                  if (!added)
                     variableVector.push_back(postfix[i]);
               }
               else
               {
                  string oper1p, oper2p;
                  int type1 = 0, type2 = 0;

                  oper2p = Stack.top();
                  Stack.pop();
                  oper1p = Stack.top();
                  Stack.pop();

                  if (la.GetSymbolType(oper1p, index) == SymbolType::Letter)
                  {
                     if (postfix[i] != "=")
                        asmCode << "\tfild\t" << oper1p << "\n";
                  }
                  else 
                     if (postfix[i] != "=" && oper1p != "last")
                     {
                        la.const_table.SetIsSet(la.const_table.GetIndexByName(oper1p), 0);
                        asmCode << "\tfild\tconst_" << oper1p << "\n";
                     }

                  if (la.GetSymbolType(oper2p, index) == SymbolType::Letter)
                     asmCode << "\tfild\t" << oper2p << "\n";
                  else if (oper2p != "last")
                  {
                     la.const_table.SetIsSet(la.const_table.GetIndexByName(oper2p), 0);
                     asmCode << "\tfild\tconst_" << oper2p << "\n";
                  }

                  if (postfix[i] == "+")
                     asmCode << "\tfadd\n";
                  else if (postfix[i] == "-")
                  {
                     if (oper2p == "last" && oper1p != "last")
                        asmCode << "\tfsubr\n";
                     else
                        asmCode << "\tfsub\n";
                  }
                  else if (postfix[i] == "*")
                  {
                     asmCode << "\tfmul\n";
                  }
                  else if (postfix[i] == "/")
                  {
                     if (oper2p == "last" && oper1p != "last")
                        asmCode << "\tfdivr\n";
                     else
                        asmCode << "\tfdiv\n";
                  }
                  else if (postfix[i] == "==")
                  {
                     asmCode << "\tfcomp\n";
                     asmCode << "\tfstsw\tax\n\tsahf\n";
                     asmCode << "\tjne " << postfix[i + 1] << "\n";
                     i += 2;
                  }
                  else if (postfix[i] == "!=")
                  {
                     asmCode << "\tfcomp\n";
                     asmCode << "\tfstsw\tax\n\tsahf\n";
                     asmCode << "\tje " << postfix[i + 1] << "\n";
                     i += 2;
                  }
                  else if (postfix[i] == "<")
                  {
                     asmCode << "\tfcomp\n";
                     asmCode << "\tfstsw\tax\n\tsahf\n";
                     if (oper2p == "last" && oper1p != "last")
                        asmCode << "\tjae " << postfix[i + 1] << "\n";
                     else
                        asmCode << "\tjbe " << postfix[i + 1] << "\n";
                     i += 2;
                  }
                  else if (postfix[i] == "=")
                     asmCode << "\tfistp\t" << oper1p << "\n";

                  Stack.push("last");
               }
				}
			while (!Stack.empty()) { Stack.pop(); }
		}
		fpostfix.close();

		fasm << ".data\n";
		for (int i = 0; i < la.var_table.table.size(); i++)
		{
			if (la.var_table.table[i].value == -1 || la.var_table.table[i].value == -2)
				fasm << "\t" << la.var_table.table[i].name << "\t\tdd\t?\n";
			else
				fasm << "\t" << la.var_table.table[i].name << "\t\tdd\t" << la.var_table.table[i].value << "\n";
		}

		for(int i = 0; i < la.const_table.table.size() - 1; i++)
			if (la.const_table.table[i].is_set == false)
				fasm << "\tconst_" << la.const_table.table[i].name << "\tdd\t" << la.const_table.table[i].name << "\n";

		fasm << "\n.code\nmain proc\n";
		asmCode << "\tmov\t\teax, 0\n\tret\n";
		asmCode << "main endp\n\nend main";
		fasm << asmCode.str();
		fasm.close();
	}
};