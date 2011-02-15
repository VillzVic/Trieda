#pragma once
#include "ofbase.h"

#include "Sala.h"

#include "CreditoDisponivel.h"

class ConjuntoSala: public OFBase
   /* O <id> de um <ConjuntoSala> ser� igual � 
   <capacidade> das salas que o map de salas <salas> contem.
   Portanto, todas as salas de um <ConjuntoSala> dever�o possuir
   a mesma capacidade */
{
public:
   ConjuntoSala()
   {}

   ConjuntoSala(ConjuntoSala const & cjt_Sala)
   {}

   virtual ~ConjuntoSala() {};

   // =========== METODOS SET

   //void setIdUnidade(int idUnidade) { this->idUnidade = idUnidade; }

   //void setCapacidade(int cap)
   //{ capacidade = cap; }

//   void setTipoSalas(int tpSalas)
//   { tipoSalas = tpSalas; }

   // =========== METODOS GET

   //int getIdUnidade() const { return idUnidade; }

   //int getCapacidade()
   //{ return capacidade; }

   Sala * getSala(int idSala) const
   {
      std::map<int/*Id Sala*/,Sala*>::const_iterator itSala = this->salas.begin();

      if(itSala != salas.end())
      { return itSala->second; }

      return NULL; // Quando o <idSala> n�o pertence � esse tipo de sala, ou n�o existe.
   }

   /* Para n�o precisar criar os m�todos auxiliares referentes �s salas, tais como:
   addSala
   remSala
   findSala
   etc.
   */
   std::map<int/*Id Sala*/,Sala*> & getTodasSalas() { return salas; }

   /* Para n�o precisar criar os m�todos de acesso ao GGROUP.*/
   GGroup<Disciplina*> & getDiscsAssociadas() { return disciplinasAssociadas; }

//   int getTipoSalas() const
//   { return tipoSalas; }

   // =========== METODOS AUXILIARES

   bool addSala(Sala & sala)
   {
      if(salas.find(sala.getId()) != salas.end())
      {
         return false; /* Sala n�o adicionada, pois j� existe uma <sala>
                       com o <id> para o <ConjuntoSala> em quest�o. */
      }

      salas[sala.getId()] = &sala;

      return true; // Sala adicionada ao map de salas.
   }

   Sala * remSala(Sala &sala)
   {
      std::map<int/*Id Sala*/,Sala*>::iterator itSala = salas.find(sala.getId());

      if(itSala != salas.end())
      {
         Sala * salaRmvd = itSala->second;
         salas.erase(itSala);
         return salaRmvd;
      }

      return NULL; /* Sala n�o removida, pois n�o existe uma <sala>
                   com o <id> para o <ConjuntoSala> em quest�o. */
   }

   int maxCredsPermitidos(int dia)
   {
      int totMaxCreds = 0;

      std::map<int/*Id Sala*/,Sala*>::iterator itSala =
         salas.begin();

      for(; itSala != salas.end(); itSala++)
      { totMaxCreds += itSala->second->max_creds(dia); }

      return totMaxCreds;
   }

   /*
   Retorna o m�ximo de cr�ditos para um dado dia.
   */
   int maxCredsDia(int dia)
   {
      int max_Creds_Dia = 0;

      std::map<int/*Id Sala*/,Sala*>::iterator it_Sala =
         salas.begin();

      for(; it_Sala != salas.end(); it_Sala++)
      {
         if(max_Creds_Dia < it_Sala->second->max_creds(dia))
         { max_Creds_Dia = it_Sala->second->max_creds(dia); }
      }

      return max_Creds_Dia;
   }

   int capTotalSalas()
   {
      //return salas.begin()->second->capacidade;
      
      int capSalas = 0;

      std::map<int/*Id Sala*/,Sala*>::iterator itSala =
         salas.begin();

      for(; itSala != salas.end(); itSala++)
      { capSalas += itSala->second->capacidade; }

      return capSalas;
   }

   // =========== OPERADORES
   //virtual bool operator == (ConjuntoSala const & right)
   //{ 
   //   return ( (this->getId() == right.getId()) && 
   //      (this->getTipoSalas() < right.getTipoSalas()) ); 
   //}

   //virtual bool operator < (ConjuntoSala const & right) 
   //{ 
   //   if(this->getId() < right.getId())
   //   { return true; }
   //   else if( (this->getId() == right.getId()) && 
   //      (this->getTipoSalas() < right.getTipoSalas()) )
   //   { return true; }

   //   return false;
   //}

   /* Armazena os dias letivos em que, pelo menos, uma sala possui algum cr�dito dispon�vel */
   GGroup<int> diasLetivos;
   
private:

   std::map<int/*Id Sala*/,Sala*> salas;

   //int capacidade;

   int tipoSalas;

   // =========== PR�-PROCESSAMENTO

   /* Armazena refer�ncias para todas as disciplinas associadas a, pelo menos,
   uma sala pertencente ao map <salas> */
   GGroup<Disciplina*> disciplinasAssociadas;
   //std::map<int/*Id Disc.*/,Disciplina*> disciplinasAssociadas;

   //int idUnidade;
   /* Evita a busca em todas as unidades para saber a qual unidade
   as salas do <ConjuntoSala> em quest�o. */

};