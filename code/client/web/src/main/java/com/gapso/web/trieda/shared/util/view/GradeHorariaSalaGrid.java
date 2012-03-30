package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GradeHorariaSalaGrid extends GradeHorariaVisao{
	private SalaDTO salaDTO;
	
	@SuppressWarnings("unchecked")
	public void requestAtendimentos() {
		if(getSalaDTO() == null || getTurnoDTO() == null) return;

		this.grid.mask("Carregando os dados, aguarde alguns instantes", "loading");

		AsyncCallback<QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>> c = 
			(AsyncCallback<QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>>) 
			getCallback((new QuintetoDTO<Integer, Integer, Integer, List<AtendimentoRelatorioDTO>, List<String>>()).getClass());
		
		Services.atendimentos().getAtendimentosParaGradeHorariaVisaoSala(getSalaDTO(), getTurnoDTO(), c);
	}
	
	protected TrioDTO<String, String, String> getHTMLInfo(AtendimentoRelatorioDTO atendimentoDTO){
		return TrioDTO.create(
			atendimentoDTO.getDisciplinaString(),
			atendimentoDTO.getContentToolTipVisaoSala(ReportType.WEB),
			atendimentoDTO.getContentVisaoSala(ReportType.WEB)
		);
	}

	public SalaDTO getSalaDTO(){
		return this.salaDTO;
	}

	public void setSalaDTO(SalaDTO salaDTO){
		this.salaDTO = salaDTO;
	}

}
