#pragma once
#include <string>

using namespace std;

class ConstTableRow
{
public:
   string name;
   ConstTableRow() {};

   ConstTableRow(const string& t_name) :
      name(t_name) {};

   bool operator == (const ConstTableRow& lhs)
   {
      return name == lhs.name;
   }
};