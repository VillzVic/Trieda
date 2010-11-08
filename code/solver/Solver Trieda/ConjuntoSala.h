#pragma once
#include "ofbase.h"

#include "Sala.h"

class ConjuntoSala: public OFBase /* O <id> de um <ConjuntoSala> ser� igual � 
                                  <capacidade> das salas que o map de salas <salas> contem.
                                  Portanto, todas as salas de um <ConjuntoSala> dever�o possuir
                                  a mesma capacidade */
{
public:
   ConjuntoSala()
   {}
   //{ idUnidade = -1; }

   ~ConjuntoSala();

   // =========== METODOS SET

   //void setIdUnidade(int idUnidade) { this->idUnidade = idUnidade; }


   // =========== METODOS GET

   //int getIdUnidade() const { return idUnidade; }

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

private:

   std::map<int/*Id Sala*/,Sala*> salas;

   // =========== PR�-PROCESSAMENTO

   /* Armazena refer�ncias para todas as disciplinas associadas a, pelo menos,
   uma sala pertencente ao map <salas> */
   GGroup<Disciplina*> disciplinasAssociadas;
   //std::map<int/*Id Disc.*/,Disciplina*> disciplinasAssociadas; 

   //int idUnidade;
   /* Evita a busca em todas as unidades para saber a qual unidade
   as salas do <ConjuntoSala> em quest�o. */

};