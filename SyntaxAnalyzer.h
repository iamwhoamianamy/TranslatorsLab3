#pragma once
#include "LexicalAnalyzer.h"
#include <sstream>
#include <stack>
#include <queue>
#include <map>

using namespace std;

class SyntaxlAnalyzer
{
public:
	SyntaxlAnalyzer()
	{

	};

	struct token
	{
		int tableNum, index;
		char buf;

		string readToken(ifstream& ftoken, LexicalAnalyzer la)
		{
			ftoken >> buf >> tableNum;
			ftoken >> buf >> index >> buf;
			switch (tableNum)
			{
			case 10:
				return la.key_words.GetRow(index);
			case 20:
				return la.operators.GetRow(index);
			case 30:
				return "v_name";
			case 40:
				if (la.const_table.GetName(index) == "0") return "0";
				return "const";
			default:
				break;
			}
		}
	};


	struct parsingTableRow
	{
		vector<string> terminals;
		int jump, accept, stack, retrn, error;
			
		parsingTableRow(vector<string> _terminals, int _jump, int _accept, int _stack, int _retrn, int _error)
		{
			terminals = _terminals;
			jump = _jump;
			accept = _accept;
			stack = _stack;
			retrn = _retrn;
			error = _error;
		}

		bool isExist(const string& terminal)
		{
			for (size_t i = 0; i < terminals.size(); i++)
				if (terminals[i] == terminal)
					return true;
			return false;
		}
	};

	vector<parsingTableRow> parsingTable;
	map<string, int> priority = { {"+", 2}, {"-",2}, {"*",3},{"=",0},{"==",1},{"!=",1 }, {"<",1}, {">",1},{",",0} };

	void readParseTable(const string& ParseTableFile)
	{
		ifstream fin(ParseTableFile);
		string line, temp;

		while (getline(fin, line))
		{
			vector<string> str;
			vector<string> terminals;
			stringstream ss(line);
			
			while (ss >> temp)
				str.push_back(temp);
			
			int i = 0;
			for (i; i < str.size() - 5; i++)
				terminals.push_back(str[i]);

			parsingTableRow row(terminals, stoi(str[i]), stoi(str[i+1]), stoi(str[i+2]), stoi(str[i+3]), stoi(str[i+4]));
			parsingTable.push_back(row);
		}
		fin.close();
	}

	void postfix(ofstream& postfixFile, vector<token>& infix, LexicalAnalyzer& la)
	{
		stack<string> op;
		string tmpStr, sCur;
		token tknCur;
		queue<string> postfix;
		for (int i = 0; i < infix.size(); i++) 
		{
			tknCur = infix[i];
			if (tknCur.tableNum == 30) 
				postfix.push(la.var_table.GetName(tknCur.index));
			else 
				if (tknCur.tableNum == 40)
					postfix.push(la.const_table.GetName(tknCur.index));
				else 
				{
					sCur = la.operators.GetRow(tknCur.index);
					if (sCur == "(")
						op.push(sCur);
					else 
					{
						if (sCur == ")")
						{
							while (op.top() != "(")
							{
								postfix.push(op.top());
								op.pop();
							}
							op.pop();
						}
						else 
						{
							if (op.empty() || op.top() == "(")
								op.push(sCur);
							else {
								int p_in = priority[sCur];
								int p_top = priority[op.top()];
								if (p_in > p_top) { op.push(sCur); }
								else 
								{
									while ((op.top() != "(" || priority[op.top()] >= p_in)) {
										postfix.push(op.top());
										op.pop();
										if (op.empty()) break;
									}
									op.push(sCur);
								}
							}
						}
					}
				}
		}

		while (!op.empty()) {
			postfix.push(op.top());
			op.pop();
		}

		while (!postfix.empty()) {
			postfixFile << postfix.front() << " ";
			postfix.pop();
		}
		postfixFile << endl;
	}

	bool LL1(const string& tokenFile, const string& postfixFile, LexicalAnalyzer& la)
	{
		ifstream ftoken(tokenFile);
		ofstream fpostfix(postfixFile);
		token tknCur, tknNext;
		stack<int> states;
		int currState = 0;
		string sCur, sNext;
		vector<token> infix;
		bool OPZ = false;

		if (ftoken.peek() == EOF) { return true; } 

		sCur = tknCur.readToken(ftoken, la);

		do
		{
			if (parsingTable[currState].isExist(sCur)) 
			{
				if (parsingTable[currState].accept) 
				{
					if (currState == 15 || currState == 48) //объявление или идентификатор в присваивании
					{
						sCur = la.var_table.GetName(tknCur.index);
						if (currState == 15) //идентификатор в присваивании
						{
							if (!la.var_table.GetIsSet(tknCur.index)) //если не задан тип переменной
							{
								cout << "Error: Unknown identifier '" << sCur << "'!";
								return false;
							}
						}
						else //объявление
						{
							if (la.var_table.GetIsSet(tknCur.index)) //если тип переменной уже задан
							{
								cout << "Error: redescribing the type of a variable '" << sCur << "'!";
								return false;
							}
							else	{	la.var_table.SetIsSet(tknCur.index, 1);	}
						}
						if (ftoken.peek() != EOF)
						{
							sNext = tknNext.readToken(ftoken, la);

							if (sNext == "=") //идентификатор слева от =
							{
								la.var_table.SetValue(tknNext.index, 1);
								infix.push_back(tknCur);
								OPZ = true;
							}
							sCur = sNext;
							tknCur = tknNext;
						}
					}
					else
					{
						if (currState == 25) // идентификатор в выражении
						{
							if (!la.var_table.GetIsSet(tknCur.index)) //если не задан тип идентификатора
							{
								cout << "Error: Unknown identifier '" << sCur << "'!";
								return false;
							}
							if (!la.var_table.GetValue(tknCur.index)) //если не задано значение идентификатора
							{
								cout << "Error: Value of the variable " << sCur << "' is not set!";
								return false;
							}
						}

						if (currState == 58) //объявление нескольких переменных
						{
							if (OPZ)	{	postfix(fpostfix, infix, la);	}
							OPZ = false;
							infix.clear();
						}


						//if

						if (OPZ)	{ infix.push_back(tknCur); }

						if (ftoken.peek() != EOF)
							sCur = tknCur.readToken(ftoken, la);
					}
				}

				if (parsingTable[currState].stack)
					states.push(currState + 1);

				if (parsingTable[currState].jump > 0)
					currState = parsingTable[currState].jump - 1;
				else
				{
					if (parsingTable[currState].retrn)
					{
						if (!states.empty())
						{
							currState = states.top();
							states.pop();
							if (currState == 18 || currState == 46) //;
							{
								if (OPZ) { postfix(fpostfix, infix, la); }
								OPZ = false;
								infix.clear();
							}
						}
						else
						{
							cout << "Syntax error: Stack is empty!";
							return false;
						}
					}
				}
			}
			else //если символа нет в столбце terminal
			{
				if (parsingTable[currState].error) 
				{
					cout << "Error: Unexpected symbol! Possible symbols: ";
					for (size_t i = 0; i < parsingTable[currState].terminals.size(); i++)
						cout << "'" << parsingTable[currState].terminals[i] << "' ";
					return false;
				}
				else 
					currState++;
			}
		} while (ftoken.peek() != EOF);

		if (currState != 9)
		{
			cout << "Error: Incorrect end of the program! Expected '}'";
			return false;
		}
		else
			cout << "Success!";
		
		fpostfix.close();
		ftoken.close();
		return true;
	}
};