package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaAlunoGrid extends GradeHorariaVisao{
	private AlunoDTO alunoDTO;
	private CampusDTO campusDTO;

	@SuppressWarnings("unchecked")
	public void requestAtendimentos(){
		if(getTurnoDTO() == null || getAlunoDTO() == null) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");
		
		AsyncCallback<QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>> c = 
			(AsyncCallback<QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>>) 
			getCallback((new QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>()).getClass());

		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoAluno(getAlunoDTO(), getTurnoDTO(), getCampusDTO(), c);
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaString(),
			atendimentoDTO.getContentToolTipVisaoAluno(ReportType.WEB),
			atendimentoDTO.getContentVisaoAluno(ReportType.WEB)
		);
	}

	public AlunoDTO getAlunoDTO(){
		return this.alunoDTO;
	}

	public void setAlunoDTO(AlunoDTO alunoDTO){
		this.alunoDTO = alunoDTO;
	}
	
	public void setCampusDTO(CampusDTO campusDTO){
		this.campusDTO = campusDTO;
	}
	
	public CampusDTO getCampusDTO(){
		return campusDTO;
	}

}
