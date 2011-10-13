#include "ProblemSolution.h"
#include "ErrorHandler.h"

ProblemSolution::ProblemSolution( bool _modoOtmTatico )
   : modoOtmTatico( _modoOtmTatico )
{
   folgas = new RestricaoVioladaGroup();
   atendimento_campus = new GGroup< AtendimentoCampus * >();
   professores_virtuais = new GGroup< ProfessorVirtualOutput * >();
   alunosDemanda = new GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >();
   idsAtendimentos = 1;
}

ProblemSolution::~ProblemSolution()
{
   if ( folgas != NULL )
   {
      folgas->deleteElements();
      delete folgas;
   }

   if( atendimento_campus != NULL )
   {
      atendimento_campus->deleteElements();
      delete atendimento_campus;
   }

   if ( professores_virtuais != NULL )
   {
      professores_virtuais->deleteElements();
      delete professores_virtuais;
   }

   if ( alunosDemanda != NULL )
   {
      alunosDemanda->deleteElements();
      delete alunosDemanda;
   }
}

std::ostream & operator << (
   std::ostream & out, ProblemSolution & solution )
{
   // TATICO
   if ( solution.modoOtmTatico )
   {
      out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>"
          << std::endl;

      out << "<TriedaOutput>" << std::endl;

      //-----------------------------------------------------------------------
      out << "<atendimentos>" << std::endl;
      GGroup< AtendimentoCampus * >::GGroupIterator it_campus
         = solution.atendimento_campus->begin();

      for (; it_campus != solution.atendimento_campus->end();
             it_campus++ )
      {
         out << ( **it_campus );
      }

      out << "</atendimentos>" << std::endl;
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Erros e warnings:
      out << ( *ErrorHandler::getInstance() );
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Folgas:
      out << "<restricoesVioladas>\n";
      RestricaoVioladaGroup::iterator it
         = solution.getFolgas()->begin();

      for (; it != solution.getFolgas()->end(); ++it )
      {
         out << ( **it );
      }

      out << "</restricoesVioladas>" << std::endl;
      //-----------------------------------------------------------------------

      out << "<professoresVirtuais/>" << std::endl;

      //-----------------------------------------------------------------------
      if ( solution.alunosDemanda != NULL )
      {
         out << "<alunosDemanda>" << std::endl;

         GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >::GGroupIterator it_aluno_demanda
            = solution.alunosDemanda->begin();

         for (; it_aluno_demanda != solution.alunosDemanda->end();
                it_aluno_demanda++ )
         {
            out << ( **it_aluno_demanda );
         }

         out << "</alunosDemanda>" << std::endl;
      }
      else
      {
         out << "<alunosDemanda/>" << std::endl;
      }
      //-----------------------------------------------------------------------

      out << "</TriedaOutput>" << std::endl;
   }
   // OPERACIONAL
   else
   {
      out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>"
          << std::endl;

      out << "<TriedaOutput>" << std::endl;

      //-----------------------------------------------------------------------
      out << "<atendimentos>" << std::endl;
      if ( solution.atendimento_campus != NULL )
      {
         GGroup< AtendimentoCampus * >::GGroupIterator it_campus
            = solution.atendimento_campus->begin();

         for (; it_campus != solution.atendimento_campus->end();
                it_campus++ )
         {
            out << ( **it_campus );
         }

         out << "</atendimentos>" << std::endl;
      }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Erros e warnings:
      out << ( *ErrorHandler::getInstance() );
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Folgas:
      out << "<restricoesVioladas>\n";
      RestricaoVioladaGroup::iterator it
         = solution.getFolgas()->begin();

      for (; it != solution.getFolgas()->end(); ++it )
      {
         out << ( **it );
      }

      out << "</restricoesVioladas>" << std::endl;
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Código relacionado à issue TRIEDA-833 e TRIEDA-883
      out << "<professoresVirtuais>" << std::endl;
      if ( solution.professores_virtuais != NULL )
      {
         GGroup< ProfessorVirtualOutput * >::GGroupIterator it_professor_virtual
            = solution.professores_virtuais->begin();

         for (; it_professor_virtual != solution.professores_virtuais->end();
                it_professor_virtual++ )
         {
            out << ( **it_professor_virtual );
         }

         out << "</professoresVirtuais>" << std::endl;
      }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      if ( solution.alunosDemanda != NULL )
      {
         out << "<alunosDemanda>" << std::endl;

         GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >::GGroupIterator it_aluno_demanda
            = solution.alunosDemanda->begin();

         for (; it_aluno_demanda != solution.alunosDemanda->end();
                it_aluno_demanda++ )
         {
            out << ( **it_aluno_demanda );
         }

         out << "</alunosDemanda>" << std::endl;
      }
      else
      {
         out << "<alunosDemanda/>" << std::endl;
      }
      //-----------------------------------------------------------------------

      out << "</TriedaOutput>" << std::endl;
   }

   return out;
}
