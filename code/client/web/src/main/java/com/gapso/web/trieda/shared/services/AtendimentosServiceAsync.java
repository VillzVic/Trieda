package com.gapso.web.trieda.shared.services;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
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

	void getProfessoresVirtuais( CampusDTO campusDTO,
		AsyncCallback< ListLoadResult< ProfessorVirtualDTO > > callback );
}