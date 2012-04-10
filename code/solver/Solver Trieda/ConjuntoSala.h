#ifndef _CONJUNTO_SALA_H_
#define _CONJUNTO_SALA_H_

#include "ofbase.h"
#include "Sala.h"
#include "CreditoDisponivel.h"

class ConjuntoSala
	: public OFBase
{
public:
   ConjuntoSala()
   {
      cap_Representada = 0;
      tipo_Salas_Representada = 0;
   }

   ConjuntoSala( ConjuntoSala const & cjt_Sala )
   {
      this->cap_Representada = cjt_Sala.cap_Representada;
      this->tipo_Salas_Representada = cjt_Sala.tipo_Salas_Representada;

	  this->disciplinas_associadas = GGroup< Disciplina *, LessPtr< Disciplina > >( cjt_Sala.disciplinas_associadas );
	  this->dias_letivos_disciplinas = std::map< Disciplina *, GGroup< int > >( cjt_Sala.dias_letivos_disciplinas );
	  this->salas = std::map< int, Sala * >( cjt_Sala.salas );
   }

   virtual ~ConjuntoSala() { }

   // Seta a capacidade da(s) sala(s) que o conjunto representa
   void setCapacidadeRepr( int cap ) { this->cap_Representada = cap; }
   void setTipoSalasRepr( int tpSalas ) { this->tipo_Salas_Representada = tpSalas; }

   int getCapacidadeRepr() const { return this->cap_Representada; }
   int getTipoSalasRepr() const { return this->tipo_Salas_Representada; }

   Sala * getSala( int idSala ) const
   {
      std::map< int /*Id Sala*/, Sala * >::const_iterator itSala = this->salas.begin();

      if ( itSala != salas.end() )
      {
		  return itSala->second;
	  }

	  // Quando o <idSala> n�o pertence
	  // � esse tipo de sala, ou n�o existe.
      return NULL;
   }

   // Armazena refer�ncias para todas as disciplinas
   // associadas a, pelo menos, uma sala pertencente ao map <salas>
   GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas_associadas;

   // Dado uma disciplina, informamos os
   // dias da semana que essa disciplina tem aula
   std::map< Disciplina *, GGroup< int > > dias_letivos_disciplinas;

   bool addSala( Sala & sala )
   {
      if ( salas.find( sala.getId() ) != salas.end() )
      {
		 // Sala n�o adicionada, pois j� existe uma <sala>
         // com o <id> para o <ConjuntoSala> em quest�o.
         return false;
      }

      salas[ sala.getId() ] = &( sala );

	  // Sala adicionada ao map de salas.
      return true;
   }

   bool associaDisciplina( Disciplina & disc )
   {
      if ( disciplinas_associadas.find( &disc )
			!= disciplinas_associadas.end() )
      {
         return false;
      }

      disciplinas_associadas.add( &disc );
      return true;
   }

   Sala * remSala( Sala & sala )
   {
      std::map< int /*Id Sala*/, Sala * >::iterator
		  itSala = salas.find( sala.getId() );

      if ( itSala != salas.end() )
      {
         Sala * salaRmvd = itSala->second;
         salas.erase( itSala );
         return salaRmvd;
      }

	  // Sala n�o removida, pois n�o existe uma <sala>
      // com o <id> para o <ConjuntoSala> em quest�o.
      return NULL;
   }

   /*
   int maxCredsPermitidos( int dia )
   {
      int totMaxCreds = 0;

      std::map< int, Sala * >::iterator
		  itSala = salas.begin();
      for (; itSala != salas.end(); itSala++ )
      {
		  totMaxCreds += ( itSala->second->max_creds(dia) );
	  }

      return totMaxCreds;
   }
   */

   int maxTempoPermitidoPorDiaPorSL( int dia, Calendario* sl )
   {
      int totMaxTempo = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator itSala = salas.begin();
      for (; itSala != salas.end(); itSala++ )
      {
		  totMaxTempo += ( itSala->second->getTempoDispPorDiaSL(dia, sl) );
	  }

      return totMaxTempo;
   }

   int maxTempoPermitidoPorDia( int dia )
   {
      int totMaxTempo = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator itSala = salas.begin();
      for (; itSala != salas.end(); itSala++ )
      {
		  totMaxTempo += ( itSala->second->getTempoDispPorDia(dia) );
	  }

      return totMaxTempo;
   }

   /*
	   Retorna o maior dentre todos os totais de tempo dispon�vel na semana
	   para os calend�rios existentes no conjunto sala.
	   Fun��o usada na restri��o de m�ximo de cr�dito por sala na semana,
	   no modelo pre-t�tico.
   */
   int maxTempoPermitidoNaSemana( std::map< Disciplina*, Disciplina* > mapDiscSubstituidaPor )
   {
	   int maxTempo = 0;

	   GGroup< Calendario*, LessPtr<Calendario> > calendarios;
	   ITERA_GGROUP_LESSPTR (itDisc, this->disciplinas_associadas, Disciplina )
       {
		   if (itDisc->getCalendario() == NULL)
			   continue;
			   
			#pragma region Equivalencias
			if ( mapDiscSubstituidaPor.find( *itDisc ) !=
				 mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

		   if (  calendarios.find( itDisc->getCalendario() ) == calendarios.end() )
		   {
			   calendarios.add( itDisc->getCalendario() );
		   }
       }

	   GGroup<int> dias;
       std::map< int /*Id Sala*/, Sala * >::iterator itSala = salas.begin();
       for (; itSala != salas.end(); itSala++ )
       {
		   ITERA_GGROUP_N_PT( itDia, itSala->second->diasLetivos, int )
		   {
				if (  dias.find( *itDia ) == dias.end() )
				{
					dias.add( itSala->second->diasLetivos );		   
				}
		   }
       }

	   ITERA_GGROUP_N_PT (itDia, dias, int )
	   {
		    int max = 0;
			ITERA_GGROUP_LESSPTR (itCalend, calendarios, Calendario )
			{
				int tempoSL = maxTempoPermitidoPorDiaPorSL( *itDia, *itCalend );	
				if ( max < tempoSL )
					max = tempoSL;
		    }
			maxTempo += max;
	   }
	   
	   return maxTempo;
   }

   // Retorna o m�ximo de cr�ditos para um dado dia.
   int maxCredsDia( int dia )
   {
      int max_Creds_Dia = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator it_Sala = salas.begin();
      for (; it_Sala != salas.end(); it_Sala++ )
      {
         if ( max_Creds_Dia < it_Sala->second->max_creds( dia ) )
         {
			 max_Creds_Dia = it_Sala->second->max_creds( dia );
		 }
      }

      return max_Creds_Dia;
   }

   // Retorna o m�ximo de cr�ditos para um dado dia.
   int maxCredsDiaPorSL( int dia, Calendario* sl )
   {
      int max_Creds_Dia = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator it_Sala = salas.begin();
      for (; it_Sala != salas.end(); it_Sala++ )
      {
         if ( max_Creds_Dia < it_Sala->second->max_credsPorSL( dia, sl ) )
         {
			 max_Creds_Dia = it_Sala->second->max_credsPorSL( dia, sl );
		 }
      }

      return max_Creds_Dia;
   }

   int capTotalSalas()
   {
      int capSalas = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator
		  itSala = salas.begin();
      for (; itSala != salas.end(); itSala++ )
      {
		  capSalas += itSala->second->getCapacidade();
	  }

      return capSalas;
   }


   GGroup< HorarioAula* > retornaHorariosDisponiveisNoDiaSL( int dia, Calendario *sl )
   {
	   GGroup<HorarioAula*> horariosPorSala;
	   GGroup<HorarioAula*> horarios;

      std::map< int /*Id Sala*/, Sala * >::iterator itSala = salas.begin();
      for (; itSala != salas.end(); itSala++ )
      {
		  horariosPorSala = itSala->second->retornaHorariosDisponiveisNoDiaPorSL( dia, sl );

		  GGroup<HorarioAula*>::iterator it = horariosPorSala.begin();
		  ITERA_GGROUP( it, horariosPorSala, HorarioAula )
		  {
			  horarios.add( *it );
		  }
	  }

      return horarios;
   }


   std::map< int /*Id Sala*/, Sala * > salas;

private:
   int cap_Representada;
   int tipo_Salas_Representada;
};

#endif