#ifndef MAIN_MIP_PURO
#define MAIN_MIP_PURO

class CmdLine;
class ProblemSolution;
class ProblemData;
class ProblemDataLoader;

int mainMIPPuro( CmdLine *cmd, ProblemData* data, ProblemDataLoader* dataLoader, ProblemSolution* &solution );
void initSolution( ProblemSolution*& solution, ProblemData* data );

#endif