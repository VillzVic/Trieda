#ifndef _PROFESSOR_H_
#define _PROFESSOR_H_

#include "ofbase.h"
#include "Magisterio.h"
#include "TipoContrato.h"
#include "TipoTitulacao.h"
#include "AreaTitulacao.h"
#include "HorarioDia.h"
#include "Curso.h"

class Professor :
   public OFBase
{
public:
   Professor( bool = false );
   virtual ~Professor( void );

   virtual void le_arvore( ItemProfessor & );

   GGroup< Horario * > horarios;
   GGroup< HorarioDia *, LessPtr< HorarioDia > > horariosDia;
   std::map<int, std::map< DateTime /*dti*/, GGroup<DateTime /*dtf*/> > > mapDiaDtiDtf;

   // Esse eh o disciplinas do XSD de input
   GGroup< Magisterio *, LessPtr< Magisterio > > magisterio;

   TipoContrato * tipo_contrato;
   TipoTitulacao * titulacao; 
   AreaTitulacao * area;

   void setCpf( std::string s ) { cpf = s; }
   void setNome( std::string s ) { nome = s; }
   void setTipoContratoId( int s ) { tipo_contrato_id = s; }
   void setChMin( int s ) { ch_min = s; }
   void setChMax( int s ) { ch_max = s; }
   void setChAnterior( int s ) { ch_anterior = s; }
   void setTitulacaoId( int s ) { titulacao_id = s; }
   void setAreaId( int s ) { area_id = s; }
   void setValorCredito( int s ) { valor_credito = s; }
   void setIdOperacional( int s ) { id_operacional = s; }
   void setCustoDispProf( int c ) { custoDispProf = c; }
   void setIsVirtual( bool value ) { is_virtual = value; }
   void setMaxDiasSemana( int value ) { max_dias_semana = value; }
   void setMinCredsDiarios( int value ) { min_creds_diarios = value; }
   void setCursoAssociado( Curso* c ){ curso = c; }
   void addDisponibilidade( int dia, DateTime dti, DateTime dtf );

   std::string getCpf() const { return cpf; }
   std::string getNome() const { return nome; }
   int getTipoContratoId() const { return tipo_contrato_id; }
   int getChMin() const { return ch_min; }
   int getChMax() const { return ch_max; }
   int getChAnterior() const { return ch_anterior; }
   int getTitulacaoId() const { return titulacao_id; }
   int getAreaId() const { return area_id; }
   double getValorCredito() const { return valor_credito; }
   int getIdOperacional() const { return id_operacional; }
   int getCustoDispProf() const { return custoDispProf; }
   bool eVirtual() const { return is_virtual; }
   int getMaxDiasSemana() const { return max_dias_semana; }
   int getMinCredsDiarios() const { return min_creds_diarios; }
   int getTitulacao() const { return this->titulacao->getTitulacao(); }
   int getTipoContrato() const { return this->tipo_contrato->getContrato(); }
   Curso* getCursoAssociado() const { return curso; }
   int getNroDiasDisponiv() const { return (int) mapDiaDtiDtf.size(); }

   HorarioAula* getPrimeiroHorarioDisponivelDia( int dia );
   HorarioAula* getUltimoHorarioDisponivelDia( int dia );

   int getNroCredsCadastroDisc() const { return nroCredsCadastroDisc_; }
   
   int getMaxCredsDia(int dia) const;
   
   bool possuiMagisterioEm( Disciplina* disciplina );
   
   GGroup< HorarioDia*,LessPtr<HorarioDia> > getHorariosAnterioresDisponivelDia( DateTime inicio, int dia );
   GGroup< HorarioDia*,LessPtr<HorarioDia> > getHorariosPosterioresDisponivelDia( DateTime inicio, int dia );
   
   bool possuiHorariosNoDia( HorarioAula *const hi, HorarioAula *const hf, int dia ) const;

private:
   std::string cpf;
   std::string nome;
   int tipo_contrato_id;
   int ch_min;
   int ch_max;
   int ch_anterior;
   int titulacao_id;
   int area_id;
   double valor_credito;
   int id_operacional;
   int max_dias_semana;
   int min_creds_diarios;
   Curso* curso;

   int nroCredsCadastroDisc_;

   // Utilizado na função de
   // prioridade para o modelo operacional.
   int custoDispProf;
   bool is_virtual;
};

#endif