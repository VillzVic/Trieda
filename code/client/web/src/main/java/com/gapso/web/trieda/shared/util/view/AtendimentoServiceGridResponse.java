package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;

public class AtendimentoServiceGridResponse {
	private List<AtendimentoRelatorioDTO> atendimentosDTO;
	private Integer mdcTemposAula;
	private List<String> labelsDasLinhasDaGradeHoraria;
	private List<Integer> qtdColunasPorDiaSemana;

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public AtendimentoServiceGridResponse(QuintetoDTO result){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getQuarto();
		this.mdcTemposAula = (Integer) result.getPrimeiro();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getQuinto();
		this.qtdColunasPorDiaSemana = null;
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public AtendimentoServiceGridResponse(SextetoDTO result){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getQuarto();
		this.mdcTemposAula = (Integer) result.getPrimeiro();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getSexto();
		this.qtdColunasPorDiaSemana = (List<Integer>) result.getQuinto();
	}

	@SuppressWarnings({ "rawtypes" })
	public static AtendimentoServiceGridResponse create(Object t){
		if(t instanceof QuintetoDTO) return new AtendimentoServiceGridResponse((QuintetoDTO) t);
		return new AtendimentoServiceGridResponse((SextetoDTO) t);
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
