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
		int index = -1;

		fasm << ".386\n.model FLAT, C\n\n";

		while(getline(fpostfix, line))
		{
			vector<string> postfix;
			stringstream ss(line);

			while (ss >> temp)
				postfix.push_back(temp);

			if (postfix.size() == 3 && la.GetSymbolType(postfix[0], index) == SymbolType::Letter &&
				la.GetSymbolType(postfix[1], index) == SymbolType::Number && postfix[2] == "=")
			{
				index = la.var_table.GetIndexByName(postfix[0]);
				int a = stoi(postfix[1]);
				la.var_table.SetValue(index, a);
			}
			else
				for (int i = 0; i < postfix.size(); i++)
				{







					/*if (la.GetSymbolType(postfix[i], index) == SymbolType::Letter || la.GetSymbolType(postfix[i], index) == SymbolType::Number)
					{
						Stack.push(postfix[i]);
						bool added = false;

					}*/


				}
			while (!Stack.empty()) { Stack.pop(); }
		}
		fpostfix.close();

		fasm << ".data\n";
		for (int i = 0; i < la.var_table.table.size(); i++)
		{
			if (la.var_table.table[i].value == -1 || la.var_table.table[i].value == -2)
				fasm << "\t" << la.var_table.table[i].name << "\tdd\t?\n";
			else
				fasm << "\t" << la.var_table.table[i].name << "\tdd\t" << la.var_table.table[i].value << "\n";
		}
		fasm << "\n.code\nmain proc\n";
		asmCode << "\tmov\teax, 0\n\tret\n";
		asmCode << "main endp\n\nend main";
		fasm << asmCode.str();
		fasm.close();
	}
};
