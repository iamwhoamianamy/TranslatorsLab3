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

	void readParseTable() 
	{
		ifstream fin("parsingTable.txt");
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


};