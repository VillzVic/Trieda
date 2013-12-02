package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaCreditoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaTurmaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.TipoProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AtendimentosServiceAsync {
	
	void getAtendimentosParaGradeHorariaVisaoCurso(RelatorioVisaoCursoFiltro filtro, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getAtendimentosParaGradeHorariaVisaoSala(RelatorioVisaoSalaFiltro filtro, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getAtendimentosParaGradeHorariaVisaoProfessor(RelatorioVisaoProfessorFiltro filtro, boolean isVisaoProfessor, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getAtendimentosParaGradeHorariaVisaoAluno(RelatorioVisaoAlunoFiltro filtro, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getList( AsyncCallback< PagingLoadResult< AtendimentoTaticoDTO > > callback );

//	void getBusca( CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO,
//		AsyncCallback< ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > > callback );

	void getProfessoresVirtuais( AsyncCallback< ListLoadResult< ProfessorVirtualDTO > > callback );
	
	void getProfessoresVirtuais( CenarioDTO cenarioDTO, TitulacaoDTO titulacaoDTO,
			PagingLoadConfig config, AsyncCallback< PagingLoadResult< ProfessorVirtualDTO > > callback );

	void getPercentMestresDoutoresList( CenarioDTO cenarioDTO, CampusDTO campusDTO,
			AsyncCallback<List<PercentMestresDoutoresDTO>> callback );

	void getAtendimentosFaixaCredito( CenarioDTO cenarioDTO, CampusDTO campusDTO,
			AsyncCallback<List<AtendimentoFaixaCreditoDTO>> callback );

	void getAtendimentosFaixaDisciplina( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<AtendimentoFaixaCreditoDTO>> callback );

	void getProfessoresJanelasGrade( CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getProfessoresDisciplinasLecionadas( CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getAmbientesFaixaOcupacaoHorarios( CenarioDTO cenarioDTO, CampusDTO campusDTO,	AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getAmbientesFaixaUtilizacaoCapacidade( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getAtendimentosFaixaTurma( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<AtendimentoFaixaTurmaDTO>> callback );
}