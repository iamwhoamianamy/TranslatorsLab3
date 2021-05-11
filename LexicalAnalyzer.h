#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "VarTable.h"
#include "ConstTable.h"

enum class WordType
{
   Blank,
   Operator,
   Word,
   Constant
};

enum class SymbolType
{
   Separator,
   Operator,
   Letter,
   Number,
   Error
};

class LexicalAnalyzer
{
public:
   ConstTable alphabet, key_words, operators, numbers, ident_name;
   VarTable var_table, const_table;

   LexicalAnalyzer()
   {
      alphabet.FillAplhabet();
      key_words.FillKeyWords();
      operators.FillOperators();
      numbers.FillNumbers();
      ident_name.FillIdentName();
   }

   // Определение типа символа и получение его индекса
   // в соответствующей таблице
   SymbolType GetSymbolType(const string& s, int& place)
   {
      // Разделитель
      if(s == " " || s == "\n" || s == "\t")
         return SymbolType::Separator;

      // Ошибка
      place = alphabet.GetRowIndex(ConstTableRow(s));
      if(place == -1)
         return SymbolType::Error;

      // Оператор
      place = operators.GetRowIndex(ConstTableRow(s));
      if(place != -1)
         return SymbolType::Operator;

      // Символ с которого может начинаться имя переменной
      place = ident_name.GetRowIndex(ConstTableRow(s));
      if(place != -1)
         return SymbolType::Letter;

      // Цифра
      place = numbers.GetRowIndex(ConstTableRow(s));
      if(place != -1)
         return SymbolType::Number;
   }

   // Печать всех таблиц
   void PrintAllTables(const string& directory)
   {
      alphabet.Output(directory + "/aplhabet.txt");
      key_words.Output(directory + "/keyWords.txt");
      operators.Output(directory + "/operators.txt");
      numbers.Output(directory + "/numbers.txt");
      ident_name.Output(directory + "/ident_name.txt");
      const_table.Output(directory + "/const.txt");
      var_table.Output(directory + "/var.txt");
   }

