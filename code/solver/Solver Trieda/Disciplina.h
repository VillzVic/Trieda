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

class ConjuntoSala;

class Disciplina :
   public OFBase
{
public:
   Disciplina( void );
   virtual ~Disciplina( void );

   GGroup< DivisaoCreditos*, LessPtr<DivisaoCreditos> > divisao_creditos;
   TipoDisciplina * tipo_disciplina;
   NivelDificuldade * nivel_dificuldade;

   GGroup< int > ids_disciplinas_equivalentes;
   GGroup< int > ids_disciplinas_incompativeis;
   GGroup< Horario *, LessPtr<Horario> > horarios;
   std::map< int, std::map<int, GGroup<Horario*, LessPtr<Horario>>> > turnos;		// <turno, dias> que possui horários
   std::map< int, std::map<int, GGroup<Horario*, LessPtr<Horario>>> > turnosIES;	// <turnoIES, dias> que possui horários
   
   std::map<int, std::map< DateTime, GGroup<DateTime> > > mapDiaDtiDtf;			// <dia, Datetime inicial, set< Datetime final > >

   // Conjunto de combinações possíveis de divisão de créditos de uma disciplina 'd'
   std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > > combinacao_divisao_creditos;

   // Armazena os dias letivos em que a disciplina pode ser ministrada.
   GGroup< int > diasLetivos;

   GGroup< Disciplina *, LessPtr< Disciplina > > discEquivSubstitutas; // possíveis de serem substitutas desta

   GGroup< Disciplina *, LessPtr< Disciplina > > discEquivalentes; // substituídas por essa
   GGroup< Disciplina *, LessPtr< Disciplina > > discIncompativeis;
   
   std::map< int, ConjuntoSala* > cjtSalasAssociados; // map[cjtSalaId] = cjtSala
      
   void clearHors();
   void clearDivCreds();

   virtual void le_arvore( ItemDisciplina & );

   int adicionaDemandaTotal( int d ) { return ( demanda_total += d ); }

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
   void addCalendario( Calendario* sl ) { this->calendarios[sl]; }
   void addHorarioDia( Calendario* sl, HorarioDia* hd );
   void setNSalasAptas( int value ){ this->nSalasAptas = value; }
   void setProfUnicoDiscPT( bool value ) { this->prof_unico_discPT = value; }
   void setAulasContinuas( bool value ) { this->aulas_continuas_teor_prat = value; }
   void setTemParPratTeor( bool value ) { tem_par_prat_teor = value; }
   void addDiscSubstituta( GGroup<Disciplina*, LessPtr<Disciplina>> discs ){ discEquivSubstitutas.add(discs); }
   void addDiscSubstituta( Disciplina* disc ){ discEquivSubstitutas.add(disc); }
   void addDiscSubstituida( Disciplina* disc ){ discEquivalentes.add(disc); }

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
   int getNSalasAptas() const { return this->nSalasAptas; }
   int getTempoTotalSemana( Calendario* calendario );
   bool getProfUnicoDiscPT() const { return this->prof_unico_discPT; }
   bool aulasContinuas() const { return aulas_continuas_teor_prat; }
   int getNroCredsRegraDiv( int k, int dia );
   bool temParPratTeor() const { return tem_par_prat_teor; }
   bool possuiRegraCred() const;

   bool possuiHorariosNoDia( HorarioAula *hi, HorarioAula *hf, int dia );
   bool possuiHorariosOuCorrespondentesNoTurno( TurnoIES* turnoIES );
   int getNrHorsNoTurno( TurnoIES* turnoIES ) const;
   GGroup< Horario*, LessPtr<Horario> > getHorariosOuCorrespondentes( TurnoIES* turnoIES );
   GGroup< HorarioAula*, LessPtr<HorarioAula> > getHorariosDia( Calendario* sl, int dia );
   GGroup< HorarioDia*, LessPtr<HorarioDia> > getHorariosDia( Calendario* sl ) { return calendarios[sl]; } 
   
   GGroup< Calendario*, LessPtr<Calendario> > getCalendarios() 
   { 
	   GGroup< Calendario*, LessPtr<Calendario> > ggroupCalend;
	   std::map< Calendario*, GGroup< HorarioDia*, LessPtr<HorarioDia> >, LessPtr<Calendario> >::iterator
		   it = calendarios.begin();
	   for ( ; it!=calendarios.end(); it++ )
	   {
		   ggroupCalend.add(it->first);
	   }
	   return ggroupCalend;
   }

   void addCalendariosOriginais( Calendario* c ) { this->calendariosOrig.add(c); }

   GGroup< Calendario*, LessPtr<Calendario> > getCalendariosOriginais() const { return this->calendariosOrig; }
   
   double getTotalTempo() { return ( getTotalCreditos() * getTempoCredSemanaLetiva() ); }

   // tempo de duracao de 1 credito da disciplina. Obtido a partir da semana letiva a qual pertence a disciplina.
   double getTempoCredSemanaLetiva() { return this->calendarios.begin()->first->getTempoAula(); } 

   int getTotalCreditos() const { return this->getCredTeoricos() + this->getCredPraticos(); }
      
   Disciplina* substituidaPor() const { return substituta; }
   
   int getMaxTempoDiscEquivSubstituta();
   
   bool inicioTerminoValidos( HorarioAula *hi, HorarioAula *hf, int dia, GGroup<HorarioAula *, LessPtr<HorarioAula>> horariosPossiveis );
   bool inicioTerminoValidos( HorarioAula *hi, HorarioAula *hf, int dia );
   int getTotalDeInicioTerminoValidos();

   void setSubCjtSalaNumTurmas( int cjtSalaId, int nTurmasNaSala ) { mapSubCjtSalaNumTurmas[cjtSalaId] = nTurmasNaSala; }

   int getNumTurmasNaSala( int cjtSalaId );
   
   bool possuiHorarioDia( int dia, HorarioAula *hd );
   bool possuiHorarioDiaOuAnalogo( int dia, HorarioAula *hd );
   int possuiHorarioNoTurnoDia( int dia, int turno );
   
   GGroup<int> diasQuePossuiTurno( int t );
   bool possuiTurno( int t );
   void addTurno( int t,  Horario *h );
   
   bool possuiTurnoIES( TurnoIES* turnoIES );
   void addTurnoIES( int t,  Horario *h );

   void setTurmaTeorAssociada( int turmaP, int turmaT );	// info para disciplina prática em relação 1xN
   void setTurmaPratAssociada( int turmaT, int turmaP );	// info para disciplina prática em relação 1xN
   void setTurmasPratPorTeor( int nroTurmasPrat );			// info para disciplina teórica em relação 1xN
   GGroup<int> getTurmasAssociadas( int turma );			// info para disciplina em relação 1xN
   int getNroTurmasAssociadas( int turma );					// info para disciplina em relação 1xN

   std::map< Calendario*, GGroup< HorarioDia*, LessPtr<HorarioDia> >, LessPtr<Calendario> > calendarios;

   // calendarios reduzidos (usado na heurística)
   GGroup<Calendario*, LessPtr<Calendario>> calendariosReduzidos;
   // combinações de divisões por id

private:   
	
	GGroup< Calendario*, LessPtr<Calendario> > calendariosOrig;

	std::map< int, int > mapSubCjtSalaNumTurmas;	// definido pelo modelo EstimaTurmas

	// info para relação 1xN
	// para disciplina teorica: cada turma teorica informa quais são as possíveis turmas praticas associadas
	// para disciplina pratica: cada turma pratica informa quais são as possíveis turmas teoricas associadas
    std::map< int, GGroup<int> > mapTurmaPT_TurmasTP;
			  
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
   bool prof_unico_discPT;
   bool aulas_continuas_teor_prat;
   bool tem_par_prat_teor;

   // Armazena os trios possiveis (dia, horarioAula inicial, horarioAula final) para a disciplina ser ministrada,
   // e para cada trio, guarda a lista de horariosAula que está entre hi e hf, incluindo hi/hf.
   std::map< int /*dia*/, std::map< std::pair<HorarioAula* /*hi*/, HorarioAula* /*hf*/>, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > horarios_hihf_validos;

   std::map< int /*dia*/, std::map< std::pair<HorarioAula* /*hi*/, HorarioAula* /*hf*/>, bool > > horarios_hihf;
};

#endif