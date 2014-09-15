#ifndef _IS_CURSO_H_
#define _IS_CURSO_H_

#include <map>
#include <vector>

#include "Curso.h"
#include "Disciplina.h"
#include "Curriculo.h"
#include "ProblemData.h"
#include "Matrix.h"

class IS_Curso
{
public:
   IS_Curso( Curso *, ProblemData * );
   IS_Curso( IS_Curso const & );
   virtual ~IS_Curso( void );

   ProblemData * problem_Data;
   Curso * curso;

   // Estrutura respons�vel por armazenar as grades hor�rias de um curso para cada
   // per�odo letivo de um determinado curr�culo.
   std::map< std::pair< Curriculo *, int /*periodo*/ >,
      Matrix< std::vector< std::pair< Disciplina *, int /*turma*/ > > > * > grade_Horaria_Periodo;

   // M�ximo de cr�ditos dispon�veis para os dias letivos de um curso. Valor calculado com base
   // em um curr�culo.
   // std::map< Curriculo *, int /*max_Creds*/ > max_Creds_Curric;

   // Armazena os dias letivos considerados para um dado curso.
   // GGroup< int > dias_Letivos;

   // M�todo que verifica se alguma vari��o da regra de cr�dito pode ser utilizada para
   // realizar a aloca��o sem que um subbloco seja criado.
   std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > >::iterator verificaDisponibilidadeSemSubBloco(
      std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > > &, /*Indica a partir de qual varia��o da regra de cr�dito devo come�ar a verificar */
      std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > >::iterator,
      std::pair< Curriculo *, int /*periodo*/ > &, Disciplina * );

   void aloca( int,
      Disciplina *,
      std::vector< std::pair< int /*dia*/, int /*num creds*/ > > &,
      Matrix< std::vector< std::pair< Disciplina*, int /*turma*/ > > > * );

   void imprimeGradesHorarias();

   // Operadores
   bool operator < ( IS_Curso const & );
   bool operator == ( IS_Curso const & );
};

#endif