   void MakeTokens(const string& in_filename, const string& out_filename)
   {
      ifstream fin(in_filename);
      ofstream fout(out_filename);

      int symbol_n = 0, line_n = 1;
      char c;
      string word = "", symbol;

      // Тип передыдущего слова
      WordType word_type = WordType::Blank;

      // Тип символа
      SymbolType symbol_type;

      // Место символа в соответствующей таблице
      int place = 0;

      // Место предыдущего символа в соответствующей таблице
      int prev_place = 0;

      // Если комментирование оператором */
      bool is_op_comment = false;
      string prev_symbol;

      // Если комментирование оператором //
      bool is_line_comment = false;

      while(fin.get(c))
      {
         symbol = c;
         symbol_n++;
         symbol_type = GetSymbolType(symbol, place);

         if(symbol_type == SymbolType::Error)
         {
            cout << "Error at line " << line_n << " pos " << symbol_n;
            cout << ": Invalid symbol! ";
            exit(2);
         }

         if(is_op_comment)
         {
            string temp_s = prev_symbol + symbol;
            if(temp_s == "*/")
               is_op_comment = false;
            else
               prev_symbol = symbol;
         }
         else if(is_line_comment)
         {
            if(symbol == "\n")
               is_line_comment = false;
         }
         else
            switch(word_type)
            {
               // Слово не задано
               case WordType::Blank:
               {
                  switch(symbol_type)
                  {
                     case SymbolType::Separator:
                     {
                        if(symbol == "\n")
                        {
                           symbol_n = 0;
                           line_n++;
                           fout << endl;
                        }
                        word_type = WordType::Blank;
                        break;
                     }
                     case SymbolType::Operator:
                     {
                        prev_place = place;
                        word = symbol;
                        word_type = WordType::Operator;
                        break;
                     }
                     case SymbolType::Letter:
                     {
                        word = symbol;
                        word_type = WordType::Word;
                        break;
                     }
                     case SymbolType::Number:
                     {
                        word = symbol;
                        word_type = WordType::Constant;
                        break;
                     }
                  }
                  break;
               }
               // Слово - оператор
               case WordType::Operator:
               {
                  switch(symbol_type)
                  {
                     // Символ - разделитель
                     case SymbolType::Separator:
                     {
                        fout << "(20," << place << ")";
                        word_type = WordType::Blank;
                        word = "";

                        if(symbol == "\n")
                        {
                           symbol_n = 0;
                           fout << endl;
                           line_n++;
                        }
                        break;
                     }
                     // Символ - оператор
                     case SymbolType::Operator:
                     {
                        string temp_op = word + symbol;

                        if(temp_op == "/*")
                        {
                           word_type = WordType::Blank;
                           word = "";
                           is_op_comment = true;
                           break;
                        }
                        if(temp_op == "//")
                        {
                           word_type = WordType::Blank;
                           word = "";
                           is_line_comment = true;
                           break;
                        }

                        int temp_place = operators.GetRowIndex(ConstTableRow(temp_op));

                        // Если оператор - "==" или "!="
                        if(temp_place != -1)
                        {
                           fout << "(20," << temp_place << ")";
                           word_type = WordType::Blank;
                           word = "";
                        }
                        else if(temp_op == "()")
                        {

                           fout << "(20," << prev_place << ")";
                           word_type = WordType::Operator;
                           word = symbol;
                           prev_place = place;
                        }
                        else
                        {
                           cout << "Error at line " << line_n << " pos " << symbol_n;
                           cout << ": Invalid operator! ";
                           exit(2);
                           break;
                        }
                        break;
                     }
                     // Символ - буква
                     case SymbolType::Letter:
                     {
                        fout << "(20," << place << ")";
                        word_type = WordType::Word;
                        word = symbol;
                        break;
                     }
                     // Символ - цифра
                     case SymbolType::Number:
                     {
                        fout << "(20," << place << ")";
                        word_type = WordType::Constant;
                        word = symbol;
                        break;
                     }
                  }
                  break;
               }
               // Слово - слово
               case WordType::Word:
               {
                  switch(symbol_type)
                  {
                     // Символ - разделитель
                     case SymbolType::Separator:
                     {
                        int kw_place = key_words.GetRowIndex(ConstTableRow(word));

                        // Если слово - ключеваое слово
                        if(kw_place != -1)
                           fout << "(10," << kw_place << ")";
                        else
                           fout << "(30," << var_table.AddRow(VarTableRow(0, word, false)) << ")";

                        word_type = WordType::Blank;
                        word = "";

                        if(symbol == "\n")
                        {
                           symbol_n = 0;
                           line_n++;
                           fout << endl;
                        }
                        break;
                     }
                     // Символ - оператор
                     case SymbolType::Operator:
                     {
                        int kw_place = key_words.GetRowIndex(ConstTableRow(word));

                        // Если слово - ключеваое слово
                        if(kw_place != -1)
                           fout << "(10," << kw_place << ")";
                        else
                           fout << "(30," << var_table.AddRow(VarTableRow(0, word, false)) << ")";

                        word_type = WordType::Operator;
                        prev_place = place;
                        word = symbol;
                        break;
                     }
                     // Символ - буква
                     case SymbolType::Letter:
                     {
                        word += symbol;
                        break;
                     }
                     // Символ - цифра
                     case SymbolType::Number:
                     {
                        word += symbol;
                        break;
                     }
                  }
                  break;
               }
               // Слово - константа
               case WordType::Constant:
               {
                  switch(symbol_type)
                  {
                     // Символ - разделитель
                     case SymbolType::Separator:
                     {
                        fout << "(40," << const_table.AddRow(VarTableRow(0, word, false)) << ")";
                        word_type = WordType::Blank;
                        word = "";

                        if(symbol == "\n")
                        {
                           symbol_n = 0;
                           line_n++;
                           fout << endl;
                        }
                        break;
                     }
                     // Символ - оператор
                     case SymbolType::Operator:
                     {
                        fout << "(40," << const_table.AddRow(VarTableRow(0, word, false)) << ")";
                        word_type = WordType::Operator;
                        word = symbol;
                        prev_place = place;
                        break;
                     }
                     // Символ - буква
                     case SymbolType::Letter:
                     {
                        cout << "Error at line " << line_n << " pos " << symbol_n;
                        cout << ": Invalid constant (identifier)! ";
                        exit(2);
                        break;
                     }
                     // Символ - цифра
                     case SymbolType::Number:
                     {
                        word += symbol;
                        break;
                     }
                  }
                  break;
               }
            }
      }

      if(is_op_comment)
      {
         cout << "Unclosed comment!";
         exit(2);
      }

      fout.close();
      fin.close();
   }
};