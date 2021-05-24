#pragma once
#include "LexicalAnalyzer.h"
#include <sstream>

class SyntaxlAnalyzer
{
public:
	SyntaxlAnalyzer()
	{

	};

	struct parsingTableRow
	{
		vector<string> terminals;
		int jump;
		int accept;
		int stack;
		int retrn;
		int error;
			
		parsingTableRow(vector<string> _terminals, int _jump, int _accept, int _stack, int _retrn, int _error)
		{
			terminals = _terminals;
			jump = _jump;
			accept = _accept;
			stack = _stack;
			retrn = _retrn;
			error = _error;
		}
	};

	vector<parsingTableRow> parsingTable;

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

	string readToken(int& tableNum, int& index, LexicalAnalyzer la)
	{
	
		switch (tableNum)
		{
		case 10:
			return la.key_words.GetRow(index);
		case 20:
			return la.operators.GetRow(index);
		case 30:
			return "var";
		case 40:
			return "int";
		default:
			break;
		}
	}

	bool LL1(const string& tokenFile, LexicalAnalyzer& la)
	{
		ifstream ftoken(tokenFile);

		int tableNum, index;
		char buf;
		string s;
		bool error = false;

		if (ftoken.peek() == EOF) { return true; }

		ftoken >> buf >> tableNum;
		ftoken >> buf >> index >> buf;

		s = readToken(tableNum, index, la);

		do
		{

		} while (ftoken.peek() != EOF && !error);

		

	}
};