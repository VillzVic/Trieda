#pragma once

#include "Curso.h"
#include "Disciplina.h"
#include "Curriculo.h"

#include "ProblemData.h"

#include "Matrix.h"

#include <map>
#include <vector>

using namespace std;

class IS_Curso
{
public:
   IS_Curso(Curso * curso, ProblemData * problem_Data);

   IS_Curso(IS_Curso const & is_curso);

   virtual ~IS_Curso(void);

   ProblemData * problem_Data;

   Curso * curso;

   /*
   Estrutura respons�vel por armazenar as grades hor�rias de um curso para cada
   per�odo letivo de um determinado curr�culo.
   */
   map<pair<Curriculo*,int/*periodo*/>,
      //vector<vector<vector<pair<Disciplina*,int/*turma*/> > > > > grade_Horaria_Periodos;
      Matrix<vector<pair<Disciplina*,int/*turma*/> > > * > grade_Horaria_Periodo;

   /*
   M�ximo de cr�ditos dispon�veis para os dias letivos de um curso. Valor calculado com base
   em um curr�culo.
   */
   //map<Curriculo*,int/*max_Creds*/> max_Creds_Curric;

   /*
   Armazena os dias letivos considerados para um dado curso.
   */
   //GGroup<int> dias_Letivos;

   /*
   M�todo que verifica se alguma vari��o da regra de cr�dito pode ser utilizada para
   realizar a aloca��o sem que um subbloco seja criado.
   */
   vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator verificaDisponibilidadeSemSubBloco(
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > > & variacoes_Regra_Credito, /*Indica a partir de qual varia��o da regra de cr�dito devo come�ar a verificar */
      vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Variacoes_Regra_Credito,
      pair<Curriculo*,int/*periodo*/> & chave,
      Disciplina * disciplina
      //vector<vector<pair<int/*dia*/, int/*numCreditos*/> > >::iterator it_Variacoes_Regra_Credito_END
      );

   /*
   */
   void aloca(
      int turma,
      Disciplina * disciplina,
      vector<pair<int/*dia*/,int/*num creds*/> > & regra_De_Credito,
      Matrix<vector<pair<Disciplina*,int/*turma*/> > > * pt_Grade_Horaria_Curso
      );

   // Operadores

   bool operator < (IS_Curso const & right);

   bool operator == (IS_Curso const & right);
};
