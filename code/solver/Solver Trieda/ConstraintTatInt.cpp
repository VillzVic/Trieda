#include "ConstraintTatInt.h"
#include "HashUtil.h"

#define E_MENOR( a, b ) \
   ( a == NULL && b != NULL) || \
   ( b != NULL && a != NULL && ( *a < *b ) )

#define E_MENOR_PAR( a, b ) \
   ( ( ( a.first == NULL && b.first != NULL) || \
       ( b.first != NULL && a.first != NULL && ( *a.first < *b.first ) ) ) \
	 || \
	 ( ( a.first == b.first) && \
	   ( ( a.second == NULL && b.second != NULL) || \
		 ( b.second != NULL && a.second != NULL && ( *a.second < *b.second ) ) \
	   ) \
	 ) \
   )

ConstraintTatInt::ConstraintTatInt()
{
   reset();
}

ConstraintTatInt::ConstraintTatInt( const ConstraintTatInt & cons )
{
   *this = cons;
}

ConstraintTatInt::~ConstraintTatInt()
{
   reset();
}

ConstraintTatInt& ConstraintTatInt::operator = ( const ConstraintTatInt & cons )
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

   return *this;
}

bool ConstraintTatInt::operator < ( const ConstraintTatInt & cons ) const
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

   /*if ( E_MENOR( this->getHorarioAulaInicial(), cons.getHorarioAulaInicial() ) ) return true;
   if ( E_MENOR( cons.getHorarioAulaInicial(), this->getHorarioAulaInicial() ) ) return false;

   if ( E_MENOR( this->getHorarioAulaFinal(), cons.getHorarioAulaFinal() ) ) return true;
   if ( E_MENOR( cons.getHorarioAulaFinal(), this->getHorarioAulaFinal() ) ) return false;*/

   if ( E_MENOR( this->getDateTimeInicial(), cons.getDateTimeInicial() ) ) return true;
   if ( E_MENOR( cons.getDateTimeInicial(), this->getDateTimeInicial() ) ) return false;

   if ( E_MENOR( this->getDateTimeFinal(), cons.getDateTimeFinal() ) ) return true;
   if ( E_MENOR( cons.getDateTimeFinal(), this->getDateTimeFinal() ) ) return false;
   
   if ( E_MENOR_PAR( this->getParAlunos(), cons.getParAlunos() ) ) return true;
   if ( E_MENOR_PAR( cons.getParAlunos(), this->getParAlunos() ) ) return false;
   
   if (this->getPeriodo() < cons.getPeriodo()) return true;
   if (this->getPeriodo() > cons.getPeriodo()) return false;

   return false;

}

bool ConstraintTatInt::operator== (const ConstraintTatInt& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void ConstraintTatInt::reset()
{
   cp = NULL;
   u = NULL;
   s = NULL;
   cjtSalaCompart = NULL;
   tps = NULL;
   i = -1;
   c = NULL;
   c_incompat = NULL;
   b = NULL;
   d = NULL;
   j = -1;
   t = -1;
   o = NULL;
   sl = NULL;
   parCursos.first = NULL;
   parCursos.second = NULL;
   parOfts.first = NULL;
   parOfts.second = NULL;
   h = NULL;
   parDiscs.first = NULL;
   parDiscs.second = NULL;
   aluno = NULL;
   turma1 = -1;
   turma2 = -1;
   disc1 = NULL;
   disc2 = NULL;
   horarioAulaI = NULL;
   horarioAulaF = NULL;
   dateTimeI = NULL;
   dateTimeF = NULL;
   parAlunos.first = NULL;
   parAlunos.second = NULL;
   periodo = -1;
}

std::string ConstraintTatInt::toString( int etapa )
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
	case C_FORMANDOS1:
      ss << "_FORMANDOS1):"; break;	  
	case C_FORMANDOS2:
      ss << "_FORMANDOS2):"; break;	    
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
	case C_CAPACIDADE_SALA:
      ss << "C_CAPACIDADE_SALA):"; break;  				  
	case C_SALA_USADA:
      ss << "C_SALA_USADA):"; break;  				   
	case C_ATEND_TOTAL:
      ss << "C_ATEND_TOTAL):"; break;  				  
	case C_UNID_PERIODO:
      ss << "C_UNID_PERIODO):"; break;  				  
	case C_USA_UNID_PERIODO:
      ss << "C_USA_UNID_PERIODO):"; break;  				  
	case C_PROF_MIN_DESCANSO:
      ss << "C_PROF_MIN_DESCANSO):"; break;				  
	case C_MAX_ALUNOS_TURMA:
      ss << "C_MAX_ALUNOS_TURMA):"; break;	  

   default:
      ss << "!";
   }

   ss << "__{";

   if ( i >= 0 )
   {
      ss << "_Turma" << i;
   }
   
   if ( d != NULL )
   {
      ss << "_Disc" << d->getId();
   }

   if ( u != NULL )
   {
      ss << "_Unid" << u->getId();
   }

   if ( c != NULL )
   {
      ss << "_Curso" << c->getId();
   }

   if ( c_incompat != NULL )
   {
      ss << "_CursoIncomp" << c_incompat->getId();
   }

   if ( b != NULL )
   {
	   ss << "_Bc" << b->getId();
   }

   if ( j >=0 )
   {
      ss << "_Sbc" << j;
   }

   if ( parCursos.first != NULL && parCursos.second != NULL )
   {
	   ss << "_(Curso" << parCursos.first->getId(); 	
	   ss << ",Curso" << parCursos.second->getId() << ")";
   }

   if ( tps != NULL )
   {
      ss << "_Tps" << tps->getId();
	  if ( tps->salas.size()==1 ) ss << "idS" << tps->salas.begin()->first;
   }

   if ( cjtSalaCompart != NULL )
   {
	   ss << "_TpsCompart" << cjtSalaCompart->getId();
	   if ( tps->salas.size()==1 ) ss << "idS" << cjtSalaCompart->salas.begin()->first;
   }

   if ( s != NULL )
   {
      ss << "_Sala" << s->getId();
   }

   if ( t >= 0 )
   {
      ss << "_Dia" << t;
   }

   if ( o != NULL )
   {
      ss << "_Oferta" << o->getId();
   }
   
   if ( parOfts.first != NULL && parOfts.second != NULL )
   {
      ss << "_(Oft" << parOfts.first->getId();
      ss << ",Oft" << parOfts.second->getId() << ")";
   }
   
   if ( sl != NULL )
   {
	   ss << "_SL" << sl->getId();
   }

   if ( h != NULL )
   {
	   ss << "_h" << h->getId();
   }

   if ( parDiscs.first != NULL && parDiscs.second != NULL )
   {
      ss << "_(Disc" << parDiscs.first->getId();
      ss << ",Disc" << parDiscs.second->getId() << ")";
   }

   if ( aluno != NULL )
   {
	   ss << "_Aluno" << aluno->getAlunoId();
   }
      
   if ( turma1 >= 0 )
   {
      ss << "_Turma1." << turma1;
   }

   if ( disc1 != NULL )
   {
      ss << "_Disc1." << disc1->getId();
   }
   
   if ( turma2 >= 0 )
   {
      ss << "_Turma2." << turma2;
   }

   if ( disc2 != NULL )
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

   if ( dateTimeI )
   {
      ss << "_Dti" << dateTimeI->getHour() << "_" << dateTimeI->getMinute();
   }

   if ( dateTimeF )
   {
      ss << "_Dtf" << dateTimeF->getHour() << "_" << dateTimeF->getMinute();
   }
   
   if ( periodo >= 0 )
   {
      ss << "_Periodo" << periodo;
   }

   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}

