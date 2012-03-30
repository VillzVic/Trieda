package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaProfessorGrid extends GradeHorariaVisao{
	private ProfessorDTO professorDTO;
	private ProfessorVirtualDTO professorVirtualDTO;

	private boolean isVisaoProfessor;

	public GradeHorariaProfessorGrid(boolean isVisaoProfessor){
		super();

		this.isVisaoProfessor = isVisaoProfessor; 
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@SuppressWarnings("unchecked")
	public void requestAtendimentos(){
		if(getTurnoDTO() == null || (getProfessorDTO() == null && getProfessorVirtualDTO() == null)) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");
		
		AsyncCallback<QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>> c = 
			(AsyncCallback<QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>>) 
			getCallback((new QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>()).getClass());

		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoProfessor(getProfessorDTO(), getProfessorVirtualDTO(), getTurnoDTO(), this.isVisaoProfessor(), c);
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaNome(),
			((AtendimentoOperacionalDTO) atendimentoDTO).getContentToolTipVisaoProfessor(ReportType.WEB),
			((AtendimentoOperacionalDTO)atendimentoDTO).getContentVisaoProfessor(ReportType.WEB)
		);
	}

	public ProfessorDTO getProfessorDTO(){
		return this.professorDTO;
	}

	public void setProfessorDTO(ProfessorDTO professorDTO){
		this.professorDTO = professorDTO;
	}

	public ProfessorVirtualDTO getProfessorVirtualDTO(){
		return this.professorVirtualDTO;
	}

	public void setProfessorVirtualDTO(ProfessorVirtualDTO professorVirtualDTO){
		this.professorVirtualDTO = professorVirtualDTO;
	}

}
