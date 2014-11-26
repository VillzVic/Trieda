#include "ConstraintMIPUnico.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == nullptr && b != nullptr) || \
   ( b != nullptr && a != nullptr && ( *a < *b ) )

#define E_MENOR_PAR( a, b ) \
   ( ( ( a.first == nullptr && b.first != nullptr) || \
       ( b.first != nullptr && a.first != nullptr && ( *a.first < *b.first ) ) ) \
	 || \
	 ( ( a.first == b.first) && \
	   ( ( a.second == nullptr && b.second != nullptr) || \
		 ( b.second != nullptr && a.second != nullptr && ( *a.second < *b.second ) ) \
	   ) \
	 ) \
   )

ConstraintMIPUnico::ConstraintMIPUnico()
{
   reset();
}

ConstraintMIPUnico::ConstraintMIPUnico( const ConstraintMIPUnico & cons )
{
   *this = cons;
}

ConstraintMIPUnico::~ConstraintMIPUnico()
{
   reset();
}

ConstraintMIPUnico& ConstraintMIPUnico::operator = ( const ConstraintMIPUnico & cons )
{   
   this->type = cons.getType();

   this->cp = cons.getCampus();
   this->u = cons.getUnidade();
   this->s = cons.getSala();
   this->tps = cons.getSubCjtSala();
   this->i = cons.getTurma();
   this->c = cons.getCurso();
   this->c_incompat = cons.getCursoIncompat();
   this->b = cons.getBloco();
   this->d = cons.getDisciplina();
   this->j = cons.getSubBloco();
   this->t = cons.getDia();
   this->o = cons.getOferta();
   this->sl = cons.getSemanaLetiva();
   this->parCursos = cons.getParCursos();
   this->cjtSalaCompart = cons.getSubCjtSalaCompart();
   this->parOfts = cons.getParOfertas();
   this->h = cons.getHorarioAula();
   this->parDiscs = cons.getParDisciplinas();
   this->aluno = cons.getAluno();
   this->turma1 = cons.getTurma1();
   this->turma2 = cons.getTurma2();
   this->disc1 = cons.getDisciplina1();
   this->disc2 = cons.getDisciplina2();
   this->horarioAulaI = cons.getHorarioAulaInicial();
   this->horarioAulaF = cons.getHorarioAulaFinal();
   this->dateTimeI = cons.getDateTimeInicial();
   this->dateTimeF = cons.getDateTimeFinal();
   this->parAlunos = cons.getParAlunos();
   this->periodo = cons.getPeriodo();
   this->professor_ = cons.getProfessor();
   this->faseDoDia_ = cons.getFaseDoDia();
   this->u_orig = cons.getUnidOrig();
   this->u_dest = cons.getUnidDest();
   this->u_atual = cons.getUnidAtual();
   this->h_atual = cons.getHorarioAulaAtual();
   this->h_dest = cons.getHorarioAulaDest();
   this->h_orig = cons.getHorarioAulaOrig();

   return *this;
}

