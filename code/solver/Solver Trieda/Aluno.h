#ifndef _ALUNO_H_
#define _ALUNO_H_

#include "ofbase.h"
#include "AlunoDemanda.h"
#include "Oferta.h"
#include <unordered_set>

class Curso;

class Aluno
{
public:
   Aluno( void );
   Aluno( int id, std::string nome, bool formando, Oferta* oft );
   virtual ~Aluno( void );
   
   void setAlunoId( int value ) { this->alunoId = value; }
   void setNomeAluno( std::string s ) { this->nomeAluno = s; }
   void setFormando( bool value ) { this->formando = value; }
   void setPrioridadeDoAluno( int value ) { this->prioridadeDoAluno = value; }

   void setOferta( Oferta* o ) { this->oferta = o; }	// TODO: isso perde o sentido assim que um aluno puder pedir por discs de diferentes ofertas
   void setOfertaId( int oftId ) { this->ofertaId = oftId; } // TODO: isso perde o sentido assim que um aluno puder pedir por discs de diferentes ofertas
   void setCalouro( bool c ) { this->calouro = c; }

   void setCombinaCredSL( int dia, int k, Calendario* sl , int n );
   void setCombinaCredSLSize(int dia, int size) { combinaCredSLSize[dia] = size; }
   void addCargaHorariaOrigRequeridaP1( int ch ) { cargaHorariaOrigRequeridaP1 += ch; }
   void addNroCredsOrigRequeridosP1( int ch ) { nroCredsOrigRequeridosP1 += ch; }

   void addNroDiscsOrigRequeridosP1( int addNroDiscs ) { this->nroDiscsOrigRequeridosP1 += addNroDiscs; }
   void setNroCreditosJaAlocados( int nroCreds ) { this->nCredsJaAlocados = nroCreds; }
   void addNroCreditosJaAlocados( int addNroCreds ) { this->nCredsJaAlocados += addNroCreds; }
   

   void addHorarioDiaOcupado( HorarioDia* hd );
   void addHorarioDiaOcupado( GGroup<HorarioDia*> hds );
   void removeHorarioDiaOcupado( HorarioDia* hd );

   void clearHorariosDiaOcupados(){ horariosDiaOcupados.clear(); }

   Calendario* getCalendario( Demanda* demanda ){ return this->getOferta( demanda )->curriculo->calendario; }
   
   // Retorna calendario associado a alguma das demandas do aluno.
   // Isso não é o ideal, mas atualmente não existe forma precisa de obter isso.
   Calendario* getCalendario(){ return (oferta!=nullptr? oferta->curriculo->calendario : nullptr); }
   
   int getAlunoId() const { return this->alunoId; }
   std::string getNomeAluno() const { return this->nomeAluno; }

   Oferta* getOferta( Demanda* demanda ) const { return demanda->oferta; }
   int getOfertaId( Demanda* demanda ) const { return demanda->getOfertaId(); }

   bool hasCampusId( int cpId ){ 
	   if ( campusIds.find(cpId) != campusIds.end() )
		   return true;
	   else return false;
   }

   GGroup<int> getCampusIds() const { return campusIds; }

   void fillCampusIds(){ // preenche campusIds a partir dos AlunoDemandas
	   ITERA_GGROUP_LESSPTR( it, this->demandas, AlunoDemanda )
		   campusIds.add( it->getOferta()->getCampusId() );	   
   }
         
   // IMPLEMENTAR!!!!
   int getPriorAluno(void) { return prioridadeDoAluno; }

   Curso* getCurso( void ) { if(oferta == NULL || oferta == nullptr) { return nullptr; } else { return oferta->curso; } }

   double getReceita( Disciplina *disciplina );
   
   int getNroDiscsOrigRequeridosP1() const { return this->nroDiscsOrigRequeridosP1; }
   int getNroCredsOrigRequeridosP1() const { return this->nroCredsOrigRequeridosP1; }
   int getCargaHorariaOrigRequeridaP1() const { return this->cargaHorariaOrigRequeridaP1; }
   int getNroCreditosJaAlocados() const { return this->nCredsJaAlocados;}
   bool totalmenteAtendido();
   int getNroCreditosNaoAtendidos();

