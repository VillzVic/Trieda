#pragma once
#include "ofbase.h"
#include "TipoDisciplina.h"
#include "DivisaoCreditos.h"
#include "NivelDificuldade.h"
#include "Horario.h"

class Disciplina :
   public OFBase
{
public:
   Disciplina(void);
   ~Disciplina(void);
   //private:
   std::string codigo;
   std::string nome;
   int cred_teoricos;
   int cred_praticos;
   bool e_lab;
   int max_alunos_t;
   int max_alunos_p;
   int tipo_disciplina_id;
   int nivel_dificuldade_id;

   DivisaoCreditos* divisao_creditos;
   GGroup<int> equivalentes;
   GGroup<int> incompativeis;
   GGroup<Horario*> horarios;

   TipoDisciplina* tipo_disciplina;
   NivelDificuldade* nivel_dificuldade;

   //conjunto de combinações possíveis de divisão de créditos de uma uma disciplina d
   std::vector<std::vector<std::pair<int/*dia*/, int/*numCreditos*/> > > combinacao_divisao_creditos;

private:
   // Soma das demandas de uma disciplina.
   int demandaTotal;

   // Maior demanda dentre a(s) demanda(s) de uma dada disciplina.
   int max_demanda;

public:

   // Estrutura responsável por armazenar os id's das turmas da disciplina em questão.
   //GGroup<int/*Id Turma*/> turmas;

   int num_turmas;
   int min_creds;
   int max_creds;

   // >>> 14/10/2010
   //bool foi_dividida;
   // <<< 14/10/2010

public:

   // >>> Métodos para realizar o pré processamento dos dados.

   // =========== METODOS SET
   void setMaxCreds(int _max_creds) {
      max_creds = _max_creds;
   }

   void setCredsPraticos(int _creds_praticos) {
      cred_praticos = _creds_praticos;
   }

   void setDemandaTotal(int demanda_total) { demandaTotal = demanda_total; }

   void setMaxDemanda(int _max_demanda) { max_demanda = _max_demanda; }

   void setMaxAlunosTeo(int _max_alunos_t) { max_alunos_t = _max_alunos_t; }

   void setMaxAlunosPrat(int _max_alunos_p) { max_alunos_p = _max_alunos_p; }

   // =========== METODOS GET
   int getDemandaTotal() const { return demandaTotal; }

   int getMaxDemanda() const { return max_demanda; }

   int getMaxAlunosTeo() const { return max_alunos_t; }

   int getMaxAlunosPrat() const { return max_alunos_p; }

   // =========== METODOS AUXILIARES
   virtual void le_arvore(ItemDisciplina& elem);

   bool eLab() const { return e_lab; }

   int adicionaDemandaTotal(int demanda_total) { return (demandaTotal += demanda_total); }

   // <<<

   // Armazena os dias letivos em que a disciplina pode ser ministrada.
   GGroup<int> diasLetivos;

   //Disciplina& operator= (const Disciplina& d);
};

/*
Disciplina& Disciplina::operator= (const Disciplina& d)
{
if (this == &d) return *this;   // self assignment

this->codigo = d.codigo;
this->nome = d.nome;
this->cred_teoricos = d.cred_teoricos;
this->cred_praticos = d.cred_praticos;
this->e_lab = d.e_lab;
this->max_alunos_t = d.max_alunos_t;
this->max_alunos_p = d.max_alunos_p;
this->tipo_disciplina_id = d.tipo_disciplina_id;
this->nivel_dificuldade_id = d.nivel_dificuldade_id;

//DivisaoCreditos* divisao_creditos;
this->divisao_creditos = new DivisaoCreditos();
this->divisao_creditos->setId(d.divisao_creditos->getId());
this->divisao_creditos->creditos = d.divisao_creditos->creditos;
for(int i=0;i<8;i++) {
this->divisao_creditos->dia[i] = d.divisao_creditos->dia[i];		
}

GGroup<int>::iterator it_eq = d.equivalentes.begin();

for(unsigned i=0;i<d.equivalentes.size();i++) {
this->equivalentes.add(*it_eq);
it_eq++;
}

GGroup<int>::iterator it_inc = d.incompativeis.begin();

for(unsigned i=0;i<d.incompativeis.size();i++) {
this->incompativeis.add(*it_inc);
it_inc++;
}

//>>> Copying HORARIO
ITERA_GGROUP(it_hr,d.horarios,Horario) {
Horario *h =  new Horario;
h->setId(it_hr->getId());

//>>> >>> Copying DiaSemana
GGroup<int>::iterator it_dia = it_hr->dias_semana.begin();
for(unsigned dia =0;dia<it_hr->dias_semana.size();dia++) {
h->dias_semana.add(*it_dia);
it_dia++;
}
// <<< <<<

h->horarioAulaId = it_hr->horarioAulaId;

h->turnoId = it_hr->turnoId;

// >>> >>> Copying TURNO
Turno *tur = new Turno();

tur->setId(it_hr->turno->getId());
tur->nome = it_hr->turno->nome;
tur->tempoAula = it_hr->turno->tempoAula;

// >>> >>> >>> Copying HorariosAula
HorarioAula *hr_aula;
ITERA_GGROUP(it_hr_aula,tur->horarios_aula,HorarioAula) {
//HorarioAula *hr_aula = new HorarioAula();
hr_aula = new HorarioAula();
hr_aula->setId(it_hr_aula->getId());
hr_aula->inicio = it_hr_aula->inicio;

GGroup<int>::iterator it_dia_sem = it_hr_aula->diasSemana.begin();
for(unsigned dia =0;dia<it_hr_aula->diasSemana.size();dia++) {
hr_aula->diasSemana.add(*it_dia_sem);
it_dia_sem++;
}
//tur->horarios_aula = hr_aula;
}
tur->horarios_aula.add(hr_aula);
// <<< <<< <<<
h->turno = tur;
// <<< <<<
this->horarios.add(h);
}
// <<<

this->tipo_disciplina->setId(d.tipo_disciplina->getId());
this->tipo_disciplina->nome = d.tipo_disciplina->nome;

this->nivel_dificuldade->setId(d.nivel_dificuldade->getId());
this->nivel_dificuldade->nome = d.nivel_dificuldade->nome;

this->demanda_total = d.demanda_total;
this->max_demanda = d.max_demanda;
this->num_turmas = d.num_turmas;
this->min_creds = d.min_creds;
this->max_creds = d.max_creds;

std::cout << "Disciplina::Operator= finalized\n";

return *this;
}
*/