bool ConstraintMIPUnico::operator < ( const ConstraintMIPUnico & cons ) const
{

   if( (int)this->getType() < (int) cons.getType() )
      return true;
   else if( (int)this->getType() > (int) cons.getType() )
      return false;

   if(E_MENOR(this->getCampus(), cons.getCampus())) return true;
   if(E_MENOR(cons.getCampus(), this->getCampus())) return false;

   if (E_MENOR(this->getUnidade(),cons.getUnidade())) return true;
   if (E_MENOR(cons.getUnidade(), this->getUnidade())) return false;

   if (E_MENOR(this->getSala(),cons.getSala())) return true;
   if (E_MENOR(cons.getSala(), this->getSala())) return false;

   if (E_MENOR(this->getSubCjtSala(),cons.getSubCjtSala())) return true;
   if (E_MENOR(cons.getSubCjtSala(), this->getSubCjtSala())) return false;

   if (this->getTurma() < cons.getTurma()) return true;
   if (this->getTurma() > cons.getTurma()) return false;

   if(E_MENOR(this->getCurso(),cons.getCurso())) return true;
   if(E_MENOR(cons.getCurso(), this->getCurso())) return false;

   if(E_MENOR(this->getCursoIncompat(),cons.getCursoIncompat())) return true;
   if(E_MENOR(cons.getCursoIncompat(), this->getCursoIncompat())) return false;

   if (E_MENOR(this->getBloco(),cons.getBloco())) return true;
   if (E_MENOR(cons.getBloco(), this->getBloco())) return false;

   if (E_MENOR(this->getDisciplina(),cons.getDisciplina())) return true;
   if (E_MENOR(cons.getDisciplina(), this->getDisciplina())) return false;

   if ( this->getSubBloco() < cons.getSubBloco() ) return true;
   if ( cons.getSubBloco() < this->getSubBloco() ) return false;

   if (this->getDia() < cons.getDia()) return true;
   if (this->getDia() > cons.getDia()) return false;

   if (E_MENOR(this->getOferta(),cons.getOferta())) return true;
   if (E_MENOR(cons.getOferta(), this->getOferta())) return false;

   if (this->getSemanaLetiva() < cons.getSemanaLetiva()) return true;
   if (this->getSemanaLetiva() > cons.getSemanaLetiva()) return false;

   if (E_MENOR(this->getSubCjtSalaCompart(),cons.getSubCjtSalaCompart())) return true;
   if (E_MENOR(cons.getSubCjtSalaCompart(), this->getSubCjtSalaCompart())) return false;

   if ( E_MENOR_PAR( this->getParCursos(), cons.getParCursos() ) ) return true;
   if ( E_MENOR_PAR( cons.getParCursos(), this->getParCursos() ) ) return false;

   if ( E_MENOR_PAR( this->getParOfertas(), cons.getParOfertas() ) ) return true;
   if ( E_MENOR_PAR( cons.getParOfertas(), this->getParOfertas() ) ) return false;

   if (E_MENOR(this->getHorarioAula(),cons.getHorarioAula())) return true;
   if (E_MENOR(cons.getHorarioAula(), this->getHorarioAula())) return false;

   if ( E_MENOR_PAR( this->getParDisciplinas(), cons.getParDisciplinas() ) ) return true;
   if ( E_MENOR_PAR( cons.getParDisciplinas(), this->getParDisciplinas() ) ) return false;

   if (E_MENOR(this->getAluno(),cons.getAluno())) return true;
   if (E_MENOR(cons.getAluno(), this->getAluno())) return false;
      
   if ( this->getAluno() < cons.getAluno() ) return true;
   if ( this->getAluno() > cons.getAluno() ) return false;
   
   if ( this->getTurma1() < cons.getTurma1() ) return true;
   if ( this->getTurma1() > cons.getTurma1() ) return false;
   
   if ( this->getTurma2() < cons.getTurma2() ) return true;
   if ( this->getTurma2() > cons.getTurma2() ) return false;

   if ( E_MENOR( this->getDisciplina1(),cons.getDisciplina1() ) ) return true;
   if ( E_MENOR( cons.getDisciplina1(), this->getDisciplina1() ) ) return false;

   if ( E_MENOR( this->getDisciplina2(),cons.getDisciplina2() ) ) return true;
   if ( E_MENOR( cons.getDisciplina2(), this->getDisciplina2() ) ) return false;
   
   if ( this->getDateTimeInicial() < cons.getDateTimeInicial() ) return true;
   if ( this->getDateTimeInicial() > cons.getDateTimeInicial() ) return false;
   
   if ( this->getDateTimeFinal() < cons.getDateTimeFinal() ) return true;
   if ( this->getDateTimeFinal() > cons.getDateTimeFinal() ) return false;
   
   if ( E_MENOR_PAR( this->getParAlunos(), cons.getParAlunos() ) ) return true;
   if ( E_MENOR_PAR( cons.getParAlunos(), this->getParAlunos() ) ) return false;
   
   if (this->getPeriodo() < cons.getPeriodo()) return true;
   if (this->getPeriodo() > cons.getPeriodo()) return false;

   if ( E_MENOR( this->getProfessor(),cons.getProfessor() ) ) return true;
   if ( E_MENOR( cons.getProfessor(), this->getProfessor() ) ) return false;
   
   if (this->getFaseDoDia() < cons.getFaseDoDia()) return true;
   if (this->getFaseDoDia() > cons.getFaseDoDia()) return false;
   
   if (E_MENOR(this->getUnidOrig(),cons.getUnidOrig())) return true;
   if (E_MENOR(cons.getUnidOrig(), this->getUnidOrig())) return false;
   
   if (E_MENOR(this->getUnidDest(),cons.getUnidDest())) return true;
   if (E_MENOR(cons.getUnidDest(), this->getUnidDest())) return false;
   
   if (E_MENOR(this->getUnidAtual(),cons.getUnidAtual())) return true;
   if (E_MENOR(cons.getUnidAtual(), this->getUnidAtual())) return false;
   
   if ( compLessHorarioAula( this->getHorarioAulaDest(), cons.getHorarioAulaDest() ) ) return true;
   if ( compLessHorarioAula( cons.getHorarioAulaDest(), this->getHorarioAulaDest() ) ) return false;
   
   if ( compLessHorarioAula( this->getHorarioAulaAtual(), cons.getHorarioAulaAtual() ) ) return true;
   if ( compLessHorarioAula( cons.getHorarioAulaAtual(), this->getHorarioAulaAtual() ) ) return false;

   if ( compLessHorarioAula( this->getHorarioAulaOrig(), cons.getHorarioAulaOrig() ) ) return true;
   if ( compLessHorarioAula( cons.getHorarioAulaOrig(), this->getHorarioAulaOrig() ) ) return false;
      
   return false;

}