   int getNroMaxCredCombinaSL( int k, Calendario *c, int dia );
   double getNroCreditosJaAlocados( Calendario* c, int dia );
   AlunoDemanda* getAlunoDemanda( int disciplinaId );
   AlunoDemanda* getAlunoDemandaEquiv( Disciplina *disciplina );
   std::map< int /*dia*/, int /*size*/ > getCombinaCredSLSize() const { return combinaCredSLSize; }
   void removeCombinaCredSL( Trio< int/*dia*/, int /*id*/, Calendario* /*sl*/ > t );
   std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > getCorrequisitos() const { return ids_discs_correquisito; }

   GGroup< Calendario*, LessPtr<Calendario> > retornaSemanasLetivas();
   
   bool combinaCredSL_eh_dominado( int i, Calendario *sl1, int maxSL2, Calendario *sl2, int dia );
   bool combinaCredSL_domina( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
   bool combinaCredSL_eh_repetido( int i, Calendario *sl1, int j, Calendario *sl2, int dia );
   std::map< Trio<int, int, Calendario*>, int > retornaCombinaCredSL_Dominados( int dia );
   
   std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >, int/*nroCreds*/ > combinaCredSL;
   
   bool demandaDisciplina( int idDisc );

   GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > demandas;
   // demandas P2. Povoadas no inicio da heuristica
   unordered_set<AlunoDemanda*> demandasP2;
   AlunoDemanda* getAlunoDemandaEquivP2( Disciplina *disciplina );

   bool sobrepoeHorarioDiaOcupado( HorarioDia *hd );
   bool sobrepoeHorarioDiaOcupado( HorarioAula *ha, int dia );
   bool sobrepoeAulaJaAlocada( HorarioAula *hi, HorarioAula *hf, int dia );
   
   bool ehFormando() const { return formando; }
   bool ehCalouro() const { return calouro; }
    
   void defineSeEhCalouro();

   std::string getHorariosOcupadosStr( int dia );

   // abordagem diferente (Heuristica)
   void addDemP1(void) { nrDemsP1++; }
   int getNrDemsP1(void) { return nrDemsP1; }
   void addNrCredsReqP1(int creds) { nrCredsReqP1 += creds; } 
   int getNrCredsReqP1(void) { return nrCredsReqP1; }

   // Usado para tentar distribuir bem os créditos do aluno (MIP-Escola)
   void setNrMedioCredsDia();
   int getNrMedioCredsDia();

   bool possuiEquivForcada();

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

   bool operator > ( const Aluno & var ) const
   {
      return ( !( *this < var ) && !( var == *this ) );
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
   bool calouro;
   int prioridadeDoAluno;

   // Media de creditos por dia, de acordo com a demanda do aluno.
   // Usado no MIP-Escola para tentar equilibrar a alocação do aluno na semana
   int nrCredsMedioDia_;

   std::map< int /*dia*/, int /*size*/ > combinaCredSLSize; // informa o numero total de combinações existentes para cada dia

   // só usado a partir de prioridade 2 para a primeira heuristica (modelo tatico sem horarios)
   std::map< Calendario*, std::map< int /*dia*/, double /*nCreds*/> > nCredsAlocados;

   std::map< int /*dia*/, std::map< DateTime, HorarioDia*> > horariosDiaOcupados;

   int nroDiscsOrigRequeridosP1;
   int nroCredsOrigRequeridosP1;
   int cargaHorariaOrigRequeridaP1;
   int nCredsJaAlocados;	// só começa ser preenchido após tático (horários/dias definidos)

   // diferente
   int nrCredsReqP1;
   int nrDemsP1;

   // tuplas de disciplinas com correquisito filtradas de acordo com a demanda do aluno
   std::map< int /*tuplaId*/, GGroup< int/*discId*/ > > ids_discs_correquisito; 
    
   GGroup<int> campusIds;
};

std::ostream & operator << ( std::ostream &, Aluno & );

#endif