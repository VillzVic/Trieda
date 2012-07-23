#include "ConstraintTatico.h"
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

ConstraintTatico::ConstraintTatico()
{
   reset();
}

ConstraintTatico::ConstraintTatico( const ConstraintTatico & cons )
{
   *this = cons;
}

ConstraintTatico::~ConstraintTatico()
{
   reset();
}

ConstraintTatico& ConstraintTatico::operator = ( const ConstraintTatico & cons )
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

   return *this;
}

bool ConstraintTatico::operator < ( const ConstraintTatico & cons ) const
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

   return false;

}

bool ConstraintTatico::operator== (const ConstraintTatico& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void ConstraintTatico::reset()
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

}

std::string ConstraintTatico::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";

   switch( type )
   {
   case C_CARGA_HORARIA:
      ss << "__(C_CARGA_HORARIA):"; break;
   case C_SALA_HORARIO:
      ss << "__(C_SALA_HORARIO):"; break;	  
   case C_UNICO_ATEND_TURMA_DISC_DIA:
      ss << "__(C_UNICO_ATEND_TURMA_DISC_DIA):"; break;
   case C_DEMANDA_DISC:
      ss << "__(C_DEMANDA_DISC):"; break;
//   case C_DEMANDA_DISC_ALUNO:
//      ss << "__(C_DEMANDA_DISC_ALUNO):"; break;
   case C_TURMA_DISC_DIAS_CONSEC:
      ss << "__(C_TURMAS_DISC_DIAS_CONSEC):"; break;	  
   case C_LIMITA_ABERTURA_TURMAS:
      ss << "__(C_LIMITA_ABERTURA_TURMAS):"; break;
   case C_DIVISAO_CREDITO:
      ss << "__(C_DIVISAO_CREDITO):"; break;	  
   case C_COMBINACAO_DIVISAO_CREDITO:
      ss << "__(C_COMBINACAO_DIVISAO_CREDITO):"; break;	  
   case C_VAR_ZC:
      ss << "__(C_VAR_ZC):"; break;	  
   case C_DISC_INCOMPATIVEIS:
      ss << "__(C_DISC_INCOMPATIVEIS):"; break;	  
   case C_EVITA_SOBREPOSICAO_TURMA_DISC_ALUNO:
      ss << "__(C_EVITA_SOBREPOSICAO_TURMA_DISC_ALUNO):"; break;	  
   case C_ALUNO_HORARIO:
      ss << "__(C_ALUNO_HORARIO):"; break;	  	  
   case C_ATIVA_Y:
      ss << "__(C_ATIVA_Y):"; break;	  
   case C_ALUNO_VARIAS_UNIDADES_DIA:
      ss << "__(C_ALUNO_VARIAS_UNIDADES_DIA):"; break;	  
   case C_MIN_CREDS_ALUNO:
      ss << "__(C_MIN_CREDS_ALUNO):"; break;	  
   case C_MAX_CREDS_ALUNO:
      ss << "__(C_MAX_CREDS_ALUNO):"; break;
   case C_ALUNO_DISC_PRATICA_TEORICA:
      ss << "__(C_ALUNO_DISC_PRATICA_TEORICA):"; break;	  
   case C_DISC_PRATICA_TEORICA:
      ss << "__(C_DISC_PRATICA_TEORICA):"; break;	  
	  
   default:
      ss << "!";
   }
   //return "";
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

   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}

