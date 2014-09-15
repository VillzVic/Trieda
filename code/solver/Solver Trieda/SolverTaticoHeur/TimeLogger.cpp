#include "TimeLogger.h"

#include "GUtil.h"

std::hash_map<std::string, clock_t> TimeLogger::timeStart;
std::hash_map<std::string, clock_t> TimeLogger::elapsedTime;
std::hash_map<std::string, int> TimeLogger::timeCount;
std::hash_map<std::string, int> TimeLogger::iterCount;

void TimeLogger::start(std::string name)
{
   timeStart[name] = clock();
}

void TimeLogger::stop(std::string name)
{
   if (timeStart.find(name) == timeStart.end())
      return;

   clock_t startTime = timeStart[name];
   clock_t endTime = clock();
   timeStart[name] = 0;

   elapsedTime[name] += (endTime - startTime);
   timeCount[name]++;
}

double TimeLogger::getElapsedTime(std::string name)
{
   clock_t time = elapsedTime[name];
   return (time / (double)CLOCKS_PER_SEC);
}

void TimeLogger::inc(std::string name)
{
   iterCount[name]++;
}

void TimeLogger::summary()
{
   clock_t current = clock();
   std::cout << "[TimeLogger::summary]==================================================\n";
   for (std::hash_map<std::string, clock_t>::iterator it = elapsedTime.begin(); it != elapsedTime.end(); ++it)
   {
      std::string name = it->first;
      clock_t time = timeStart[name];
      int count = timeCount[name];
      clock_t avg = it->second / (clock_t)count;
      
      std::cout << " - " << name << ": " << GUtil::doubleToString(it->second / (double)CLOCKS_PER_SEC, 3) << " sec [" << count << " => avg: " << GUtil::doubleToString((double)avg / (double)CLOCKS_PER_SEC, 3) << " sec]";
      if (time > 0)
         std::cout << " { running: " << GUtil::doubleToString((current - time) / (double)CLOCKS_PER_SEC, 3) << " sec }";
      std::cout << "\n";
   }   
   std::cout << "-----------------------------------------------------------------------\n";
   for (std::hash_map<std::string, int>::iterator it = iterCount.begin(); it != iterCount.end(); ++it)
   {
      std::cout << " - " << it->first << ": " << it->second << "\n";
   }
   std::cout << "=======================================================================\n";
}
