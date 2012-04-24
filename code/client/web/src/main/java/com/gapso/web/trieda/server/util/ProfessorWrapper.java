package com.gapso.web.trieda.server.util;

import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;

public class ProfessorWrapper {

	private Professor professor;
	private ProfessorVirtual professorVirtual;
	
	public ProfessorWrapper(Professor professor){
		this.professor = professor;
		this.professorVirtual = null;
	}
	
	public ProfessorWrapper(ProfessorVirtual professorVirtual){
		this.professor = null;
		this.professorVirtual = professorVirtual;
	}
	
	public boolean isVirtual(){
		return professor == null;
	}
	
	public Professor getProfessor() {
		return professor;
	}

	public ProfessorVirtual getProfessorVirtual() {
		return professorVirtual;
	}
	
	@Override
	public int hashCode(){
		if(this.professorVirtual != null) return this.professorVirtual.hashCode();
		if(this.professor != null) return this.professor.hashCode();
		
		return super.hashCode();
	}
	
	@Override
	public boolean equals(Object obj){
		if(obj == null || !(obj instanceof ProfessorWrapper)) return false;
		
		ProfessorWrapper professor = (ProfessorWrapper) obj;
		
		return (isVirtual()) ? this.professorVirtual == professor.getProfessorVirtual() : this.professor == professor.getProfessor();
	}
}
