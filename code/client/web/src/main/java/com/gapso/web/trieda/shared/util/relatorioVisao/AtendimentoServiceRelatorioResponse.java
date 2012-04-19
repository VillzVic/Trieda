package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.io.Serializable;
import java.util.List;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;

public class AtendimentoServiceRelatorioResponse implements Serializable{
	private static final long serialVersionUID = -3735259680645261872L;
	private List<AtendimentoRelatorioDTO> atendimentosDTO;
	private Integer mdcTemposAula;
	private List<String> labelsDasLinhasDaGradeHoraria;
	private List<Integer> qtdColunasPorDiaSemana;

	public AtendimentoServiceRelatorioResponse(){}
	
	@SuppressWarnings("unchecked")
	public <A, B, C, D, E> AtendimentoServiceRelatorioResponse(QuintetoDTO<A, B, C, D, E> result){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getQuarto();
		this.mdcTemposAula = (Integer) result.getPrimeiro();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getQuinto();
		this.qtdColunasPorDiaSemana = null;
	}
	
	@SuppressWarnings("unchecked")
	public <A, B, C, D, E , F> AtendimentoServiceRelatorioResponse(SextetoDTO<A, B, C, D, E , F> result){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getQuarto();
		this.mdcTemposAula = (Integer) result.getPrimeiro();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getSexto();
		this.qtdColunasPorDiaSemana = (List<Integer>) result.getQuinto();
	}

	@SuppressWarnings("unchecked")
	public static <A, B, C, D, E , F> AtendimentoServiceRelatorioResponse create(Object t){
		if(t instanceof QuintetoDTO) return new AtendimentoServiceRelatorioResponse((QuintetoDTO<A, B, C, D, E>) t);
		return new AtendimentoServiceRelatorioResponse((SextetoDTO<A, B, C, D, E , F>) t);
	}

	public List<AtendimentoRelatorioDTO> getAtendimentosDTO() {
		return atendimentosDTO;
	}

	public Integer getMdcTemposAula() {
		return mdcTemposAula;
	}

	public List<String> getLabelsDasLinhasDaGradeHoraria() {
		return labelsDasLinhasDaGradeHoraria;
	}

	public List<Integer> getQtdColunasPorDiaSemana() {
		return qtdColunasPorDiaSemana;
	}
	
}
