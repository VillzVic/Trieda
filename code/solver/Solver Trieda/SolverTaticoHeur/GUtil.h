#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>

class GUtil
{
public:
   static const double DBLINFINITE;
   static const double PRECISION;
   static const double DECIMAL_DIGITS;

   static bool isINFINITE(double value);
   static bool isZERO(double value);
   static std::string intToString(int value);
   static std::string intToString(int value, int ndigits);
   static std::string intToString(int value, int ndigits, char blank, char zero);
   static std::string doubleToString(double value);
   static std::string doubleToString(double value, int ndec);
   static int stringToInt(std::string value);
   static double stringToDouble(std::string value);
   static std::string allTrim(std::string str);

   static void fill(bool* v, int n, bool value);
   static bool or(bool* v, int n);

   static int random(int n, int diff);

   static double getDiffClock(clock_t endTime, clock_t iniTime);
};