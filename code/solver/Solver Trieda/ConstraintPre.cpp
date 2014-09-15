#include "ConstraintPre.h"
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

ConstraintPre::ConstraintPre()
{
   reset();
}

ConstraintPre::ConstraintPre( const ConstraintPre & cons )
{
   *this = cons;
}

ConstraintPre::~ConstraintPre()
{
   reset();
}

ConstraintPre& ConstraintPre::operator = ( const ConstraintPre & cons )
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
   this->o = cons.getOferta();
   this->sl = cons.getSemanaLetiva();
   this->parSl = cons.getParCalendarios();
   this->parCursos = cons.getParCursos();
   this->cjtSalaCompart = cons.getSubCjtSalaCompart();
   this->parOfts = cons.getParOfertas();
   this->aluno = cons.getAluno();
   this->dia = cons.getDia();
   this->turma1 = cons.getTurma1();
   this->turma2 = cons.getTurma2();
   this->disc1 = cons.getDisciplina1();
   this->disc2 = cons.getDisciplina2();
   this->parAlunos = cons.getParAlunos();
   this->turno = cons.getTurno();
   this->turnoIES = cons.getTurnoIES();
   this->parTurnosIES = cons.getParTurnosIES();
   this->periodo = cons.getPeriodo();

   return *this;
}

bool ConstraintPre::operator < ( const ConstraintPre & cons ) const
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

   if (E_MENOR(this->getOferta(),cons.getOferta())) return true;
   if (E_MENOR(cons.getOferta(), this->getOferta())) return false;

   if (this->getSemanaLetiva() < cons.getSemanaLetiva()) return true;
   if (this->getSemanaLetiva() > cons.getSemanaLetiva()) return false;

   if ( E_MENOR_PAR( this->getParCalendarios(), cons.getParCalendarios() ) ) return true;
   if ( E_MENOR_PAR( cons.getParCalendarios(), this->getParCalendarios() ) ) return false;

   if (E_MENOR(this->getSubCjtSalaCompart(),cons.getSubCjtSalaCompart())) return true;
   if (E_MENOR(cons.getSubCjtSalaCompart(), this->getSubCjtSalaCompart())) return false;

   if ( E_MENOR_PAR( this->getParCursos(), cons.getParCursos() ) ) return true;
   if ( E_MENOR_PAR( cons.getParCursos(), this->getParCursos() ) ) return false;

   if ( E_MENOR_PAR( this->getParOfertas(), cons.getParOfertas() ) ) return true;
   if ( E_MENOR_PAR( cons.getParOfertas(), this->getParOfertas() ) ) return false;

   if ( E_MENOR( this->getAluno(), cons.getAluno() ) ) return true;
   if ( E_MENOR( cons.getAluno(), this->getAluno() ) ) return false;

   if (this->getDia() < cons.getDia()) return true;
   if (this->getDia() > cons.getDia()) return false;
   
   if ( this->getTurma1() < cons.getTurma1() ) return true;
   if ( this->getTurma1() > cons.getTurma1() ) return false;
   
   if ( this->getTurma2() < cons.getTurma2() ) return true;
   if ( this->getTurma2() > cons.getTurma2() ) return false;

   if ( E_MENOR( this->getDisciplina1(),cons.getDisciplina1() ) ) return true;
   if ( E_MENOR( cons.getDisciplina1(), this->getDisciplina1() ) ) return false;

   if ( E_MENOR( this->getDisciplina2(),cons.getDisciplina2() ) ) return true;
   if ( E_MENOR( cons.getDisciplina2(), this->getDisciplina2() ) ) return false;
   
   if ( E_MENOR_PAR( this->getParAlunos(), cons.getParAlunos() ) ) return true;
   if ( E_MENOR_PAR( cons.getParAlunos(), this->getParAlunos() ) ) return false;
   
   if ( this->getTurno() < cons.getTurno() ) return true;
   if ( this->getTurno() > cons.getTurno() ) return false;
   
   if ( E_MENOR( this->getTurnoIES(),cons.getTurnoIES() ) ) return true;
   if ( E_MENOR( cons.getTurnoIES(), this->getTurnoIES() ) ) return false;
   
   if ( E_MENOR_PAR( this->getParTurnosIES(),cons.getParTurnosIES() ) ) return true;
   if ( E_MENOR_PAR( cons.getParTurnosIES(), this->getParTurnosIES() ) ) return false;
   
   if ( this->getPeriodo() < cons.getPeriodo() ) return true;
   if ( cons.getPeriodo() < this->getPeriodo() ) return false;

   return false;

}

