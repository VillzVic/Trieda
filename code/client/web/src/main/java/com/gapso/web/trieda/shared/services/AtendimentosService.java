package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaCreditoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaTurmaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DicaEliminacaoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.MotivoUsoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "atendimentos" )
public interface AtendimentosService extends RemoteService {

	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoCurso(RelatorioVisaoCursoFiltro filtro);

	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoSala(CenarioDTO cenarioDTO, RelatorioVisaoSalaFiltro filtro) throws TriedaException;
	
	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoProfessor(CenarioDTO cenarioDTO, RelatorioVisaoProfessorFiltro filtro, boolean isVisaoProfessor) throws TriedaException;
	
	AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoAluno(CenarioDTO cenarioDTO, RelatorioVisaoAlunoFiltro filtro) throws TriedaException;
	
	PagingLoadResult< AtendimentoTaticoDTO > getList();

//	ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > getBusca(
//		CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO );

	ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais(CenarioDTO cenarioDTO);
	
	PagingLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CenarioDTO cenarioDTO, TitulacaoDTO titulacaoDTO, PagingLoadConfig config ) throws TriedaException;

	List< PercentMestresDoutoresDTO > getPercentMestresDoutoresList( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<AtendimentoFaixaCreditoDTO> getAtendimentosFaixaCredito( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<AtendimentoFaixaCreditoDTO> getAtendimentosFaixaDisciplina( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<RelatorioQuantidadeDTO> getProfessoresJanelasGrade( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<RelatorioQuantidadeDTO> getProfessoresDisciplinasLecionadas( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<RelatorioQuantidadeDTO> getAmbientesFaixaOcupacaoHorarios( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<RelatorioQuantidadeDTO> getAmbientesFaixaUtilizacaoCapacidade( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	List<AtendimentoFaixaTurmaDTO> getAtendimentosFaixaTurma( CenarioDTO cenarioDTO, CampusDTO campusDTO );

	PagingLoadResult<ConfirmacaoTurmaDTO> getConfirmacaoTurmasList(
			CenarioDTO cenarioDTO, PagingLoadConfig config);

	void saveConfirmacoes(CenarioDTO cenarioDTO, List<ConfirmacaoTurmaDTO> list);

	void confirmarTodasTurmas(CenarioDTO cenarioDTO);
	void desconfirmarTodasTurmas(CenarioDTO cenarioDTO);
	void confirmarTurmasComAlunosFormandos(CenarioDTO cenarioDTO);

	void confirmarTurmasComNumAlunos(Integer qtdeAlunos, CenarioDTO cenarioDTO);
	void desconfirmarTurmasComNumAlunos(Integer qtdeAlunos, CenarioDTO cenarioDTO);

	TrioDTO<String, String, String> carregaIndicadores(CenarioDTO cenarioDTO);

	ListLoadResult<ConfirmacaoTurmaDTO> getTurmasProfessoresVirtuaisList(CenarioDTO cenarioDTO, ProfessorVirtualDTO professorVirtualDTO);

	ListLoadResult<MotivoUsoProfessorVirtualDTO> getMotivosUsoProfessorVirtual(	CenarioDTO cenarioDTO, Long disciplinaId, String turma,
			Boolean credTeorico);

	ListLoadResult<DicaEliminacaoProfessorVirtualDTO> getDicasEliminacaoProfessorVirtual( CenarioDTO cenarioDTO, Long disciplinaId, String turma,
			Boolean credTeorico);

	ListLoadResult<TurmaStatusDTO> getTurmasStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO);

	void deleteTurmasStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, List<TurmaStatusDTO> turmasStatusDTO);

	void saveTurma(TurmaDTO turmaDTO) throws TriedaException;

	ParDTO<TurmaDTO, List<AulaDTO>> selecionarTurma(TurmaStatusDTO turmaStatusDTO, CenarioDTO cenarioDTO, DemandaDTO demandaDTO);
}
