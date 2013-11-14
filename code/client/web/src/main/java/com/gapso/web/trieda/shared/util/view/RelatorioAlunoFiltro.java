package com.gapso.web.trieda.shared.util.view;

import java.io.Serializable;

import com.gapso.web.trieda.shared.dtos.CursoDTO;

public class RelatorioAlunoFiltro implements Serializable{

	private static final long serialVersionUID = -7927603110212921297L;
	private CursoDTO curso;
	private Boolean formando;
	private Integer periodo;
	
	public void setCurso(CursoDTO curso){
		this.curso = curso;
	}
	
	public CursoDTO getCurso(){
		return this.curso;
	}

	public void setFormando(Boolean formando){
		this.formando = formando;
	}
	
	public Boolean getFormando(){
		return this.formando;
	}
	
	public void setPeriodo(Integer periodo){
		this.periodo = periodo;
	}
	
	public Integer getPeriodo(){
		return this.periodo;
	}
}
