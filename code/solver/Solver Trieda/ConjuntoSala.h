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
      std::cout << "Construtor de copia de ConuntoSala nao esta implementado. SAINDO.";
      exit(1);
   }

   virtual ~ConjuntoSala() { }

   // Seta a capacidade da(s) sala(s) que o conjunto representa
   void setCapacidadeRepr( int cap ) { cap_Representada = cap; }
   void setTipoSalasRepr( int tpSalas ) { tipo_Salas_Representada = tpSalas; }

   int getCapacidadeRepr() const { return cap_Representada; }
   int getTipoSalasRepr() const { return tipo_Salas_Representada; }

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

   // Para n�o precisar criar os m�todos auxiliares
   // referentes �s salas, tais como: addSala, remSala, findSala, etc.
   std::map< int /*Id Sala*/, Sala * > & getTodasSalas() { return salas; }

   // Para n�o precisar criar os m�todos de acesso ao GGROUP.
   GGroup< Disciplina * > & getDiscsAssociadas() { return disciplinasAssociadas; }

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
      if ( disciplinasAssociadas.find( &disc )
			!= disciplinasAssociadas.end() )
      {
         return false;
      }

      disciplinasAssociadas.add( &disc );
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

   int maxCredsPermitidos( int dia )
   {
      int totMaxCreds = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator
		  itSala = salas.begin();
      for (; itSala != salas.end(); itSala++ )
      {
		  totMaxCreds += ( itSala->second->max_creds(dia) );
	  }

      return totMaxCreds;
   }

   // Retorna o m�ximo de cr�ditos para um dado dia.
   int maxCredsDia( int dia )
   {
      int max_Creds_Dia = 0;

      std::map< int /*Id Sala*/, Sala * >::iterator
		  it_Sala = salas.begin();
      for (; it_Sala != salas.end(); it_Sala++ )
      {
         if ( max_Creds_Dia < it_Sala->second->max_creds( dia ) )
         {
			 max_Creds_Dia = it_Sala->second->max_creds( dia );
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

   GGroup< int > diasLetivos;

private:
   std::map< int /*Id Sala*/, Sala * > salas;

   int cap_Representada;
   int tipo_Salas_Representada;

   // Armazena refer�ncias para todas as disciplinas
   // associadas a, pelo menos, uma sala pertencente ao map <salas>
   GGroup< Disciplina * > disciplinasAssociadas;
};

#endif