#include "MoveShiftValidator.h"

MoveShiftValidator::MoveShiftValidator(ProblemData * pD) : MoveValidator(pD)
{
}

MoveShiftValidator::~MoveShiftValidator()
{
}

bool MoveShiftValidator::isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos)
{
	std::cout << "\n\nWarnning: IMPLEMENTAR O METODO <bool MoveShiftValidator::isValid(Aula & aula, std::vector< std::pair< Professor*, Horario* > > blocoHorariosVagos)>. Retornando sempre false, por padrao.\n\n";

   /* PAREI AQUI !!! PARA CHECAR SE O HORARIO ESTA VAGO, TEM QUE RECEBER A SOLUCAO COMO ENTRADA NESSE METODO.
   ASSIM, CONSEGUE-SE CHECAR SE OS HORARIOS ESTAO APONTANDO PARA NULL. 
   
   FAZER ISSO NO MOVEVALIDATOR.
   */

	/*
	checks a serem realizados (NA ORDEM ABAIXO):

	canShiftSchedule(aula,blocoHorariosVagos);

	CHECAR SE OS HORARIOS PERTENCEM AO MESMO DIA

	!aula.eVirtual();

	!aula.eLab();
	*/

   return false;

   //return(
   //   (canSwapSchedule(aX,aY)) &&

   //   (aX.getTotalCreditos() == aY.getTotalCreditos()) &&

   //   (aX.getDiaSemana() == aY.getDiaSemana()) &&

   //   (aX.getSala() == aY.getSala()) &&

   //   (aX != aY) &&

   //   !(aX.getDisciplina()->eLab()) &&
   //   !(aY.getDisciplina()->eLab()) &&

   //   !(aX.eVirtual()) &&
   //   !(aY.eVirtual())   
   //   );
}