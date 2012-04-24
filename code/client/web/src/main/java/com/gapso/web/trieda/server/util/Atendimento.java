package com.gapso.web.trieda.server.util;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Turno;

public class Atendimento {
	
	private AtendimentoTatico tatico;
	private AtendimentoOperacional operacional;
	
	static public enum TipoCredito {TEORICO,PRATICO};
	
	public Atendimento(AtendimentoTatico atendimento) {
		tatico = atendimento;
		operacional = null;
	}
	
	public Atendimento(AtendimentoOperacional atendimento) {
		operacional = atendimento;
		tatico = null;
	}

	public boolean isTatico() {
		return tatico != null;
	}
	
	public boolean isOperacional() {
		return operacional != null;
	}
	
	public Campus getCampus() {
		return (isTatico()) ? tatico.getOferta().getCampus() : operacional.getOferta().getCampus();
	}

	public Disciplina getDisciplina() {
		return (isTatico()) ? tatico.getDisciplina() : operacional.getDisciplina();
	}
	
	public Disciplina getDisciplinaSubstituta() {
		return (isTatico()) ? tatico.getDisciplinaSubstituta() : operacional.getDisciplinaSubstituta();
	}

	public Professor getProfessor() {
		return (isTatico()) ? null : operacional.getProfessor();
	}
	
	public ProfessorVirtual getProfessorVirtual() {
		return (isTatico()) ? null : operacional.getProfessorVirtual();
	}
	
	public int getQuantidadeAlunos() {
		return (isTatico()) ? tatico.getQuantidadeAlunos() : operacional.getQuantidadeAlunos();
	}
	
	public TipoCredito getTipoCredito() {
		TipoCredito tipo = null;
		if (isTatico()) {
			tipo = (tatico.getCreditosTeorico() > 0) ? TipoCredito.TEORICO : TipoCredito.PRATICO;
		} else {
			tipo = (operacional.getCreditoTeorico()) ? TipoCredito.TEORICO : TipoCredito.PRATICO;
		}
		return tipo;
	}
	
	public int getTotalCreditos() {
		return (isTatico()) ? tatico.getTotalCreditos() : 1;
	}

	public String getTurma() {
		return (isTatico()) ? tatico.getTurma() : operacional.getTurma();
	}
	
	public Turno getTurno() {
		return (isTatico()) ? tatico.getOferta().getTurno() : operacional.getOferta().getTurno();
	}
	
}