bool ConstraintMIPUnico::compLessHorarioAula( HorarioAula* h1, HorarioAula* h2 ) const
{
   if ( ( h1 == nullptr && h2 != nullptr) ||
	   ( h2 != nullptr && h1 != nullptr && ( h1->comparaMenor(*h2) ) ) )
		return true;
   return false;
}

bool ConstraintMIPUnico::operator== (const ConstraintMIPUnico& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void ConstraintMIPUnico::reset()
{
   cp = nullptr;
   u = nullptr;
   s = nullptr;
   cjtSalaCompart = nullptr;
   tps = nullptr;
   i = -1;
   c = nullptr;
   c_incompat = nullptr;
   b = nullptr;
   d = nullptr;
   j = -1;
   t = -1;
   o = nullptr;
   sl = nullptr;
   parCursos.first = nullptr;
   parCursos.second = nullptr;
   parOfts.first = nullptr;
   parOfts.second = nullptr;
   h = nullptr;
   parDiscs.first = nullptr;
   parDiscs.second = nullptr;
   aluno = nullptr;
   turma1 = -1;
   turma2 = -1;
   disc1 = nullptr;
   disc2 = nullptr;
   horarioAulaI = nullptr;
   horarioAulaF = nullptr;
   dateTimeI = DateTime();
   dateTimeF = DateTime();
   parAlunos.first = nullptr;
   parAlunos.second = nullptr;
   periodo = -1;
   professor_ = nullptr;
   faseDoDia_ = -1;
   u_orig = nullptr;
   u_dest = nullptr;
   u_atual = nullptr;
   h_orig = nullptr;
   h_dest = nullptr;
   h_atual = nullptr;
   
}

std::string ConstraintMIPUnico::toString( int etapa )
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]__(C_INT" << etapa;

   switch( type )
   {
   case C_SALA_HORARIO:
      ss << "_SALA_HORARIO):"; break;	  
   case C_UNICO_ATEND_TURMA_DISC_DIA:
      ss << "_UNICO_ATEND_TURMA_DISC_DIA):"; break;
   case C_PROIBE_COMPARTILHAMENTO:
      ss << "_PROIBE_COMPARTILHAMENTO):"; break;
   case C_TURMA_DISC_DIAS_CONSEC:
      ss << "_TURMA_DISC_DIAS_CONSEC):"; break;	  
   case C_MIN_ALUNOS_TURMA:
      ss << "_MIN_ALUNOS_TURMA):"; break;
   case C_DIVISAO_CREDITO:
      ss << "_DIVISAO_CREDITO):"; break;	  
   case C_COMBINACAO_DIVISAO_CREDITO:
      ss << "_COMBINACAO_DIVISAO_CREDITO):"; break;	  
   case C_VAR_ZC:
      ss << "_VAR_ZC):"; break;	  
   case C_DISC_INCOMPATIVEIS:
      ss << "_DISC_INCOMPATIVEIS):"; break;	   
   case C_ALUNO_HORARIO:
      ss << "_ALUNO_HORARIO):"; break;	  	  
   case C_ALUNO_VARIAS_UNIDADES_DIA:
      ss << "_ALUNO_VARIAS_UNIDADES_DIA):"; break;	  
   case C_ALUNO_DISC_PRATICA_TEORICA:
      ss << "_ALUNO_DISC_PRATICA_TEORICA):"; break;	  
   case C_DISC_PRATICA_TEORICA:
      ss << "_DISC_PRATICA_TEORICA):"; break;	  
   case C_MIN_DIAS_ALUNO:
      ss << "_MIN_DIAS_ALUNO):"; break;	  
   case C_MAX_DIAS_ALUNO:
      ss << "_MAX_DIAS_ALUNO):"; break;	  
   case C_ASSOCIA_V_X:
      ss << "_ASSOCIA_V_X):"; break;	  
	case C_DEMANDA_DISC_ALUNO:
      ss << "_DEMANDA_DISC_ALUNO):"; break;	  
	case C_SALA_UNICA:
      ss << "_SALA_UNICA):"; break;
	case C_SALA_TURMA:
      ss << "_SALA_TURMA):"; break;	  
	case C_ASSOCIA_S_V:
      ss << "_ASSOCIA_S_V):"; break; 
	case C_ABRE_TURMAS_EM_SEQUENCIA:
      ss << "_ABRE_TURMAS_EM_SEQUENCIA):"; break;
	case C_ALUNO_CURSO:
      ss << "_ALUNO_CURSO):"; break;
	case C_ALUNO_PRIORIDADES_DEMANDA:
      ss << "_ALUNO_PRIORIDADES_DEMANDA):"; break;
	case C_DEMANDA_EQUIV_ALUNO:
      ss << "_DEMANDA_EQUIV_ALUNO):"; break;  
	case C_ALUNO_DISC_PRATICA_TEORICA_EQUIV:
      ss << "_ALUNO_DISC_PRATICA_TEORICA_EQUIV):"; break;  	  
	case C_ATIVA_Z:
      ss << "_ATIVA_Z):"; break;  	    
	case C_TURMA_COM_MESMOS_ALUNOS_POR_AULA:
      ss << "_TURMA_COM_MESMOS_ALUNOS_POR_AULA):"; break;  	      
	case C_DISC_DIA_HOR_PROF:
      ss << "_DISC_DIA_HOR_PROF):"; break;  
	case C_ALUNO_DISC_PRATICA_TEORICA_1x1:
      ss << "_ALUNO_DISC_PRATICA_TEORICA_1x1):"; break;  
	case C_AULA_PT_SEQUENCIAL:
      ss << "_AULA_PT_SEQUENCIAL):"; break;  
	case C_ALUNO_DISC_PRATICA_TEORICA_1xN:
      ss << "_ALUNO_DISC_PRATICA_TEORICA_1xN):"; break;  
	case C_ALUNOS_MESMA_TURMA_PRAT:
      ss << "_ALUNOS_MESMA_TURMA_PRAT):"; break;  
	case C_ALOC_MIN_ALUNO:
      ss << "_ALOC_MIN_ALUNO):"; break;  			  		  	
	case C_FOLGA_OCUPACAO_SALA:
      ss << "C_FOLGA_OCUPACAO_SALA):"; break;
	case C_PROF_MIN_DESCANSO:
      ss << "C_PROF_MIN_DESCANSO):"; break;				  
	case C_MAX_ALUNOS_TURMA:
      ss << "C_MAX_ALUNOS_TURMA):"; break;	  
	  
	  
	case C_PROF_DIA_HOR:
      ss << "C_PROF_DIA_HOR):"; break;
	case C_PROF_AULA:
      ss << "C_PROF_AULA):"; break;
	case C_PROF_AULA_SUM:
      ss << "C_PROF_AULA_SUM):"; break;
	case C_PROF_TURMA:
      ss << "C_PROF_TURMA):"; break;
	case C_PROF_UNICO:
      ss << "C_PROF_UNICO):"; break;	  
   case C_GAPS_PROFESSORES_I_F:
      ss << "C_GAPS_PROFESSORES_I_F):"; break;
   case C_GAPS_PROFESSORES_I:
      ss << "C_GAPS_PROFESSORES_I):"; break;
   case C_GAPS_PROFESSORES_F:
      ss << "C_GAPS_PROFESSORES_F):"; break;
   case C_GAPS_ALUNOS_I_F:
      ss << "C_GAPS_ALUNOS_I_F):"; break;
   case C_GAPS_ALUNOS_I:
      ss << "C_GAPS_ALUNOS_I):"; break;
   case C_GAPS_ALUNOS_F:
      ss << "C_GAPS_ALUNOS_F):"; break;
	  
      case C_PROF_HOR_INIC_LB:
        ss << "C_PROF_HOR_INIC_LB"; break;
      case C_PROF_HOR_INIC_UB:
        ss << "C_PROF_HOR_INIC_UB"; break;	
      case C_PROF_HOR_FIN_LB:
        ss << "C_PROF_HOR_FIN_LB"; break;
      case C_PROF_HOR_FIN_UB:
        ss << "C_PROF_HOR_FIN_UB"; break;	
      case C_PROF_GAP:
        ss << "C_PROF_GAP"; break;	

      case C_ALUNO_HOR_INIC_LB:
        ss << "C_ALUNO_HOR_INIC_LB"; break;
      case C_ALUNO_HOR_INIC_UB:
        ss << "C_ALUNO_HOR_INIC_UB"; break;	
      case C_ALUNO_HOR_FIN_LB:
        ss << "C_ALUNO_HOR_FIN_LB"; break;
      case C_ALUNO_HOR_FIN_UB:
        ss << "C_ALUNO_HOR_FIN_UB"; break;	
      case C_ALUNO_GAP:
        ss << "C_ALUNO_GAP"; break;	
		
      case C_ALUNO_MIN_CREDS_DIA:
        ss << "C_ALUNO_MIN_CREDS_DIA"; break;		
      case C_TEMPO_DESLOC_PROF:
        ss << "C_TEMPO_DESLOC_PROF"; break;
	  case C_NR_DESLOC_PROF:
        ss << "C_NR_DESLOC_PROF"; break;	

   default:
      ss << "!";
   }

   ss << "__{";
   
   if ( cp != nullptr )
   {
      ss << "_Cp" << cp->getId();
   }

   if ( professor_ != nullptr )
   {
      ss << "_Prof" << professor_->getId();
   }

   if ( i >= 0 )
   {
      ss << "_Turma" << i;
   }
   
   if ( d != nullptr )
   {
      ss << "_Disc" << d->getId();
   }

   if ( u != nullptr )
   {
      ss << "_Unid" << u->getId();
   }

   if ( c != nullptr )
   {
      ss << "_Curso" << c->getId();
   }

   if ( c_incompat != nullptr )
   {
      ss << "_CursoIncomp" << c_incompat->getId();
   }

   if ( b != nullptr )
   {
	   ss << "_Bc" << b->getId();
   }

   if ( j >=0 )
   {
      ss << "_Sbc" << j;
   }

   if ( parCursos.first != nullptr && parCursos.second != nullptr )
   {
	   ss << "_(Curso" << parCursos.first->getId(); 	
	   ss << ",Curso" << parCursos.second->getId() << ")";
   }

   if ( tps != nullptr )
   {
      ss << "_Tps" << tps->getId();
	  if ( tps->salas.size()==1 ) ss << "idS" << tps->salas.begin()->first;
   }

   if ( cjtSalaCompart != nullptr )
   {
	   ss << "_TpsCompart" << cjtSalaCompart->getId();
	   if ( tps->salas.size()==1 ) ss << "idS" << cjtSalaCompart->salas.begin()->first;
   }

   if ( s != nullptr )
   {
      ss << "_Sala" << s->getId();
   }

   if ( t >= 0 )
   {
      ss << "_Dia" << t;
   }

   if ( o != nullptr )
   {
      ss << "_Oferta" << o->getId();
   }
   
   if ( parOfts.first != nullptr && parOfts.second != nullptr )
   {
      ss << "_(Oft" << parOfts.first->getId();
      ss << ",Oft" << parOfts.second->getId() << ")";
   }
   
   if ( sl != nullptr )
   {
	   ss << "_SL" << sl->getId();
   }

   if ( h != nullptr )
   {
	   ss << "_h" << h->getId();
   }

   if ( parDiscs.first != nullptr && parDiscs.second != nullptr )
   {
      ss << "_(Disc" << parDiscs.first->getId();
      ss << ",Disc" << parDiscs.second->getId() << ")";
   }

   if ( aluno != nullptr )
   {
	   ss << "_Aluno" << aluno->getAlunoId();
   }
      
   if ( turma1 >= 0 )
   {
      ss << "_Turma1." << turma1;
   }

   if ( disc1 != nullptr )
   {
      ss << "_Disc1." << disc1->getId();
   }
   
   if ( turma2 >= 0 )
   {
      ss << "_Turma2." << turma2;
   }

   if ( disc2 != nullptr )
   {
      ss << "_Disc2." << disc2->getId();
   }
   
   if ( horarioAulaI )
   {
      ss << "_Hi" << horarioAulaI->getId();
   }

   if ( horarioAulaF )
   {
      ss << "_Hf" << horarioAulaF->getId();
   }

   if ( dateTimeI != DateTime() )
   {
      ss << "_Dti" << dateTimeI.getHour() << "_" << dateTimeI.getMinute();
   }

   if ( dateTimeF != DateTime() )
   {
      ss << "_Dtf" << dateTimeF.getHour() << "_" << dateTimeF.getMinute();
   }
   
   if ( periodo >= 0 )
   {
      ss << "_Periodo" << periodo;
   }

   if ( faseDoDia_ >= 0 )
   {
      ss << "_Fase" << faseDoDia_;
   }

   if ( u_orig )
   {
      ss << "_uOrig" << u_orig->getId();
   }
     
   if ( u_atual )
   {
      ss << "_uAtual" << u_atual->getId();
   }

   if ( u_dest )
   {
      ss << "_uDest" << u_dest->getId();
   }

   if ( h_orig )
   {
      ss << "_hOrig" << h_orig->getId();
   }
     
   if ( h_atual )
   {
      ss << "_hAtual" << h_atual->getId();
   }

   if ( h_dest )
   {
      ss << "_hDest" << h_dest->getId();
   }



   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}

