#include "VariableTatico.h"
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

VariableTatico::VariableTatico()
{
   reset();
}

VariableTatico::VariableTatico( const VariableTatico & var )
{
   *this = var;
}

void VariableTatico::reset()
{
   value = -1;
   i = -1;
   j = -1;
   t = -1;
   k = -1;
   cp = NULL;
   u = NULL;
   s = NULL;
   tps = NULL;
   c = NULL;
   c_incompat = NULL;
   b = NULL;
   d = NULL;
   o = NULL;   
   parCursos.first = NULL;
   parCursos.second = NULL;
   parOft.first = NULL;
   parOft.second = NULL;
   horarioAulaI = NULL;
   horarioAulaF = NULL;
   aluno = NULL;
   turma1 = -1;
   turma2 = -1;
   disc1 = NULL;
   disc2 = NULL;
}

VariableTatico::~VariableTatico()
{
   reset();
}

VariableTatico & VariableTatico::operator = ( const VariableTatico & var )
{
   this->value = var.getValue();
   this->type = var.getType();
   this->cp = var.getCampus();
   this->u = var.getUnidade();
   this->s = var.getSala();
   this->tps = var.getSubCjtSala();
   this->i = var.getTurma();
   this->c = var.getCurso();
   this->c_incompat = var.getCursoIncompat();
   this->b = var.getBloco();
   this->d = var.getDisciplina();
   this->j = var.getSubBloco();
   this->t = var.getDia();
   this->o = var.getOferta();
   this->k = var.getK();
   this->parCursos = var.getParCursos();
   this->parOft = var.getParOfertas();
   this->horarioAulaI = var.getHorarioAulaInicial();
   this->horarioAulaF = var.getHorarioAulaFinal();
   this->aluno = var.getAluno();
   this->turma1 = var.getTurma1();
   this->turma2 = var.getTurma2();
   this->disc1 = var.getDisciplina1();
   this->disc2 = var.getDisciplina2();

   return *this;
}

bool VariableTatico::operator < ( const VariableTatico & var ) const
{
   if( (int) this->getType() < (int) var.getType() )
   {
      return true;
   }
   else if( (int) this->getType() > (int) var.getType() )
   {
      return false;
   }

   if ( E_MENOR( this->getCampus(), var.getCampus() ) ) return true;
   if ( E_MENOR( var.getCampus(), this->getCampus() ) ) return false;

   if ( E_MENOR( this->getUnidade(), var.getUnidade() ) ) return true;
   if ( E_MENOR( var.getUnidade(), this->getUnidade() ) ) return false;

   if ( E_MENOR( this->getSala(), var.getSala() ) ) return true;
   if ( E_MENOR( var.getSala(), this->getSala() ) ) return false;

   if ( E_MENOR( this->getSubCjtSala(), var.getSubCjtSala() ) ) return true;
   if ( E_MENOR( var.getSubCjtSala(), this->getSubCjtSala() ) ) return false;

   if ( this->getTurma() < var.getTurma() ) return true;
   if ( this->getTurma() > var.getTurma() ) return false;
   
   if ( E_MENOR( this->getCurso(), var.getCurso() ) ) return true;
   if ( E_MENOR( var.getCurso(), this->getCurso() ) ) return false;   

   if ( E_MENOR( this->getCursoIncompat(), var.getCursoIncompat() ) ) return true;
   if ( E_MENOR( var.getCursoIncompat(), this->getCursoIncompat() ) ) return false;

   if ( E_MENOR( this->getBloco(), var.getBloco() ) ) return true;
   if ( E_MENOR( var.getBloco(), this->getBloco() ) ) return false;

   if ( E_MENOR( this->getDisciplina(),var.getDisciplina() ) ) return true;
   if ( E_MENOR( var.getDisciplina(), this->getDisciplina() ) ) return false;

   if ( this->getSubBloco() < var.getSubBloco() ) return true;
   if ( this->getSubBloco() > var.getSubBloco() ) return false;

   if ( this->getDia() < var.getDia() ) return true;
   if ( this->getDia() > var.getDia() ) return false;

   if ( E_MENOR( this->getOferta(), var.getOferta() ) ) return true;
   if ( E_MENOR( var.getOferta(), this->getOferta() ) ) return false;

   if ( this->getK() < var.getK() ) return true;
   if ( this->getK() > var.getK() ) return false;

   if ( E_MENOR_PAR( this->getParCursos(), var.getParCursos() ) ) return true;
   if ( E_MENOR_PAR( var.getParCursos(), this->getParCursos() ) ) return false;

   if ( E_MENOR_PAR( this->getParOfertas(), var.getParOfertas() ) ) return true;
   if ( E_MENOR_PAR( var.getParOfertas(), this->getParOfertas() ) ) return false;
   
   if ( E_MENOR( this->getHorarioAulaInicial(), var.getHorarioAulaInicial() ) ) return true;
   if ( E_MENOR( var.getHorarioAulaInicial(), this->getHorarioAulaInicial() ) ) return false;

   if ( E_MENOR( this->getHorarioAulaFinal(), var.getHorarioAulaFinal() ) ) return true;
   if ( E_MENOR( var.getHorarioAulaFinal(), this->getHorarioAulaFinal() ) ) return false;
   
   if ( this->getAluno() < var.getAluno() ) return true;
   if ( this->getAluno() > var.getAluno() ) return false;
   
   if ( this->getTurma1() < var.getTurma1() ) return true;
   if ( this->getTurma1() > var.getTurma1() ) return false;
   
   if ( this->getTurma2() < var.getTurma2() ) return true;
   if ( this->getTurma2() > var.getTurma2() ) return false;

   if ( E_MENOR( this->getDisciplina1(),var.getDisciplina1() ) ) return true;
   if ( E_MENOR( var.getDisciplina1(), this->getDisciplina1() ) ) return false;

   if ( E_MENOR( this->getDisciplina2(),var.getDisciplina2() ) ) return true;
   if ( E_MENOR( var.getDisciplina2(), this->getDisciplina2() ) ) return false;

   return false;
}

