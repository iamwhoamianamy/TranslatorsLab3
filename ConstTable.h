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

   // Создание пустой таблицы
   ConstTable()
   {
      table = vector<ConstTableRow>(0);
   }

   // Создание таблицы с ключевыми словами
   void FillKeyWords()
   {
      const int k = 5;
      table.resize(k);
      string key_words[k] = {"if", "else", "main", "return", "int"};
      for(size_t i = 0; i < k; i++)
         table[i] = ConstTableRow(key_words[i]);
   }

   // Создание таблицы с операторами
   void FillOperators()
   {
      const int k = 14;
      table.resize(k);
      string operators[k] = { "=", "+", "-", "*" , "/", "==", "!=", "<", "(", ")", "{", "}", ",", ";"};
      for(size_t i = 0; i < k; i++)
         table[i] = ConstTableRow(operators[i]);
   }

   // Создание таблицы со всеми символами алфавита языка
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

   // Создание таблицы со всеми символами алфавита языка с которых
   // могут начинаться идентификаторы
   void FillIdentName()
   {
      table.resize(1 + 26 + 26);

      for (int i = 0; i < 26; i++)
         table[i] = ConstTableRow(string(1, (char)('a' + i)));

      for (int i = 0; i < 26; i++)
         table[i + 26] = ConstTableRow(string(1, (char)('A' + i)));

      table[52] = ConstTableRow(string(1, (char)('_')));
   }

   // Создание таблицы со всеми цифрами алфавита языка
   void FillNumbers()
   {
      table.resize(10);

      for(int i = 0; i < 10; i++)
         table[i] = ConstTableRow(string(1, (char)('0' + i)));
   }

   // Функция поиска номера строки таблицы по идентификатору,
   // возращает -1 в случае отсутствия строки с таким идентификатором в таблице
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