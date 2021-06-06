#pragma once
#include "LexicalAnalyzer.h"
#include <sstream>
#include <stack>
#include <deque>
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

		string nameByToken(LexicalAnalyzer& la)
		{
			switch (tableNum)
			{
			case 10:
				return la.key_words.GetRow(index);
			case 20:
				return la.operators.GetRow(index);
			case 30:
				return la.var_table.GetName(index);
			case 40:
				return la.const_table.GetName(index);
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
	map<string, int> priority = { {"+", 2}, {"-",2}, {"*",3},{"=",0},{"==",1},{"!=",1 }, {"<",1}, {"/",3},{",",0} };

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

	struct tree
	{
		string elem;
		tree* left, * right;
		tree(string _elem = "@", tree* _left = NULL, tree* _right = NULL)
			: elem(_elem), left(_left), right(_right) {}
	};

	tree* buildTree(deque<string>& postfix)
	{
		string c;
		tree* t;

		c = postfix.back();
		postfix.pop_back();

		if (c == "*" || c == "/" || c == "+" || c == "-" || c == "=")
		{
			t = new tree(c);
			t->right = buildTree(postfix);
			t->left = buildTree(postfix);
			return t;
		}
		else
		{
			t = new tree(c);
			return t;
		}
	}

	bool equalTrees(tree* t1, tree* t2)
	{
		if (t1 == NULL && t2 == NULL)
			return true;
		if (t1->elem == t2->elem)
		{
			if(equalTrees(t1->left, t2->left) && equalTrees(t1->right, t2->right) || equalTrees(t1->left, t2->right) && equalTrees(t1->right, t2->left) && t1->elem == "+")
				return true;
			else return false;
		}
		else return false;
	}

	void standIn(tree** t)
	{
		tree* d = *t, * f1, * f2, * temp;
		bool isProcessed;
		stack<tree*> s;
		stack<bool> was;
		stack<int> count;

		do
		{
			while (d)
			{
				s.push(d);
				was.push(false);
				d = d->left;
			}
			if (!s.empty())
			{
				do
				{
					d = s.top();
					s.pop();
					isProcessed = was.top();
					was.pop();
					if (isProcessed)
					{
						

						if (d->elem == "/" && d->left->elem == "/")
						{
							d->right = new tree("*", d->left->right, d->right);
							d->left = d->left->left;
						}

						if (d->elem == "/" && d->right->elem == "/")
						{
							d->left = new tree("*", d->left, d->right->right);
							d->right = d->right->left;
						}

						if (d->elem == "+" || d->elem == "-")
						{
							if (d->left->elem == "*" && d->right->elem == "*")
							{
								f1 = d->left;
								f2 = d->right;
								if (equalTrees(f1->left, f2->left))
								{
									d->left = f1->left;
									d->right = new tree(d->elem, f1->right, f2->right);
									d->elem = "*";
								}
								else
									if (equalTrees(f1->left, f2->right))
									{
										d->left = f1->left;
										d->right = new tree(d->elem, f1->right, f2->left);
										d->elem = "*";
									}
									else
										if (equalTrees(f1->right, f2->left))
										{
											d->left = f1->right;
											d->right = new tree(d->elem, f1->left, f2->right);
											d->elem = "*";
										}
										else
											if (equalTrees(f1->right, f2->right))
											{
												d->left = f1->right;
												d->right = new tree(d->elem, f1->left, f2->left);
												d->elem = "*";
											}
							}
							
							if (d->left->elem == "/" && d->right->elem == "/")
							{
								f1 = d->left;
								f2 = d->right;
								if (equalTrees(f1->right, f2->right))
								{
									d->right = f1->right;
									d->left = new tree(d->elem, f1->left, f2->left);
									d->elem = "/";
								}
							}
						}
					}
						
				} while (isProcessed && !s.empty());
				if (!isProcessed)
				{
					s.push(d);
					was.push(true);
					d = d->right;
				}
			}
		} while (!s.empty());
	}

	void PostOrder(tree* t, deque<string>& postfix)
	{
		stack<tree*> s;
		stack<bool> was;
		bool isProcessed;

		do
		{
			while (t)
			{
				s.push(t);
				was.push(false);
				t = t->left;
			}
			if (!s.empty())
			{
				do
				{
					t = s.top();
					s.pop();
					isProcessed = was.top();
					was.pop();
					if (isProcessed)
						postfix.push_back(t->elem);
				} while (isProcessed && !s.empty());
				if (!isProcessed)
				{
					s.push(t);
					was.push(true);
					t = t->right;
				}
			}
		} while (!s.empty());
	};

	void simplification(deque<string>& postfix)
	{
		tree* t = buildTree(postfix);
		standIn(&t);
		PostOrder(t, postfix);
	}

	void postfix(ofstream& postfixFile, ofstream& postfixSimpl, vector<token>& infix, LexicalAnalyzer& la)
	{
		stack<string> op;
		string tmpStr, sCur;
		token tknCur;
		deque<string> postfix, postfixSimple;

		for (int i = 0; i < infix.size(); i++) 
		{
			tknCur = infix[i];
			if (tknCur.tableNum == 30 || tknCur.tableNum == 40)
				postfix.push_back(tknCur.nameByToken(la));
			else 
			{
				sCur = tknCur.nameByToken(la);

				if (sCur == "(")
					op.push(sCur);
				else
					if (sCur == ")")
					{
						while (op.top() != "(")
						{
							postfix.push_back(op.top());
							op.pop();
						}
						op.pop();
					}
					else
						if (op.empty() || priority[sCur] == 0 || priority[sCur] > priority[op.top()])
							op.push(sCur);
						else
						{
							while (priority[op.top()] >= priority[sCur])
							{
								postfix.push_back(op.top());
								op.pop();
							}
							op.push(sCur);
						}
			}
		}

		while (!op.empty()) 
		{
			postfix.push_back(op.top());
			op.pop();
		}

		while (!postfix.empty())
		{
			if (postfix.front() != ";")
			{
				postfixFile << postfix.front() << " ";
				postfixSimple.push_back(postfix.front());
			}
			postfix.pop_front();
		}

		simplification(postfixSimple);

		while (!postfixSimple.empty())
		{
			if (postfixSimple.front() != ";")
				postfixSimpl << postfixSimple.front() << " ";
			postfixSimple.pop_front();
		}
	}

	bool LL1(const string& tokenFile, const string& postfixFile, const string& postfixSimple, LexicalAnalyzer& la)
	{
		ifstream ftoken(tokenFile);
		ofstream fpostfix(postfixFile);
		ofstream fsimple(postfixSimple);
		token tknCur, tknNext;
		stack<int> states, m1, m2;
		int currState = 0, index = 0, if_count = 0;
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
								la.var_table.SetValue(tknCur.index, -2);
								infix.push_back(tknCur);
								OPZ = true;
							}

							sCur = sNext;
							tknCur = tknNext;
							sNext = "";
						}
					}
					else
					{
						if (currState == 25) // идентификатор в выражении
						{
							sCur = la.var_table.GetName(tknCur.index);
							if (!la.var_table.GetIsSet(tknCur.index)) //если не задан тип идентификатора
							{
								cout << "Error: Unknown identifier '" << sCur << "'!";
								return false;
							}
							if (la.var_table.GetValue(tknCur.index) == -1) //если не задано значение идентификатора
							{
								cout << "Error: Value of the variable '" << sCur << "' is not set!";
								return false;
							}
						}

						if (currState == 58) //объявление нескольких переменных
							if (OPZ)	
							{	
								postfix(fpostfix, fsimple, infix, la);
								fpostfix << endl;
								fsimple << endl;
								OPZ = false;
								infix.clear();
							}
						//if
						if (currState == 66) // ) в условии
						{
							if (OPZ) 
							{ 
								postfix(fpostfix, fsimple, infix, la);
								OPZ = false;
								infix.clear();
								index++;
								m1.push(index);
								fpostfix << "m" << index << " CJF ";
								fsimple << "m" << index << " CJF ";
							}
						}

						if (currState == 69) // } в if
						{
							if (OPZ)
							{
								postfix(fpostfix, fsimple, infix, la);
								OPZ = false;
								infix.clear();
							}
						}

						if (currState == 85) // } в else
						{
							if (OPZ)
							{
								index++;
								m2.push(index);
								fpostfix << "m" << index << " UJ ";
								fpostfix << "m" << m1.top() << ": ";
								fsimple << "m" << index << " UJ ";
								fsimple << "m" << m1.top() << ": ";
								m1.pop();

								postfix(fpostfix, fsimple, infix, la);
								OPZ = false;
								infix.clear();
								fpostfix << "m" << m2.top() << ": " << endl;
								fsimple << "m" << m2.top() << ": " << endl;
								m2.pop();
								if_count--;
							}
						}

						if (OPZ)	{ infix.push_back(tknCur); }

						if (ftoken.peek() != EOF)
						{
							sNext = tknNext.readToken(ftoken, la);
							if (sNext == "==" || sNext == "!=" || sNext == "<") //идентификатор слева от == != <
							{
								infix.push_back(tknCur);
								OPZ = true;
								if_count++;
							}

							sCur = sNext;
							tknCur = tknNext;
							sNext = "";
						}
					}
				}

				if (currState == 81) // нет else
				{
					fpostfix << "m" << m1.top() << ": " << endl;
					fsimple << "m" << m1.top() << ": " << endl;
					m1.pop();
					if_count--;
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
								if (OPZ && if_count == 0)
								{ 
									postfix(fpostfix, fsimple, infix, la);
									fpostfix << endl;
									fsimple << endl;
									OPZ = false;
									infix.clear();
								}
						}
						else
						{
							if (currState != 8)
							{
								cout << "Syntax error: Stack is empty!";
								return false;
							}
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

		if (currState == 8 && la.operators.GetRow(tknCur.index) == "}")
			cout << "Success!";
		else
		{
			cout << "Error: Incorrect end of the program! Expected '}'";
			return false;
		}
		
		fpostfix.close();
		fsimple.close();
		ftoken.close();
		return true;
	}
	
};