bool VariableTatico::operator == ( const VariableTatico & var ) const
{
   return ( !( *this < var ) && !( var < *this ) );
}

std::string VariableTatico::toString()
{
   std::stringstream str( "" );
   std::string output;
   
   switch( type )
   {
	 case V_ERROR:
        str << "?"; break;
     case V_CREDITOS:
        str << "x"; break;     
     case V_ABERTURA:
        str << "z"; break;
     case V_DIAS_CONSECUTIVOS:
        str << "c"; break;
     case V_SLACK_DEMANDA:
        str << "fd"; break;
     case V_MAX_CRED_SEMANA:
        str << "H"; break;
     case V_MIN_CRED_SEMANA:
        str << "h"; break;
     case V_SLACK_DIST_CRED_DIA_SUPERIOR:
        str << "fcp"; break;
     case V_SLACK_DIST_CRED_DIA_INFERIOR:
        str << "fcm"; break;
	 case V_COMBINACAO_DIVISAO_CREDITO:
        str << "m"; break;
     case V_SLACK_COMBINACAO_DIVISAO_CREDITO_M:
        str << "fkm"; break;
     case V_SLACK_COMBINACAO_DIVISAO_CREDITO_P:
        str << "fkp"; break;
     case V_ABERTURA_COMPATIVEL:
		str <<"zc"; break;
     case V_ALUNO_UNID_DIA:
		str <<"y"; break;	
     case V_ALUNO_VARIAS_UNID_DIA:
		str <<"w"; break;		
  	 case V_SLACK_ALUNO_VARIAS_UNID_DIA:
		str <<"fu"; break;			
  	 case V_SLACK_SLACKDEMANDA_PT:
		str <<"ffd"; break;			
  	 case V_ALUNO_DIA:
		str <<"du"; break;		
	 case V_DESALOCA_ALUNO:
		str <<"fa"; break;	
	 case V_DESALOCA_ALUNO_DIA:
		str <<"fad"; break;	

    default:
        str << "!";
   }

   str << "_{";

   if ( b != NULL )
   {
      str << "_Bc" << b->getId();
   }

   if ( i >= 0 )
   {
      str << "_Turma" << i;
   }

   if ( d != NULL )
   {
      str << "_Disc" << d->getId();
   }
   
   if ( u != NULL )
   {
      str << "_Unid" << u->getId();
   }

   if ( s != NULL )
   {
      str << "_Sala" << s->getId();
   }

   if ( tps )
   {
      str << "_Tps" << tps->getId();
   }

   if ( t >= 0 )
   {
      str << "_Dia" << t;
   }

   if ( j >= 0 )
   {
      str << "_Sbc" << j;
   }

   if ( c )
   {
      str << "_Curso" << c->getId();
   }

   if ( c_incompat )
   {
      str << "_CursoIncomp" << c_incompat->getId();
   }

   if ( parCursos.first != NULL && parCursos.second != NULL )
   {
	   str << "_(c" << parCursos.first->getId() << ",c" << parCursos.second->getId()<<")";
   }

   if ( cp )
   {
      str << "_Cp" << cp->getId();
   }

   if ( o )
   {
      str << "_Of" << o->getId();
   }
   
   if ( parOft.first != NULL && parOft.second != NULL )
   {
	   str << "_(Of" << parOft.first->getId() << ",Of" << parOft.second->getId()<<")";
   }

   if ( k >= 0 )
   {
      str << "_DivCred" << k;
   }

   if ( horarioAulaI )
   {
      str << "_Hi" << horarioAulaI->getId();
   }

   if ( horarioAulaF )
   {
      str << "_Hf" << horarioAulaF->getId();
   }

   if ( aluno )
   {
	   str << "_Aluno" << aluno->getAlunoId();
   }
   
   if ( turma1 >= 0 )
   {
      str << "_Turma1:" << turma1;
   }

   if ( disc1 != NULL )
   {
      str << "_Disc1:" << disc1->getId();
   }
   
   if ( turma2 >= 0 )
   {
      str << "_Turma2:" << turma2;
   }

   if ( disc2 != NULL )
   {
      str << "_Disc2:" << disc2->getId();
   }

   str << "}";
   str >> output;

   return output;
}
