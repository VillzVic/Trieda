package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoStatusDTO;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaCreditoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaTurmaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DicaEliminacaoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.MotivoUsoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorStatusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.PesquisaPorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDoubleDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaStatusDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AtendimentosServiceAsync {
	
	void getAtendimentosParaGradeHorariaVisaoCurso(RelatorioVisaoCursoFiltro filtro, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getAtendimentosParaGradeHorariaVisaoSala(CenarioDTO cenarioDTO,
			RelatorioVisaoSalaFiltro filtro, boolean buscaAulasParciais,
			AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getAtendimentosParaGradeHorariaVisaoProfessor(CenarioDTO cenarioDTO, RelatorioVisaoProfessorFiltro filtro, boolean isVisaoProfessor, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getAtendimentosParaGradeHorariaVisaoAluno(CenarioDTO cenarioDTO, RelatorioVisaoAlunoFiltro filtro, AsyncCallback<AtendimentoServiceRelatorioResponse> callback);
	
	void getList( AsyncCallback< PagingLoadResult< AtendimentoTaticoDTO > > callback );

//	void getBusca( CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO,
//		AsyncCallback< ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > > callback );

	void getProfessoresVirtuais(CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< ProfessorVirtualDTO > > callback );
	
	void getProfessoresVirtuais( CenarioDTO cenarioDTO, TitulacaoDTO titulacaoDTO,  TipoContratoDTO tipoContratoDTO, AreaTitulacaoDTO areaTitulacaoDTO,String nome, 
			PagingLoadConfig config, AsyncCallback< PagingLoadResult< ProfessorVirtualDTO > > callback );

	void getProfessoresVirtuais( CenarioDTO cenarioDTO, TitulacaoDTO titulacaoDTO, TipoContratoDTO tipoContratoDTO, AreaTitulacaoDTO areaTitulacaoDTO,String nome, Long campusId,
			PagingLoadConfig config, AsyncCallback< PagingLoadResult< ProfessorVirtualDTO > > callback );
	
	void getPercentMestresDoutoresList( CenarioDTO cenarioDTO, CampusDTO campusDTO,	AsyncCallback<List<PercentMestresDoutoresDTO>> callback );

	void getAtendimentosFaixaCredito( CenarioDTO cenarioDTO, CampusDTO campusDTO,
			AsyncCallback<List<AtendimentoFaixaCreditoDTO>> callback );

	void getAtendimentosFaixaDisciplina( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<AtendimentoFaixaCreditoDTO>> callback );

	void getProfessoresJanelasGrade( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getProfessoresDisciplinasLecionadas( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getAmbientesFaixaOcupacaoHorarios( CenarioDTO cenarioDTO, CampusDTO campusDTO,	AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getAmbientesFaixaUtilizacaoCapacidade( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback );

	void getAtendimentosFaixaTurma( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<AtendimentoFaixaTurmaDTO>> callback );

	void getConfirmacaoTurmasList(CenarioDTO cenarioDTO, PagingLoadConfig config,
			AsyncCallback<PagingLoadResult<ConfirmacaoTurmaDTO>> callback);
	
	void getAtendimentosPesquisaPorDisciplinaList(CenarioDTO cenarioDTO,
			String codigo, String nome, CampusDTO campusDTO, String turma,
			PagingLoadConfig config,
			AsyncCallback<PagingLoadResult<PesquisaPorDisciplinaDTO>> callback);

	void saveConfirmacoes(CenarioDTO cenarioDTO, List<ConfirmacaoTurmaDTO> list, AsyncCallback<Void> callback);

	void confirmarTodasTurmas(CenarioDTO cenarioDTO,
			AsyncCallback<Void> callback);

	void desconfirmarTodasTurmas(CenarioDTO cenarioDTO,
			AsyncCallback<Void> callback);
	
	void confirmarTurmasComAlunosFormandos(CenarioDTO cenarioDTO,
			AsyncCallback<Void> callback);

	void confirmarTurmasComNumAlunos(Integer qtdeAlunos, CenarioDTO cenarioDTO,
			AsyncCallback<Void> callback);
	
	void desconfirmarTurmasComNumAlunos(Integer qtdeAlunos, CenarioDTO cenarioDTO,
			AsyncCallback<Void> callback);

	void carregaIndicadores(CenarioDTO cenarioDTO,
			AsyncCallback<TrioDTO<String, String, String>> callback);

	void getTurmasProfessoresVirtuaisList(CenarioDTO cenarioDTO, ProfessorVirtualDTO professorVirtualDTO,
			AsyncCallback<ListLoadResult<ConfirmacaoTurmaDTO>> callback);

	void getMotivosUsoProfessorVirtual(CenarioDTO cenarioDTO, Long disciplinaId, Long salaId, String turma, Boolean credTeorico,
			AsyncCallback<ListLoadResult<MotivoUsoProfessorVirtualDTO>> callback);

	void getDicasEliminacaoProfessorVirtual( CenarioDTO cenarioDTO, Long disciplinaId, Long salaId, String turma, Boolean credTeorico,
			AsyncCallback<ListLoadResult<DicaEliminacaoProfessorVirtualDTO>> callback);

	void getTurmasStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			AsyncCallback<ListLoadResult<TurmaStatusDTO>> callback);

	void deleteTurmasStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			List<TurmaStatusDTO> turmasStatusDTO, AsyncCallback<Void> callback);

	void saveTurma(TurmaDTO turmaDTO, AsyncCallback<TurmaDTO> callback);

	void selecionarTurma(TurmaStatusDTO turmaStatusDTO, CenarioDTO cenarioDTO,
			DemandaDTO demandaDTO,
			AsyncCallback<ParDTO<TurmaDTO, List<AulaDTO>>> callback);

	void getAlunosStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			TurmaDTO turmaDTO, List<AulaDTO> aulasDTO,
			AsyncCallback<ListLoadResult<AlunoStatusDTO>> callback);

	void getProfessoresStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			TurmaDTO turmaDTO, AulaDTO aulaDTO,
			AsyncCallback<ListLoadResult<ProfessorStatusDTO>> callback);

	void getHorariosDisponiveisAula(CenarioDTO cenarioDTO, SalaDTO salaDTO,
			DisciplinaDTO disciplinaDTO, SemanaLetivaDTO semanaLetivaDTO,
			String semana,
			AsyncCallback<ListLoadResult<HorarioDisponivelCenarioDTO>> callback);

	void verificaViabilidadeAula(CenarioDTO cenarioDTO, TurmaDTO turmaDTO,
			AulaDTO aulaDTO, List<AulaDTO> aulasTurma,
			AsyncCallback<TrioDTO<Boolean, List<String>, List<String>>> callback);

	void saveAula(DisciplinaDTO disciplinaDTO, CampusDTO campusDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO,
			AsyncCallback<TurmaDTO> callback);

	void alocaAlunosTurma(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			TurmaDTO turmaDTO, List<AlunoStatusDTO> alunos,
			AsyncCallback<Void> callback);

	void verificaViabilidadeAulasNovosAlunos(CenarioDTO cenarioDTO,
			TurmaDTO turmaDTO, List<AulaDTO> aulasDTO,
			List<AlunoStatusDTO> alunos,
			AsyncCallback<TrioDTO<Boolean, List<String>, List<String>>> callback);

	void alocaProfessoresAula(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			TurmaDTO turmaDTO, AulaDTO aulaDTO,
			ProfessorStatusDTO professorStatusDTO, AsyncCallback<Void> callback);

	void verificaViabilidadeAulasNovoProfessor(CenarioDTO cenarioDTO,
			TurmaDTO turmaDTO, AulaDTO aulaDTO,
			ProfessorStatusDTO professorStatusDTO,
			AsyncCallback<TrioDTO<Boolean, List<String>, List<String>>> callback);

	void verificaViabilidadeSalvarTurma(CenarioDTO cenarioDTO,
			TurmaDTO turmaDTO, List<AulaDTO> aulasDTO,
			AsyncCallback<TrioDTO<Boolean, List<String>, List<String>>> callback);

	void salvarTurma(TurmaDTO turmaDTO, AsyncCallback<Void> callback);

	void deleteTurmaSelecionada(CenarioDTO cenarioDTO, DemandaDTO demandaDTO,
			TurmaDTO turmaDTO, AsyncCallback<Void> callback);

	void editTurma(TurmaDTO turmaDTO, String turmaEditadaNome,
			AsyncCallback<Void> callback);

	void confirmarTurmaSelecionada(DemandaDTO demandaDTO, TurmaDTO turmaDTO,
			AsyncCallback<Void> callback);
	
	void desconfirmarTurmaSelecionada(DemandaDTO demandaDTO, TurmaDTO turmaDTO,
			AsyncCallback<Void> callback);

	void removeAula(TurmaDTO turmaDTO, AulaDTO aulaDTO,
			AsyncCallback<Void> callback);

	void getAmbientesTurma(CenarioDTO cenarioDTO, TurmaDTO turmaDTO,
			List<AulaDTO> aulas, CampusDTO campusDTO,
			AsyncCallback<ListLoadResult<SalaStatusDTO>> callback);

	void getAmbientesFaixaUtilizacaoPorDiaSemana(CenarioDTO cenarioDTO,
			CampusDTO campusDTO,
			AsyncCallback<List<RelatorioQuantidadeDoubleDTO>> callback);

	void getAmbientesFaixaOcupacaoHorariosPorDiaSemana(CenarioDTO cenarioDTO,
			CampusDTO campusDTO,
			AsyncCallback<List<RelatorioQuantidadeDoubleDTO>> callback);
}