/*
Disciplina& Disciplina::operator= (const Disciplina& d)
{
if (this == &d) return *this;   // self assignment

Disciplina *dd = new Disciplina();

dd->codigo = d.codigo;
dd->nome = d.nome;
dd->cred_teoricos = d.cred_teoricos;
dd->cred_praticos = d.cred_praticos;
dd->e_lab = d.e_lab;
dd->max_alunos_t = d.max_alunos_t;
dd->max_alunos_p = d.max_alunos_p;
dd->tipo_disciplina_id = d.tipo_disciplina_id;
dd->nivel_dificuldade_id = d.nivel_dificuldade_id;

//DivisaoCreditos* divisao_creditos;
dd->divisao_creditos = new DivisaoCreditos();
dd->divisao_creditos->setId(d.divisao_creditos->getId());
dd->divisao_creditos->creditos = d.divisao_creditos->creditos;
for(int i=0;i<8;i++) {
dd->divisao_creditos->dia[i] = d.divisao_creditos->dia[i];		
}

GGroup<int>::iterator it_eq = d.equivalentes.begin();

for(unsigned i=0;i<d.equivalentes.size();i++) {
dd->equivalentes.add(*it_eq);
it_eq++;
}

GGroup<int>::iterator it_inc = d.incompativeis.begin();

for(unsigned i=0;i<d.incompativeis.size();i++) {
dd->incompativeis.add(*it_inc);
it_inc++;
}

//>>> Copying HORARIO
ITERA_GGROUP(it_hr,d.horarios,Horario) {
Horario *h =  new Horario;
h->setId(it_hr->getId());

//>>> >>> Copying DiaSemana
GGroup<int>::iterator it_dia = it_hr->dias_semana.begin();
for(unsigned dia =0;dia<it_hr->dias_semana.size();dia++) {
h->dias_semana.add(*it_dia);
it_dia++;
}
// <<< <<<

h->horarioAulaId = it_hr->horarioAulaId;

h->turnoId = it_hr->turnoId;

// >>> >>> Copying TURNO
Turno *tur = new Turno();

tur->setId(it_hr->turno->getId());
tur->nome = it_hr->turno->nome;
tur->tempoAula = it_hr->turno->tempoAula;

// >>> >>> >>> Copying HorariosAula
HorarioAula *hr_aula;
ITERA_GGROUP(it_hr_aula,tur->horarios_aula,HorarioAula) {
//HorarioAula *hr_aula = new HorarioAula();
hr_aula = new HorarioAula();
hr_aula->setId(it_hr_aula->getId());
hr_aula->inicio = it_hr_aula->inicio;

GGroup<int>::iterator it_dia_sem = it_hr_aula->diasSemana.begin();
for(unsigned dia =0;dia<it_hr_aula->diasSemana.size();dia++) {
hr_aula->diasSemana.add(*it_dia_sem);
it_dia_sem++;
}
//tur->horarios_aula = hr_aula;
}
tur->horarios_aula.add(hr_aula);
// <<< <<< <<<
h->turno = tur;
// <<< <<<
dd->horarios.add(h);
}
// <<<

dd->tipo_disciplina->setId(d.tipo_disciplina->getId());
dd->tipo_disciplina->nome = d.tipo_disciplina->nome;

dd->nivel_dificuldade->setId(d.nivel_dificuldade->getId());
dd->nivel_dificuldade->nome = d.nivel_dificuldade->nome;

dd->demanda_total = d.demanda_total;
dd->max_demanda = d.max_demanda;
dd->num_turmas = d.num_turmas;
dd->min_creds = d.min_creds;
dd->max_creds = d.max_creds;

std::cout << "Disciplina::Operator= finalized\n";

return *dd;
}
*/