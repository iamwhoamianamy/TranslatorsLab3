#pragma once
#include <vector>
#include <string>
#include <iomanip>
#include "VarTableRow.h"

using namespace std;

class VarTable
{
public:
   vector<VarTableRow> table;

   // Создание пустой таблицы
   VarTable()
   {
      table = vector<VarTableRow>(0);
   }

   // Функция поиска номера строки таблицы по идентификатору,
   // возращает -1 в случае отсутствия строки с таким идентификатором в таблице
   int GetRowIndex(const VarTableRow& t_row)
   {
      for(size_t i = 0; i < table.size(); i++)
         if(table[i] == t_row)
            return i;

      return -1;
   }

   // Функция добавления строки в таблицу, если такого вхождения нет,
   // возвращает номер строки
   int AddRow(const VarTableRow& t_row)
   {
      int index = GetRowIndex(t_row);
      if(index == -1)
      {
         table.push_back(t_row);
         return table.size() - 1;
      }
      else
         return index;
   }

   // Функция, возвращающая 
   VarTableRow GetRow(const int& t_index)
   {
      if(t_index < table.size())
         return table[t_index];
      else
         printf_s("Error!");
   }

   void Output(const string& OUT_FILE)
   {
      ofstream fout(OUT_FILE);
      fout << "i   value   name  is set" << endl;
      for (size_t i = 0; i < table.size(); i++)
      {
         fout << i << setw(5) << table[i].value;
         fout << setw(8) << table[i].name;
         fout << setw(5) << table[i].is_set;
         fout << endl;
      }
      fout.close();
   }

   // Получение значений атрибутов
   int    GetValue(const int& t_index) { return table[t_index].value; }
   string GetName(const int& t_index)  { return table[t_index].name; }
   bool   GetIsSet(const int& t_index) { return table[t_index].is_set; }

   // Установление значений атрибутов
   void SetValue(const int& t_index, const int& t_value)   { table[t_index].value = t_value; }
   void SetName(const int& t_index, const string& t_name)  { table[t_index].name = t_name; }
   void SetIsSet(const int& t_index, const bool t_is_set) { table[t_index].is_set = t_is_set; }
};