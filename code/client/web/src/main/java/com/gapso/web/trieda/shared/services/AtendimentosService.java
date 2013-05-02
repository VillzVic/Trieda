package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "atendimentos" )
public interface AtendimentosService extends RemoteService {

	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoCurso(RelatorioVisaoCursoFiltro filtro);

	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoSala(RelatorioVisaoSalaFiltro filtro);
	
	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoProfessor(RelatorioVisaoProfessorFiltro filtro, boolean isVisaoProfessor);
	
	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoAluno(RelatorioVisaoAlunoFiltro filtro);
	
	PagingLoadResult< AtendimentoTaticoDTO > getList();

//	ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > getBusca(
//		CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO );

	ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CampusDTO campusDTO );

	List<PercentMestresDoutoresDTO> getPercentMestresDoutoresList( CampusDTO campusDTO );

}
