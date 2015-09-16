#ifndef _ATENDIMENTO_TATICO_SOLUCAO_H_
#define _ATENDIMENTO_TATICO_SOLUCAO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "AtendimentoOfertaSolucao.h"
#include "AtendimentoTatico.h"
#include "AtendimentoTurno.h"

class AtendimentoTaticoSolucao :
   public OFBase
{
public:
   AtendimentoTaticoSolucao( void );
   AtendimentoTaticoSolucao( AtendimentoTatico & );

   virtual ~AtendimentoTaticoSolucao( void );

   virtual void le_arvore( ItemAtendimentoTaticoSolucao & );

   AtendimentoOfertaSolucao * atendimento_oferta;

   virtual bool operator < ( AtendimentoTaticoSolucao & right ) const
   { 
	   return ( ( this->qtdeCreditosTeoricos < right.getQtdeCreditosTeoricos() )
         && ( this->qtdeCreditosPraticos < right.getQtdeCreditosPraticos() ) );
   }

   virtual bool operator == ( AtendimentoTaticoSolucao & right ) const
   { 
	   return ( ( this->qtdeCreditosTeoricos == right.getQtdeCreditosTeoricos() )
         && ( this->qtdeCreditosPraticos == right.getQtdeCreditosPraticos() ) );
   }

   void setQtdeCreditosTeoricos( int v ) { this->qtdeCreditosTeoricos = v; }
   void setQtdeCreditosPraticos( int v ) { this->qtdeCreditosPraticos = v; }
   
   void setFixaAbertura( bool v ) { this->fixaAbertura = v; }
   void setFixaSala( bool v ) { this->fixaSala = v; }
   void setFixaDia( bool v ) { this->fixaDia = v; }
   void setFixaHorarios( bool v ) { this->fixaHorarios = v; }

   void addHorariosAula( int h ) { this->horariosAula.add(h); }

   int getQtdeCreditosTeoricos() const { return this->qtdeCreditosTeoricos; }
   int getQtdeCreditosPraticos() const { return this->qtdeCreditosPraticos; }
   
   bool getFixaAlunos( void ) const { return this->fixaAbertura; }
   bool getFixaAbertura( void ) const { return this->fixaAbertura; }
   bool getFixaSala( void ) const { return this->fixaSala; }
   bool getFixaDia( void ) const { return this->fixaDia; }
   bool getFixaHorarios( void ) const { return this->fixaHorarios; }

   GGroup< int > getHorariosAula() const { return this->horariosAula;}

private:
   int qtdeCreditosTeoricos;
   int qtdeCreditosPraticos;
   
   bool fixaAbertura;
   bool fixaSala;
   bool fixaDia;
   bool fixaHorarios;

   GGroup< int > horariosAula;
};

#endif