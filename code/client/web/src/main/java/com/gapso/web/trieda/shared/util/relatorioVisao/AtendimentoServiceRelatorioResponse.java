package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.io.Serializable;
import java.util.List;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;

public class AtendimentoServiceRelatorioResponse implements Serializable{
	private static final long serialVersionUID = -3735259680645261872L;
	private List<AtendimentoRelatorioDTO> atendimentosDTO;
	private ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas;
	private List<String> labelsDasLinhasDaGradeHoraria;
	private List<String> horariosDeInicioDeAula;
	private List<String> horariosDeFimDeAula;
	private List<Integer> qtdColunasPorDiaSemana;
	private List<ParDTO<String, Integer>> horariosDisponiveis;

	public AtendimentoServiceRelatorioResponse(){}
	
	@SuppressWarnings("unchecked")
	public <A, B, C, D, E> AtendimentoServiceRelatorioResponse(QuintetoDTO<A, B, C, D, E> result){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getPrimeiro();
		this.mdcTemposAulaNumSemanasLetivas = (ParDTO<Integer, Boolean>) result.getSegundo();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getTerceiro();
		this.horariosDeInicioDeAula = (List<String>) result.getQuarto();
		this.horariosDeFimDeAula = (List<String>) result.getQuinto();
		this.qtdColunasPorDiaSemana = null;
	}
	
	@SuppressWarnings("unchecked")
	public <A, B, C, D, E> AtendimentoServiceRelatorioResponse(QuintetoDTO<A, B, C, D, E> result,
			List<ParDTO<String, Integer>> horariosDisponiveis){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getPrimeiro();
		this.mdcTemposAulaNumSemanasLetivas = (ParDTO<Integer, Boolean>) result.getSegundo();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getTerceiro();
		this.horariosDeInicioDeAula = (List<String>) result.getQuarto();
		this.horariosDeFimDeAula = (List<String>) result.getQuinto();
		this.qtdColunasPorDiaSemana = null;
		this.horariosDisponiveis = horariosDisponiveis;
	}
	
	@SuppressWarnings("unchecked")
	public <A, B, C, D, E , F> AtendimentoServiceRelatorioResponse(SextetoDTO<A, B, C, D, E, F> result){
		this.atendimentosDTO = (List<AtendimentoRelatorioDTO>) result.getPrimeiro();
		this.mdcTemposAulaNumSemanasLetivas = (ParDTO<Integer, Boolean>) result.getSegundo();
		this.labelsDasLinhasDaGradeHoraria = (List<String>) result.getTerceiro();
		this.horariosDeInicioDeAula = (List<String>) result.getQuarto();
		this.horariosDeFimDeAula = (List<String>) result.getQuinto();
		this.qtdColunasPorDiaSemana = (List<Integer>) result.getSexto();
	}

	@SuppressWarnings("unchecked")
	public static <A, B, C, D, E , F> AtendimentoServiceRelatorioResponse create(Object t){
		if(t instanceof QuintetoDTO) return new AtendimentoServiceRelatorioResponse((QuintetoDTO<A, B, C, D, E>) t);
		return new AtendimentoServiceRelatorioResponse((SextetoDTO<A, B, C, D, E, F>) t);
	}
	
	@SuppressWarnings("unchecked")
	public static <A, B, C, D, E , F> AtendimentoServiceRelatorioResponse create(Object t, List<ParDTO<String, Integer>> horarioDisponiveis){
		if(t instanceof QuintetoDTO) return new AtendimentoServiceRelatorioResponse((QuintetoDTO<A, B, C, D, E>) t, horarioDisponiveis);
		return new AtendimentoServiceRelatorioResponse((SextetoDTO<A, B, C, D, E, F>) t);
	}

	public List<AtendimentoRelatorioDTO> getAtendimentosDTO() {
		return atendimentosDTO;
	}

	public ParDTO<Integer, Boolean> getMdcTemposAulaNumSemanasLetivas() {
		return mdcTemposAulaNumSemanasLetivas;
	}

	public List<String> getLabelsDasLinhasDaGradeHoraria() {
		return labelsDasLinhasDaGradeHoraria;
	}
	
	public List<String> getHorariosDeInicioDeAula() {
		return horariosDeInicioDeAula;
	}
	
	public List<String> getHorariosDeFimDeAula() {
		return horariosDeFimDeAula;
	}

	public List<Integer> getQtdColunasPorDiaSemana() {
		return qtdColunasPorDiaSemana;
	}
	
	public List<ParDTO<String, Integer>> getHorariosDisponiveis()
	{
		return horariosDisponiveis;
	}
}
