package com.gapso.web.trieda.shared.util.view;

import java.io.Serializable;

import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

public class RelatorioProfessorFiltro implements Serializable{

	private static final long serialVersionUID = -7088111219059190546L;
	
	private TurnoDTO turno;
	private CursoDTO curso;
	private TitulacaoDTO titulacao;
	private AreaTitulacaoDTO areaTitulacao;
	private TipoContratoDTO tipoContrato;
	
	public TurnoDTO getTurno() {
		return turno;
	}
	public void setTurno(TurnoDTO turno) {
		this.turno = turno;
	}
	public CursoDTO getCurso() {
		return curso;
	}
	public void setCurso(CursoDTO curso) {
		this.curso = curso;
	}
	public TitulacaoDTO getTitulacao() {
		return titulacao;
	}
	public void setTitulacao(TitulacaoDTO titulacao) {
		this.titulacao = titulacao;
	}
	public AreaTitulacaoDTO getAreaTitulacao() {
		return areaTitulacao;
	}
	public void setAreaTitulacao(AreaTitulacaoDTO areaTitulacao) {
		this.areaTitulacao = areaTitulacao;
	}
	public TipoContratoDTO getTipoContrato() {
		return tipoContrato;
	}
	public void setTipoContrato(TipoContratoDTO tipoContrato) {
		this.tipoContrato = tipoContrato;
	}
	
}
