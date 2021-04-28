#pragma once
#pragma once
#include <vector>
#include <string>
#include <iomanip>
#include "ConstTableRow.h"

using namespace std;

class ConstTable
{
public:
   vector<ConstTableRow> table;

   // �������� ������ �������
   ConstTable()
   {
      table = vector<ConstTableRow>(0);
   }

   // �������� ������� � ��������� �������
   void FillKeyWords()
   {
      const int k = 5;
      table.resize(k);
      string key_words[k] = {"if", "else", "main", "return", "int"};
      for(size_t i = 0; i < k; i++)
         table[i] = ConstTableRow(key_words[i]);
   }

   // �������� ������� � �����������
   void FillOperators()
   {
      const int k = 14;
      table.resize(k);
      string operators[k] = { "=", "+", "-", "*" , "/", "==", "!=", "<", "(", ")", "{", "}", ",", ";"};
      for(size_t i = 0; i < k; i++)
         table[i] = ConstTableRow(operators[i]);
   }

   // �������� ������� �� ����� ��������� �������� �����
   void FillAplhabet()
   {
      const int k = 15;
      table.resize(k + 26 + 26 + 10);
      string operators[k] = { "=", "+", "-", "*", "/", "=", "!", "<", "(", ")", "{", "}", ",", ";", "_" };
      for(size_t i = 0; i < k; i++)
         table[i] = ConstTableRow(operators[i]);

      for(int i = 0; i < 26; i++)
         table[i + k] = ConstTableRow(string(1, (char)('a' + i)));

      for(int i = 0; i < 26; i++)
         table[i + k + 26] = ConstTableRow(string(1, (char)('A' + i)));

      for (int i = 0; i < 10; i++)
         table[i + k + 26 + 26] = ConstTableRow(string(1, (char)('0' + i)));
   }

   // �������� ������� �� ����� ��������� �������� ����� � �������
   // ����� ���������� ��������������
   void FillIdentName()
   {
      table.resize(1 + 26 + 26);

      for (int i = 0; i < 26; i++)
         table[i] = ConstTableRow(string(1, (char)('a' + i)));

      for (int i = 0; i < 26; i++)
         table[i + 26] = ConstTableRow(string(1, (char)('A' + i)));

      table[52] = ConstTableRow(string(1, (char)('_')));
   }

   // �������� ������� �� ����� ������� �������� �����
   void FillNumbers()
   {
      table.resize(10);

      for(int i = 0; i < 10; i++)
         table[i] = ConstTableRow(string(1, (char)('0' + i)));
   }

   // ������� ������ ������ ������ ������� �� ��������������,
   // ��������� -1 � ������ ���������� ������ � ����� ��������������� � �������
   int GetRowIndex(const ConstTableRow& t_row)
   {
      for(size_t i = 0; i < table.size(); i++)
         if(table[i] == t_row)
            return i;

      return -1;
   }

   void Output(const string& OUT_FILE)
   {
      ofstream fout(OUT_FILE);
      fout << "i    name" << endl;
      for (size_t i = 0; i < table.size(); i++)
      {
         fout << setw(2) << i;
         fout << setw(10) << table[i].name;
         fout << endl;
      }
      fout.close();
   }
};