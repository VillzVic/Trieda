#include "GUtil.h"

const double GUtil::DBLINFINITE = 1e12;
const double GUtil::PRECISION = 1e-10;
const double GUtil::DECIMAL_DIGITS = 16;

bool GUtil::isINFINITE(double value)
{
   return !(value < DBLINFINITE && value > -DBLINFINITE);
}

bool GUtil::isZERO(double value)
{
   return !(value < -PRECISION || value > PRECISION);
}

std::string GUtil::intToString(int value)
{
   std::string str = "";
   char buffer[64];
   sprintf_s(buffer, sizeof(buffer), "%d", value);
   str += buffer;
   return str;
}

std::string GUtil::intToString(int value, int ndigits)
{
   return GUtil::intToString(value, ndigits, ' ', '0');
}

std::string GUtil::intToString(int value, int ndigits, char blank, char zero)
{
   std::string str = "";
   if (value == 0)
      str += zero;
   else
      str += intToString(value);

   int nzeros = ndigits - str.length();
   while ( nzeros > 0 )
   {
      str = blank + str;
      nzeros--;
   }
   return str;
}

std::string GUtil::doubleToString(double value)
{
   std::string str = "";
   char buffer[128];
   sprintf_s(buffer, sizeof(buffer), "%.18f", value);
   str += buffer;
   return str;
}

std::string GUtil::doubleToString(double value, int ndec)
{
   std::string str = "%.";
   char buffer[128];
   if ( isINFINITE(value) )
   {
      str = "INFINITE";
      if ( value < 0 )
         str = "-" + str;
   }
   else
   {
      if ( value < PRECISION && value > -PRECISION )
         value = 0.0;
      sprintf_s(buffer, sizeof(buffer), "%d", ndec);
      str += buffer;
      str += "f";
      sprintf_s(buffer, sizeof(buffer), str.c_str(), value);
      str = "";
      str += buffer;
   }
   return str;
}

int GUtil::stringToInt(std::string value)
{
   int n;
   sscanf_s(value.c_str(), "%d", &n);
   return n;
}

double GUtil::stringToDouble(std::string value)
{
   double n;
   sscanf_s(value.c_str(), "%f", &n);
   return n;
}

void GUtil::fill(bool* v, int n, bool value)
{
   for (int i=0; i<n; i++)
      v[i] = value;
}

bool GUtil::or(bool* v, int n)
{
   for (int i=0; i<n; i++)
      if (v[i])
         return true;
   return false;
}

int GUtil::random(int n, int diff)
{
   int val;
   do
   {
      val = rand() % n;
   } while (val == diff);
   
   return val;
}

double GUtil::getDiffClock(clock_t endTime, clock_t iniTime)
{
	return (endTime - iniTime) / (double)CLOCKS_PER_SEC;
}
