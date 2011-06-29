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

   // Estrutura responsável por armazenar as grades horárias de um curso para cada
   // período letivo de um determinado currículo.
   std::map< std::pair< Curriculo *, int /*periodo*/ >,
      Matrix< std::vector< std::pair< Disciplina *, int /*turma*/ > > > * > grade_Horaria_Periodo;

   // Máximo de créditos disponíveis para os dias letivos de um curso. Valor calculado com base
   // em um currículo.
   // std::map< Curriculo *, int /*max_Creds*/ > max_Creds_Curric;

   // Armazena os dias letivos considerados para um dado curso.
   // GGroup< int > dias_Letivos;

   // Método que verifica se alguma varição da regra de crédito pode ser utilizada para
   // realizar a alocação sem que um subbloco seja criado.
   std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > >::iterator verificaDisponibilidadeSemSubBloco(
      std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > > &, /*Indica a partir de qual variação da regra de crédito devo começar a verificar */
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