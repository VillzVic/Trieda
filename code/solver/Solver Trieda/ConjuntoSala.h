#pragma once
#include "ofbase.h"

#include "Sala.h"

#include "CreditoDisponivel.h"

class ConjuntoSala: public OFBase
{
public:
   ConjuntoSala()
   {
      cap_Representada = 0;
      tipo_Salas_Representada = 0;
   }

   ConjuntoSala(ConjuntoSala const & cjt_Sala)
   {
      std::cout << "Construto de copia de ConuntoSala nao esta implementado. SAINDO.";
      exit(1);
   }

   virtual ~ConjuntoSala() {};

   // =========== METODOS SET

   //void setIdUnidade(int idUnidade) { this->idUnidade = idUnidade; }

   // Seta a capacidade da(s) sala(s) que o conjunto representa
   void setCapacidadeRepr(int cap)
   { cap_Representada = cap; }

   void setTipoSalasRepr(int tpSalas)
   { tipo_Salas_Representada = tpSalas; }

   // =========== METODOS GET

   //int getIdUnidade() const { return idUnidade; }

   int getCapacidadeRepr()
   { return cap_Representada; }

   int getTipoSalasRepr()
   { return tipo_Salas_Representada; }

   Sala * getSala(int idSala) const
   {
      std::map<int/*Id Sala*/,Sala*>::const_iterator itSala = this->salas.begin();

      if(itSala != salas.end())
      { return itSala->second; }

      return NULL; // Quando o <idSala> não pertence à esse tipo de sala, ou não existe.
   }

   /* Para não precisar criar os métodos auxiliares referentes às salas, tais como:
   addSala
   remSala
   findSala
   etc.
   */
   std::map<int/*Id Sala*/,Sala*> & getTodasSalas() { return salas; }

   /* Para não precisar criar os métodos de acesso ao GGROUP. */
   GGroup<Disciplina*> & getDiscsAssociadas() { return disciplinasAssociadas; }

//   int getTipoSalas() const
//   { return tipoSalas; }

   // =========== METODOS AUXILIARES

   bool addSala(Sala & sala)
   {
      if(salas.find(sala.getId()) != salas.end())
      {
         return false; /* Sala não adicionada, pois já existe uma <sala>
                       com o <id> para o <ConjuntoSala> em questão. */
      }

      salas[sala.getId()] = &sala;

      return true; // Sala adicionada ao map de salas.
   }

   bool associaDisciplina(Disciplina & disc)
   {
      if(disciplinasAssociadas.find(&disc) != disciplinasAssociadas.end())
      {
         return false;
      }

      disciplinasAssociadas.add(&disc);
      return true;
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

      return NULL; /* Sala não removida, pois não existe uma <sala>
                   com o <id> para o <ConjuntoSala> em questão. */
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
   Retorna o máximo de créditos para um dado dia.
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
	  {
		  capSalas += itSala->second->getCapacidade();
	  }

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

   /* Armazena os dias letivos em que, pelo menos, uma sala possui algum crédito disponível */
   GGroup<int> diasLetivos;
   
private:

   std::map<int/*Id Sala*/,Sala*> salas;

   int cap_Representada;

   int tipo_Salas_Representada;

   // =========== PRÉ-PROCESSAMENTO

   /* Armazena referências para todas as disciplinas associadas a, pelo menos,
   uma sala pertencente ao map <salas> */
   GGroup<Disciplina*> disciplinasAssociadas;
   //std::map<int/*Id Disc.*/,Disciplina*> disciplinasAssociadas;

   //int idUnidade;
   /* Evita a busca em todas as unidades para saber a qual unidade
   as salas do <ConjuntoSala> em questão. */

};