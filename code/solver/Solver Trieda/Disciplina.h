#ifndef _DISCIPLINA_H_
#define _DISCIPLINA_H_

#include "ofbase.h"
#include "GGroup.h"
#include "TipoDisciplina.h"
#include "DivisaoCreditos.h"
#include "NivelDificuldade.h"
#include "Horario.h"
#include "HorarioDia.h"
#include "Calendario.h"

class Disciplina :
   public OFBase
{
public:
   Disciplina( void );
   virtual ~Disciplina( void );

   GGroup< DivisaoCreditos*, LessPtr<DivisaoCreditos> > divisao_creditos;
 //  DivisaoCreditos * divisao_creditos;
   TipoDisciplina * tipo_disciplina;
   NivelDificuldade * nivel_dificuldade;

   GGroup< int > ids_disciplinas_equivalentes;
   GGroup< int > ids_disciplinas_incompativeis;
   GGroup< Horario * > horarios;

   GGroup< HorarioDia * > horariosDia;

   // Conjunto de combinações possíveis de divisão de créditos de uma disciplina 'd'
   std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > > combinacao_divisao_creditos;

   virtual void le_arvore( ItemDisciplina & );

   int adicionaDemandaTotal( int d ) { return ( demanda_total += d ); }

   // Armazena os dias letivos em que a disciplina pode ser ministrada.
   GGroup< int > diasLetivos;

   GGroup< Disciplina *, LessPtr< Disciplina > > discEquivSubstitutas; // possíveis de serem substitutas desta

   GGroup< Disciplina *, LessPtr< Disciplina > > discEquivalentes; // substituídas por essa
   GGroup< Disciplina *, LessPtr< Disciplina > > discIncompativeis;

   void setDemandaTotal( int value ) { demanda_total = value; }
   void setMaxDemanda( int value ) { max_demanda = value; }
   void setCodigo( std::string s ) { codigo = s; }
   void setNome( std::string s ) { nome = s; }
   void setCredTeoricos( int value ) { cred_teoricos = value; }
   void setCredPraticos( int value ) { cred_praticos = value; }
   void setELab( bool value ) { e_lab = value; }
   void setMaxAlunosT( int value ) { max_alunos_t = value; }
   void setMaxAlunosP( int value ) { max_alunos_p = value; }
   void setTipoDisciplinaId( int value ) { tipo_disciplina_id = value; }
   void setNivelDificuldadeId( int value ) { nivel_dificuldade_id = value; }
   void setNumTurmas( int value ) { num_turmas = value; }
   void setMinCreds( int value ) { min_creds = value; }
   void setMaxCreds( int value ) { max_creds = value; }
   void setMenorCapacSala( int value, int campusId ) { this->menorCapacSala[campusId] = value; }
   void setCapacMediaSala( int value, int campusId ) { this->capacMediaSala[campusId] = value; }
   void setCalendario( Calendario* sl ) { this->calendario = sl; }
   void setNSalasAptas( int value ){ this->nSalasAptas = value; }

   int getDemandaTotal() const { return demanda_total; }
   int getMaxDemanda() const { return max_demanda; }
   std::string getCodigo() const { return codigo; }
   std::string getNome() const { return nome; }
   int getCredTeoricos() const { return cred_teoricos; }
   int getCredPraticos() const { return cred_praticos; }
   bool eLab() const { return e_lab; }
   int getMaxAlunosT() const { return max_alunos_t; }
   int getMaxAlunosP() const { return max_alunos_p; }
   int getTipoDisciplinaId() const { return tipo_disciplina_id; }
   int getNivelDificuldadeId() const { return nivel_dificuldade_id; }
   int getNumTurmas() const { return num_turmas; }
   int getMinCreds() const { return min_creds; }
   int getMaxCreds() const { return max_creds; }
   int getMenorCapacSala( int campusId );
   int getCapacMediaSala( int campusId );
   Calendario* getCalendario() const { return calendario; }
   int getNSalasAptas() const { return this->nSalasAptas; }
   int getTempoTotalSemana();

   // tempo de duracao de 1 credito da disciplina. Obtido a partir da semana letiva a qual pertence a disciplina.
   double getTempoCredSemanaLetiva() const { return this->calendario->getTempoAula(); } 

   int getTotalCreditos() const { return this->getCredTeoricos() + this->getCredPraticos(); }

   Disciplina* substituidaPor() const { return substituta; }
   
   int getMaxTempoDiscEquivSubstituta();

   bool inicioTerminoValidos( HorarioAula *&hi, HorarioAula *&hf );
   
private:
	
   Calendario* calendario;

   // Soma das demandas de uma disciplina.
   int demanda_total;

   // Maior demanda dentre a(s) demanda(s) de uma dada disciplina.
   int max_demanda;

   std::string codigo;
   std::string nome;
   int cred_teoricos;
   int cred_praticos;
   bool e_lab;
   int max_alunos_t;
   int max_alunos_p;
   int tipo_disciplina_id;
   int nivel_dificuldade_id;
   int num_turmas;
   int min_creds;
   int max_creds;
   std::map<int, int> menorCapacSala; // menor capacidade dentre as salas de cada campus aonde a disciplina pode ser ministrada
   std::map<int, int> capacMediaSala; // capacidade media dentre as salas de cada campus aonde a disciplina pode ser ministrada
   int nSalasAptas;
   Disciplina* substituta;

};

#endif