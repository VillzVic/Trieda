#ifndef _ALUNO_H_
#define _ALUNO_H_

#include "ofbase.h"
#include "AlunoDemanda.h"

class Aluno
{
public:
   Aluno( void );
   Aluno( int id, std::string nome, bool formando, Oferta* oft );
   virtual ~Aluno( void );
         
   void setAlunoId( int value ) { this->alunoId = value; }
   void setNomeAluno( std::string s ) { this->nomeAluno = s; }
   void setFormando( bool value ) { this->formando = value; }
   void setOferta( Oferta* o ) { this->oferta = o; }
   void setOfertaId( int oftId ) { this->ofertaId = oftId; }
   void setCombinaCredSL( int dia, int k, Calendario* sl , int n );
   void setCombinaCredSLSize(int dia, int size) { combinaCredSLSize[dia] = size; }

   void addHorarioDiaOcupado( HorarioDia* hd ){ horariosDiaOcupados.add(hd); }
   void addHorarioDiaOcupado( GGroup<HorarioDia*> hds );
   void removeHorarioDiaOcupado( HorarioDia* hd ){ horariosDiaOcupados.remove(hd); }
   void clearHorariosDiaOcupados(){ horariosDiaOcupados.clear(); }

   // s� usado a partir de prioridade 2 para a primeira heuristica (modelo tatico sem horarios)
   void addNCredsAlocados( Calendario* sl, int dia, double value );

   int getAlunoId() const { return this->alunoId; }
   std::string getNomeAluno() const { return this->nomeAluno; }
   Oferta* getOferta() const { return this->oferta; }
   int getOfertaId() const { return this->ofertaId; }
   inline int getCampusId() { return this->oferta->getCampusId(); }

   int getNroMaxCredCombinaSL( int k, Calendario *c, int dia );
   double getNroCreditosJaAlocados( Calendario* c, int dia );
   double getTempoJaAlocado( int dia );
   AlunoDemanda* getAlunoDemanda( int disciplinaId );
   std::map< int /*dia*/, int /*size*/ > getCombinaCredSLSize() const { return combinaCredSLSize; }
   void removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t );
   
   GGroup< Calendario*, LessPtr<Calendario> > retornaSemanasLetivas();
   
   bool combinaCredSL_eh_dominado( int i, Calendario *sl1, int maxSL2, Calendario *sl2, int dia );
   bool combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
   bool combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
   std::map< Trio<int, int, Calendario*>, int > retornaCombinaCredSL_Dominados( int dia );
   
   std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ > combinaCredSL;
   
   bool demandaDisciplina( int idDisc );

   GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > demandas;

   bool sobrepoeHorarioDiaOcupado( HorarioDia *hd );
   bool sobrepoeHorarioDiaOcupado( HorarioAula *ha, int dia );
   bool sobrepoeAulaJaAlocada( HorarioAula *hi, HorarioAula *hf, int dia );

   bool ehFormando() const { return this->formando; }

   void le_arvore( ItemAluno & elem );

   virtual bool operator < ( const Aluno & var ) const
   {
      if ( (int) this->getAlunoId() < (int) var.getAlunoId() )
      {
         return true;
      }
      else if ( (int) this->getAlunoId() > (int) var.getAlunoId() )
      {
         return false;
      }

      return false;
   }

   virtual bool operator == ( const Aluno & var ) const
   {
      return ( !( *this < var ) && !( var < *this ) );
   }   

private:

   int alunoId;
   std::string nomeAluno;
   Oferta *oferta;
   int ofertaId;
   bool formando;
   std::map< int /*dia*/, int /*size*/ > combinaCredSLSize; // informa o numero total de combina��es existentes para cada dia

   // s� usado a partir de prioridade 2 para a primeira heuristica (modelo tatico sem horarios)
   std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> > nCredsAlocados;

   GGroup< HorarioDia*, LessPtr<HorarioDia> > horariosDiaOcupados;

};

std::ostream & operator << ( std::ostream &, Aluno & );

#endif