bool ConstraintPre::operator== (const ConstraintPre& cons) const
{
   return (!(*this < cons) && !(cons < *this));
}

void ConstraintPre::reset()
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
   o = NULL;
   sl = NULL;
   parSl.first = NULL;
   parSl.second = NULL;
   parCursos.first = NULL;
   parCursos.second = NULL;
   parOfts.first = NULL;
   parOfts.second = NULL;
   aluno = NULL;
   dia = -1;
   turma1 = -1;
   turma2 = -1;
   disc1 = NULL;
   disc2 = NULL;
   parAlunos.first = NULL;
   parAlunos.second = NULL;
   turno = -1;
   turnoIES = NULL;
   parTurnosIES.first = NULL;
   parTurnosIES.second = NULL;
   periodo = -1;
}

std::string ConstraintPre::toString()
{
   std::stringstream ss;
   ss << "CType[" << (int) type << "]";

   switch( type )
   {
   case C_PRE_CARGA_HORARIA:
      ss << "__(C_PRE_CARGA_HORARIA):"; break;
   case C_PRE_MAX_CRED_SALA:
      ss << "__(C_PRE_MAX_CRED_SALA):"; break;
   case C_PRE_VAR_O:
      ss << "__(C_PRE_VAR_O):"; break;
   case C_EVITA_MUDANCA_DE_SALA:
      ss << "__(C_PRE_EVITA_MUDANCA_DE_SALA):"; break;	  
   case C_PRE_CAP_ALOC_DEM_DISC:
      ss << "__(C_PRE_CAP_ALOC_DEM_DISC):"; break;
   case C_ALUNO_OFT_DISC:
      ss << "__(C_PRE_ALUNO_OFT_DISC):"; break;
   case C_CAP_SALA:
      ss << "__(C_PRE_CAP_SALA):"; break;
   case C_PRE_ALUNOS_CURSOS_INCOMP:
      ss << "__(C_PRE_ALUNOS_CURSOS_INCOMP):"; break;
   case C_PRE_PROIBE_COMPARTILHAMENTO:
      ss << "__(C_PRE_PROIBE_COMPARTILHAMENTO):"; break;
   case C_PRE_ATIVA_Z:
      ss << "__(C_PRE_ATIVA_Z):"; break;
   case C_PRE_EVITA_TURMA_DISC_CAMP_D:
      ss << "__(C_PRE_EVITA_TURMA_DISC_CAMP_D):"; break;
   case C_PRE_LIMITA_ABERTURA_TURMAS:
      ss << "__(C_PRE_LIMITA_ABERTURA_TURMAS):"; break;
   case C_PRE_ABRE_TURMAS_EM_SEQUENCIA:
      ss << "__(C_PRE_ABRE_TURMAS_EM_SEQUENCIA):"; break;
   case C_PRE_TURMA_MESMA_DISC_OFT_SALA_DIF:
      ss << "__(C_PRE_TURMA_MESMA_DISC_OFT_SALA_DIF):"; break;
   case C_PRE_TURMA_MESMA_DISC_SALA_DIF:
      ss << "__(C_PRE_TURMA_MESMA_DISC_SALA_DIF):"; break;
   case C_PRE_LIM_SUP_CREDS_SALA:
      ss << "__(C_PRE_LIM_SUP_CREDS_SALA):"; break;	  
   case C_PRE_ATIVA_C:
      ss << "__(C_PRE_ATIVA_C):"; break;	  
   case C_PRE_FIXA_NAO_COMPARTILHAMENTO:
      ss << "__(C_PRE_FIXA_NAO_COMPARTILHAMENTO):"; break;	 
   case C_ATENDIMENTO_ALUNO:
      ss << "__(C_PRE_ATENDIMENTO_ALUNO):"; break;	 
   case C_ALUNO_UNICA_TURMA_DISC:
      ss << "__(C_PRE_ALUNO_UNICA_TURMA_DISC):"; break;	 
   case C_ALUNO_DISC_PRATICA_TEORICA:
      ss << "__(C_PRE_ALUNO_DISC_PRATICA_TEORICA):"; break;	    
   case C_ALUNO_PRIORIDADES_DEMANDA:
      ss << "__(C_PRE_ALUNO_PRIORIDADES_DEMANDA):"; break;	  
   case C_PRE_EVITA_SOBREPOS_SALA_DIA_ALUNO:
      ss << "__(C_PRE_EVITA_SOBREPOS_SALA_DIA_ALUNO):"; break;	  
   case C_PRE_ATIVA_VAR_COMPART_TURMA:
      ss << "__(C_PRE_ATIVA_VAR_COMPART_TURMA):"; break;	  
   case C_PRE_MAX_CREDS_ALUNO_DIA:
      ss << "__(C_PRE_MAX_CREDS_ALUNO_DIA):"; break; 	
   case C_PRE_DISTR_ALUNOS:
	   ss <<"_(C_PRE_DISTR_ALUNOS):"; break;
   case C_PRE_SOMA_CRED_SALA:
      ss <<"_(C_PRE_SOMA_CRED_SALA):"; break;
   case C_PRE_MAX_CREDS_ALUNO_SEMANA:
      ss <<"_(C_PRE_MAX_CREDS_ALUNO_SEMANA):"; break;
   case C_PRE_ALUNO_SALA:
      ss <<"_(C_PRE_ALUNO_SALA):"; break;
   case C_PRE_FORMANDOS1:
      ss <<"_(C_PRE_FORMANDOS1):"; break;
   case C_PRE_FORMANDOS2:
      ss <<"_(C_PRE_FORMANDOS2):"; break;
   case C_PRE_TURMA_CALENDARIOS_INTERSECAO:
      ss <<"_(C_PRE_TURMA_CALENDARIOS_INTERSECAO):"; break;
   case C_PRE_SET_V_1:
      ss <<"_(C_PRE_SET_V_1):"; break;
   case C_PRE_SET_V_2:
      ss <<"_(C_PRE_SET_V_2):"; break;	  	  
   case C_PRE_ALUNO_EM_TURMA_ABERTA:
      ss <<"_(C_PRE_ALUNO_EM_TURMA_ABERTA):"; break;  
   case C_PRE_NRO_ALUNOS_POR_TURMA:
      ss <<"_(C_PRE_NRO_ALUNOS_POR_TURMA):"; break;  
   case C_PRE_DISC_PRATICA_TEORICA_OFERTA:
      ss <<"_(C_PRE_DISC_PRATICA_TEORICA_OFERTA):"; break;  
   case C_PRE_ALOC_DEM_DISC_OFERTA:
      ss <<"_(C_PRE_ALOC_DEM_DISC_OFERTA):"; break;  
   case C_PRE_ALOC_TURMA_SAB:
      ss <<"_(C_PRE_ALOC_TURMA_SAB):"; break;  
   case C_PRE_MAX_CRED_SAB:
      ss <<"_(C_PRE_MAX_CRED_SAB):"; break;  
   case C_PRE_FORCA_MIN_ATEND:
      ss <<"_(C_PRE_FORCA_MIN_ATEND):"; break;  
   case C_ALUNO_DISC_PRATICA_TEORICA_1x1:
      ss <<"_(C_PRE_ALUNO_DISC_PRATICA_TEORICA_1x1):"; break;  
   case C_ALUNO_DISC_PRATICA_TEORICA_1xN:
      ss <<"_(C_PRE_ALUNO_DISC_PRATICA_TEORICA_1xN):"; break;  
   case C_ALUNOS_MESMA_TURMA_PRAT:
      ss <<"_(C_PRE_ALUNOS_MESMA_TURMA_PRAT):"; break;	  	  
   case C_PRE_ALOC_MIN_ALUNO:
      ss <<"_(C_PRE_ALOC_MIN_ALUNO):"; break;	  	  	  
   case C_PRE_FOLGA_OCUPACAO_SALA:
      ss <<"_(C_PRE_FOLGA_OCUPACAO_SALA):"; break;	  	  	  
   case C_PRE_MAX_CRED_SALA_TURNO:
      ss <<"_(C_PRE_MAX_CRED_SALA_TURNO):"; break;	  	  	  	  
   case C_PRE_SET_TURNO_POR_TURMA:
      ss <<"_(C_PRE_SET_TURNO_POR_TURMA):"; break;	  	  	  	  	  
   case C_PRE_LIMITA_TURNO_POR_TURMA:
      ss <<"_(C_PRE_LIMITA_TURNO_POR_TURMA):"; break;	  	  	  	  	  
   case C_PRE_TURNO_VIAVEL_POR_TURMA:
      ss <<"_(C_PRE_TURNO_VIAVEL_POR_TURMA):"; break;
   case C_PRE_TEMPO_MAX_ALUNO_SAB:
      ss <<"_(C_PRE_TEMPO_MAX_ALUNO_SAB):"; break;
   case C_PRE_TEMPO_MAX_ALUNO_TURNO:
      ss <<"_(C_PRE_TEMPO_MAX_ALUNO_TURNO):"; break;
   case C_PRE_ALUNOS_TURNO:
      ss <<"_(C_PRE_ALUNOS_TURNO):"; break;
   case C_PRE_USA_DISC:
      ss <<"_(C_PRE_USA_DISC):"; break;
   case C_PRE_UNID_PERIODO:
      ss <<"_(C_PRE_UNID_PERIODO):"; break;
   case C_PRE_USA_UNID_PERIODO:
      ss <<"_(C_PRE_USA_UNID_PERIODO):"; break;
  case C_PRE_USA_SALA_PERIODO:
      ss <<"_(C_PRE_USA_SALA_PERIODO):"; break;
  case C_PRE_SALA_PERIODO:
      ss <<"_(C_PRE_SALA_PERIODO):"; break;
  case C_PRE_DISTRIB_ALUNOS:
      ss <<"_(C_PRE_DISTRIB_ALUNOS):"; break;
  case C_PRE_DISPONIB_PROF:
      ss <<"_(C_PRE_DISPONIB_PROF):"; break;
	  	  
	  

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

   if ( parSl.first != NULL && parSl.second != NULL )
   {
	   ss << "_(Sl1" << parSl.first->getId(); 	
	   ss << ",Sl2" << parSl.second->getId() << ")";
   }

   if ( aluno )
   {
	   ss << "_Aluno" << aluno->getAlunoId();
   }
   
   if ( dia != -1 )
   {
	   ss << "_Dia" << dia;
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

   if ( parAlunos.first != NULL && parAlunos.second != NULL )
   {
	   ss << "_(a" << parAlunos.first->getAlunoId(); 	
	   ss << ",a" << parAlunos.second->getAlunoId() << ")";
   }
   
   if ( turno >= 1 )
   {
      ss << "_Turno" << turno;
   }
   
   if ( turnoIES != NULL )
   {
      ss << "_TurnoIES" << turnoIES->getId();
   }

   if ( parTurnosIES.first != NULL && parTurnosIES.second != NULL )
   {
	   ss << "_(TurnoIES" << parTurnosIES.first->getId(); 	
	   ss << ",TurnoIES" << parTurnosIES.second->getId() << ")";
   }

   if ( periodo > 0 )
   {
      ss << "_Per" << periodo;
   }

   if ( cp != NULL )
   {
      ss << "_Cp" << cp->getId();
   }

   ss << "_}";
   std::string consName = "";
   ss >> consName;

   return consName;
}
