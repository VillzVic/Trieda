#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>
#include <hash_map>

class TimeLogger
{
public:
   static void start(std::string name);
   static void stop(std::string name);
   static double getElapsedTime(std::string name);

   static void inc(std::string name);

   static void summary();

private:
   static std::hash_map<std::string, clock_t> timeStart;
   static std::hash_map<std::string, clock_t> elapsedTime;
   static std::hash_map<std::string, int> timeCount;

   static std::hash_map<std::string, int> iterCount;
};