#ifndef INPUT_H
#define INPUT_H

#include "OFBase.h"
#include "Calendario.h"
#include "Disciplina.h"
#include "DivisaoCreditos.h"
#include "ProblemData.h"
#include "Professor.h"
#include "BlocoCurricular.h"
#include "Turma.h"
#include "Sala.h"
#include "GGroup.h"
#include "Demanda.h"

#define ITERA_GGROUP(it,ggroup,type) for ( GGroup<type *>::iterator it = (ggroup).begin(); it != (ggroup).end(); ++it )

#endif