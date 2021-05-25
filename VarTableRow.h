#pragma once
#include <string>

using namespace std;

class VarTableRow
{
public:
   bool value;
   string name;
   bool is_set;

   VarTableRow() {};

   VarTableRow(const bool& t_value, const string& t_name, const bool t_is_set) :
      value(t_value), name(t_name), is_set(t_is_set) {};

   bool operator == (VarTableRow lhs)
   {
      return value == lhs.value  && name == lhs.name && is_set == lhs.is_set;
   }
};