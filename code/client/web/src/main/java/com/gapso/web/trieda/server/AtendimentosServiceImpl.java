package com.gapso.web.trieda.server;

import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.TimeUnit;

import org.eclipse.jdt.internal.compiler.ast.DoStatement;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.format.number.NumberFormatter;
import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Aula;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.DicaEliminacaoProfessorVirtual;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Disponibilidade;
import com.gapso.trieda.domain.DisponibilidadeDisciplina;
import com.gapso.trieda.domain.DisponibilidadeSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.MotivoUsoProfessorVirtual;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.trieda.domain.TriedaTrio;
import com.gapso.trieda.domain.Turma;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.shared.dtos.AlunoStatusDTO;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaCreditoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaTurmaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.AulaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DicaEliminacaoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.MotivoUsoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.dtos.PesquisaPorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorStatusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuartetoDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDoubleDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaStatusDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.relatorioVisao.AtendimentoServiceRelatorioResponse;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.dev.util.Pair;

public class AtendimentosServiceImpl extends RemoteService implements AtendimentosService {
	private static final long serialVersionUID = -1505176338607927637L;
	
	static public interface IAtendimentosServiceDAO {
		List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO);
		List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(Cenario cenario, String alunoNome, String alunoMatricula);
		List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO);
		List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(Cenario cenario, String alunoNome, String alunoMatricula);
		Map<Long,SemanaLetiva> buscaSemanasLetivas();
	}
	
	public IAtendimentosServiceDAO getDAO() {
		IAtendimentosServiceDAO dao = new IAtendimentosServiceDAO() {
			@Override
			public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
				return buscaNoBancoDadosDTOsDeAtendimentoTatico(curriculoDTO,periodo,turnoDTO,campusDTO,cursoDTO);
			}
			
			@Override
			public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
				return buscaNoBancoDadosDTOsDeAtendimentoOperacional(curriculoDTO,periodo,turnoDTO,campusDTO,cursoDTO);
			}

			@Override
			public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(Cenario cenario, String alunoNome, String alunoMatricula) {
				return buscaNoBancoDadosDTOsDeAtendimentoTatico(cenario, alunoNome,alunoMatricula);
			}

			@Override
			public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(Cenario cenario, String alunoNome, String alunoMatricula) {
				return buscaNoBancoDadosDTOsDeAtendimentoOperacional(cenario, alunoNome,alunoMatricula);
			}

			@Override
			public Map<Long,SemanaLetiva> buscaSemanasLetivas() {
				return buscaNoBancoDadosSemanasLetivas();
			}
		};
		
		return dao;
	}
	
	/**
	 * @param atendimentos lista de atendimentos a serem ordenados pelo horário de início do atendimento
	 * @return lista de atendimentos ordenados pelo horário de início
	 */
	public List<AtendimentoOperacionalDTO> ordenaPorHorarioAula(List<AtendimentoOperacionalDTO> atendimentos) {
		if (atendimentos == null || atendimentos.isEmpty()) {
			return Collections.<AtendimentoOperacionalDTO>emptyList();
		}
		
		List<AtendimentoOperacionalDTO> atendimentosOrdenados = new ArrayList<AtendimentoOperacionalDTO>(atendimentos);
		if (atendimentos.size() == 1) {
			return atendimentosOrdenados;
		}

		// [HorarioAulaId -> HorarioAula]
		//final Map<Long,HorarioAula> idToHorarioAulaMap = HorarioAula.buildHorarioAulaIdToHorarioAulaMap(HorarioAula.findAll(getInstituicaoEnsinoUser()));

		Collections.sort(atendimentosOrdenados, new Comparator<AtendimentoOperacionalDTO>() {
			@Override
			public int compare(AtendimentoOperacionalDTO arg1, AtendimentoOperacionalDTO arg2) {
				Pair<Calendar,Calendar> par1 = extraiHorariosInicial_e_Final(arg1);
				Pair<Calendar,Calendar> par2 = extraiHorariosInicial_e_Final(arg2);
				
				Calendar hi_1 = par1.getLeft();
				Calendar hi_2 = par2.getLeft();
				
				return hi_1.compareTo(hi_2);
				
//				HorarioAula h1 = idToHorarioAulaMap.get(arg1.getHorarioId());
//				HorarioAula h2 = idToHorarioAulaMap.get(arg2.getHorarioId());
//
//				return h1.getHorario().compareTo( h2.getHorario() );
			}
		});

		return atendimentosOrdenados;
	}
	
	/**
	 * @throws TriedaException 
	 * @see com.gapso.web.trieda.shared.services.AtendimentosService#getAtendimentosParaGradeHorariaVisaoSala(com.gapso.web.trieda.shared.dtos.SalaDTO, com.gapso.web.trieda.shared.dtos.TurnoDTO)
	 */
	@Override
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoSala(CenarioDTO cenarioDTO, RelatorioVisaoSalaFiltro filtro, boolean buscaAulasParciais) throws TriedaException{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		QuintetoDTO<List<AtendimentoRelatorioDTO>, ParDTO<Integer, Boolean>, List<String>, List<String>, List<String>> q;
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		List<TrioDTO<String, Integer, Integer>> horariosDisponiveis = new ArrayList<TrioDTO<String, Integer, Integer>>();
		boolean temInfoDeHorario = true;
		DateFormat df = new SimpleDateFormat( "HH:mm" );

		// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
		try {
			List<AtendimentoTaticoDTO> atendimentosTaticoDTO = buscaNoBancoDadosDTOsDeAtendimentoTatico(cenario, filtro.getSalaCodigo());
			Sala sala = Sala.findByCodigo(getInstituicaoEnsinoUser(), cenario, filtro.getSalaCodigo());
			if (!atendimentosTaticoDTO.isEmpty()) {
				temInfoDeHorario = (atendimentosTaticoDTO.iterator().next().getHorarioAulaId() != null);
				// insere os atendimentos do modo tático na lista de atendimentos
				aulas.addAll(atendimentosTaticoDTO);
			}
			else {
				temInfoDeHorario = true;
				// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
				List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = buscaNoBancoDadosDTOsDeAtendimentoOperacional(cenario, filtro.getSalaCodigo());
				// processa os atendimentos do operacional e os transforma em aulas
				List<AtendimentoOperacionalDTO> aulasOperacional = extraiAulas(atendimentosOperacionalDTO);
				// insere as aulas do modo operacional na lista de atendimentos
				aulas.addAll(aulasOperacional);
			}
			if (buscaAulasParciais)
			{
				aulas.addAll(getAulasParciais(cenario, filtro.getSalaCodigo()));
			}
			
			for (Disponibilidade horario : sala.getDisponibilidades())
			{
				for (Semanas semana : horario.getDiasSemana())
				{
					String inicio = df.format( horario.getHorarioInicio() );
					Calendar horInicio = Calendar.getInstance();
					horInicio.setTime(horario.getHorarioInicio());
					horInicio.set(1979,Calendar.NOVEMBER,6);
					
					Calendar horFim = Calendar.getInstance();
					horFim.setTime(horario.getHorarioFim());
					horFim.set(1979,Calendar.NOVEMBER,6);
					
					horariosDisponiveis.add(TrioDTO.create(inicio, Semanas.toInt(semana),
							(int) TimeUnit.MILLISECONDS.toMinutes(horFim.getTimeInMillis() - horInicio.getTimeInMillis())));
				}
			}
			
			if (!aulas.isEmpty()) {
		 		// trata compartilhamento de turmas entre cursos
				List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = uneAulasQuePodemSerCompartilhadas(aulas);
				
				// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão
				// juntamente com o numero de semanas letivas utilizadas e o máximo de 
				// créditos em um dia das semanas letivas relacionadas com as aulas em questão
				Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(aulasComCompartilhamentos);
				Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = buscaNoBancoDadosSemanasLetivas();
				Set<Long> turnosIDsDasAulas = new HashSet<Long>();
				for (AtendimentoRelatorioDTO aula : aulas)
				{
					turnosIDsDasAulas.add(aula.getTurnoId());
				}
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, turnosIDsDasAulas); 
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
				List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
				List<String> horariosDeFimDeAula = quarteto.getQuarto();
				
				q = QuintetoDTO.create(aulasComCompartilhamentos,mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			}
			else if (buscaAulasParciais)
			{
				// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão
				// juntamente com o numero de semanas letivas utilizadas e o máximo de 
				// créditos em um dia das semanas letivas relacionadas com as aulas em questão
				Set<Long> semanasLetivasIDsDasAulasNaSala = new HashSet<Long>();
				for (SemanaLetiva semanaLetiva : SemanaLetiva.findByCenario(getInstituicaoEnsinoUser(), cenario))
				{
					semanasLetivasIDsDasAulasNaSala.add(semanaLetiva.getId());
				}
				Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = buscaNoBancoDadosSemanasLetivas();
				Set<Long> turnosIDsDasAulas = new HashSet<Long>();
				for (Turno turno : Turno.findByCenario(getInstituicaoEnsinoUser(), cenario))
				{
					turnosIDsDasAulas.add(turno.getId());
				}
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, turnosIDsDasAulas); 
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
				List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
				List<String> horariosDeFimDeAula = quarteto.getQuarto();
				
				q = QuintetoDTO.create(aulas,mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			}
			else q = QuintetoDTO.create(aulas,ParDTO.create(0, false),Collections.<String>emptyList(),Collections.<String>emptyList(),Collections.<String>emptyList());
		}
		catch (EmptyResultDataAccessException ex){
			throw new TriedaException("Os campos digitados no filtro não foram encontrados");
		}
		
		return AtendimentoServiceRelatorioResponse.create(q, horariosDisponiveis);
	}
	
	private List<AtendimentoOperacionalDTO> getAulasParciais(Cenario cenario, String salaCodigo) {
		Sala sala = Sala.findByCodigo(getInstituicaoEnsinoUser(), cenario, salaCodigo);
		
		List<Aula> aulas = Aula.findBy(getInstituicaoEnsinoUser(), cenario, sala);
		List<AtendimentoOperacionalDTO> result = new ArrayList<AtendimentoOperacionalDTO>();
		for(Aula aula : aulas)
		{
			AtendimentoOperacionalDTO dto = new AtendimentoOperacionalDTO();

			dto.setCenarioId( aula.getCenario().getId() );
			dto.setCampusId( aula.getSala().getUnidade().getCampus().getId() );
			dto.setCampusString( aula.getSala().getUnidade().getCampus().getCodigo() );
			dto.setUnidadeId( aula.getSala().getUnidade().getId() );
			dto.setUnidadeString( aula.getSala().getUnidade().getCodigo() );
			dto.setSalaId( aula.getSala().getId() );
			dto.setSalaString( aula.getSala().getCodigo() );

			HorarioDisponivelCenario hdc = aula.getHorarioDisponivelCenario();
			dto.setHorarioDisponivelCenarioId( hdc.getId() );
			dto.setSemana( Semanas.toInt( hdc.getDiaSemana() ) );
			dto.setDiaSemana( dto.getSemana() );

			HorarioAula ha = hdc.getHorarioAula();
			dto.setHorarioAulaId( ha.getId() );
			dto.setHorarioAulaString( TriedaUtil.shortTimeString( ha.getHorario() ) );
			dto.setTurnoId( ha.getTurno().getId() );
			dto.setTurnoString( ha.getTurno().getNome() );

			if ( aula.getProfessor() != null )
			{
				dto.setProfessorId( aula.getProfessor().getId() );
				dto.setProfessorCPF( aula.getProfessor().getCpf() );
				dto.setProfessorString( aula.getProfessor().getNome() );
				dto.setProfessorCustoCreditoSemanal(aula.getProfessor().getValorCredito());
			}
			else if ( aula.getProfessorVirtual() != null )
			{
				dto.setProfessorVirtualId( aula.getProfessorVirtual().getId() );
				dto.setProfessorCPF( aula.getProfessorVirtual().getId().toString() );
				dto.setProfessorVirtualString("Professor Virtual " + aula.getProfessorVirtual().getId() );
			}
			
			Iterator<Turma> iter = aula.getTurmas().iterator();

			Turma turma = iter.next();

			dto.setCreditoTeoricoBoolean( aula.getCreditosTeoricos() > 0 );
			dto.setDisciplinaId( turma.getDisciplina().getId() );
			dto.setDisciplinaString( turma.getDisciplina().getCodigo() );
			dto.setDisciplinaNome( turma.getDisciplina().getNome() );
			dto.setDisciplinaOriginalCodigo(turma.getDisciplina().getCodigo());
			dto.setDisciplinaUsaLaboratorio(turma.getDisciplina().getLaboratorio());
			dto.setTotalCreditosTeoricosDisciplina( turma.getDisciplina().getCreditosTeorico() );
			dto.setTotalCreditosPraticosDisciplina( turma.getDisciplina().getCreditosPratico() );
			dto.setTotalCreditoDisciplina( turma.getDisciplina().getTotalCreditos() );
			dto.setQuantidadeAlunos( turma.getAlunos().size() );
			dto.setQuantidadeAlunosString( String.valueOf(turma.getAlunos().size()) );
			dto.setTurma( turma.getNome() );

			dto.setTotalCreditos(aula.getCreditosPraticos() + aula.getCreditosTeoricos()); // todo atendimento operacional representa apenas 1 crédito atendido
			dto.setDisplayText( dto.getNaturalKey() );
			dto.setCompartilhamentoCursosString( "" );
			dto.setSemanaLetivaId( aula.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getId() );
			dto.setSemanaLetivaTempoAula( aula.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva().getTempo());
			
			Map<Oferta,Map<Disciplina,Set<AlunoDemanda>>> ofeToDiscSubstToAlnDemMap = new HashMap<Oferta,Map<Disciplina,Set<AlunoDemanda>>>();
			for (AlunoDemanda ald : turma.getAlunos())
			{
				Oferta ald_ofe = ald.getDemanda().getOferta();
				Disciplina ald_dis = ald.getDemanda().getDisciplina();
				
				Map<Disciplina,Set<AlunoDemanda>> discSubstToAlnDemMap = ofeToDiscSubstToAlnDemMap.get(ald_ofe);
				if (discSubstToAlnDemMap == null) {
					discSubstToAlnDemMap = new HashMap<Disciplina,Set<AlunoDemanda>>();
					ofeToDiscSubstToAlnDemMap.put(ald_ofe,discSubstToAlnDemMap);
				}		
				
				Set<AlunoDemanda> alnDem = discSubstToAlnDemMap.get(ald_dis);
				if (alnDem == null) {
					alnDem = new HashSet<AlunoDemanda>();
					discSubstToAlnDemMap.put(ald_dis,alnDem);
				}
				alnDem.add(ald);
			}
			
			dto.setCursoNome("");
			dto.setPeriodoString("");
			dto.setCurricularString("");
			for (Oferta oferta : ofeToDiscSubstToAlnDemMap.keySet() )
			{
				dto.setCursoNome((dto.getCursoNome().isEmpty() ? dto.getCursoNome() : (dto.getCursoNome() + " / ")) + oferta.getCurso().getNome());
				Integer periodo = oferta.getCurriculo().getPeriodo(Disciplina.find(dto.getDisciplinaId(), getInstituicaoEnsinoUser()));
				dto.setPeriodoString((dto.getPeriodoString().isEmpty() ? dto.getPeriodoString() : (dto.getPeriodoString() + " / ")) + (periodo == null ? "" : periodo));
				dto.setCurricularString((dto.getCurriculoString().isEmpty() ? dto.getCurriculoString() : (dto.getCurriculoString() + " / ")) + oferta.getCurriculo().getCodigo());
			}
			
			result.add(dto);
		}
		return result;
	}
	
	public QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(Set<Long> semanasLetivasIDsDasAulas, Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap, boolean temInfoDeHorario, Set<Long> turnoId) {
		List<String> labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		List<String> horariosDeInicioDeAula = new ArrayList<String>();
		List<String> horariosDeFimDeAula = new ArrayList<String>();
		
		if (!semanasLetivasIDsDasAulas.isEmpty()) {
//			List<SemanaLetiva> todasSemanasLetivas = SemanaLetiva.findAll(getInstituicaoEnsinoUser());
//			Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = SemanaLetiva.buildSemanaLetivaIDToSemanaLetivaMap(todasSemanasLetivas);
			List<SemanaLetiva> semanasLetivasDasAulas = new ArrayList<SemanaLetiva>();
			for (Long semanaLetivaId : semanasLetivasIDsDasAulas) {
				semanasLetivasDasAulas.add(semanaLetivaIdToSemanaLetivaMap.get(semanaLetivaId));
			}
			TriedaPar<Integer, Boolean> parMDCHorarioInteresecao= SemanaLetiva.caculaMaximoDivisorComumParaTemposDeAulaDasSemanasLetivas(semanasLetivasDasAulas);
			ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = ParDTO.create(parMDCHorarioInteresecao.getPrimeiro(), parMDCHorarioInteresecao.getSegundo()) ;
			
			calculaLabelsDasLinhasDaGradeHoraria(temInfoDeHorario,turnoId,semanasLetivasDasAulas,mdcTemposAulaNumSemanasLetivas.getPrimeiro(),labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			
			return QuartetoDTO.create(mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
		}
		return QuartetoDTO.create(ParDTO.create(0, false),labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
	}
	
	private void calculaLabelsDasLinhasDaGradeHoraria(boolean temInfoDeHorario, Set<Long> turnoId, List<SemanaLetiva> semanasLetivasDasAulas, int mdcTemposAula,
			List<String> labelsDasLinhasDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {		
		// coleta todos os pares (HoraInicio,HoraFim) dos horários de aula das semanas letivas
		Set<Pair<Calendar,Calendar>> horarios = new HashSet<Pair<Calendar,Calendar>>();
		for (SemanaLetiva semanaLetiva : semanasLetivasDasAulas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				if (turnoId.contains( horarioAula.getTurno().getId())) {
					Calendar hi = TriedaServerUtil.dateToCalendar(horarioAula.getHorario());
					Calendar hf = Calendar.getInstance();
					hf.clear();
					hf.setTime(hi.getTime());
					hf.add(Calendar.MINUTE,semanaLetiva.getTempo());
					
					horarios.add(Pair.create(hi,hf));
				}
			}
		}
		
		List<Pair<Calendar,Calendar>> horariosOrdenados = new ArrayList<Pair<Calendar,Calendar>>(horarios);
		// ordena os pares (HoraInicio,HoraFim)
		TriedaServerUtil.ordenaParesDeHorarios(horariosOrdenados);
		
		// processa lista de pares de horários e une aqueles que possuem interseção em um único par (HorarioInicio,HorarioFim)
		List<Pair<Calendar,Calendar>> horariosProcessados = new ArrayList<Pair<Calendar,Calendar>>();
		for (Pair<Calendar,Calendar> parAtual : horariosOrdenados) {
			horariosDeInicioDeAula.add(TriedaUtil.shortTimeString(parAtual.getLeft().getTime()));
			horariosDeFimDeAula.add(TriedaUtil.shortTimeString(parAtual.getRight().getTime()));
			
			if (horariosProcessados.isEmpty()) {
				horariosProcessados.add(parAtual);
			} else {
				Pair<Calendar,Calendar> parProcessado = horariosProcessados.get(horariosProcessados.size()-1);
				if (temIntersecao(parProcessado,parAtual) || saoConsecutivos(parProcessado,parAtual)) {
					if (parAtual.getRight().after(parProcessado.getRight())) {
						parProcessado.getRight().setTime(parAtual.getRight().getTime());
					}
				} else {
					horariosProcessados.add(parAtual);
				}
			}
		}
		
		// escreve os labels de cada linha da grade de horários
		Integer cargaHorariaAcumuladaEmMinutos = mdcTemposAula;
		for (Pair<Calendar,Calendar> par : horariosProcessados) {
			Calendar h = par.getLeft();
			Calendar hf = par.getRight();
			while (h.before(hf)) {
				if (temInfoDeHorario) {
					labelsDasLinhasDaGradeHoraria.add(TriedaUtil.shortTimeString(h.getTime()));
				} else {
					labelsDasLinhasDaGradeHoraria.add(cargaHorariaAcumuladaEmMinutos.toString()+" (min)");
					cargaHorariaAcumuladaEmMinutos += mdcTemposAula;
				}
				h.add(Calendar.MINUTE,mdcTemposAula);
			}
			if (temInfoDeHorario) {
				labelsDasLinhasDaGradeHoraria.add(TriedaUtil.shortTimeString(hf.getTime()));
			}
		}
	}

	public QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(Set<Long> semanasLetivasIDsDasAulas, Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap, boolean temInfoDeHorario) {
		List<String> labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		List<String> horariosDeInicioDeAula = new ArrayList<String>();
		List<String> horariosDeFimDeAula = new ArrayList<String>();
		
		if (!semanasLetivasIDsDasAulas.isEmpty()) {
//			List<SemanaLetiva> todasSemanasLetivas = SemanaLetiva.findAll(getInstituicaoEnsinoUser());
//			Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = SemanaLetiva.buildSemanaLetivaIDToSemanaLetivaMap(todasSemanasLetivas);
			List<SemanaLetiva> semanasLetivasDasAulas = new ArrayList<SemanaLetiva>();
			for (Long semanaLetivaId : semanasLetivasIDsDasAulas) {
				semanasLetivasDasAulas.add(semanaLetivaIdToSemanaLetivaMap.get(semanaLetivaId));
			}
			TriedaPar<Integer, Boolean> parMDCHorarioInteresecao= SemanaLetiva.caculaMaximoDivisorComumParaTemposDeAulaDasSemanasLetivas(semanasLetivasDasAulas);
			ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = ParDTO.create(parMDCHorarioInteresecao.getPrimeiro(), parMDCHorarioInteresecao.getSegundo()) ;
			
			calculaLabelsDasLinhasDaGradeHoraria(temInfoDeHorario,semanasLetivasDasAulas,mdcTemposAulaNumSemanasLetivas.getPrimeiro(),labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			
			return QuartetoDTO.create(mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
		}
		return QuartetoDTO.create(ParDTO.create(0, false),labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
	}
	
	private void calculaLabelsDasLinhasDaGradeHoraria(boolean temInfoDeHorario, List<SemanaLetiva> semanasLetivasDasAulas, int mdcTemposAula,
			List<String> labelsDasLinhasDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {		
		// coleta todos os pares (HoraInicio,HoraFim) dos horários de aula das semanas letivas
		Set<Pair<Calendar,Calendar>> horarios = new HashSet<Pair<Calendar,Calendar>>();
		for (SemanaLetiva semanaLetiva : semanasLetivasDasAulas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				Calendar hi = TriedaServerUtil.dateToCalendar(horarioAula.getHorario());
				Calendar hf = Calendar.getInstance();
				hf.clear();
				hf.setTime(hi.getTime());
				hf.add(Calendar.MINUTE,semanaLetiva.getTempo());
				
				horarios.add(Pair.create(hi,hf));
			}
		}
		
		List<Pair<Calendar,Calendar>> horariosOrdenados = new ArrayList<Pair<Calendar,Calendar>>(horarios);
		// ordena os pares (HoraInicio,HoraFim)
		TriedaServerUtil.ordenaParesDeHorarios(horariosOrdenados);
		
		// processa lista de pares de horários e une aqueles que possuem interseção em um único par (HorarioInicio,HorarioFim)
		List<Pair<Calendar,Calendar>> horariosProcessados = new ArrayList<Pair<Calendar,Calendar>>();
		for (Pair<Calendar,Calendar> parAtual : horariosOrdenados) {
			horariosDeInicioDeAula.add(TriedaUtil.shortTimeString(parAtual.getLeft().getTime()));
			horariosDeFimDeAula.add(TriedaUtil.shortTimeString(parAtual.getRight().getTime()));
			
			if (horariosProcessados.isEmpty()) {
				horariosProcessados.add(parAtual);
			} else {
				Pair<Calendar,Calendar> parProcessado = horariosProcessados.get(horariosProcessados.size()-1);
				if (temIntersecao(parProcessado,parAtual) || saoConsecutivos(parProcessado,parAtual)) {
					if (parAtual.getRight().after(parProcessado.getRight())) {
						parProcessado.getRight().setTime(parAtual.getRight().getTime());
					}
				} else {
					horariosProcessados.add(parAtual);
				}
			}
		}
		
		// escreve os labels de cada linha da grade de horários
		Integer cargaHorariaAcumuladaEmMinutos = mdcTemposAula;
		for (Pair<Calendar,Calendar> par : horariosProcessados) {
			Calendar h = par.getLeft();
			Calendar hf = par.getRight();
			while (h.before(hf)) { // while (!h.equals(hf)) { .. entrava em loop
				if (temInfoDeHorario) {
					labelsDasLinhasDaGradeHoraria.add(TriedaUtil.shortTimeString(h.getTime()));
				} else {
					labelsDasLinhasDaGradeHoraria.add(cargaHorariaAcumuladaEmMinutos.toString()+" (min)");
					cargaHorariaAcumuladaEmMinutos += mdcTemposAula;
				}
				h.add(Calendar.MINUTE,mdcTemposAula);
			}
			if (temInfoDeHorario) {
				//System.out.println("Horario: " + TriedaUtil.shortTimeString(h.getTime()) + " " + TriedaUtil.shortTimeString(hf.getTime()));
				labelsDasLinhasDaGradeHoraria.add(TriedaUtil.shortTimeString(hf.getTime()));
			}
		}
	}

	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoCurso(RelatorioVisaoCursoFiltro filtro, IAtendimentosServiceDAO dao){
		SextetoDTO<List<AtendimentoRelatorioDTO>,ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>,List<Integer>> s;
		// Par<Aulas, Qtd de Colunas para cada Dia da Semana da Grade Horária>
		ParDTO<List<AtendimentoRelatorioDTO>, List<Integer>> parResultante = ParDTO.<List<AtendimentoRelatorioDTO>, List<Integer>>create(new ArrayList<AtendimentoRelatorioDTO>(), null);
		boolean temInfoDeHorario = true;
		
		// verifica se o campus foi otimizado no modo tático ou no operacional
		if (filtro.getCampusDTO().getOtimizadoTatico()) {
			// Otimização no modo Tático
			
			// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
			List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>(dao.buscaDTOsDeAtendimentoTatico(filtro.getCurriculoDTO(), filtro.getPeriodo(), filtro.getTurnoDTO(), filtro.getCampusDTO(), filtro.getCursoDTO()));
			
			temInfoDeHorario = (aulas.iterator().next().getHorarioAulaId() != null);
			
			// Par<Aulas, Qtd de Colunas para cada Dia da Semana da Grade Horária>
			ParDTO<List<AtendimentoRelatorioDTO>,List<Integer>> parDTO = montaEstruturaParaGradeHorariaVisaoCurso(temInfoDeHorario,aulas,filtro.getTurnoDTO());
			
			// preenche o par resultante
			parResultante.getPrimeiro().addAll(parDTO.getPrimeiro());
			parResultante.setSegundo(parDTO.getSegundo());
		} else {
			// Otimização no modo Operacional
			temInfoDeHorario = true;
			
			// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
			List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = dao.buscaDTOsDeAtendimentoOperacional(filtro.getCurriculoDTO(), filtro.getPeriodo(), filtro.getTurnoDTO(), filtro.getCampusDTO(), filtro.getCursoDTO());
			// processa os atendimentos do operacional e os transforma em aulas
			List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>(extraiAulas(atendimentosOperacionalDTO));
			// Par<Aulas, Qtd de Colunas para cada Dia da Semana da Grade Horária>
			ParDTO<List<AtendimentoRelatorioDTO>,List<Integer>> parDTO = montaEstruturaParaGradeHorariaVisaoCurso(temInfoDeHorario,aulas,filtro.getTurnoDTO());
			// preenche o par resultante
			parResultante.getPrimeiro().addAll(parDTO.getPrimeiro());
			parResultante.setSegundo(parDTO.getSegundo());
		}
		
		List<AtendimentoRelatorioDTO> aulas = parResultante.getPrimeiro();
		if (!aulas.isEmpty()) {
			// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão e o máximo de 
			// créditos em um dia das semanas letivas relacionadas com as aulas em questão
			Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(parResultante.getPrimeiro());
			Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = dao.buscaSemanasLetivas();
			Set<Long> turnosIDsDasAulas = new HashSet<Long>();
			turnosIDsDasAulas.add(filtro.getTurnoDTO().getId());
			QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala,semanaLetivaIdToSemanaLetivaMap,temInfoDeHorario,turnosIDsDasAulas);
			ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
			List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
			List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
			List<String> horariosDeFimDeAula = quarteto.getQuarto();
			
			s = SextetoDTO.create(parResultante.getPrimeiro(),mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula,parResultante.getSegundo());
		} else {
			s = SextetoDTO.create(parResultante.getPrimeiro(),ParDTO.create(0, false),Collections.<String>emptyList(),Collections.<String>emptyList(),Collections.<String>emptyList(),parResultante.getSegundo());
		}
		
		return AtendimentoServiceRelatorioResponse.create(s);
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.AtendimentosService#getAtendimentosParaGradeHorariaVisaoCurso(com.gapso.web.trieda.shared.dtos.CurriculoDTO, java.lang.Integer, com.gapso.web.trieda.shared.dtos.TurnoDTO, com.gapso.web.trieda.shared.dtos.CampusDTO, com.gapso.web.trieda.shared.dtos.CursoDTO)
	 */
	@Override
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoCurso(RelatorioVisaoCursoFiltro filtro){
		return getAtendimentosParaGradeHorariaVisaoCurso(filtro,getDAO());
	}
	
	@Override
	public void saveConfirmacoes(CenarioDTO cenarioDTO, List< ConfirmacaoTurmaDTO > list)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		for (ConfirmacaoTurmaDTO confirmacao : list)
		{
			Disciplina disciplina = Disciplina.find(confirmacao.getDisciplinaId(), getInstituicaoEnsinoUser());
			Sala sala = Sala.find(confirmacao.getSalaId(), getInstituicaoEnsinoUser());
			List<AtendimentoOperacional> atendimentosOperacional = 
					AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, sala.getUnidade().getCampus(), confirmacao.getTurma(), null);
			for (AtendimentoOperacional atendimento : atendimentosOperacional)
			{
				atendimento.setConfirmada(confirmacao.getConfirmada());
				atendimento.merge();
			}
			List<AtendimentoTatico> atendimentosTatico = 
					AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, sala.getUnidade().getCampus(), confirmacao.getTurma());
			for (AtendimentoTatico atendimento : atendimentosTatico)
			{
				atendimento.setConfirmada(confirmacao.getConfirmada());
				atendimento.merge();
			}
		}
	}
	
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoProfessor(CenarioDTO cenarioDTO, RelatorioVisaoProfessorFiltro filtro, boolean isVisaoProfessor) throws TriedaException{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		QuintetoDTO<List<AtendimentoRelatorioDTO>,ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> q;
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		List<TrioDTO<String, Integer, Integer>> horariosDisponiveis = null;
		boolean temInfoDeHorario = true;
		boolean isAdmin = isAdministrador();
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		
		String professorNome = (filtro.getProfessorNome() == null ? "" : filtro.getProfessorNome());
		String professorCpf = (filtro.getProfessorCpf() == null ? "" : filtro.getProfessorCpf());

		try {
			Professor professor = (professorCpf.isEmpty() && professorNome.isEmpty()) ? null : Professor.findByNomeCpf(getInstituicaoEnsinoUser(), cenario, professorNome, professorCpf);
			ProfessorVirtual professorVirtual = ((filtro.getProfessorVirtualDTO() == null || professor != null) ? null : ProfessorVirtual.find(filtro.getProfessorVirtualDTO().getId(), getInstituicaoEnsinoUser() ) );
			List<AtendimentoOperacional> atendimentosOperacional = AtendimentoOperacional.getAtendimentosOperacional(getInstituicaoEnsinoUser(), isAdmin, professor, professorVirtual, null, isVisaoProfessor);
			List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacional);
			
			// processa os atendimentos do operacional e os transforma em aulas
			List<AtendimentoOperacionalDTO> aulasOperacional = extraiAulas(atendimentosOperacionalDTO);
			// insere as aulas do modo operacional na lista de atendimentos
			aulas.addAll(aulasOperacional);
			
			if(!aulas.isEmpty()){
				// trata compartilhamento de turmas entre cursos
				List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = uneAulasQuePodemSerCompartilhadas(aulas);
				// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão e o máximo de 
				// créditos em um dia das semanas letivas relacionadas com as aulas em questão
				Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(aulasComCompartilhamentos);
				
				List<AtendimentoOperacionalDTO> atendimentosOperacionais = new ArrayList<AtendimentoOperacionalDTO>();
				for(AtendimentoRelatorioDTO dto : aulasComCompartilhamentos){
					atendimentosOperacionais.add((AtendimentoOperacionalDTO) dto);
				}
				
				List<AtendimentoRelatorioDTO> atendimentosParaEscrita = new ArrayList<AtendimentoRelatorioDTO>();
				for(AtendimentoOperacionalDTO dto : this.ordenaPorHorarioAula(atendimentosOperacionais)){
					atendimentosParaEscrita.add((AtendimentoRelatorioDTO) dto);
				}
				Set<Long> turnosIDsDasAulas = new HashSet<Long>();
				for (AtendimentoRelatorioDTO aula : aulas)
				{
					turnosIDsDasAulas.add(aula.getTurnoId());
				}
				Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = buscaNoBancoDadosSemanasLetivas();
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, turnosIDsDasAulas);
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
				List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
				List<String> horariosDeFimDeAula = quarteto.getQuarto();
				if (professor != null)
				{
					horariosDisponiveis = new ArrayList<TrioDTO<String, Integer, Integer>>();
					for (Disponibilidade horario : professor.getDisponibilidades())
					{
						for (Semanas semana : horario.getDiasSemana())
						{
							String inicio = df.format( horario.getHorarioInicio() );
							horariosDisponiveis.add(TrioDTO.create(inicio, Semanas.toInt(semana),
									(int) TimeUnit.MILLISECONDS.toMinutes(horario.getHorarioFim().getTime() - horario.getHorarioInicio().getTime())));
						}
					}
				}
				q = QuintetoDTO.create(atendimentosParaEscrita,mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			}
			else q = QuintetoDTO.create(aulas,ParDTO.create(0, false),Collections.<String>emptyList(),Collections.<String>emptyList(),Collections.<String>emptyList());
		}
		catch (EmptyResultDataAccessException ex){
			throw new TriedaException("Os campos do digitados no filtro não foram encontrados");
		}
		
		return AtendimentoServiceRelatorioResponse.create(q, horariosDisponiveis);
	}
	
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoAluno(CenarioDTO cenarioDTO, RelatorioVisaoAlunoFiltro filtro, IAtendimentosServiceDAO dao) throws TriedaException{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<AtendimentoRelatorioDTO> atendimentos = new ArrayList<AtendimentoRelatorioDTO>();
		QuintetoDTO<List<AtendimentoRelatorioDTO>,ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> q;
		boolean temInfoDeHorario = true;

		// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
		try {
			List<AtendimentoTaticoDTO> atendimentosTaticoDTO = dao.buscaDTOsDeAtendimentoTatico(cenario, filtro.getAlunoNome(), filtro.getAlunoMatricula());
			if(!atendimentosTaticoDTO.isEmpty()){
				temInfoDeHorario = (atendimentosTaticoDTO.iterator().next().getHorarioAulaId() != null);
				// insere os atendimentos do modo tático na lista de atendimentos
				atendimentos.addAll(atendimentosTaticoDTO);
			}
			else{
				temInfoDeHorario = true;
				// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
				List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = dao.buscaDTOsDeAtendimentoOperacional(cenario, filtro.getAlunoNome(), filtro.getAlunoMatricula());
				// processa os atendimentos do operacional e os transforma em aulas
				List<AtendimentoOperacionalDTO> aulasOperacional = extraiAulas(atendimentosOperacionalDTO);
				//ordena os atendimentos
				ordenaPorHorarioAula(aulasOperacional);
				// insere as aulas do modo operacional na lista de atendimentos
				atendimentos.addAll(aulasOperacional);
			}
			
			if(!atendimentos.isEmpty()){
				// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão e o máximo de 
				// créditos em um dia das semanas letivas relacionadas com as aulas em questão
				Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(atendimentos);
				Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = dao.buscaSemanasLetivas();
				Set<Long> turnosIDsDasAulas = new HashSet<Long>();
				for (AtendimentoRelatorioDTO aula : atendimentos)
				{
					turnosIDsDasAulas.add(aula.getTurnoId());
				}
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, turnosIDsDasAulas);
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
				List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
				List<String> horariosDeFimDeAula = quarteto.getQuarto();
				
				q = QuintetoDTO.create(atendimentos,mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			}
			else q = QuintetoDTO.create(atendimentos,ParDTO.create(0, false),Collections.<String>emptyList(),Collections.<String>emptyList(),Collections.<String>emptyList());
		}
		catch (EmptyResultDataAccessException ex){
			throw new TriedaException("Os campos do digitados no filtro não foram encontrados");
		}
		return AtendimentoServiceRelatorioResponse.create(q);
	}
	
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoAluno(CenarioDTO cenarioDTO, RelatorioVisaoAlunoFiltro filtro) throws TriedaException{
		return getAtendimentosParaGradeHorariaVisaoAluno(cenarioDTO, filtro,getDAO());
	}
	
	/**
	 * Busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs.
	 * @param salaDTO sala
	 * @param turnoDTO turno
	 * @param semanaLetivaDTO semana letiva
	 * @return uma lista com DTOs que representam atendimentos do modo tático
	 */
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(SalaDTO salaDTO, TurnoDTO turnoDTO, SemanaLetivaDTO semanaLetivaDTO) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.find(salaDTO.getId(),getInstituicaoEnsinoUser());
		Turno turno = Turno.find(turnoDTO.getId(),getInstituicaoEnsinoUser());
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId(),getInstituicaoEnsinoUser());

		// busca no BD os atendimentos do modo tático
		List<AtendimentoTatico> atendimentosTaticoBD = AtendimentoTatico.findBySalaAndTurno(getInstituicaoEnsinoUser(),sala,turno,semanaLetiva);
		
		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoTaticoDTO(atendimentosTaticoBD);
	}
	
	/**
	 * Busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs.
	 * @param salaDTO sala
	 * @param turnoDTO turno
	 * @return uma lista com DTOs que representam atendimentos do modo tático
	 */
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(Cenario cenario, String salaCodigo) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.findByCodigo(getInstituicaoEnsinoUser(), cenario, salaCodigo);

		// busca no BD os atendimentos do modo tático
		List<AtendimentoTatico> atendimentosTaticoBD = AtendimentoTatico.findBySalaAndTurno(getInstituicaoEnsinoUser(), cenario, sala,null,null);
		
		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoTaticoDTO(atendimentosTaticoBD);
	}
	
	/**
	 * Busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs.
	 * @param curriculoDTO matriz curricular
	 * @param periodo período da matriz curricular
	 * @param turnoDTO turno
	 * @param campusDTO campus
	 * @param cursoDTO curso
	 * @return uma lista com DTOs que representam atendimentos do modo tático
	 */
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
		// obtém os beans de Banco de Dados
		Curriculo curriculo = Curriculo.find( curriculoDTO.getId(), getInstituicaoEnsinoUser() );
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );
		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );
		Curso curso = null;
		if (cursoDTO != null) {
			curso = Curso.find(cursoDTO.getId(),getInstituicaoEnsinoUser());
		}

		// busca no BD os atendimentos do modo tático
		List<AtendimentoTatico> atendimentosTaticoBD = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(),campus,curriculo,periodo,turno,curso);

		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoTaticoDTO(atendimentosTaticoBD);
	}
	
	/**
	 * Busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs.
	 * @param alunoDTO aluno
	 * @param turnoDTO turno
	 * @return uma lista com DTOs que representam atendimentos do modo tático
	 */
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(Cenario cenario, String alunoNome, String alunoMatricula) {
		// obtém os beans de Banco de Dados
		Aluno aluno = Aluno.findOneByNomeMatricula(getInstituicaoEnsinoUser(), cenario, alunoNome, alunoMatricula);

		// busca no BD os atendimentos do modo tático
		List<AtendimentoTatico> atendimentosTaticoBD = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), aluno, null, null);
		
		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoTaticoDTO(atendimentosTaticoBD);
	}
	
	/**
	 * Busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs.
	 * @param salaDTO sala
	 * @param turnoDTO turno
	 * @param semanaLetivaDTO semana letiva
	 * @return uma lista com DTOs que representam atendimentos do modo operacional
	 */
	public List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(SalaDTO salaDTO, TurnoDTO turnoDTO, SemanaLetivaDTO semanaLetivaDTO) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.find(salaDTO.getId(),getInstituicaoEnsinoUser());
		Turno turno = Turno.find(turnoDTO.getId(),getInstituicaoEnsinoUser());
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId(),getInstituicaoEnsinoUser());

		// busca no BD os atendimentos do modo operacional
		List<AtendimentoOperacional> atendimentosOperacionalBD = AtendimentoOperacional.findBySalaAndTurno(sala,turno,semanaLetiva,getInstituicaoEnsinoUser());

		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacionalBD);		
	}
	
	/**
	 * Busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs.
	 * @param salaDTO sala
	 * @param turnoDTO turno
	 * @return uma lista com DTOs que representam atendimentos do modo operacional
	 */
	public List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(Cenario cenario, String salaCodigo) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.findByCodigo(getInstituicaoEnsinoUser(), cenario, salaCodigo);

		// busca no BD os atendimentos do modo operacional
		List<AtendimentoOperacional> atendimentosOperacionalBD = AtendimentoOperacional.findBySalaAndTurno(sala,null,null,getInstituicaoEnsinoUser(), cenario);

		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacionalBD);		
	}
	
	/**
	 * Busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs.
	 * @param alunoDTO sala
	 * @param turnoDTO turno
	 * @return uma lista com DTOs que representam atendimentos do modo operacional
	 */
	public List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(Cenario cenario, String alunoNome, String alunoMatricula){
		// obtém os beans de Banco de Dados
		Aluno aluno = Aluno.findOneByNomeMatricula(getInstituicaoEnsinoUser(), cenario, alunoNome, alunoMatricula);

		// busca no BD os atendimentos do modo operacional
		List<AtendimentoOperacional> atendimentosOperacionalBD = AtendimentoOperacional.findBy(aluno, null, null, getInstituicaoEnsinoUser());
		
		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacionalBD);
	}
	
	/**
	 * Busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs.
	 * @param curriculoDTO matriz curricular
	 * @param periodo período da matriz curricular
	 * @param turnoDTO turno
	 * @param campusDTO campus
	 * @param cursoDTO curso
	 * @return uma lista com DTOs que representam atendimentos do modo operacional
	 */
	private List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
		// obtém os beans de Banco de Dados
		Curriculo curriculo = Curriculo.find(curriculoDTO.getId(),getInstituicaoEnsinoUser());
		Turno turno = Turno.find(turnoDTO.getId(),getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(),getInstituicaoEnsinoUser());
		Curso curso = null;
		if (cursoDTO != null) {
			curso = Curso.find(cursoDTO.getId(),getInstituicaoEnsinoUser());
		}

		// busca no BD os atendimentos do modo operacional 
		List<AtendimentoOperacional> atendimentosOperacionalBD = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(),campus,curriculo,periodo,turno,curso);
		
		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacionalBD);
	}
	
	private Map<Long,SemanaLetiva> buscaNoBancoDadosSemanasLetivas() {
		List<SemanaLetiva> todasSemanasLetivas = SemanaLetiva.findAll(getInstituicaoEnsinoUser());
		return SemanaLetiva.buildSemanaLetivaIDToSemanaLetivaMap(todasSemanasLetivas);
	}
	
	/*
	 * Esse método tem como objetivo agrupar DTO's que
	 * correspondam a créditos de uma mesma aula, que
	 * foram armazenadas como linhas distintas no banco de dados.
	 * */
	public List< AtendimentoRelatorioDTO > transformaAtendimentosPorHorarioEmAtendimentosPorAula(
		List< AtendimentoOperacionalDTO > list )
	{
		List< AtendimentoRelatorioDTO > ret = new ArrayList< AtendimentoRelatorioDTO >();

		// Agrupa os DTOS pela chave [ Curso - Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List<AtendimentoRelatorioDTO > > atendimentosDTOMap = new HashMap< String, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			String key = dto.getCursoNome() + "-" + dto.getDisciplinaString()
				+ "-" + dto.getTurma() + "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoRelatorioDTO > dtoList = atendimentosDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentosDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		for ( Entry< String, List< AtendimentoRelatorioDTO > > entry
			: atendimentosDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario
				= new ArrayList< AtendimentoOperacionalDTO >();

			for ( AtendimentoRelatorioDTO ar : entry.getValue() )
			{
				ordenadoPorHorario.add( (AtendimentoOperacionalDTO) ar );
			}

			ordenadoPorHorario = this.ordenaPorHorarioAula( ordenadoPorHorario );

			AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 );

			int count = 1;
			for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
			{
				AtendimentoOperacionalDTO h0 = ordenadoPorHorario.get( i - 1 );
				AtendimentoOperacionalDTO h1 = ordenadoPorHorario.get( i );

				if ( !h0.getHorarioAulaId().equals( h1.getHorarioAulaId() ) )
				{
					count++;
				}
			}

			dtoMain.setTotalCreditos( count );
			ret.add( dtoMain );
		}

		return ret;
	}
	
	/**
	 * Processa os atendimentos do operacional e os transforma em aulas. Uma aula compreende um ou mais atendimentos do operacional,
	 * mais especificamente, dois atendimentos pertencem a uma mesma aula sempre que:
	 *    - se referem ao mesmo turno, e
	 *    - se referem ao mesmo curso, e
	 *    - se referem à mesma disciplina, e
	 *    - se referem à mesma turma, e
	 *    - se referem à mesma sala de aula
	 *    - se referem ao mesmo dia da semana, e
	 *    - possuem uma relação de horários consecutivos
	 * @param atendimentosDTO lista de atendimentos do operacional obtidos do BD
	 * @return lista com as aulas
	 */
	public List<AtendimentoOperacionalDTO> extraiAulas(List<AtendimentoOperacionalDTO> atendimentosDTO) {
		// [Turno-Curso-Curriculo-Disciplina-Turma-DiaSemana-Sala -> List<AtendimentoOperacionalDTO>]
		Map<String,List<AtendimentoOperacionalDTO>> atendimentosAgrupadosMap = new HashMap<String,List<AtendimentoOperacionalDTO>>();
		// Agrupa os DTOS pela chave Turno-Curso-Curriculo-Disciplina-Turma-DiaSemana-Sala
		for (AtendimentoOperacionalDTO atendimento : atendimentosDTO) {			
			String key = atendimento.getTurnoString() + "-" + atendimento.getCursoString() + "-" + atendimento.getCurriculoString() + "-" + atendimento.getDisciplinaString() + "-" + atendimento.getTurma() + "-" + atendimento.getSemana() + "-" + atendimento.getSalaId();

			List<AtendimentoOperacionalDTO> grupoAtendimentos = atendimentosAgrupadosMap.get(key);
			if (grupoAtendimentos == null) {
				grupoAtendimentos = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentosAgrupadosMap.put(key,grupoAtendimentos);
			}
			grupoAtendimentos.add(atendimento);
		}
		
		// Quando há mais de um DTO por chave [Turno-Curso-Curriculo-Disciplina-Turma-DiaSemana-Sala], concatena as informações de todos em um único DTO.
		// Na prática, um grupo de atendimentos concentra os atendimentos relacionados a uma mesma aula. Por exemplo, para uma aula da
		// disciplina INF110, na segunda-feira, na sala 103, para a turma 1, do curso de Computação, de 07:00h às  08:40h, supondo que
		// 1 crédito seja equivalente a 50min de hora-aula teremos 2 atendimentos para representar esta aula, são eles:
		//    - INF110, Segunda-Feira, Sala 103, Turmar 1, Computação, 07:00h às 07:50h, 1 Crédito
		//    - INF110, Segunda-Feira, Sala 103, Turmar 1, Computação, 07:50h às 08:40h, 1 Crédito
		// Logo, o objetivo deste trecho de código é transformar os dois atendimentos acima em somente um atendimento, no caso:
        //	  - INF110, Segunda-Feira, Sala 103, Turmar 1, Computação, 07:00h às 08:40h, 2 Créditos
		List<AtendimentoOperacionalDTO> aulas = new ArrayList<AtendimentoOperacionalDTO>();
		for (Entry<String,List<AtendimentoOperacionalDTO>> entryAgrupamento : atendimentosAgrupadosMap.entrySet()) {
			// ordena os atendimentos de um mesmo grupo pelo horário de início
			List<AtendimentoOperacionalDTO> grupoAtendimentosOrdenadosPorHorario = ordenaPorHorarioAula(entryAgrupamento.getValue());

			// verifica se há ou não necessidade de consolidar vários horários em um só
			if (grupoAtendimentosOrdenadosPorHorario.size() == 1) {
				AtendimentoOperacionalDTO aula = grupoAtendimentosOrdenadosPorHorario.get(0);
				aula.setTotalCreditos(1);
				aulas.add(aula);
			}
			else {
				// processa um grupo de atendimentos ordenados e, se necessário, separa em subgrupos de atendimentos com horários consecutivos
				List<List<AtendimentoOperacionalDTO>> subgruposDeAtendimentosConsecutivos = separaAtendimentosNaoConsecutivos(grupoAtendimentosOrdenadosPorHorario);

				// transforma um subgrupo de atendimentos consecutivos em apenas um atendimento (na prática, uma aula)
				for (List<AtendimentoOperacionalDTO> subgrupoDeAtendimentosConsecutivos : subgruposDeAtendimentosConsecutivos) {
					AtendimentoOperacionalDTO aula = new AtendimentoOperacionalDTO(subgrupoDeAtendimentosConsecutivos.get(0));
					for (AtendimentoOperacionalDTO atendimentoConsecutivo : subgrupoDeAtendimentosConsecutivos)
					{
						aula.getIdsAtendimentosConcatenados().add(atendimentoConsecutivo.getId());
					}
					aula.setTotalCreditos(subgrupoDeAtendimentosConsecutivos.size());
					aulas.add(aula);
				}
			}
		}
		return aulas;
	}
	
	/**
	 * Se necessário, separa a lista de atendimentos atendimentosOrdenadosPorHorario em várias listas de forma que cada nova lista represente
	 * um grupo de atendimentos com horários consecutivos.
	 * @param atendimentosOrdenadosPorHorario lista de atendimentos do operacional ordenados pelo horário de início e pertencentes ao mesmo dia da semana
	 * @return listas de atendimentos operacionais, cada lista representa um grupo de atendimentos com horários consecutivos, isto é, 
	 * caso a lista atendimentosOrdenadosPorHorario do parâmetro de entrada seja composta pela sequência de atendimentos abaixo:
	 *    - 07:00 -> 07:50
	 *    - 07:50 -> 08:40
	 *    - 09:00 -> 09:50
	 * então, a lista original será dividida em 2 novas listas, isto é,
	 *    - Lista1: - 07:00 -> 07:50 / 07:50 -> 08:40
	 *    - Lista2: - 09:00 -> 09:50
	 */
	private List<List<AtendimentoOperacionalDTO>> separaAtendimentosNaoConsecutivos(List<AtendimentoOperacionalDTO> atendimentosOrdenadosPorHorario) {
		List<List<AtendimentoOperacionalDTO>> subgruposDeAtendimentosConsecutivos = new ArrayList<List<AtendimentoOperacionalDTO>>();
		if (!atendimentosOrdenadosPorHorario.isEmpty()) {
			subgruposDeAtendimentosConsecutivos.add(new ArrayList<AtendimentoOperacionalDTO>());
			subgruposDeAtendimentosConsecutivos.get(0).add(atendimentosOrdenadosPorHorario.get(0));
			
			for (int i = 1; i < atendimentosOrdenadosPorHorario.size(); i++) {
				AtendimentoOperacionalDTO aulaAnterior = atendimentosOrdenadosPorHorario.get(i-1);
				AtendimentoOperacionalDTO aulaAtual = atendimentosOrdenadosPorHorario.get(i);
				
				Pair<Calendar,Calendar> horariosAulaAnterior = extraiHorariosInicial_e_Final(aulaAnterior);
				Pair<Calendar,Calendar> horariosAulaAtual = extraiHorariosInicial_e_Final(aulaAtual);
				
				boolean disciplinasSubstitutasDiferentes = aulaAnterior.getDisciplinaSubstitutaString() != aulaAtual.getDisciplinaSubstitutaString();
	
				if (!saoConsecutivos(horariosAulaAnterior,horariosAulaAtual) || disciplinasSubstitutasDiferentes) {
					subgruposDeAtendimentosConsecutivos.add(new ArrayList<AtendimentoOperacionalDTO>());
				}
	
				subgruposDeAtendimentosConsecutivos.get(subgruposDeAtendimentosConsecutivos.size()-1).add(aulaAtual);
			}
		}

		return subgruposDeAtendimentosConsecutivos;
	}
	
	/**
	 * A partir de uma lista de aulas que deverão ser desenhadas na grade horária visão sala, este método identifica se há aulas
	 * que podem ser compartilhadas e, se necessário, agrupa em uma mesma aula todos as aulas que podem ser compartilhadas de
	 * forma que dentro de uma mesma sala existam alunos de cursos distintos assistindo aula de uma mesma disciplina. 
	 * @param aulas aulas que serão desenhadas na grade horária visão sala (ainda não compartilhadas)
	 * @return lista de aulas que serão desenhadas na grade horária visão sala e que já consideram compartilhamento
	 * de turmas entre cursos distintos
	 */
	public List<AtendimentoRelatorioDTO> uneAulasQuePodemSerCompartilhadas(List<AtendimentoRelatorioDTO> aulas) {
		// Agrupa os DTOS pela chave [ Disciplina - Turma - DiaSemana - Sala - HorárioInício (caso seja o operacional) ]
		Map<String,List<AtendimentoRelatorioDTO>> atendimentoTaticoDTOMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();

		for (AtendimentoRelatorioDTO aula : aulas) {
			String disciplinaInfo = (aula.getDisciplinaSubstitutaId() != null) ? aula.getDisciplinaSubstitutaString() : aula.getDisciplinaString();
			String key = disciplinaInfo + "-" + aula.getTurma() + "-" + aula.getSemana() + "-" + aula.getSalaString() + (aula.getHorarioAulaString() != null ? ("-" + aula.getHorarioAulaString()) : "");
			List<AtendimentoRelatorioDTO> aulasASeremCompartilhadas = atendimentoTaticoDTOMap.get(key);
			if (aulasASeremCompartilhadas == null) {
				aulasASeremCompartilhadas = new ArrayList<AtendimentoRelatorioDTO>();
				atendimentoTaticoDTOMap.put(key,aulasASeremCompartilhadas);
			}
			aulasASeremCompartilhadas.add(aula);
		}

		// Quando há mais de um DTO por chave [Disciplina-Turma-DiaSemana-Sala-HorárioInício(caso seja o operacional)],
		// concatena as informações de todos em um único DTO.
		List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = new ArrayList<AtendimentoRelatorioDTO>();
		for (Entry<String,List<AtendimentoRelatorioDTO>> entry : atendimentoTaticoDTOMap.entrySet()) {
			if (entry.getValue().size() == 1) {
				aulasComCompartilhamentos.addAll(entry.getValue());
			} else {
				AtendimentoRelatorioDTO aulaComCompartilhamento = entry.getValue().get(0);

				for (int i = 1; i < entry.getValue().size(); i++) {
					AtendimentoRelatorioDTO aula = entry.getValue().get(i);
					aulaComCompartilhamento.concatenateVisaoSala(aula);
				}
				aulasComCompartilhamentos.add(aulaComCompartilhamento);
			}
		}

		return aulasComCompartilhamentos;
	}
	
	/**
	 * A partir de uma lista de aulas que deverão ser desenhadas na grade horária visão curso, este método monta um par que contém:
	 *    - a lista das aulas que serão desenhadas, atualizadas com a correta coluna na qual serão desenhadas na grade horária
	 *    - a lista de quantidade de colunas existentes dentro de cada dia da semana 
	 * @param aulas aulas que serão desenhadas na grade horária visão curso
	 * @return um par com
	 *    - a lista das aulas que serão desenhadas, atualizadas com a correta coluna na qual serão desenhadas na grade horária
	 *    - a lista de quantidade de colunas existentes dentro de cada dia da semana
	 */
	private ParDTO<List<AtendimentoRelatorioDTO>,List<Integer>> montaEstruturaParaGradeHorariaVisaoCurso(boolean temInfoDeHorario, List<AtendimentoRelatorioDTO> aulas, TurnoDTO turnoDTO) {
		// [DiaSemana -> Aulas]
		Map<Integer,List<AtendimentoRelatorioDTO>> diaSemanaToAulasMap = new TreeMap<Integer,List<AtendimentoRelatorioDTO>>();
		// mapeia as aulas com o dia da semana em questão
		for (AtendimentoRelatorioDTO aula : aulas) {
			List<AtendimentoRelatorioDTO> aulasNoMesmoDiaSemana = diaSemanaToAulasMap.get(aula.getSemana());
			if (aulasNoMesmoDiaSemana == null) {
				aulasNoMesmoDiaSemana = new ArrayList<AtendimentoRelatorioDTO>();
				diaSemanaToAulasMap.put(aula.getSemana(),aulasNoMesmoDiaSemana);
			}

			aulasNoMesmoDiaSemana.add(aula);
		}

		// preenche as entradas nulas do mapa diaSemanaToAulasMap com uma lista vazia.
		// 1=DOM, 2=SEG, 3=TER, 4=QUA, 5=QUI, 6=SEX, 7=SAB
		for (int i = 2; i <= 7; i++) {
			if (diaSemanaToAulasMap.get(i) == null) {
				diaSemanaToAulasMap.put(i,Collections.<AtendimentoRelatorioDTO>emptyList());
			}
		}

		// Lista com a qtde de colunas de cada dia da semana. Cada posição da lista representa um dia
		// da semana (com exceção da posição 0 que não serve para nada), então:
		//    - posicão1 = DOM, posição 2 = SEG, ... , posição 7 = SAB
		// As vezes, num mesmo dia da semana, haverá aulas em paralelo (isto é, aulas que possuem interseção em seus horários)
		// logo, ao desenhá-las, será necessário mais de uma coluna para um mesmo dia da semana. O objetivo desta lista é armazenar
		// a qtde de colunas necessárias por dia da semana para que não haja sobreposição de aulas no desenho da grade horária.
		List<Integer> qtdColunasGradeHorariaPorDiaSemana = new ArrayList<Integer>(8);
		
		// inicializa a lista como se todo dia da semana tivesse apenas uma coluna
		Collections.addAll(qtdColunasGradeHorariaPorDiaSemana,1,1,1,1,1,1,1,1,1);

		List<AtendimentoRelatorioDTO> listaResultanteComAulas = new ArrayList<AtendimentoRelatorioDTO>();

		// para cada dia da semana, agrupa as aulas que ocorrerão em paralelo, atualiza a coluna em que cada aula
		// será desenhada na grade horária e atualiza a quantidade de colunas que serão necessárias em cada dia
		// da semana
		int colunaNaGradeHoraria = 2;
		for (Entry<Integer,List<AtendimentoRelatorioDTO>> entry : diaSemanaToAulasMap.entrySet()) {
			Integer diaSemana = entry.getKey();
			List<AtendimentoRelatorioDTO> aulasMesmoDiaSemana = entry.getValue();
			
			List<List<AtendimentoRelatorioDTO>> colunasAulas = temInfoDeHorario ? 
				agrupaAulasPorColunas(aulasMesmoDiaSemana) :
				agrupaAulasQueOcorreraoEmParalelo(turnoDTO,diaSemana,aulasMesmoDiaSemana);

			for (int j = 0; j < colunasAulas.size(); j++) {

				AtendimentoRelatorioDTO primeiraAulaParalela = colunasAulas.get(j).get(0);
				primeiraAulaParalela.setSemana(colunaNaGradeHoraria+j);
				for (int i = 1; i < colunasAulas.get(j).size(); i++) {
					AtendimentoRelatorioDTO proximaAulaParalela = colunasAulas.get(j).get(i);
					proximaAulaParalela.setSemana(primeiraAulaParalela.getSemana());
				}

				listaResultanteComAulas.addAll(colunasAulas.get(j));
			}
			
			if (!temInfoDeHorario && colunasAulas.isEmpty()) {
				for (AtendimentoRelatorioDTO aula : aulasMesmoDiaSemana) {
					aula.setSemana(colunaNaGradeHoraria);
				}
				listaResultanteComAulas.addAll(aulasMesmoDiaSemana);
			}
			qtdColunasGradeHorariaPorDiaSemana.add(diaSemana,colunasAulas.size() == 0 ? 1 : colunasAulas.size());
			colunaNaGradeHoraria += colunasAulas.size() == 0 ? 1 : colunasAulas.size();
		}

		return ParDTO.create(listaResultanteComAulas,qtdColunasGradeHorariaPorDiaSemana);
	}
	
	private List<List<AtendimentoRelatorioDTO>> agrupaAulasQueOcorreraoEmParalelo(TurnoDTO turnoDTO, Integer diaSemana, List<AtendimentoRelatorioDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoRelatorioDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoRelatorioDTO>>();
		
		// obtém os id's de todas as semanas letivas associadas com as aulas
		Set<Long> semanasLetivasIDsDasAulas = obtemIDsDasSemanasLetivasAssociadasComAsAulas(aulasMesmoDiaSemana);
		
		// obtem o numero maximo de creditos de um dia da semana, de acordo com o tipo de semana letiva associado a ele.
		int maxCreditosDiaSemana = 0;
		for (Long semanaLetivaId : semanasLetivasIDsDasAulas) {
			int maxLocal = turnoDTO.getMaxCreditos(semanaLetivaId,diaSemana);
			if (maxLocal > maxCreditosDiaSemana) {
				maxCreditosDiaSemana = maxLocal;
			}
		}

		// verifica se as aulas no dia da semana em questão extrapolam a quantidade máxima de créditos para o dia
		if (AtendimentoTaticoDTO.calculaTotalDeCreditos(aulasMesmoDiaSemana) > maxCreditosDiaSemana) {
			// identifica aulas paralelas pela abordagem 1
			gruposAulasParalelas = agrupaAulasQueOcorreraoEmParaleloAbordagem1(aulasMesmoDiaSemana);

			// verifica se o dia da semana continua extrapolando a quantidade máxima de créditos após a execução da abordagem 1
			if (AtendimentoTaticoDTO.calculaTotalDeCreditosDasAulas(gruposAulasParalelas) > maxCreditosDiaSemana) {
				// identifica aulas paralelas pela abordagem 2
				gruposAulasParalelas.clear();
				gruposAulasParalelas = agrupaAulasQueOcorreraoEmParaleloAbordagem2(aulasMesmoDiaSemana,maxCreditosDiaSemana);
			}
		}
		
		return gruposAulasParalelas;
	}

	private <T extends AtendimentoRelatorioDTO> Set<Long> obtemIDsDasSemanasLetivasAssociadasComAsAulas(List<T> aulas) {
		Set<Long> semanasLetivasIDsDasAulas = new HashSet<Long>();
		for (T aula : aulas) {
			if (aula.getDisciplinaSubstitutaSemanaLetivaId() != null) {
				semanasLetivasIDsDasAulas.add(aula.getDisciplinaSubstitutaSemanaLetivaId());
			} else {
				semanasLetivasIDsDasAulas.add(aula.getSemanaLetivaId());
			}
		}
		return semanasLetivasIDsDasAulas;
	}
	
	/**
	 * A partir de uma lista com as aulas que ocorrerão no mesmo dia da semana, identifica as aulas que deverão ocorrer em paralelo e agrupa
	 * as mesmas em listas. 
	 * @param aulasMesmoDiaSemana aulas no mesmo dia da semana
	 * @return listas de aulas de forma que cada lista representa aulas que ocorrerão em paralelo umas com as outras
	 */
	private List<List<AtendimentoRelatorioDTO>> agrupaAulasQueOcorreraoEmParaleloAbordagem1(List<AtendimentoRelatorioDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoRelatorioDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoRelatorioDTO>>();
		
		// ordena as aulas pelo número da turma
		List<AtendimentoRelatorioDTO> aulasOrdenadasPelaTurma = new ArrayList<AtendimentoRelatorioDTO>(aulasMesmoDiaSemana);
		Collections.sort(aulasOrdenadasPelaTurma, new Comparator<AtendimentoRelatorioDTO>() {
			@Override
			public int compare(AtendimentoRelatorioDTO o1, AtendimentoRelatorioDTO o2) {
				return o1.getTurma().compareTo(o2.getTurma());
			}
		});

		for (AtendimentoRelatorioDTO aulaAtual : aulasMesmoDiaSemana) {
			if (gruposAulasParalelas.isEmpty()) {
				gruposAulasParalelas.add(new ArrayList<AtendimentoRelatorioDTO>());
				gruposAulasParalelas.get(0).add(aulaAtual);
			} else {
				boolean paralelismoIdentificadoComAulaAtual = false;

				// verifica se há paralelismo entre a aula atual e alguma aula processada anteriormente
				for (List<AtendimentoRelatorioDTO> aulasParalelas : gruposAulasParalelas) {
					boolean naoSaoCandidatasAOcorreremEmParelelo = false;
					for (AtendimentoRelatorioDTO aulaProcessadaAnteriormente : aulasParalelas) {
						if (!AtendimentoTaticoDTO.podemOcorrerEmParaleloAbordagem1(aulaAtual,aulaProcessadaAnteriormente)) {
							naoSaoCandidatasAOcorreremEmParelelo = true;
							break;
						}
					}

					if (!naoSaoCandidatasAOcorreremEmParelelo) {
						aulasParalelas.add(aulaAtual);
						paralelismoIdentificadoComAulaAtual = true;
						break;
					}
				}

				if (!paralelismoIdentificadoComAulaAtual) {
					gruposAulasParalelas.add(new ArrayList<AtendimentoRelatorioDTO>());
					gruposAulasParalelas.get(gruposAulasParalelas.size() - 1).add(aulaAtual);
				}
			}
		}

		return gruposAulasParalelas;
	}
	
	/**
	 * A partir de uma lista com as aulas que ocorrerão no mesmo dia da semana, identifica as aulas que deverão ocorrer em paralelo e agrupa
	 * as mesmas em listas. 
	 * @param aulasMesmoDiaSemana aulas no mesmo dia da semana
	 * @return listas de aulas de forma que cada lista representa aulas que ocorrerão em paralelo umas com as outras
	 */
	private List<List<AtendimentoRelatorioDTO>> agrupaAulasQueOcorreraoEmParaleloAbordagem2(List<AtendimentoRelatorioDTO> aulasMesmoDiaSemana, int maxCreditosDiaSemana) {
		List<List<AtendimentoRelatorioDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoRelatorioDTO>>();

		// ordena as aulas pelo número da turma
		List<AtendimentoRelatorioDTO> aulasOrdenadasPelaTurma = new ArrayList<AtendimentoRelatorioDTO>(aulasMesmoDiaSemana);
		Collections.sort(aulasOrdenadasPelaTurma, new Comparator<AtendimentoRelatorioDTO>() {
			@Override
			public int compare(AtendimentoRelatorioDTO o1, AtendimentoRelatorioDTO o2) {
				return -o1.getTotalCreditos().compareTo(o2.getTotalCreditos());
			}
		});

		for (AtendimentoRelatorioDTO aulaAtual : aulasOrdenadasPelaTurma) {
			if (gruposAulasParalelas.isEmpty()) {
				gruposAulasParalelas.add(new ArrayList<AtendimentoRelatorioDTO>());
				gruposAulasParalelas.get(0).add(aulaAtual);
			} else {
				boolean paralelismoIdentificadoComAulaAtual = false;
				
				// verifica se há paralelismo entre a aula atual e alguma aula processada anteriormente
				for (List<AtendimentoRelatorioDTO> aulasParalelas : gruposAulasParalelas) {
					boolean naoSaoCandidatasAOcorreremEmParelelo = false;
					for (AtendimentoRelatorioDTO aulaProcessadaAnteriormente : aulasParalelas) {
						if (!AtendimentoTaticoDTO.podemOcorrerEmParaleloAbordagem1(aulaAtual,aulaProcessadaAnteriormente) &&
							!AtendimentoTaticoDTO.podemOcorrerEmParaleloAbordagem2(aulaAtual,aulaProcessadaAnteriormente,maxCreditosDiaSemana)) {
							naoSaoCandidatasAOcorreremEmParelelo = true;
							break;
						}
					}

					if (!naoSaoCandidatasAOcorreremEmParelelo) {
						aulasParalelas.add(aulaAtual);
						paralelismoIdentificadoComAulaAtual = true;
						break;
					}
				}

				if (!paralelismoIdentificadoComAulaAtual) {
					gruposAulasParalelas.add(new ArrayList<AtendimentoRelatorioDTO>());
					gruposAulasParalelas.get(gruposAulasParalelas.size() - 1).add(aulaAtual);
				}
			}
		}

		return gruposAulasParalelas;
	}
	
	/**
	 * A partir de uma lista com as aulas que ocorrerão no mesmo dia da semana, identifica as aulas que deverão ocorrer em paralelo e agrupa
	 * as mesmas em listas. 
	 * @param aulasMesmoDiaSemana aulas no mesmo dia da semana
	 * @return listas de aulas de forma que cada lista representa aulas que ocorrerão em paralelo umas com as outras
	 */
	private List<List<AtendimentoRelatorioDTO>> agrupaAulasPorColunas(List<AtendimentoRelatorioDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoRelatorioDTO>> gruposAulasColunas = new ArrayList<List<AtendimentoRelatorioDTO>>();
		
		// ordena as aulas pela turma para garantir que o paralelismo viável seja construído corretamente
		Collections.sort(aulasMesmoDiaSemana,new Comparator<AtendimentoRelatorioDTO>() {
			@Override
			public int compare(AtendimentoRelatorioDTO o1, AtendimentoRelatorioDTO o2) {
				if( o1.getDisciplinaNome().compareTo(o2.getDisciplinaNome()) == 0 )
					return o1.getTurma().compareTo(o2.getTurma());
				else
					return o1.getDisciplinaNome().compareTo(o2.getDisciplinaNome());
			}
		});

		for (AtendimentoRelatorioDTO aulaAtual : aulasMesmoDiaSemana) {
			if (gruposAulasColunas.isEmpty()) {
				gruposAulasColunas.add(new ArrayList<AtendimentoRelatorioDTO>());
				gruposAulasColunas.get(0).add(aulaAtual);
			}
			else {
				boolean encontrouIntersecao = false;

				for (int i = 0; i < gruposAulasColunas.size(); i++) {
					encontrouIntersecao = false;
					for (int j = 0; j < gruposAulasColunas.get(i).size(); j++) {
						if (temIntersecao(aulaAtual,gruposAulasColunas.get(i).get(j))) {
							encontrouIntersecao = true;
						}
					}
					if (!encontrouIntersecao) {
						gruposAulasColunas.get(i).add(aulaAtual);
						break;
					}
				}

				if (encontrouIntersecao) {
					gruposAulasColunas.add(new ArrayList<AtendimentoRelatorioDTO>());
					gruposAulasColunas.get(gruposAulasColunas.size()-1).add(aulaAtual);
				}
			}
		}
		return gruposAulasColunas;
	}

	@Override
	public PagingLoadResult< AtendimentoTaticoDTO > getList()
	{
		List< AtendimentoTaticoDTO > list
			= new ArrayList< AtendimentoTaticoDTO >();

		List< AtendimentoTatico > atendimentosTatico
			= AtendimentoTatico.findAll( getInstituicaoEnsinoUser() );

		for ( AtendimentoTatico atendimentoTatico : atendimentosTatico )
		{
			list.add( ConvertBeans.toAtendimentoTaticoDTO( atendimentoTatico ) );
		}

		BasePagingLoadResult< AtendimentoTaticoDTO > result
			= new BasePagingLoadResult< AtendimentoTaticoDTO >( list );

		result.setOffset( 0 );
		result.setTotalLength( 100 );

		return result;
	}

	private boolean temIntersecao(AtendimentoRelatorioDTO dto1, AtendimentoRelatorioDTO dto2) {
		if (dto1.getSemana().equals(dto2.getSemana())) {
			if (dto1.getHorarioAulaId().equals(dto2.getHorarioAulaId())) {
				return true;
			} else {
				Pair<Calendar,Calendar> horarios1 = extraiHorariosInicial_e_Final(dto1);
				Pair<Calendar,Calendar> horarios2 = extraiHorariosInicial_e_Final(dto2);
				return temIntersecao(horarios1,horarios2);
			}
		}
		return false;
	}
	
	private Pair<Calendar,Calendar> extraiHorariosInicial_e_Final(AtendimentoRelatorioDTO dto) {
		Calendar horarioInicial = Calendar.getInstance();
		String[] horarioInicialArray = dto.getHorarioAulaString().split(":");
		int horarioInicialHoras = Integer.parseInt(horarioInicialArray[0]);
		int horarioInicialMinutos = Integer.parseInt(horarioInicialArray[1]);
		horarioInicial.clear();
		horarioInicial.set(2012,Calendar.FEBRUARY,1,horarioInicialHoras,horarioInicialMinutos,0);
		horarioInicial.set(Calendar.MILLISECOND,0);
		
		Calendar horarioFinal = Calendar.getInstance();
		horarioFinal.setTime(horarioInicial.getTime());
		horarioFinal.add(Calendar.MINUTE,dto.getTotalCreditos()*dto.getDuracaoDeUmaAulaEmMinutos());
		
		return Pair.create(horarioInicial,horarioFinal);
	}

	private boolean temIntersecao(Calendar horarioInicial1, Calendar horarioFinal1, Calendar horarioInicial2, Calendar horarioFinal2) {
		return (horarioInicial1.before(horarioInicial2) && horarioInicial2.before(horarioFinal1)) || // hi1 < hi2 < hf1
		       (horarioInicial1.before(horarioFinal2)   && horarioFinal2.before(horarioFinal1)) || // hi1 < hf2 < hf1
		       (horarioInicial2.before(horarioInicial1) && horarioInicial1.before(horarioFinal2)) || // hi2 < hi1 < hf2
		       (horarioInicial2.before(horarioFinal1)   && horarioFinal1.before(horarioFinal2)); // hi2 < hf1 < hf2
	}
	
	private boolean temIntersecao(Pair<Calendar,Calendar> par1, Pair<Calendar,Calendar> par2) {
		return temIntersecao(par1.getLeft(),par1.getRight(),par2.getLeft(),par2.getRight());
	}
	
	private boolean saoConsecutivos(Pair<Calendar,Calendar> par1, Pair<Calendar,Calendar> par2) {
		return par1.getRight().equals(par2.getLeft()) || par2.getRight().equals(par1.getLeft());
	}

	public List< AtendimentoOperacionalDTO > montaListaParaVisaoProfessor(
		List< AtendimentoOperacionalDTO > list )
	{
		// Agrupa os DTOS pela chave [ Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List< AtendimentoOperacionalDTO > > atendimentoOperacionalDTOMap
			= new HashMap< String, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO dto : list )
		{
			String key = dto.getDisciplinaString() + "-" + dto.getTurma()
				+ "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoOperacionalDTO > dtoList
				= atendimentoOperacionalDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentoOperacionalDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave
		// [ Disciplina - Turma - DiaSemana - Sala ],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoOperacionalDTO > processedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( Entry< String, List< AtendimentoOperacionalDTO > > entry
			: atendimentoOperacionalDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario = null;
			
			if ( entry.getValue().size() == 1 )
			{
				ordenadoPorHorario = new ArrayList< AtendimentoOperacionalDTO >( entry.getValue() );
			}
			else
			{
				ordenadoPorHorario = this.ordenaPorHorarioAula( entry.getValue() );
			}

			if ( ordenadoPorHorario.size() == 1 )
			{
				AtendimentoOperacionalDTO dto = ordenadoPorHorario.get( 0 );
				dto.setTotalCreditos( 1 );
				processedList.add( dto );
			}
			else
			{
				AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 );

				// Procura pelo horário correspondente ao início da aula
				HorarioAula menorHorario = AtendimentoOperacional.retornaAtendimentoMenorHorarioAula(
					ConvertBeans.toListAtendimentoOperacional( ordenadoPorHorario ) );

				for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
				{
					AtendimentoOperacionalDTO dtoCurrent = ordenadoPorHorario.get( i );
					dtoMain.concatenateVisaoProfessor( dtoCurrent );
				}

				dtoMain.setHorarioAulaId( menorHorario.getId() );
				dtoMain.setHorarioAulaString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
				dtoMain.setTotalCreditos( ordenadoPorHorario.size() );

				processedList.add( dtoMain );
			}
		}

		return processedList;
	}

	@Override
	public ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais(CenarioDTO cenarioDTO)
	{
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< ProfessorVirtual > professoresVirtuais
			= ProfessorVirtual.findBy( getInstituicaoEnsinoUser(), cenario );

		List< ProfessorVirtualDTO > professoresVirtuaisDTO
			= new ArrayList< ProfessorVirtualDTO >();

		for ( ProfessorVirtual professorVirtual : professoresVirtuais )
		{
			professoresVirtuaisDTO.add(
				ConvertBeans.toProfessorVirtualDTO( professorVirtual ) );
		}

		return new BaseListLoadResult< ProfessorVirtualDTO >( professoresVirtuaisDTO );
	}
	
	@Override
	public PagingLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CenarioDTO cenarioDTO, TitulacaoDTO titulacaoDTO,
			 TipoContratoDTO tipoContratoDTO, AreaTitulacaoDTO areaTitulacaoDTO,String nome, 
			PagingLoadConfig config ) throws TriedaException
	{
		Titulacao titulacao = titulacaoDTO == null ? null : Titulacao.find(titulacaoDTO.getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = areaTitulacaoDTO == null ? null : AreaTitulacao.find(areaTitulacaoDTO.getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = tipoContratoDTO == null ? null : TipoContrato.find(tipoContratoDTO.getId(), getInstituicaoEnsinoUser());
		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		String orderBy = config.getSortField();
		
		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
					&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}
		
		List< ProfessorVirtual > professoresVirtuais
			= ProfessorVirtual.findBy( getInstituicaoEnsinoUser(), cenario, titulacao, tipoContrato, areaTitulacao, nome,  orderBy );

		List< ProfessorVirtualDTO > professoresVirtuaisDTO
			= new ArrayList< ProfessorVirtualDTO >();

		for ( ProfessorVirtual professorVirtual : professoresVirtuais )
		{
			professoresVirtuaisDTO.add(
				ConvertBeans.toProfessorVirtualDTO( professorVirtual ) );
		}

		BasePagingLoadResult< ProfessorVirtualDTO > result
		= new BasePagingLoadResult< ProfessorVirtualDTO >( professoresVirtuaisDTO );

		result.setOffset( config.getOffset() );

		result.setTotalLength( ProfessorVirtual.findBy(
				getInstituicaoEnsinoUser(), cenario, titulacao, tipoContrato, areaTitulacao, nome,  orderBy ).size() );
		
		return result;
	}
	
	@Override
	public PagingLoadResult<ProfessorVirtualDTO> getProfessoresVirtuais(
			CenarioDTO cenarioDTO, TitulacaoDTO titulacaoDTO,
			 TipoContratoDTO tipoContratoDTO, AreaTitulacaoDTO areaTitulacaoDTO, String nome, 
			Long campusId, PagingLoadConfig config) {
		Titulacao titulacao = titulacaoDTO == null ? null : Titulacao.find(titulacaoDTO.getId(), getInstituicaoEnsinoUser());
		AreaTitulacao areaTitulacao = areaTitulacaoDTO == null ? null : AreaTitulacao.find(areaTitulacaoDTO.getId(), getInstituicaoEnsinoUser());
		TipoContrato tipoContrato = tipoContratoDTO == null ? null : TipoContrato.find(tipoContratoDTO.getId(), getInstituicaoEnsinoUser());
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusId, getInstituicaoEnsinoUser());
		
		String orderBy = config.getSortField();
		
		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
					&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}
		
		List< ProfessorVirtual > professoresVirtuais
			= ProfessorVirtual.findBy( getInstituicaoEnsinoUser(), cenario, titulacao, tipoContrato, areaTitulacao, nome, campus, orderBy );

		List< ProfessorVirtualDTO > professoresVirtuaisDTO
			= new ArrayList< ProfessorVirtualDTO >();

		for ( ProfessorVirtual professorVirtual : professoresVirtuais )
		{
			professoresVirtuaisDTO.add(
				ConvertBeans.toProfessorVirtualDTO( professorVirtual ) );
		}

		BasePagingLoadResult< ProfessorVirtualDTO > result
		= new BasePagingLoadResult< ProfessorVirtualDTO >( professoresVirtuaisDTO );

		result.setOffset( config.getOffset() );

		result.setTotalLength( ProfessorVirtual.findBy(
				getInstituicaoEnsinoUser(), cenario, titulacao, tipoContrato, areaTitulacao, nome,  orderBy ).size() );
		
		return result;
	}

	public int deslocarLinhasExportExcel(
		InstituicaoEnsino instituicaoEnsino,
		List< AtendimentoOperacionalDTO > atendimentosDia )
	{
		if ( atendimentosDia == null || atendimentosDia.size() == 0 )
		{
			return 0;
		}

		Turno turno = Turno.find(
			atendimentosDia.get( 0 ).getTurnoId() , instituicaoEnsino );

		final List< HorarioAula > listAll
			= HorarioAula.findByTurno( instituicaoEnsino, turno );

		final Map< Long, HorarioAula > mapHorarios
			= HorarioAula.buildHorarioAulaIdToHorarioAulaMap( listAll );

		// Procura pelo menor horário de
		// início de aula entre os atendimentos do dia
		HorarioAula menorHorario = mapHorarios.get(
			atendimentosDia.get( 0 ).getHorarioAulaId() );

		for ( int i = 1; i < atendimentosDia.size(); i++ )
		{
			AtendimentoOperacionalDTO atDTO = atendimentosDia.get( i );
			HorarioAula ha = mapHorarios.get( atDTO.getHorarioAulaId() );

			if ( ha.getHorario().compareTo( menorHorario.getHorario() ) < 0 )
			{
				menorHorario = ha;
			}
		}
		////

		// Ordenamos os atendimentos por horário de início da aula
		List< HorarioAula > horariosOrdenados
			= new ArrayList< HorarioAula >( listAll );

		Collections.sort( horariosOrdenados,
			new Comparator< HorarioAula >()
		{
			@Override
			public int compare( HorarioAula arg1,
								HorarioAula arg2 )
			{
				return arg1.getHorario().compareTo( arg2.getHorario() );
			}
		});
		////

		// Procuramos a posição do primeiro horário de aula do dia
		// em relação a todos os horários de aula cadastrados na base de dados.
		// Com isso, sabemos quantas colunas da planilha excel deverão ser
		// deixadas em branco antes de se preencher o primeiro atendimento do dia.
		for ( int index = 0; index < horariosOrdenados.size(); index++ )
		{
			HorarioAula ha = horariosOrdenados.get( index );

			if ( ha.getId() == menorHorario.getId() )
			{
				return index;
			}
		}

		return 0;
	}
	
	@Override
	public List<PercentMestresDoutoresDTO> getPercentMestresDoutoresList( CenarioDTO cenarioDTO, CampusDTO campusDTO ) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = null;
		boolean ehTatico = false;
		List< PercentMestresDoutoresDTO > result = new ArrayList< PercentMestresDoutoresDTO >();
		Map<Curso, Map<String, String>> cursoToProfessoresMap = new HashMap<Curso, Map<String, String>>(); 
		List<AtendimentoOperacional> atendimentoOperacionalList;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
		}
		
		List<Campus> listCampus = new ArrayList<Campus>();
		if ( campus.getNome().equals("TODOS") )
		{
			for (Campus campiOtimizados : Campus.findAllOtimized(getInstituicaoEnsinoUser(), cenario) )
			{
				if(	campiOtimizados.isOtimizadoOperacional(getInstituicaoEnsinoUser()) )
				{
					listCampus.add(campiOtimizados);
				}
			}
		}
		else
		{
			if ( campus.isOtimizado(getInstituicaoEnsinoUser()) )
			{
				ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			}
		}
		
		if ( !ehTatico )
		{
			if ( campus.getNome().equals("TODOS") )
			{
				atendimentoOperacionalList = AtendimentoOperacional.findAllByCampi(getInstituicaoEnsinoUser(), cenario, listCampus);
			}
			else
			{
				atendimentoOperacionalList = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
			}
			for ( AtendimentoOperacional atendimento : atendimentoOperacionalList )
			{
				if ( cursoToProfessoresMap.get(atendimento.getOferta().getCurso()) == null )
				{
					Map<String, String> professoresTitulacao = new HashMap<String, String>();
					if ( atendimento.getProfessor().getId() != null )
					{
						professoresTitulacao.put(atendimento.getProfessor().getNome(), atendimento.getProfessor().getTitulacao().getNome());
					}
					else
					{
						professoresTitulacao.put(atendimento.getProfessorVirtual().getNome(), atendimento.getProfessorVirtual().getTitulacao().getNome());
					}
					cursoToProfessoresMap.put( atendimento.getOferta().getCurso(), professoresTitulacao );
				}
				else
				{
					if ( atendimento.getProfessor().getId() != null )
					{
						if ( cursoToProfessoresMap.get(atendimento.getOferta().getCurso()).get(atendimento.getProfessor().getNome()) != null);
							cursoToProfessoresMap.get(atendimento.getOferta().getCurso()).put(atendimento.getProfessor().getNome(), atendimento.getProfessor().getTitulacao().getNome());
					}
					else
					{
						if ( cursoToProfessoresMap.get(atendimento.getOferta().getCurso()).get(atendimento.getProfessorVirtual().getNome()) != null);
							cursoToProfessoresMap.get(atendimento.getOferta().getCurso()).put(atendimento.getProfessorVirtual().getNome(), atendimento.getProfessorVirtual().getTitulacao().getNome());
					}					
					cursoToProfessoresMap.get(atendimento.getOferta().getCurso());
				}
			}
		}

		for ( Curso curso : cursoToProfessoresMap.keySet() ) {
			int doutores = 0;
			int mestres = 0;
			int outros = 0;
			int total = 0;
			double doutoresPercent = 0.0;
			double mestresDoutoresPercent = 0.0;
			
			PercentMestresDoutoresDTO percentMestresDoutoresDTO = new PercentMestresDoutoresDTO();
			percentMestresDoutoresDTO.setCampusString(campus.getNome());
			percentMestresDoutoresDTO.setCursoString(curso.getNome()+" ("+curso.getCodigo()+") ");
			
			for ( String professor : cursoToProfessoresMap.get(curso).values() )
			{
				if ( professor.contains("Doutor") )
				{
					doutores++;
				}
				else if ( professor.contains("Mestre") )
				{
					mestres++;
				}
				else
				{
					outros++;
				}
			}
			total = doutores + mestres + outros;
			doutoresPercent = ( (double) doutores ) / total;
			mestresDoutoresPercent = ( (double) mestres+doutores ) / total;
			
			DecimalFormat dec = new DecimalFormat("#0.00");
			
			percentMestresDoutoresDTO.setDoutores(doutores);
			percentMestresDoutoresDTO.setMestres(mestres);
			percentMestresDoutoresDTO.setOutros(outros);
			percentMestresDoutoresDTO.setTotal(total);
			percentMestresDoutoresDTO.setDoutoresPercent(dec.format(TriedaUtil.round(doutoresPercent*100.0,2))+"%");
			percentMestresDoutoresDTO.setMestresDoutoresPercent(dec.format(TriedaUtil.round(mestresDoutoresPercent*100.0,2))+"%");
			percentMestresDoutoresDTO.setDoutoresMin(dec.format((double)curso.getNumMinDoutores()) + "%");
			percentMestresDoutoresDTO.setMestresDoutoresMin(dec.format((double)curso.getNumMinMestres()) + "%");

			result.add(percentMestresDoutoresDTO);
		}
		return result;
	}
	
	@Override
	public List<AtendimentoFaixaCreditoDTO> getAtendimentosFaixaCredito(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<AlunoDemanda> alunosDemandaList = AlunoDemanda.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);
		
		Map<Aluno, List<AlunoDemanda>> alunoToListAlunoDemandaMap = new HashMap<Aluno, List<AlunoDemanda>>();
		for (AlunoDemanda alunoDemanda : alunosDemandaList)
		{
			if (alunoToListAlunoDemandaMap.get(alunoDemanda.getAluno()) == null)
			{
				List<AlunoDemanda> novoAlunoDemandaList = new ArrayList<AlunoDemanda>();
				novoAlunoDemandaList.add(alunoDemanda);
				alunoToListAlunoDemandaMap.put(alunoDemanda.getAluno(), novoAlunoDemandaList);
			}
			else
			{
				alunoToListAlunoDemandaMap.get(alunoDemanda.getAluno()).add(alunoDemanda);
			}
		}
		
		List<AtendimentoFaixaCreditoDTO> faixasCredito = new ArrayList<AtendimentoFaixaCreditoDTO>();
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("100% dos créditos atendidos"));
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("Entre 80 e 99% dos créditos atendidos"));
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("Entre 60 e 79% dos créditos atendidos"));
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("Entre 40 e 59% dos créditos atendidos"));
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("Entre 20 e 39% dos créditos atendidos"));
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("Até 19% dos créditos atendidos"));
		faixasCredito.add(new AtendimentoFaixaCreditoDTO("Nenhum crédito atendido"));
		
		for(AtendimentoFaixaCreditoDTO faixa : faixasCredito){
			faixa.setCampusNome(campusDTO.getCodigo());
		}
		
		for(Entry<Aluno, List<AlunoDemanda>> aluno : alunoToListAlunoDemandaMap.entrySet())
		{
			int totalCreditosP1 = 0;
			int totalCreditosP2 = 0;
			int creditosAtendidos = 0;
			for (AlunoDemanda alunoDemanda : aluno.getValue())
			{
				if (alunoDemanda.getPrioridade() == 1)
				{
					totalCreditosP1 += alunoDemanda.getDemanda().getDisciplina().getTotalCreditos();
				}
				else if (alunoDemanda.getPrioridade() == 2)
				{
					totalCreditosP2 += alunoDemanda.getDemanda().getDisciplina().getTotalCreditos();
				}
				if(alunoDemanda.getAtendido())
				{
					creditosAtendidos += alunoDemanda.getDemanda().getDisciplina().getTotalCreditos();
				}
			}
			double creditosAtendidosPercent = ((double) creditosAtendidos)/ (totalCreditosP1 + totalCreditosP2);
			
			if (creditosAtendidosPercent == 1)
			{
				insereNaFaixaDeCredito(faixasCredito.get(0), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
			else if (creditosAtendidosPercent < 1 && creditosAtendidosPercent >= 0.8)
			{
				insereNaFaixaDeCredito(faixasCredito.get(1), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
			else if (creditosAtendidosPercent < 0.8 && creditosAtendidosPercent >= 0.6)
			{
				insereNaFaixaDeCredito(faixasCredito.get(2), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
			else if (creditosAtendidosPercent < 0.6 && creditosAtendidosPercent >= 0.4)
			{
				insereNaFaixaDeCredito(faixasCredito.get(3), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
			else if (creditosAtendidosPercent < 0.4 && creditosAtendidosPercent >= 0.2)
			{
				insereNaFaixaDeCredito(faixasCredito.get(4), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
			else if (creditosAtendidosPercent < 0.2 && creditosAtendidosPercent > 0)
			{
				insereNaFaixaDeCredito(faixasCredito.get(5), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
			else if (creditosAtendidosPercent == 0)
			{
				insereNaFaixaDeCredito(faixasCredito.get(6), totalCreditosP1, totalCreditosP2, creditosAtendidos );
			}
		}
		return faixasCredito;
	}
	
	private void insereNaFaixaDeCredito(AtendimentoFaixaCreditoDTO atendimentoFaixaCredito,	int totalCreditosP1,
			int totalCreditosP2, int creditosAtendidos)
	{
		atendimentoFaixaCredito.setCreditosDemandadosP1(atendimentoFaixaCredito.getCreditosDemandadosP1() + totalCreditosP1);
		atendimentoFaixaCredito.setCreditosAtendidosP1(atendimentoFaixaCredito.getCreditosAtendidosP1() + creditosAtendidos);
		atendimentoFaixaCredito.setQuantidadeAlunos(atendimentoFaixaCredito.getQuantidadeAlunos() + 1);
		atendimentoFaixaCredito.setCreditosDemandadosP2(atendimentoFaixaCredito.getCreditosDemandadosP2() + totalCreditosP2);
	}
	
	@Override
	public List<AtendimentoFaixaCreditoDTO> getAtendimentosFaixaDisciplina(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<AlunoDemanda> alunosDemandaList = AlunoDemanda.findByCampus(getInstituicaoEnsinoUser(), cenario, campus);		
		Map<Aluno, List<AlunoDemanda>> alunoToListAlunoDemandaMap = new HashMap<Aluno, List<AlunoDemanda>>();
		for (AlunoDemanda alunoDemanda : alunosDemandaList)
		{
			if (alunoToListAlunoDemandaMap.get(alunoDemanda.getAluno()) == null)
			{
				List<AlunoDemanda> novoAlunoDemandaList = new ArrayList<AlunoDemanda>();
				novoAlunoDemandaList.add(alunoDemanda);
				alunoToListAlunoDemandaMap.put(alunoDemanda.getAluno(), novoAlunoDemandaList);
			}
			else
			{
				alunoToListAlunoDemandaMap.get(alunoDemanda.getAluno()).add(alunoDemanda);
			}
		}
		
		List<AtendimentoFaixaCreditoDTO> faixasCredito = new ArrayList<AtendimentoFaixaCreditoDTO>();
		for(Entry<Aluno, List<AlunoDemanda>> aluno : alunoToListAlunoDemandaMap.entrySet())
		{
			int disciplinasNaoAtendidasP1 = 0;
			int disciplinasNaoAtendidasP2 = 0;
			for (AlunoDemanda alunoDemanda : aluno.getValue())
			{

				if(!alunoDemanda.getAtendido())
				{
					if (alunoDemanda.getPrioridade() == 1)
					{
						disciplinasNaoAtendidasP1++;
					}
					else if (alunoDemanda.getPrioridade() == 2)
					{
						disciplinasNaoAtendidasP2++;
					}
				}
			}
			if (faixasCredito.size() <= disciplinasNaoAtendidasP1)
			{
				for (int i = faixasCredito.size(); i <= disciplinasNaoAtendidasP1; i++)
				{
					String titulo = "";
					if (i == 0)
					{
						titulo = "Todas as disciplinas atendidas";
					}
					else
					{
						titulo = i + " disciplina" + ((i != 1) ? "s " : "") + " não atendida" + ((i != 1) ? "s" : "");
					}
					AtendimentoFaixaCreditoDTO novaFaixa = new AtendimentoFaixaCreditoDTO(titulo);
					novaFaixa.setCampusNome(campus.getCodigo());
					faixasCredito.add(novaFaixa);
				}
			}
			faixasCredito.get(disciplinasNaoAtendidasP1).setCreditosDemandadosP1(faixasCredito.get(disciplinasNaoAtendidasP1).getCreditosDemandadosP1() + 1);
			if (disciplinasNaoAtendidasP2 > 0)
			{
				faixasCredito.get(disciplinasNaoAtendidasP1).setCreditosDemandadosP2(faixasCredito.get(disciplinasNaoAtendidasP1).getCreditosDemandadosP2() + 1);
			}
		}
		return faixasCredito;
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getProfessoresJanelasGrade(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentosList = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
		
		Map<TriedaPar<String,Boolean>, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<TriedaPar<String,Boolean>, Set<HorarioDisponivelCenario>>();

		for (AtendimentoOperacional atendimento : atendimentosList)
		{
			TriedaPar<String, Boolean> professor;
			if (atendimento.getProfessorVirtual() != null)
			{
				professor = TriedaPar.create(atendimento.getProfessorVirtual().getNome(), true);
			}
			else
			{
				professor = TriedaPar.create(atendimento.getProfessor().getNome(), false);
			}
			if (professorToListHorarioDisponivelCenario.get(professor) == null)
			{
				Set<HorarioDisponivelCenario> novoHorarioList = new HashSet<HorarioDisponivelCenario>();
				novoHorarioList.add(atendimento.getHorarioDisponivelCenario());
				professorToListHorarioDisponivelCenario.put(professor, novoHorarioList);
			}
			else
			{
				professorToListHorarioDisponivelCenario.get(professor).add(atendimento.getHorarioDisponivelCenario());
			}
		}
		
		List<RelatorioQuantidadeDTO> janelasGradeHorariaDocente = new ArrayList<RelatorioQuantidadeDTO>();
		
		for (Entry<TriedaPar<String,Boolean>, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
		{
			Map<Semanas, List<HorarioDisponivelCenario>> diaMapHorarios = new HashMap<Semanas, List<HorarioDisponivelCenario>>();
					
			for (HorarioDisponivelCenario horario : professor.getValue())
			{
				if (diaMapHorarios.get(horario.getDiaSemana()) == null)
				{
					List<HorarioDisponivelCenario> novoHorario = new ArrayList<HorarioDisponivelCenario>();
					novoHorario.add(horario);
					diaMapHorarios.put(horario.getDiaSemana(), novoHorario);
				}
				else
				{
					diaMapHorarios.get(horario.getDiaSemana()).add(horario);
				}
			}
			int janelas = 0;

			for (Entry<Semanas, List<HorarioDisponivelCenario>> horarioSemana : diaMapHorarios.entrySet())
			{
				List<HorarioDisponivelCenario> horarioProfessorOrdenado = horarioSemana.getValue();
				
				Collections.sort(horarioProfessorOrdenado, new Comparator<HorarioDisponivelCenario>() {
					@Override
					public int compare(HorarioDisponivelCenario arg1, HorarioDisponivelCenario arg2) {
						Calendar h1 = Calendar.getInstance();
						h1.setTime(arg1.getHorarioAula().getHorario());
						h1.set(1979,Calendar.NOVEMBER,6);
						
						Calendar h2 = Calendar.getInstance();
						h2.setTime(arg2.getHorarioAula().getHorario());
						h2.set(1979,Calendar.NOVEMBER,6);
						
						
						return h1.compareTo(h2);
					}
				});
				if (!horarioProfessorOrdenado.isEmpty())
				{
					for (int i = 1; i < horarioProfessorOrdenado.size(); i++)
					{
						Calendar h1 = Calendar.getInstance();
						h1.setTime(horarioProfessorOrdenado.get(i-1).getHorarioAula().getHorario());
						h1.add(Calendar.MINUTE,horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo());
						Calendar h2 = Calendar.getInstance();
						h2.setTime(horarioProfessorOrdenado.get(i).getHorarioAula().getHorario());
						if (h2.getTimeInMillis() - h1.getTimeInMillis() > (horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().calculaMaiorIntervalo()))
						{
							janelas++;
						}
					}
				}
			}
			if (janelasGradeHorariaDocente.size() <= janelas)
			{
				for (int i = janelasGradeHorariaDocente.size(); i <= janelas; i++)
				{
					String titulo = "";
					if (i == 0)
					{
						titulo = "Nenhuma janela";
					}
					else
					{
						titulo = i + " janela" + ((i != 1) ? "s " : "") + " na grade";
					}
					RelatorioQuantidadeDTO novaFaixa = new RelatorioQuantidadeDTO(titulo);
					janelasGradeHorariaDocente.add(novaFaixa);
				}
			}
			if (professor.getKey().getSegundo())
			{
				janelasGradeHorariaDocente.get(janelas).setQuantidade(janelasGradeHorariaDocente.get(janelas).getQuantidade() + 1);
			}
			else
			{
				janelasGradeHorariaDocente.get(janelas).setQuantidade2(janelasGradeHorariaDocente.get(janelas).getQuantidade2() + 1);
			}
		}
		return janelasGradeHorariaDocente;
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getProfessoresDisciplinasLecionadas(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentosList = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
		
		Map<String, Set<String>> professorToQtdeDisciplinasLecionadas = new HashMap<String, Set<String>>();
		
		for (AtendimentoOperacional atendimento : atendimentosList)
		{
			String professor;
			if (atendimento.getProfessorVirtual() != null)
			{
				professor = atendimento.getProfessorVirtual().getNome();
			}
			else
			{
				professor = atendimento.getProfessor().getNome();
			}
			if (professorToQtdeDisciplinasLecionadas.get(professor) == null)
			{
				Set<String> turmas = new HashSet<String>();
				turmas.add((atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
						+ "-" + atendimento.getTurma());
				professorToQtdeDisciplinasLecionadas.put(professor, turmas);
			}
			else
			{
				professorToQtdeDisciplinasLecionadas.get(professor).add(
						(atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
						+ "-" + atendimento.getTurma());
			}
		}
		
		List<RelatorioQuantidadeDTO> professoresDisciplinasLecionadas = new ArrayList<RelatorioQuantidadeDTO>();
		
		Map<String, Boolean> todosProfessoresMapVirtual = new HashMap<String, Boolean>();
		for (Professor professorInstitucional : Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus))
		{
			todosProfessoresMapVirtual.put(professorInstitucional.getNome(), false);
		}
		for (ProfessorVirtual professorVirtual : ProfessorVirtual.findBy(getInstituicaoEnsinoUser(), cenario, campus))
		{
			todosProfessoresMapVirtual.put(professorVirtual.getNome(), true);
		}
		
		for (Entry<String, Boolean> professor : todosProfessoresMapVirtual.entrySet() )
		{
			int qtdeDisciplinasLecionadas = professorToQtdeDisciplinasLecionadas.get(professor.getKey()) == null
					? 0 : professorToQtdeDisciplinasLecionadas.get(professor.getKey()).size() ;
			if (professoresDisciplinasLecionadas.size() <= qtdeDisciplinasLecionadas)
			{
				for (int i = professoresDisciplinasLecionadas.size(); i <= qtdeDisciplinasLecionadas; i++)
				{
					String titulo = "";
					if (i == 0)
					{
						titulo = "Professor não ativado";
					}
					else
					{
						titulo = i + " disciplina" + ((i != 1) ? "s " : "") + " turma" + ((i != 1) ? "s " : "");
					}
					RelatorioQuantidadeDTO novaFaixa = new RelatorioQuantidadeDTO(titulo);
					professoresDisciplinasLecionadas.add(novaFaixa);
				}
			}
			if (professor.getValue())
			{
				professoresDisciplinasLecionadas.get(qtdeDisciplinasLecionadas).setQuantidade(professoresDisciplinasLecionadas.get(qtdeDisciplinasLecionadas).getQuantidade() + 1);
			}
			else
			{
				professoresDisciplinasLecionadas.get(qtdeDisciplinasLecionadas).setQuantidade2(professoresDisciplinasLecionadas.get(qtdeDisciplinasLecionadas).getQuantidade2() + 1);
			}
		}
		return professoresDisciplinasLecionadas;
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getAmbientesFaixaOcupacaoHorarios(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> todasSalas = new HashSet<Sala>();
		for(Unidade unidade : campus.getUnidades() ){
			todasSalas.addAll(unidade.getSalas());
		}
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		for (Oferta oferta : campus.getOfertas()) {
			turnosConsiderados.add(oferta.getTurno());
			if (ehTatico) {
				// atendimentos táticos
				for (AtendimentoTatico aula : oferta.getAtendimentosTaticos()) {
					String key = aula.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> aulasPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (aulasPorSalaTurno == null) {
						aulasPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,aulasPorSalaTurno);
					}
					aulasPorSalaTurno.add(ConvertBeans.toAtendimentoTaticoDTO(aula));
					
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta, campus.getCenario());
				for (AtendimentoOperacional atendimento : atendimentos) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTOSemAlunosDemanda(atendimento));
					
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
		
		List<RelatorioQuantidadeDTO> faixasOcupacaoHorario = new ArrayList<RelatorioQuantidadeDTO>();
		faixasOcupacaoHorario.add(new RelatorioQuantidadeDTO("Entre 81% e 100%"));
		faixasOcupacaoHorario.add(new RelatorioQuantidadeDTO("Entre 61% e 80%"));
		faixasOcupacaoHorario.add(new RelatorioQuantidadeDTO("Entre 41% e 60%"));
		faixasOcupacaoHorario.add(new RelatorioQuantidadeDTO("Entre 21% e 40%"));
		faixasOcupacaoHorario.add(new RelatorioQuantidadeDTO("Entre 0 e 20%"));
		
		for(RelatorioQuantidadeDTO faixa : faixasOcupacaoHorario){
			faixa.setCampusNome(campusDTO.getCodigo());
		}
		
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			Map<Long,Integer> salaIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Turno turno : turnosConsiderados) {
				for (Sala sala : todasSalas) {
					String key = sala.getId() + "-" + turno.getId();
					List<AtendimentoRelatorioDTO> atendimentosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimentosPorSalaTurno != null) {
						List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
						if (ehTatico) {
							aulas.addAll(atendimentosPorSalaTurno);
						} else {
							List<AtendimentoOperacionalDTO> atendimentosOperacional = new ArrayList<AtendimentoOperacionalDTO>(atendimentosPorSalaTurno.size());
							for (AtendimentoRelatorioDTO atendimento : atendimentosPorSalaTurno) {
								atendimentosOperacional.add((AtendimentoOperacionalDTO)atendimento);
							}
							// processa os atendimentos do operacional e os transforma em aulas
							List<AtendimentoOperacionalDTO> aulasOperacional = atService.extraiAulas(atendimentosOperacional);
							// insere as aulas do modo operacional na lista de atendimentos
							aulas.addAll(aulasOperacional);
						}
						
						// trata compartilhamento de turmas entre cursos
						List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = atService.uneAulasQuePodemSerCompartilhadas(aulas);
						
						for (AtendimentoRelatorioDTO aula : aulasComCompartilhamentos) {
							Integer tempoUsoSemanalEmMinutos = salaIdToTempoUsoSemanalEmMinutosMap.get(aula.getSalaId());
							if (tempoUsoSemanalEmMinutos == null) tempoUsoSemanalEmMinutos = 0;
							salaIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
						}
					} else {
							salaIdToTempoUsoSemanalEmMinutosMap.put(sala.getId(), 0);
					}
				}
			}
			
			//calculo do indicador de taxa de uso dos horarios das salas de aula
			SemanaLetiva maiorSemanaLetiva = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivasUtilizadas);
			Map<Integer, Integer> countHorariosAula = new HashMap<Integer, Integer>();
			for(HorarioAula ha : maiorSemanaLetiva.getHorariosAula()){
				if(turnosConsiderados.contains(ha.getTurno()))
					for(HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario()){
						int semanaInt = Semanas.toInt(hdc.getDiaSemana());
						Integer value = countHorariosAula.get(semanaInt);
						value = ((value == null) ? 0 : value);
						countHorariosAula.put(semanaInt, value + 1);
					}
			}
			int cargaHorariaSemanalEmMinutos = 0;
			for(Integer i : countHorariosAula.keySet()){
				cargaHorariaSemanalEmMinutos += countHorariosAula.get(i) * maiorSemanaLetiva.getTempo();
			}
			
			Map<Long, Sala> salaIdToSalaMap = Sala.buildSalaIdToSalaMap(Sala.findAll(getInstituicaoEnsinoUser()));
			for(Long salaId : salaIdToTempoUsoSemanalEmMinutosMap.keySet()){
				Integer tempoUsoSalaSemanalEmMinutos = salaIdToTempoUsoSemanalEmMinutosMap.get(salaId);
				Double mediaUtilizacaoHorarioSalas = ((double)tempoUsoSalaSemanalEmMinutos / cargaHorariaSemanalEmMinutos);
				
				if (mediaUtilizacaoHorarioSalas <= 1 && mediaUtilizacaoHorarioSalas > 0.8)
				{
					if (!salaIdToSalaMap.get(salaId).getExterna())
					{
						faixasOcupacaoHorario.get(0).setQuantidade(faixasOcupacaoHorario.get(0).getQuantidade() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(0).setQuantidade2(faixasOcupacaoHorario.get(0).getQuantidade2() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(0).setQuantidade3(faixasOcupacaoHorario.get(0).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasOcupacaoHorario.get(0).setQuantidade4(faixasOcupacaoHorario.get(0).getQuantidade4() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(0).setQuantidade5(faixasOcupacaoHorario.get(0).getQuantidade5() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(0).setQuantidade6(faixasOcupacaoHorario.get(0).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoHorarioSalas <= 0.8 && mediaUtilizacaoHorarioSalas > 0.6)
				{
					if (!salaIdToSalaMap.get(salaId).getExterna())
					{
						faixasOcupacaoHorario.get(1).setQuantidade(faixasOcupacaoHorario.get(1).getQuantidade() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(1).setQuantidade2(faixasOcupacaoHorario.get(1).getQuantidade2() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(1).setQuantidade3(faixasOcupacaoHorario.get(1).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasOcupacaoHorario.get(1).setQuantidade4(faixasOcupacaoHorario.get(1).getQuantidade4() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(1).setQuantidade5(faixasOcupacaoHorario.get(1).getQuantidade5() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(1).setQuantidade6(faixasOcupacaoHorario.get(1).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoHorarioSalas <= 0.6 && mediaUtilizacaoHorarioSalas > 0.4)
				{
					if (!salaIdToSalaMap.get(salaId).getExterna())
					{
						faixasOcupacaoHorario.get(2).setQuantidade(faixasOcupacaoHorario.get(2).getQuantidade() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(2).setQuantidade2(faixasOcupacaoHorario.get(2).getQuantidade2() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(2).setQuantidade3(faixasOcupacaoHorario.get(2).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasOcupacaoHorario.get(2).setQuantidade4(faixasOcupacaoHorario.get(2).getQuantidade4() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(2).setQuantidade5(faixasOcupacaoHorario.get(2).getQuantidade5() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(2).setQuantidade6(faixasOcupacaoHorario.get(2).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoHorarioSalas <= 0.4 && mediaUtilizacaoHorarioSalas > 0.2)
				{
					if (!salaIdToSalaMap.get(salaId).getExterna())
					{
						faixasOcupacaoHorario.get(3).setQuantidade(faixasOcupacaoHorario.get(3).getQuantidade() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(3).setQuantidade2(faixasOcupacaoHorario.get(3).getQuantidade2() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(3).setQuantidade3(faixasOcupacaoHorario.get(3).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasOcupacaoHorario.get(3).setQuantidade4(faixasOcupacaoHorario.get(3).getQuantidade4() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(3).setQuantidade5(faixasOcupacaoHorario.get(3).getQuantidade5() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(3).setQuantidade6(faixasOcupacaoHorario.get(3).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoHorarioSalas <= 0.2)
				{
					if (!salaIdToSalaMap.get(salaId).getExterna())
					{
						faixasOcupacaoHorario.get(4).setQuantidade(faixasOcupacaoHorario.get(4).getQuantidade() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(4).setQuantidade2(faixasOcupacaoHorario.get(4).getQuantidade2() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(4).setQuantidade3(faixasOcupacaoHorario.get(4).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasOcupacaoHorario.get(4).setQuantidade4(faixasOcupacaoHorario.get(4).getQuantidade4() + 1);
						if (!salaIdToSalaMap.get(salaId).isLaboratorio())
						{
							faixasOcupacaoHorario.get(4).setQuantidade5(faixasOcupacaoHorario.get(4).getQuantidade5() + 1);
						}
						else
						{
							faixasOcupacaoHorario.get(4).setQuantidade6(faixasOcupacaoHorario.get(4).getQuantidade6() + 1);
						}
					}
				}
			}
		}
		return faixasOcupacaoHorario;		
	}
	
	@Override
	public List<RelatorioQuantidadeDoubleDTO> getAmbientesFaixaOcupacaoHorariosPorDiaSemana(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> todasSalas = new HashSet<Sala>();
		for(Unidade unidade : campus.getUnidades() ){
			todasSalas.addAll(unidade.getSalas());
		}
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		for (Oferta oferta : campus.getOfertas()) {
			turnosConsiderados.add(oferta.getTurno());
			if (ehTatico) {
				// atendimentos táticos
				for (AtendimentoTatico aula : oferta.getAtendimentosTaticos()) {
					String key = aula.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> aulasPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (aulasPorSalaTurno == null) {
						aulasPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,aulasPorSalaTurno);
					}
					aulasPorSalaTurno.add(ConvertBeans.toAtendimentoTaticoDTO(aula));
					
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta, campus.getCenario());
				for (AtendimentoOperacional atendimento : atendimentos) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTOSemAlunosDemanda(atendimento));
					
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
				
		List<RelatorioQuantidadeDoubleDTO> faixasUtilizacaoCapacidade = new ArrayList<RelatorioQuantidadeDoubleDTO>();
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDoubleDTO("Externos", 0.0));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDoubleDTO("Laboratórios", 0.0));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDoubleDTO("Salas", 0.0));
		
		for(RelatorioQuantidadeDoubleDTO faixa : faixasUtilizacaoCapacidade){
			faixa.setCampusNome(campusDTO.getCodigo());
		}
		
		Map<Sala, Map<Integer, Integer>> salaToDiaSemanaToTempoUsoMap = new HashMap<Sala, Map<Integer, Integer>>();
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Turno turno : turnosConsiderados) {
				for (Sala sala : todasSalas) {
					String key = sala.getId() + "-" + turno.getId();
					List<AtendimentoRelatorioDTO> atendimentosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimentosPorSalaTurno != null) {
						List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
						if (ehTatico) {
							aulas.addAll(atendimentosPorSalaTurno);
						} else {
							List<AtendimentoOperacionalDTO> atendimentosOperacional = new ArrayList<AtendimentoOperacionalDTO>(atendimentosPorSalaTurno.size());
							for (AtendimentoRelatorioDTO atendimento : atendimentosPorSalaTurno) {
								atendimentosOperacional.add((AtendimentoOperacionalDTO)atendimento);
							}
							// processa os atendimentos do operacional e os transforma em aulas
							List<AtendimentoOperacionalDTO> aulasOperacional = atService.extraiAulas(atendimentosOperacional);
							// insere as aulas do modo operacional na lista de atendimentos
							aulas.addAll(aulasOperacional);
						}
						
						// trata compartilhamento de turmas entre cursos
						List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = atService.uneAulasQuePodemSerCompartilhadas(aulas);
						
						for (AtendimentoRelatorioDTO aula : aulasComCompartilhamentos) {
							if (salaToDiaSemanaToTempoUsoMap.get(sala) == null )
							{
								Map<Integer, Integer> diaSemanaMapTempoUso = new HashMap<Integer, Integer>();
								diaSemanaMapTempoUso.put(aula.getSemana(), aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								salaToDiaSemanaToTempoUsoMap.put(sala, diaSemanaMapTempoUso);
							}
							else
							{
								if (salaToDiaSemanaToTempoUsoMap.get(sala).get(aula.getSemana()) == null)
								{
									salaToDiaSemanaToTempoUsoMap.get(sala).put(aula.getSemana(), aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								}
								else
								{
									salaToDiaSemanaToTempoUsoMap.get(sala).put(aula.getSemana(), salaToDiaSemanaToTempoUsoMap.get(sala).get(aula.getSemana()) + aula.getTotalCreditos()*aula.getDuracaoDeUmaAulaEmMinutos());
								}
							}
						}
					}
				}
			}
			
			//calculo do indicador de taxa de uso dos horarios das salas de aula
			SemanaLetiva maiorSemanaLetiva = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivasUtilizadas);
			Map<Integer, Integer> countHorariosAula = new HashMap<Integer, Integer>();
			for(HorarioAula ha : maiorSemanaLetiva.getHorariosAula()){
				if(turnosConsiderados.contains(ha.getTurno()))
					for(HorarioDisponivelCenario hdc : ha.getHorariosDisponiveisCenario()){
						int semanaInt = Semanas.toInt(hdc.getDiaSemana());
						Integer value = countHorariosAula.get(semanaInt) == null ? maiorSemanaLetiva.getTempo() : countHorariosAula.get(semanaInt) + maiorSemanaLetiva.getTempo();
						countHorariosAula.put(semanaInt, value);
					}
			}

			for(Entry<Sala, Map<Integer, Integer>> sala : salaToDiaSemanaToTempoUsoMap.entrySet()){
				for (Integer diaSemana : sala.getValue().keySet())
				{
					Integer tempoUsoSalaSemanalEmMinutos = sala.getValue().get(diaSemana);
					Double mediaUtilizacaoHorarioSalas = ((double)tempoUsoSalaSemanalEmMinutos / countHorariosAula.get(diaSemana));
					double faixa;
					if (sala.getKey().getExterna())
					{
						switch (diaSemana) {
						case 2:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade(faixa);
							break;
						case 3:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade2() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade2() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade2(faixa);
							break;
						case 4:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade3() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade3() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade3(faixa);
							break;
						case 5:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade4() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade4() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade4(faixa);
							break;
						case 6:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade5() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade5() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade5(faixa);
							break;
						case 7:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade6() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade6() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade6(faixa);
							break;
						default:
							break;
						}
					}
					else if (sala.getKey().isLaboratorio())
					{
						switch (diaSemana) {
						case 2:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade(faixa);
							break;
						case 3:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade2() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade2() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade2(faixa);
							break;
						case 4:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade3() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade3() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade3(faixa);
							break;
						case 5:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade4() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade4() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade4(faixa);
							break;
						case 6:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade5() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade5() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade5(faixa);
							break;
						case 7:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade6() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade6() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade6(faixa);
							break;
						default:
							break;
						}
					}
					else
					{
						switch (diaSemana) {
						case 2:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade(faixa);
							break;
						case 3:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade2() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade2() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade2(faixa);
							break;
						case 4:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade3() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade3() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade3(faixa);
							break;
						case 5:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade4() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade4() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade4(faixa);
							break;
						case 6:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade5() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade5() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade5(faixa);
							break;
						case 7:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade6() == 0 ? mediaUtilizacaoHorarioSalas :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade6() + mediaUtilizacaoHorarioSalas)/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade6(faixa);
							break;
						default:
							break;
						}
					}
				}
			}
		}
		for(RelatorioQuantidadeDoubleDTO faixa : faixasUtilizacaoCapacidade){
			faixa.setQuantidade(TriedaUtil.round( faixa.getQuantidade()*100, 2 ));
			faixa.setQuantidade2(TriedaUtil.round( faixa.getQuantidade2()*100, 2 ));
			faixa.setQuantidade3(TriedaUtil.round( faixa.getQuantidade3()*100, 2 ));
			faixa.setQuantidade4(TriedaUtil.round( faixa.getQuantidade4()*100, 2 ));
			faixa.setQuantidade5(TriedaUtil.round( faixa.getQuantidade5()*100, 2 ));
			faixa.setQuantidade6(TriedaUtil.round( faixa.getQuantidade6()*100, 2 ));
		}
		return faixasUtilizacaoCapacidade;		
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getAmbientesFaixaUtilizacaoCapacidade(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> todasSalas = new HashSet<Sala>();
		for(Unidade unidade : campus.getUnidades() ){
			todasSalas.addAll(unidade.getSalas());
		}
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		for (Oferta oferta : campus.getOfertas()) {
			turnosConsiderados.add(oferta.getTurno());
			if (ehTatico) {
				// atendimentos táticos
				for (AtendimentoTatico aula : oferta.getAtendimentosTaticos()) {
					String key = aula.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> aulasPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (aulasPorSalaTurno == null) {
						aulasPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,aulasPorSalaTurno);
					}
					aulasPorSalaTurno.add(ConvertBeans.toAtendimentoTaticoDTO(aula));
					
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta, campus.getCenario());
				for (AtendimentoOperacional atendimento : atendimentos) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTOSemAlunosDemanda(atendimento));
					
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
		
		List<RelatorioQuantidadeDTO> faixasUtilizacaoCapacidade = new ArrayList<RelatorioQuantidadeDTO>();
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDTO("Entre 81% e 100%"));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDTO("Entre 61% e 80%"));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDTO("Entre 41% e 60%"));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDTO("Entre 21% e 40%"));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDTO("Entre 0 e 20%"));
		
		for(RelatorioQuantidadeDTO faixa : faixasUtilizacaoCapacidade){
			faixa.setCampusNome(campusDTO.getCodigo());
		}
		
		Map<Sala, List<Integer>> salasToQtdeAlunosPorAulaMap = new HashMap<Sala, List<Integer>>();
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Turno turno : turnosConsiderados) {
				for (Sala sala : todasSalas) {
					String key = sala.getId() + "-" + turno.getId();
					List<AtendimentoRelatorioDTO> atendimentosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimentosPorSalaTurno != null) {
						List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
						if (ehTatico) {
							aulas.addAll(atendimentosPorSalaTurno);
						} else {
							List<AtendimentoOperacionalDTO> atendimentosOperacional = new ArrayList<AtendimentoOperacionalDTO>(atendimentosPorSalaTurno.size());
							for (AtendimentoRelatorioDTO atendimento : atendimentosPorSalaTurno) {
								atendimentosOperacional.add((AtendimentoOperacionalDTO)atendimento);
							}
							// processa os atendimentos do operacional e os transforma em aulas
							List<AtendimentoOperacionalDTO> aulasOperacional = atService.extraiAulas(atendimentosOperacional);
							// insere as aulas do modo operacional na lista de atendimentos
							aulas.addAll(aulasOperacional);
						}
						
						// trata compartilhamento de turmas entre cursos
						List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = atService.uneAulasQuePodemSerCompartilhadas(aulas);
						
						for (AtendimentoRelatorioDTO aula : aulasComCompartilhamentos) {
							if (salasToQtdeAlunosPorAulaMap.get(sala) == null )
							{
								List<Integer> alunosAula = new ArrayList<Integer>();
								alunosAula.add(aula.getQuantidadeAlunos());
								salasToQtdeAlunosPorAulaMap.put(sala, alunosAula);
							}
							else
							{
								salasToQtdeAlunosPorAulaMap.get(sala).add(aula.getQuantidadeAlunos());
							}
						}
					} else {
						if (salasToQtdeAlunosPorAulaMap.get(sala) == null )
						{
							List<Integer> alunosAula = new ArrayList<Integer>();
							alunosAula.add(0);
							salasToQtdeAlunosPorAulaMap.put(sala, alunosAula);
						}
						else
						{
							salasToQtdeAlunosPorAulaMap.get(sala).add(0);
						}
					}
				}
			}

			for(Entry<Sala, List<Integer>> sala : salasToQtdeAlunosPorAulaMap.entrySet()){
				Integer utilizacaoSala = 0;
				for (Integer utilizacao : sala.getValue())
				{
					utilizacaoSala += utilizacao;
				}
				Double mediaUtilizacaoCapacidadeSala = ((double)utilizacaoSala/sala.getValue().size() / sala.getKey().getCapacidadeInstalada());
				
				if (mediaUtilizacaoCapacidadeSala <= 1 && mediaUtilizacaoCapacidadeSala > 0.8)
				{
					if (!sala.getKey().getExterna())
					{
						faixasUtilizacaoCapacidade.get(0).setQuantidade(faixasUtilizacaoCapacidade.get(0).getQuantidade() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(0).setQuantidade2(faixasUtilizacaoCapacidade.get(0).getQuantidade2() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(0).setQuantidade3(faixasUtilizacaoCapacidade.get(0).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasUtilizacaoCapacidade.get(0).setQuantidade4(faixasUtilizacaoCapacidade.get(0).getQuantidade4() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(0).setQuantidade5(faixasUtilizacaoCapacidade.get(0).getQuantidade5() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(0).setQuantidade6(faixasUtilizacaoCapacidade.get(0).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoCapacidadeSala <= 0.8 && mediaUtilizacaoCapacidadeSala > 0.6)
				{
					if (!sala.getKey().getExterna())
					{
						faixasUtilizacaoCapacidade.get(1).setQuantidade(faixasUtilizacaoCapacidade.get(1).getQuantidade() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(1).setQuantidade2(faixasUtilizacaoCapacidade.get(1).getQuantidade2() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(1).setQuantidade3(faixasUtilizacaoCapacidade.get(1).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasUtilizacaoCapacidade.get(1).setQuantidade4(faixasUtilizacaoCapacidade.get(1).getQuantidade4() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(1).setQuantidade5(faixasUtilizacaoCapacidade.get(1).getQuantidade5() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(1).setQuantidade6(faixasUtilizacaoCapacidade.get(1).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoCapacidadeSala <= 0.6 && mediaUtilizacaoCapacidadeSala > 0.4)
				{
					if (!sala.getKey().getExterna())
					{
						faixasUtilizacaoCapacidade.get(2).setQuantidade(faixasUtilizacaoCapacidade.get(2).getQuantidade() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(2).setQuantidade2(faixasUtilizacaoCapacidade.get(2).getQuantidade2() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(2).setQuantidade3(faixasUtilizacaoCapacidade.get(2).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasUtilizacaoCapacidade.get(2).setQuantidade4(faixasUtilizacaoCapacidade.get(2).getQuantidade4() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(2).setQuantidade5(faixasUtilizacaoCapacidade.get(2).getQuantidade5() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(2).setQuantidade6(faixasUtilizacaoCapacidade.get(2).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoCapacidadeSala <= 0.4 && mediaUtilizacaoCapacidadeSala > 0.2)
				{
					if (!sala.getKey().getExterna())
					{
						faixasUtilizacaoCapacidade.get(3).setQuantidade(faixasUtilizacaoCapacidade.get(3).getQuantidade() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(3).setQuantidade2(faixasUtilizacaoCapacidade.get(3).getQuantidade2() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(3).setQuantidade3(faixasUtilizacaoCapacidade.get(3).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasUtilizacaoCapacidade.get(3).setQuantidade4(faixasUtilizacaoCapacidade.get(3).getQuantidade4() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(3).setQuantidade5(faixasUtilizacaoCapacidade.get(3).getQuantidade5() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(3).setQuantidade6(faixasUtilizacaoCapacidade.get(3).getQuantidade6() + 1);
						}
					}
				}
				else if (mediaUtilizacaoCapacidadeSala <= 0.2)
				{
					if (!sala.getKey().getExterna())
					{
						faixasUtilizacaoCapacidade.get(4).setQuantidade(faixasUtilizacaoCapacidade.get(4).getQuantidade() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(4).setQuantidade2(faixasUtilizacaoCapacidade.get(4).getQuantidade2() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(4).setQuantidade3(faixasUtilizacaoCapacidade.get(4).getQuantidade3() + 1);
						}
					}
					else
					{
						faixasUtilizacaoCapacidade.get(4).setQuantidade4(faixasUtilizacaoCapacidade.get(4).getQuantidade4() + 1);
						if(!sala.getKey().isLaboratorio())
						{
							faixasUtilizacaoCapacidade.get(4).setQuantidade5(faixasUtilizacaoCapacidade.get(4).getQuantidade5() + 1);
						}
						else
						{
							faixasUtilizacaoCapacidade.get(4).setQuantidade6(faixasUtilizacaoCapacidade.get(4).getQuantidade6() + 1);
						}
					}
				}
			}
		}
		return faixasUtilizacaoCapacidade;		
	}
	
	@Override
	public List<RelatorioQuantidadeDoubleDTO> getAmbientesFaixaUtilizacaoPorDiaSemana(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> todasSalas = new HashSet<Sala>();
		for(Unidade unidade : campus.getUnidades() ){
			todasSalas.addAll(unidade.getSalas());
		}
		Set<SemanaLetiva> semanasLetivasUtilizadas = new HashSet<SemanaLetiva>();
		Map<String,List<AtendimentoRelatorioDTO>> salaIdTurnoIdToAtendimentosMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();
		for (Oferta oferta : campus.getOfertas()) {
			turnosConsiderados.add(oferta.getTurno());
			if (ehTatico) {
				// atendimentos táticos
				for (AtendimentoTatico aula : oferta.getAtendimentosTaticos()) {
					String key = aula.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> aulasPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (aulasPorSalaTurno == null) {
						aulasPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,aulasPorSalaTurno);
					}
					aulasPorSalaTurno.add(ConvertBeans.toAtendimentoTaticoDTO(aula));
					
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta, campus.getCenario());
				for (AtendimentoOperacional atendimento : atendimentos) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTOSemAlunosDemanda(atendimento));
					
					semanasLetivasUtilizadas.add(atendimento.getOferta().getCurriculo().getSemanaLetiva());
				}
			}
		}
		
		Double externos = 0.0;
		Double laboratorios = 0.0;
		Double salas = 0.0;
		for (Sala sala : todasSalas)
		{
			if(sala.getExterna())
			{
				externos++;
			}
			else if(sala.isLaboratorio())
			{
				laboratorios++;
			}
			else
			{
				salas++;
			}
		}
		
		
		List<RelatorioQuantidadeDoubleDTO> faixasUtilizacaoCapacidade = new ArrayList<RelatorioQuantidadeDoubleDTO>();
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDoubleDTO("Externos", externos));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDoubleDTO("Laboratórios", laboratorios));
		faixasUtilizacaoCapacidade.add(new RelatorioQuantidadeDoubleDTO("Salas", salas));
		
		for(RelatorioQuantidadeDoubleDTO faixa : faixasUtilizacaoCapacidade){
			faixa.setCampusNome(campusDTO.getCodigo());
		}
		
		Map<Sala, Map<Integer, Double>> salaToDiaSemanaParOcupacaoMap = new HashMap<Sala, Map<Integer, Double>>();
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Turno turno : turnosConsiderados) {
				for (Sala sala : todasSalas) {
					String key = sala.getId() + "-" + turno.getId();
					List<AtendimentoRelatorioDTO> atendimentosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimentosPorSalaTurno != null) {
						List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
						if (ehTatico) {
							aulas.addAll(atendimentosPorSalaTurno);
						} else {
							List<AtendimentoOperacionalDTO> atendimentosOperacional = new ArrayList<AtendimentoOperacionalDTO>(atendimentosPorSalaTurno.size());
							for (AtendimentoRelatorioDTO atendimento : atendimentosPorSalaTurno) {
								atendimentosOperacional.add((AtendimentoOperacionalDTO)atendimento);
							}
							// processa os atendimentos do operacional e os transforma em aulas
							List<AtendimentoOperacionalDTO> aulasOperacional = atService.extraiAulas(atendimentosOperacional);
							// insere as aulas do modo operacional na lista de atendimentos
							aulas.addAll(aulasOperacional);
						}
						
						// trata compartilhamento de turmas entre cursos
						List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = atService.uneAulasQuePodemSerCompartilhadas(aulas);
						
						for (AtendimentoRelatorioDTO aula : aulasComCompartilhamentos) {
							if (salaToDiaSemanaParOcupacaoMap.get(sala) == null )
							{
								Map<Integer, Double> diaSemanaMapOcupacao = new HashMap<Integer, Double>();
								diaSemanaMapOcupacao.put(aula.getSemana(), (double)aula.getQuantidadeAlunos()/sala.getCapacidadeInstalada());
								salaToDiaSemanaParOcupacaoMap.put(sala, diaSemanaMapOcupacao);
							}
							else
							{
								if (salaToDiaSemanaParOcupacaoMap.get(sala).get(aula.getSemana()) == null)
								{
									salaToDiaSemanaParOcupacaoMap.get(sala).put(aula.getSemana(), (double)aula.getQuantidadeAlunos()/sala.getCapacidadeInstalada());
								}
								else
								{
									Double ocupacao = salaToDiaSemanaParOcupacaoMap.get(sala).get(aula.getSemana());
									ocupacao += (double)aula.getQuantidadeAlunos()/sala.getCapacidadeInstalada();
									salaToDiaSemanaParOcupacaoMap.get(sala).put(aula.getSemana(), ocupacao/2);
								}
							}
						}
					}
				}
			}

			for(Entry<Sala, Map<Integer, Double>> sala : salaToDiaSemanaParOcupacaoMap.entrySet()){
				for (Integer diaSemana : sala.getValue().keySet())
				{
					double faixa;
					if (sala.getKey().getExterna())
					{
						switch (diaSemana) {
						case 2:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade(faixa);
							break;
						case 3:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade2() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade2() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade2(faixa);
							break;
						case 4:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade3() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade3() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade3(faixa);
							break;
						case 5:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade4() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade4() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade4(faixa);
							break;
						case 6:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade5() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade5() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade5(faixa);
							break;
						case 7:
							faixa = faixasUtilizacaoCapacidade.get(0).getQuantidade6() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(0).getQuantidade6() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(0).setQuantidade6(faixa);
							break;
						default:
							break;
						}
					}
					else if (sala.getKey().isLaboratorio())
					{
						switch (diaSemana) {
						case 2:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade(faixa);
							break;
						case 3:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade2() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade2() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade2(faixa);
							break;
						case 4:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade3() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade3() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade3(faixa);
							break;
						case 5:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade4() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade4() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade4(faixa);
							break;
						case 6:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade5() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade5() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade5(faixa);
							break;
						case 7:
							faixa = faixasUtilizacaoCapacidade.get(1).getQuantidade6() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(1).getQuantidade6() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(1).setQuantidade6(faixa);
							break;
						default:
							break;
						}
					}
					else
					{
						switch (diaSemana) {
						case 2:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade(faixa);
							break;
						case 3:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade2() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade2() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade2(faixa);
							break;
						case 4:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade3() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade3() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade3(faixa);
							break;
						case 5:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade4() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade4() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade4(faixa);
							break;
						case 6:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade5() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade5() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade5(faixa);
							break;
						case 7:
							faixa = faixasUtilizacaoCapacidade.get(2).getQuantidade6() == 0 ? sala.getValue().get(diaSemana) :
								(faixasUtilizacaoCapacidade.get(2).getQuantidade6() + sala.getValue().get(diaSemana))/2;
							faixasUtilizacaoCapacidade.get(2).setQuantidade6(faixa);
							break;
						default:
							break;
						}
					}
				}
			}
		}
		
		for(RelatorioQuantidadeDoubleDTO faixa : faixasUtilizacaoCapacidade){
			faixa.setQuantidade(TriedaUtil.round( faixa.getQuantidade()*100, 2 ));
			faixa.setQuantidade2(TriedaUtil.round( faixa.getQuantidade2()*100, 2 ));
			faixa.setQuantidade3(TriedaUtil.round( faixa.getQuantidade3()*100, 2 ));
			faixa.setQuantidade4(TriedaUtil.round( faixa.getQuantidade4()*100, 2 ));
			faixa.setQuantidade5(TriedaUtil.round( faixa.getQuantidade5()*100, 2 ));
			faixa.setQuantidade6(TriedaUtil.round( faixa.getQuantidade6()*100, 2 ));
		}
		return faixasUtilizacaoCapacidade;		
	}
	
	@Override
	public List<AtendimentoFaixaTurmaDTO> getAtendimentosFaixaTurma(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		boolean ehTatico = false;
		
		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
			if ( campus.isOtimizado(getInstituicaoEnsinoUser()) )
			{
				ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			}
		}
		
		List<AtendimentoTatico> atendimentosTatico = new ArrayList<AtendimentoTatico>();
		List<AtendimentoOperacional> atendimentosOperacional = new ArrayList<AtendimentoOperacional>();
		if (ehTatico)
		{
			atendimentosTatico = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
		}
		else
		{
			atendimentosOperacional = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
		}
		
		if (ehTatico)
		{
			return getAtendimentosFaixaTurmaTatico(atendimentosTatico);
		}
		else
		{
			return getAtendimentosFaixaTurmaOperacional(atendimentosOperacional);
		}
		
		
	}

	private List<AtendimentoFaixaTurmaDTO> getAtendimentosFaixaTurmaOperacional(
			List<AtendimentoOperacional> atendimentosOperacional)
	{
		List<AtendimentoFaixaTurmaDTO> atendimentosFaixaTurma = new ArrayList<AtendimentoFaixaTurmaDTO>();

		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Abaixo de 10 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 10 e 19 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 10 e 29 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 30 e 39 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 40 e 49 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 50 e 59 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 60 e 69 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 70 e 79 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 80 e 89 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("Entre 90 e 99 alunos"));
		atendimentosFaixaTurma.add(new AtendimentoFaixaTurmaDTO("100 ou mais alunos"));
		Map<String, List<AtendimentoOperacional>> turmaMapAtendimentos = new HashMap<String, List<AtendimentoOperacional>>();
		for (AtendimentoOperacional atendimento : atendimentosOperacional)
		{
			String turmaKey = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getId() : atendimento.getDisciplina().getId())
					+ "-" + atendimento.getTurma();
			
			if (turmaMapAtendimentos.get(turmaKey) == null)
			{
				List<AtendimentoOperacional> novoAtendimento = new ArrayList<AtendimentoOperacional>();
				novoAtendimento.add(atendimento);
				turmaMapAtendimentos.put(turmaKey, novoAtendimento);
			}
			else
			{
				turmaMapAtendimentos.get(turmaKey).add(atendimento);
			}
			
		}
		int atendimentoCred = 0;
		int atendimentoAluno = 0;
		Set<AlunoDemanda> alunosDemanda = new HashSet<AlunoDemanda>();
		for (Entry<String, List<AtendimentoOperacional>> turma : turmaMapAtendimentos.entrySet())
		{
			Set<String> horariosKey = new HashSet<String>();
			Set<Long> ofertasKey = new HashSet<Long>();
			if (!turma.getValue().isEmpty())
			{
				String key = turma.getValue().get(0).getHorarioDisponivelCenario().getId() + "-" + turma.getValue().get(0).getCreditoTeorico();
				horariosKey.add(key);
				ofertasKey.add(turma.getValue().get(0).getOferta().getId());
				atendimentoCred++;
				//atendimentoAluno += turma.getValue().get(0).getQuantidadeAlunos();
				for (AtendimentoOperacional atendimento : turma.getValue())
				{
					alunosDemanda.addAll(atendimento.getAlunosDemanda());
					String atendimentoKey = atendimento.getHorarioDisponivelCenario().getId() + "-" + atendimento.getCreditoTeorico();
					if (!ofertasKey.contains(atendimento.getOferta().getId()) && atendimento.getCreditoTeorico() == turma.getValue().get(0).getCreditoTeorico())
					{
						//atendimentoAluno += atendimento.getQuantidadeAlunos();
						ofertasKey.add(atendimento.getOferta().getId());
					}
					if (!horariosKey.contains(atendimentoKey))
					{
						atendimentoCred++;
						horariosKey.add(atendimentoKey);
					}
				}
			}
		}
		System.out.println("Atendimentos Cred " + atendimentoCred);
		System.out.println("Atendimentos Aln " + alunosDemanda.size());
		return atendimentosFaixaTurma;
	}

	private List<AtendimentoFaixaTurmaDTO> getAtendimentosFaixaTurmaTatico(
			List<AtendimentoTatico> atendimentosTatico) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public ListLoadResult< ConfirmacaoTurmaDTO > getTurmasProfessoresVirtuaisList( CenarioDTO cenarioDTO, ProfessorVirtualDTO professorVirtualDTO ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		ProfessorVirtual professorVirtual = ProfessorVirtual.find(professorVirtualDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		Map<String, TriedaPar<Set<String>, ConfirmacaoTurmaDTO>> turmaMapParOfertasConfirmacao = new HashMap<String, TriedaPar<Set<String>, ConfirmacaoTurmaDTO>>();
		Map<String, Map<Semanas, Set<HorarioAula>>> turmaMapSemanaMapHorarios = new HashMap<String, Map<Semanas, Set<HorarioAula>>>();
		Map<String, Set<String>> turmaMapSalaCodigo = new HashMap<String, Set<String>>();
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (!ehTatico)
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(campus, cenario, professorVirtual, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma() + "-" + atendimento.getCreditoTeorico();
					if (turmaMapSalaCodigo.get(key) == null)
					{
						Set<String> newSalas = new HashSet<String>();
						newSalas.add(atendimento.getSala().getCodigo());
						turmaMapSalaCodigo.put(key, newSalas);
					}
					else
					{
						turmaMapSalaCodigo.get(key).add(atendimento.getSala().getCodigo());
					}
					if (turmaMapSemanaMapHorarios.get(key) == null)
					{
						Map<Semanas, Set<HorarioAula>> newSemanaMapHorarios = new HashMap<Semanas, Set<HorarioAula>>();
						Set<HorarioAula> newHorarios = new HashSet<HorarioAula>();
						newHorarios.add(atendimento.getHorarioDisponivelCenario().getHorarioAula());
						newSemanaMapHorarios.put(atendimento.getHorarioDisponivelCenario().getDiaSemana(), newHorarios);
						turmaMapSemanaMapHorarios.put(key, newSemanaMapHorarios);
					}
					else
					{
						if (turmaMapSemanaMapHorarios.get(key).get(atendimento.getHorarioDisponivelCenario().getDiaSemana()) == null)
						{
							Set<HorarioAula> newHorarios = new HashSet<HorarioAula>();
							newHorarios.add(atendimento.getHorarioDisponivelCenario().getHorarioAula());
							turmaMapSemanaMapHorarios.get(key).put(atendimento.getHorarioDisponivelCenario().getDiaSemana(), newHorarios);
						}
						else
						{
							turmaMapSemanaMapHorarios.get(key).get(atendimento.getHorarioDisponivelCenario().getDiaSemana()).add(atendimento.getHorarioDisponivelCenario().getHorarioAula());
						}
					}
					if (turmaMapParOfertasConfirmacao.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						ConfirmacaoTurmaDTO newConfirmacao = new ConfirmacaoTurmaDTO();
						newConfirmacao.setConfirmada( atendimento.getConfirmada() );
						newConfirmacao.setTurma( atendimento.getTurma() );
						newConfirmacao.setCreditosPratico(atendimento.getCreditoTeorico() ? 0 : 1);
						newConfirmacao.setCreditosTeorico(atendimento.getCreditoTeorico() ? 1 : 0);
						newConfirmacao.setDisciplinaId(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getId() : atendimento.getDisciplina().getId());
						newConfirmacao.setDisciplinaCodigo(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo());
						newConfirmacao.setDisciplinaNome(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getNome() : atendimento.getDisciplina().getNome());
						newConfirmacao.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
						newConfirmacao.setSalaId(atendimento.getSala().getId());
						newConfirmacao.setProfessorId(atendimento.getProfessorVirtual() != null ?
								atendimento.getProfessorVirtual().getId() : atendimento.getProfessor().getId());
						newConfirmacao.setProfessorString(atendimento.getProfessorVirtual() != null ?
								atendimento.getProfessorVirtual().getNome() : atendimento.getProfessor().getNome());
						turmaMapParOfertasConfirmacao.put(key, TriedaPar.create(newOferta, newConfirmacao));
					}
					else
					{
						if (atendimento.getCreditoTeorico())
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setCreditosTeorico(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getCreditosTeorico() + 1);
						}
						else
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setCreditosPratico(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getCreditosPratico() + 1);
						}
						if (!turmaMapParOfertasConfirmacao.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setQuantidadeAlunos(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getQuantidadeAlunos() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasConfirmacao.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
		}
		
		List<ConfirmacaoTurmaDTO> confirmacoes = new ArrayList<ConfirmacaoTurmaDTO>();
		for (Entry<String, TriedaPar<Set<String>, ConfirmacaoTurmaDTO>> atendimento : turmaMapParOfertasConfirmacao.entrySet())
		{
			atendimento.getValue().getSegundo().setDiasHorarios(
					escreveDiasEHorarios(turmaMapSemanaMapHorarios.get(atendimento.getKey())));
			String salasString = "";
			int numSalas = 0;
			for (String salaCodigo : turmaMapSalaCodigo.get(atendimento.getKey()))
			{
				salasString += salaCodigo;
				numSalas++;
				if (numSalas < turmaMapSalaCodigo.get(atendimento.getKey()).size())
				{
					salasString += ", ";
				}
			}
			atendimento.getValue().getSegundo().setSalaString(salasString);
			confirmacoes.add(atendimento.getValue().getSegundo());
		}
		
		BaseListLoadResult< ConfirmacaoTurmaDTO > result
			= new BaseListLoadResult< ConfirmacaoTurmaDTO >( confirmacoes );
		
		return result;
	}
	
	@Override
	public PagingLoadResult<PesquisaPorDisciplinaDTO> getAtendimentosPesquisaPorDisciplinaList(
			CenarioDTO cenarioDTO, String codigo, String nome, CampusDTO campusDTO,
			String turma, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<Campus> campusList = new ArrayList<Campus>();
		DateFormat df = new SimpleDateFormat( "HH:mm" );
		
		if(campusDTO == null){
			campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		} else {
			Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
			campusList.add(campus);
		}
		
		//Map<String, TriedaPar<Set<String>, PesquisaPorDisciplinaDTO>> turmaMapParOfertasRelatorio = new HashMap<String, TriedaPar<Set<String>, PesquisaPorDisciplinaDTO>>();
		List<PesquisaPorDisciplinaDTO> pesquisa = new ArrayList<PesquisaPorDisciplinaDTO>();
		
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				
				for (AtendimentoTatico atendimento : atendimentos)
				{
					if (codigo == null || codigo.contains(atendimento.getDisciplinaSubstituta() == null ?
							atendimento.getDisciplina().getCodigo() : atendimento.getDisciplinaSubstituta().getCodigo()))
					{
						if (nome == null || nome.contains(atendimento.getDisciplinaSubstituta() == null ?
								atendimento.getDisciplina().getNome() : atendimento.getDisciplinaSubstituta().getNome()))
						{
							PesquisaPorDisciplinaDTO newRelatorio = new PesquisaPorDisciplinaDTO();
							newRelatorio.setCursoString( atendimento.getOferta().getCurso().getCodigo());
							newRelatorio.setTurma( atendimento.getTurma() );
							newRelatorio.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
							newRelatorio.setSalaId(atendimento.getSala().getId());
							newRelatorio.setDiaSemanaString(atendimento.getSemana().toString());
							newRelatorio.setHorarioId(atendimento.getHorarioAula().getId());
							newRelatorio.setDisciplinaCodigo( atendimento.getDisciplinaSubstituta() == null ?
									atendimento.getDisciplina().getCodigo() : atendimento.getDisciplinaSubstituta().getCodigo() );
							newRelatorio.setDisciplinaNome( atendimento.getDisciplinaSubstituta() == null ?
									atendimento.getDisciplina().getNome() : atendimento.getDisciplinaSubstituta().getNome() );
							pesquisa.add(newRelatorio);
							
							/*
							String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
									+ "-" + atendimento.getTurma();
		
							
									if (turmaMapParOfertasRelatorio.get(key) == null)
							{
								Set<String> newOferta = new HashSet<String>();
								newOferta.add(atendimento.getOferta().getId() + "-" + 
										atendimento.getDisciplina().getCodigo());
								PesquisaPorDisciplinaDTO newRelatorio = new PesquisaPorDisciplinaDTO();
								newRelatorio.setTurma( atendimento.getTurma() );
								newRelatorio.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
								newRelatorio.setSalaId(atendimento.getSala().getId());
								newRelatorio.setDiaSemanaString(atendimento.getSemana().toString());
								newRelatorio.setHorarioId(atendimento.getHorarioAula().getId());
								//newRelatorio.setHorarioInicial(atendimento.getHorarioAula().getHorario());
								turmaMapParOfertasRelatorio.put(key, TriedaPar.create(newOferta, newRelatorio));
							}
							else
							{
								if (!turmaMapParOfertasConfirmacao.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
										+ "-" + atendimento.getDisciplina().getCodigo()))
								{
									turmaMapParOfertasConfirmacao.get(key).getSegundo().setQuantidadeAlunos(
											turmaMapParOfertasConfirmacao.get(key).getSegundo().getQuantidadeAlunos() + atendimento.getQuantidadeAlunos());
									turmaMapParOfertasConfirmacao.get(key).getPrimeiro().add(atendimento.getOferta().getId()
											+ "-" + atendimento.getDisciplina().getCodigo());
								}
							}*/
						}
					}
					
				}
				
			} else {
				
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					if (codigo == null || codigo.contains(atendimento.getDisciplinaSubstituta() == null ?
							atendimento.getDisciplina().getCodigo() : atendimento.getDisciplinaSubstituta().getCodigo()))
					{
						if (nome == null || nome.contains(atendimento.getDisciplinaSubstituta() == null ?
								atendimento.getDisciplina().getNome() : atendimento.getDisciplinaSubstituta().getNome()))
						{
							PesquisaPorDisciplinaDTO newRelatorio = new PesquisaPorDisciplinaDTO();
							newRelatorio.setCursoString( atendimento.getOferta().getCurso().getCodigo());
							newRelatorio.setTurma( atendimento.getTurma() );
							newRelatorio.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
							newRelatorio.setSalaId(atendimento.getSala().getId());
							newRelatorio.setDiaSemanaString(atendimento.getHorarioDisponivelCenario().getDiaSemana().toString());
							SemanaLetiva semanaLetiva = atendimento.getHorarioDisponivelCenario().getHorarioAula().getSemanaLetiva();
							HorarioAula horarioAula = atendimento.getHorarioDisponivelCenario().getHorarioAula();
							newRelatorio.setHorarioId(horarioAula.getId());
							newRelatorio.setDisciplinaCodigo( atendimento.getDisciplinaSubstituta() == null ?
									atendimento.getDisciplina().getCodigo() : atendimento.getDisciplinaSubstituta().getCodigo() );
							newRelatorio.setDisciplinaNome( atendimento.getDisciplinaSubstituta() == null ?
									atendimento.getDisciplina().getNome() : atendimento.getDisciplinaSubstituta().getNome() );
							
							String inicio = df.format( horarioAula.getHorario() );
							Calendar hi = TriedaServerUtil.dateToCalendar(horarioAula.getHorario());
							Calendar hf = Calendar.getInstance();
							hf.clear();
							hf.setTime(hi.getTime());
							hf.add(Calendar.MINUTE,semanaLetiva.getTempo());
							newRelatorio.setHorarioInicial(inicio);
							newRelatorio.setHorarioFinal(TriedaUtil.shortTimeString(hf.getTime()));
							
							newRelatorio.setSalaId(atendimento.getSala().getId());
							newRelatorio.setSalaString(atendimento.getSala().getDescricao());
							newRelatorio.setProfessorString(atendimento.getProfessorVirtual() != null ?
									atendimento.getProfessorVirtual().getNome() : atendimento.getProfessor().getNome());
							
							
							pesquisa.add(newRelatorio);
							
							/*
							String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
									+ "-" + atendimento.getTurma();
							
							
							if (turmaMapParOfertasRelatorio.get(key) == null)
							{
								Set<String> newOferta = new HashSet<String>();
								newOferta.add(atendimento.getOferta().getId() + "-" + 
										atendimento.getDisciplina().getCodigo());
								PesquisaPorDisciplinaDTO newRelatorio = new PesquisaPorDisciplinaDTO();
								newRelatorio.setTurma( atendimento.getTurma() );
								newRelatorio.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
								newRelatorio.setSalaId(atendimento.getSala().getId());
								newRelatorio.setDiaSemanaString(atendimento.getHorarioDisponivelCenario().getDiaSemana().toString());
								newRelatorio.setHorarioId(atendimento.getHorarioDisponivelCenario().getHorarioAula().getId());
								//newRelatorio.setHorarioInicial(atendimento.getHorarioDisponivelCenario().getHorarioAula().getHorario());
								turmaMapParOfertasRelatorio.put(key, TriedaPar.create(newOferta, newRelatorio));
							}
							*/
						}
					}
				}
			}
		}
		
		List<PesquisaPorDisciplinaDTO> pesquisaPagina = new ArrayList<PesquisaPorDisciplinaDTO>();
		for (int i = config.getOffset(); (i < pesquisa.size() && i < (config.getOffset() + config.getLimit())); i++ )
		{
			pesquisaPagina.add(pesquisa.get(i));
		}
		
		BasePagingLoadResult< PesquisaPorDisciplinaDTO > result
		= new BasePagingLoadResult< PesquisaPorDisciplinaDTO >( pesquisaPagina );

		result.setOffset( config.getOffset() );
		result.setTotalLength( pesquisa.size() );
		
		return result;
	}
	
	@Override
	public PagingLoadResult< ConfirmacaoTurmaDTO > getConfirmacaoTurmasList( CenarioDTO cenarioDTO, PagingLoadConfig config ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		Map<String, TriedaPar<Set<String>, ConfirmacaoTurmaDTO>> turmaMapParOfertasConfirmacao = new HashMap<String, TriedaPar<Set<String>, ConfirmacaoTurmaDTO>>();
		Map<String, Map<Semanas, Set<HorarioAula>>> turmaMapSemanaMapHorarios = new HashMap<String, Map<Semanas, Set<HorarioAula>>>();
		Map<String, Set<String>> turmaMapSalaCodigo = new HashMap<String, Set<String>>();
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				
				for (AtendimentoTatico atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma();
					if (turmaMapSalaCodigo.get(key) == null)
					{
						Set<String> newSalas = new HashSet<String>();
						newSalas.add(atendimento.getSala().getCodigo());
						turmaMapSalaCodigo.put(key, newSalas);
					}
					else
					{
						turmaMapSalaCodigo.get(key).add(atendimento.getSala().getCodigo());
					}
					if (turmaMapSemanaMapHorarios.get(key) == null)
					{
						Map<Semanas, Set<HorarioAula>> newSemanaMapHorarios = new HashMap<Semanas, Set<HorarioAula>>();
						Set<HorarioAula> newHorarios = new HashSet<HorarioAula>();
						newHorarios.add(atendimento.getHorarioAula());
						newSemanaMapHorarios.put(atendimento.getSemana(), newHorarios);
						turmaMapSemanaMapHorarios.put(key, newSemanaMapHorarios);
					}
					else
					{
						if (turmaMapSemanaMapHorarios.get(key).get(atendimento.getSemana()) == null)
						{
							Set<HorarioAula> newHorarios = new HashSet<HorarioAula>();
							newHorarios.add(atendimento.getHorarioAula());
							turmaMapSemanaMapHorarios.get(key).put(atendimento.getSemana(), newHorarios);
						}
						else
						{
							turmaMapSemanaMapHorarios.get(key).get(atendimento.getSemana()).add(atendimento.getHorarioAula());
						}
					}
					if (turmaMapParOfertasConfirmacao.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						ConfirmacaoTurmaDTO newConfirmacao = new ConfirmacaoTurmaDTO();
						newConfirmacao.setConfirmada(atendimento.getConfirmada());
						newConfirmacao.setTurma( atendimento.getTurma() );
						newConfirmacao.setCreditosPratico(atendimento.getCreditosPratico());
						newConfirmacao.setCreditosTeorico(atendimento.getCreditosTeorico());
						newConfirmacao.setDisciplinaId(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getId() : atendimento.getDisciplina().getId());
						newConfirmacao.setDisciplinaCodigo(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo());
						newConfirmacao.setDisciplinaNome(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getNome() : atendimento.getDisciplina().getNome());
						newConfirmacao.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
						newConfirmacao.setSalaId(atendimento.getSala().getId());
						turmaMapParOfertasConfirmacao.put(key, TriedaPar.create(newOferta, newConfirmacao));
					}
					else
					{
						if (!turmaMapParOfertasConfirmacao.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setQuantidadeAlunos(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getQuantidadeAlunos() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasConfirmacao.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma();
					if (turmaMapSalaCodigo.get(key) == null)
					{
						Set<String> newSalas = new HashSet<String>();
						newSalas.add(atendimento.getSala().getCodigo());
						turmaMapSalaCodigo.put(key, newSalas);
					}
					else
					{
						turmaMapSalaCodigo.get(key).add(atendimento.getSala().getCodigo());
					}
					if (turmaMapSemanaMapHorarios.get(key) == null)
					{
						Map<Semanas, Set<HorarioAula>> newSemanaMapHorarios = new HashMap<Semanas, Set<HorarioAula>>();
						Set<HorarioAula> newHorarios = new HashSet<HorarioAula>();
						newHorarios.add(atendimento.getHorarioDisponivelCenario().getHorarioAula());
						newSemanaMapHorarios.put(atendimento.getHorarioDisponivelCenario().getDiaSemana(), newHorarios);
						turmaMapSemanaMapHorarios.put(key, newSemanaMapHorarios);
					}
					else
					{
						if (turmaMapSemanaMapHorarios.get(key).get(atendimento.getHorarioDisponivelCenario().getDiaSemana()) == null)
						{
							Set<HorarioAula> newHorarios = new HashSet<HorarioAula>();
							newHorarios.add(atendimento.getHorarioDisponivelCenario().getHorarioAula());
							turmaMapSemanaMapHorarios.get(key).put(atendimento.getHorarioDisponivelCenario().getDiaSemana(), newHorarios);
						}
						else
						{
							turmaMapSemanaMapHorarios.get(key).get(atendimento.getHorarioDisponivelCenario().getDiaSemana()).add(atendimento.getHorarioDisponivelCenario().getHorarioAula());
						}
					}
					if (turmaMapParOfertasConfirmacao.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						ConfirmacaoTurmaDTO newConfirmacao = new ConfirmacaoTurmaDTO();
						newConfirmacao.setConfirmada( atendimento.getConfirmada() );
						newConfirmacao.setTurma( atendimento.getTurma() );
						newConfirmacao.setCreditosPratico(atendimento.getCreditoTeorico() ? 0 : 1);
						newConfirmacao.setCreditosTeorico(atendimento.getCreditoTeorico() ? 1 : 0);
						newConfirmacao.setDisciplinaId(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getId() : atendimento.getDisciplina().getId());
						newConfirmacao.setDisciplinaCodigo(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo());
						newConfirmacao.setDisciplinaNome(atendimento.getDisciplinaSubstituta() != null ?
								atendimento.getDisciplinaSubstituta().getNome() : atendimento.getDisciplina().getNome());
						newConfirmacao.setQuantidadeAlunos(atendimento.getQuantidadeAlunos());
						newConfirmacao.setSalaId(atendimento.getSala().getId());
						newConfirmacao.setProfessorId(atendimento.getProfessorVirtual() != null ?
								atendimento.getProfessorVirtual().getId() : atendimento.getProfessor().getId());
						newConfirmacao.setProfessorString(atendimento.getProfessorVirtual() != null ?
								atendimento.getProfessorVirtual().getNome() : atendimento.getProfessor().getNome());
						turmaMapParOfertasConfirmacao.put(key, TriedaPar.create(newOferta, newConfirmacao));
					}
					else
					{
						if (atendimento.getCreditoTeorico())
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setCreditosTeorico(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getCreditosTeorico() + 1);
						}
						else
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setCreditosPratico(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getCreditosPratico() + 1);
						}
						if (!turmaMapParOfertasConfirmacao.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasConfirmacao.get(key).getSegundo().setQuantidadeAlunos(
									turmaMapParOfertasConfirmacao.get(key).getSegundo().getQuantidadeAlunos() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasConfirmacao.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
		}
		
		List<ConfirmacaoTurmaDTO> confirmacoes = new ArrayList<ConfirmacaoTurmaDTO>();
		for (Entry<String, TriedaPar<Set<String>, ConfirmacaoTurmaDTO>> atendimento : turmaMapParOfertasConfirmacao.entrySet())
		{
			atendimento.getValue().getSegundo().setDiasHorarios(
					escreveDiasEHorarios(turmaMapSemanaMapHorarios.get(atendimento.getKey())));
			String salasString = "";
			int numSalas = 0;
			for (String salaCodigo : turmaMapSalaCodigo.get(atendimento.getKey()))
			{
				salasString += salaCodigo;
				numSalas++;
				if (numSalas < turmaMapSalaCodigo.get(atendimento.getKey()).size())
				{
					salasString += ", ";
				}
			}
			atendimento.getValue().getSegundo().setSalaString(salasString);
			confirmacoes.add(atendimento.getValue().getSegundo());
		}
		
		List<ConfirmacaoTurmaDTO> confirmacoesPagina = new ArrayList<ConfirmacaoTurmaDTO>();
		for (int i = config.getOffset(); (i < confirmacoes.size() && i < (config.getOffset() + config.getLimit())); i++ )
		{
			confirmacoesPagina.add(confirmacoes.get(i));
		}
		
		BasePagingLoadResult< ConfirmacaoTurmaDTO > result
		= new BasePagingLoadResult< ConfirmacaoTurmaDTO >( confirmacoesPagina );

		result.setOffset( config.getOffset() );
		result.setTotalLength( confirmacoes.size() );
		
		return result;
	}
	

	private String escreveDiasEHorarios(Map<Semanas, Set<HorarioAula>> map) {
		SimpleDateFormat out = new SimpleDateFormat("HH:mm");   
		String result = "";
		for (Entry<Semanas, Set<HorarioAula>> horarioSemana : map.entrySet())
		{
			result += horarioSemana.getKey() + " ";
			
			List<HorarioAula> horarioOrdenado = new ArrayList<HorarioAula>();
			horarioOrdenado.addAll(horarioSemana.getValue());
			
			Collections.sort(horarioOrdenado, new Comparator<HorarioAula>() {
				@Override
				public int compare(HorarioAula arg1, HorarioAula arg2) {
					Calendar h1 = Calendar.getInstance();
					h1.setTime(arg1.getHorario());
					h1.set(1979,Calendar.NOVEMBER,6);
					
					Calendar h2 = Calendar.getInstance();
					h2.setTime(arg2.getHorario());
					h2.set(1979,Calendar.NOVEMBER,6);
					
					
					return h1.compareTo(h2);
				}
			});
			if (!horarioOrdenado.isEmpty())
			{
				result += out.format(horarioOrdenado.get(0).getHorario());
				boolean novoHorario = false;
				for (int i = 1; i < horarioOrdenado.size(); i++)
				{
					Calendar h1 = Calendar.getInstance();
					h1.setTime(horarioOrdenado.get(i-1).getHorario());
					h1.add(Calendar.MINUTE,horarioOrdenado.get(i-1).getSemanaLetiva().getTempo());
					Calendar h2 = Calendar.getInstance();
					h2.setTime(horarioOrdenado.get(i).getHorario());
					if (novoHorario)
					{
						result += out.format(horarioOrdenado.get(i-1).getHorario());
						novoHorario = false;
					}
					if (h2.getTimeInMillis() - h1.getTimeInMillis() != 0)
					{
						if (!novoHorario)
						{
							Calendar horarioFinal = Calendar.getInstance();
							horarioFinal.setTime(horarioOrdenado.get(i-1).getHorario());
							horarioFinal.add(Calendar.MINUTE,horarioOrdenado.get(i-1).getSemanaLetiva().getTempo());
							result += " às " + out.format(horarioFinal.getTime()) + " ";
							novoHorario = true;
						}
					}
				}
				Calendar horarioFinal = Calendar.getInstance();
				horarioFinal.setTime(horarioOrdenado.get(horarioOrdenado.size()-1).getHorario());
				horarioFinal.add(Calendar.MINUTE,horarioOrdenado.get(horarioOrdenado.size()-1).getSemanaLetiva().getTempo());
				if (!novoHorario)
				{
					result += " às " + out.format(horarioFinal.getTime()) + " ";
				}
				else
				{
					result += out.format(horarioOrdenado.get(horarioOrdenado.size()-1).getHorario()) + 
							" às " + out.format(horarioFinal.getTime()) + " ";
				}
			}
		}
		return result;
	}
	
	@Override
	@Transactional
	public void confirmarTodasTurmas( CenarioDTO cenarioDTO ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				for (AtendimentoTatico atendimento : atendimentos)
				{
					atendimento.setConfirmada(true);
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					atendimento.setConfirmada(true);
				}
			}
		}
	}
	
	@Override
	@Transactional
	public void desconfirmarTodasTurmas( CenarioDTO cenarioDTO ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				for (AtendimentoTatico atendimento : atendimentos)
				{
					atendimento.setConfirmada(false);
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					atendimento.setConfirmada(false);
				}
			}
		}
	}
	
	@Override
	@Transactional
	public void confirmarTurmasComAlunosFormandos( CenarioDTO cenarioDTO ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				for (AtendimentoTatico atendimento : atendimentos)
				{
					for (AlunoDemanda aluno : atendimento.getAlunosDemanda())
					{
						if (aluno.getAluno().getFormando())
						{
							atendimento.setConfirmada(true);
							break;
						}
					}
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					for (AlunoDemanda aluno : atendimento.getAlunosDemanda())
					{
						if (aluno.getAluno().getFormando())
						{
							atendimento.setConfirmada(true);
							break;
						}
					}
				}
			}
		}
	}
	
	@Override
	@Transactional
	public void confirmarTurmasComNumAlunos( Integer qtdeAlunos, CenarioDTO cenarioDTO ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		Map<String, TriedaPar<Set<String>, Integer>> turmaMapParOfertasQtdeAlunos = new HashMap<String, TriedaPar<Set<String>, Integer>>();
		Map<String, List<AtendimentoTatico>> turmaMapAtendimentosTatico = new HashMap<String, List<AtendimentoTatico>>();
		Map<String, List<AtendimentoOperacional>> turmaMapAtendimentosOperacional = new HashMap<String, List<AtendimentoOperacional>>();
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				
				for (AtendimentoTatico atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma();
					if (turmaMapAtendimentosTatico.get(key) == null)
					{
						List<AtendimentoTatico> newAtendimentos = new ArrayList<AtendimentoTatico>();
						newAtendimentos.add(atendimento);
						turmaMapAtendimentosTatico.put(key, newAtendimentos);
					}
					else
					{
						turmaMapAtendimentosTatico.get(key).add(atendimento);
					}
					if (turmaMapParOfertasQtdeAlunos.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						turmaMapParOfertasQtdeAlunos.put(key, TriedaPar.create(newOferta, atendimento.getQuantidadeAlunos()));
					}
					else
					{
						if (!turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasQtdeAlunos.get(key).setSegundo(
									turmaMapParOfertasQtdeAlunos.get(key).getSegundo() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma();
					if (turmaMapAtendimentosOperacional.get(key) == null)
					{
						List<AtendimentoOperacional> newAtendimentos = new ArrayList<AtendimentoOperacional>();
						newAtendimentos.add(atendimento);
						turmaMapAtendimentosOperacional.put(key, newAtendimentos);
					}
					else
					{
						turmaMapAtendimentosOperacional.get(key).add(atendimento);
					}
					if (turmaMapParOfertasQtdeAlunos.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						turmaMapParOfertasQtdeAlunos.put(key, TriedaPar.create(newOferta, atendimento.getQuantidadeAlunos()));
					}
					else
					{
						if (!turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasQtdeAlunos.get(key).setSegundo(
									turmaMapParOfertasQtdeAlunos.get(key).getSegundo() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
		}
		
		for (Entry<String, TriedaPar<Set<String>, Integer>> atendimento : turmaMapParOfertasQtdeAlunos.entrySet())
		{
			if (atendimento.getValue().getSegundo() >= qtdeAlunos)
			{
				if (turmaMapAtendimentosTatico.get(atendimento.getKey()) != null)
				{
					for (AtendimentoTatico atendimentoTatico : turmaMapAtendimentosTatico.get(atendimento.getKey()))
					{
						atendimentoTatico.setConfirmada(true);
					}
				}
				else if (turmaMapAtendimentosOperacional.get(atendimento.getKey()) != null)
				{
					for (AtendimentoOperacional atendimentoOperacional : turmaMapAtendimentosOperacional.get(atendimento.getKey()))
					{
						atendimentoOperacional.setConfirmada(true);
					}
				}
			}
		}
	}
	
	@Override
	@Transactional
	public void desconfirmarTurmasComNumAlunos( Integer qtdeAlunos, CenarioDTO cenarioDTO ) 
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		Map<String, TriedaPar<Set<String>, Integer>> turmaMapParOfertasQtdeAlunos = new HashMap<String, TriedaPar<Set<String>, Integer>>();
		Map<String, List<AtendimentoTatico>> turmaMapAtendimentosTatico = new HashMap<String, List<AtendimentoTatico>>();
		Map<String, List<AtendimentoOperacional>> turmaMapAtendimentosOperacional = new HashMap<String, List<AtendimentoOperacional>>();
		
		for (Campus campus : campusList)
		{
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByCampus(getInstituicaoEnsinoUser(), campus);
				
				for (AtendimentoTatico atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma();
					if (turmaMapAtendimentosTatico.get(key) == null)
					{
						List<AtendimentoTatico> newAtendimentos = new ArrayList<AtendimentoTatico>();
						newAtendimentos.add(atendimento);
						turmaMapAtendimentosTatico.put(key, newAtendimentos);
					}
					else
					{
						turmaMapAtendimentosTatico.get(key).add(atendimento);
					}
					if (turmaMapParOfertasQtdeAlunos.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						turmaMapParOfertasQtdeAlunos.put(key, TriedaPar.create(newOferta, atendimento.getQuantidadeAlunos()));
					}
					else
					{
						if (!turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasQtdeAlunos.get(key).setSegundo(
									turmaMapParOfertasQtdeAlunos.get(key).getSegundo() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
				
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
							+ "-" + atendimento.getTurma();
					if (turmaMapAtendimentosOperacional.get(key) == null)
					{
						List<AtendimentoOperacional> newAtendimentos = new ArrayList<AtendimentoOperacional>();
						newAtendimentos.add(atendimento);
						turmaMapAtendimentosOperacional.put(key, newAtendimentos);
					}
					else
					{
						turmaMapAtendimentosOperacional.get(key).add(atendimento);
					}
					if (turmaMapParOfertasQtdeAlunos.get(key) == null)
					{
						Set<String> newOferta = new HashSet<String>();
						newOferta.add(atendimento.getOferta().getId() + "-" + 
								atendimento.getDisciplina().getCodigo());
						turmaMapParOfertasQtdeAlunos.put(key, TriedaPar.create(newOferta, atendimento.getQuantidadeAlunos()));
					}
					else
					{
						if (!turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().contains(atendimento.getOferta().getId()
								+ "-" + atendimento.getDisciplina().getCodigo()))
						{
							turmaMapParOfertasQtdeAlunos.get(key).setSegundo(
									turmaMapParOfertasQtdeAlunos.get(key).getSegundo() + atendimento.getQuantidadeAlunos());
							turmaMapParOfertasQtdeAlunos.get(key).getPrimeiro().add(atendimento.getOferta().getId()
									+ "-" + atendimento.getDisciplina().getCodigo());
						}
					}
				}
			}
		}
		
		for (Entry<String, TriedaPar<Set<String>, Integer>> atendimento : turmaMapParOfertasQtdeAlunos.entrySet())
		{
			if (atendimento.getValue().getSegundo() <= qtdeAlunos)
			{
				if (turmaMapAtendimentosTatico.get(atendimento.getKey()) != null)
				{
					for (AtendimentoTatico atendimentoTatico : turmaMapAtendimentosTatico.get(atendimento.getKey()))
					{
						atendimentoTatico.setConfirmada(false);
					}
				}
				else if (turmaMapAtendimentosOperacional.get(atendimento.getKey()) != null)
				{
					for (AtendimentoOperacional atendimentoOperacional : turmaMapAtendimentosOperacional.get(atendimento.getKey()))
					{
						atendimentoOperacional.setConfirmada(false);
					}
				}
			}
		}
	}
	
	@Override
	public TrioDTO<String, String, String> carregaIndicadores( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<Campus> campusList = Campus.findByCenario(getInstituicaoEnsinoUser(), cenario);
		
		int totalTurmas = 0;
		double creditosPagos = 0;
		double creditosPagosProfessoresVirtuais = 0;
		double creditosPagosProfessoresIntituicao = 0;
		int DemandaAtendida = 0;
		int numeroAlunos = 0;
		double alunosPorTurma = 0.0;
		int qtdAlunosAtendidos = 0;
		
		double receita = 0.0;
		double custoDocente = 0.0;
		double margemContribuicao = 0.0;
		double custoMedioDocente = 0.0;
		
		int professores = 0;
		double creditosPorProfessor = 0.0;
		int professoresVirtuais = 0;
		double creditosPorProfessorVirtual = 0.0;
		
		for (Campus campus : campusList)
		{
			if (campus.isOtimizado(getInstituicaoEnsinoUser()))
			{
				boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
				
				totalTurmas += ehTatico ? AtendimentoTatico.countTurma(getInstituicaoEnsinoUser(),campus) : AtendimentoOperacional.countTurma(getInstituicaoEnsinoUser(),campus);
				creditosPagosProfessoresVirtuais += ehTatico ? 0 : AtendimentoOperacional.countCreditosProfessoresVirtuais(getInstituicaoEnsinoUser(), campus);
				creditosPagosProfessoresIntituicao += ehTatico ? 0 : AtendimentoOperacional.countCreditosProfessoresInstituicao(getInstituicaoEnsinoUser(), campus);
				double creditosPagosCampus = ehTatico ? AtendimentoTatico.countCreditos(getInstituicaoEnsinoUser(), campus) : AtendimentoOperacional.countCreditos(getInstituicaoEnsinoUser(), campus);
				creditosPagos += creditosPagosCampus;
				DemandaAtendida += AlunoDemanda.sumDemandaAtendidaPorPrioridade(getInstituicaoEnsinoUser(),campus,1) + AlunoDemanda.sumDemandaAtendidaPorPrioridade(getInstituicaoEnsinoUser(),campus,2);
				numeroAlunos += Aluno.findByCampus(getInstituicaoEnsinoUser(), campus, ehTatico).size();
				receita += (ehTatico) ? AtendimentoTatico.calcReceita(getInstituicaoEnsinoUser(),campus) : AtendimentoOperacional.calcReceita(getInstituicaoEnsinoUser(),campus);
				custoDocente += (ehTatico) ? (double)creditosPagosCampus * campus.getValorCredito() * 4.5 * 6 : AtendimentoOperacional.calcCustoDocente(getInstituicaoEnsinoUser(), campus);
				
				professores += ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
						AtendimentoOperacional.countProfessores(getInstituicaoEnsinoUser(), campus) : 0);
				professoresVirtuais += ( campus.isOtimizadoOperacional(getInstituicaoEnsinoUser()) ?
						AtendimentoOperacional.countProfessoresVirtuais(getInstituicaoEnsinoUser(), campus) : 0);
				DemandasServiceImpl demandasService = new DemandasServiceImpl();
				ParDTO<Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>>,Integer> pair = demandasService.calculaQuantidadeDeNaoAtendimentosPorDemanda(campus.getOfertas());
				int qtdAlunosNaoAtendidos = pair.getSegundo();
				qtdAlunosAtendidos += (Demanda.sumDemanda(getInstituicaoEnsinoUser(),campus ) - qtdAlunosNaoAtendidos);
			}
		}
		
		alunosPorTurma = (double)qtdAlunosAtendidos/totalTurmas;
		
		margemContribuicao = (double)receita - custoDocente;
		custoMedioDocente = (professores + professoresVirtuais) == 0 ? 0 : custoDocente / (professores + professoresVirtuais);
		
		creditosPorProfessor = professores == 0 ? 0 : creditosPagosProfessoresIntituicao / professores;
		creditosPorProfessorVirtual = professoresVirtuais  == 0 ? 0 : creditosPagosProfessoresVirtuais / professoresVirtuais;
		
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();
		
		String leftResult = "Total de turmas: " + numberFormatter.print(totalTurmas,pt_BR) + "<br/>" +
					   "Créditos pagos: " + numberFormatter.print(creditosPagos,pt_BR) + "<br/>" +
				       "Demanda atendida: " + numberFormatter.print(DemandaAtendida,pt_BR) + "<br/>" + 
				       "Número de alunos: " + numberFormatter.print(numeroAlunos,pt_BR) + "<br/>" +
				       "Alunos por turma: "  + numberFormatter.print(TriedaUtil.round(alunosPorTurma,2),pt_BR);
		
		String centerResult = "Receita: " + numberFormatter.print(receita,pt_BR) + "<br/>" +
					   "Custo docente: " + numberFormatter.print(custoDocente,pt_BR) + "<br/>" +
				       "Margem de contribuição: " + numberFormatter.print(margemContribuicao,pt_BR) + "<br/>" + 
				       "Custo docente / Receita: " + numberFormatter.print(TriedaUtil.round((((double)custoDocente / receita)*100.0),2),pt_BR) + "%<br/>" +
				       "Custo médio docente: " + numberFormatter.print(TriedaUtil.round(custoMedioDocente,2),pt_BR);
		
		String rightResult = "Professores: " + numberFormatter.print(professores,pt_BR) + "<br/>" +
				   "Créditos/professor: " + numberFormatter.print(TriedaUtil.round(creditosPorProfessor,2),pt_BR) + "<br/>" +
			       "Professores virtuais: " + numberFormatter.print(professoresVirtuais,pt_BR) + "<br/>" + 
			       "Créditos/professor virtual: " + numberFormatter.print(TriedaUtil.round(creditosPorProfessorVirtual,2),pt_BR) + "<br/>";
		
		return TrioDTO.create(leftResult, centerResult, rightResult);
	}
	
	@Override
	public ListLoadResult<MotivoUsoProfessorVirtualDTO> getMotivosUsoProfessorVirtual(CenarioDTO cenarioDTO,
			Long disciplinaId, Long salaId, String turma, Boolean credTeorico)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		Disciplina disciplina = Disciplina.find(disciplinaId, getInstituicaoEnsinoUser());
		Sala sala = Sala.find(salaId, getInstituicaoEnsinoUser());
		List<AtendimentoOperacional> atendimentosOperacional = 
				AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, sala.getUnidade().getCampus(), turma, credTeorico);
		
		Set<String> motivosKeys = new HashSet<String>();
		List<MotivoUsoProfessorVirtualDTO> result = new ArrayList<MotivoUsoProfessorVirtualDTO>();
		for (AtendimentoOperacional atendimento : atendimentosOperacional)
		{
			for (MotivoUsoProfessorVirtual motivo : atendimento.getMotivoUsoProfessorVirtual())
			{
				String key = motivo.getMotivoUso() + "-" + (motivo.getProfessor() != null ? motivo.getProfessor().getId() : "");
				if (!motivosKeys.contains(key))
				{
					result.add(ConvertBeans.toMotivoUsoProfessorVirtualDTO(motivo));
					motivosKeys.add(key);
				}
			}
		}
		return new BaseListLoadResult< MotivoUsoProfessorVirtualDTO >( result );
	}
	
	@Override
	public ListLoadResult<DicaEliminacaoProfessorVirtualDTO> getDicasEliminacaoProfessorVirtual(CenarioDTO cenarioDTO,
			Long disciplinaId, Long salaId, String turma, Boolean credTeorico)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		Disciplina disciplina = Disciplina.find(disciplinaId, getInstituicaoEnsinoUser());
		Sala sala = Sala.find(salaId, getInstituicaoEnsinoUser());
		List<AtendimentoOperacional> atendimentosOperacional = 
				AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, sala.getUnidade().getCampus(), turma, credTeorico);
		
		Set<String> dicasKeys = new HashSet<String>();
		List<DicaEliminacaoProfessorVirtualDTO> result = new ArrayList<DicaEliminacaoProfessorVirtualDTO>();
		for (AtendimentoOperacional atendimento : atendimentosOperacional)
		{
			for (DicaEliminacaoProfessorVirtual dica : atendimento.getDicasEliminacaoProfessorVirtual())
			{
				String key = dica.getDicaEliminacao() + "-" + (dica.getProfessor() != null ? dica.getProfessor().getId() : "");
				if (!dicasKeys.contains(key))
				{
					result.add(ConvertBeans.toDicaEliminacaoProfessorVirtualDTO(dica));
					dicasKeys.add(key);
				}
			}
		}
		return new BaseListLoadResult< DicaEliminacaoProfessorVirtualDTO >( result );
	}
	
	@Override
	public ListLoadResult<TurmaStatusDTO> getTurmasStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO)
	{
		Demanda demanda = ConvertBeans.toDemanda(demandaDTO);
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		Set<AtendimentoTatico> atendimentosTatico = new HashSet<AtendimentoTatico>();
		Set<AtendimentoOperacional> atendimentosOperacional = new HashSet<AtendimentoOperacional>();
		Map<String, TurmaStatusDTO> turmaKeyMapTurmaStatusDTO = new HashMap<String, TurmaStatusDTO>();
		Map<String, Set<Oferta>> turmaKeyMapOferta = new HashMap<String, Set<Oferta>>();
		List<TurmaStatusDTO> result = new ArrayList<TurmaStatusDTO>();
		
		if (ehTatico)
		{
			atendimentosTatico.addAll(AtendimentoTatico.findAllByDemanda(getInstituicaoEnsinoUser(), demanda));

			Set<String> turmaEquivaleciaKey = new HashSet<String>();
			Set<AtendimentoTatico> atendimentosEquivalentes = new HashSet<AtendimentoTatico>();
			for (AtendimentoTatico atendimento : atendimentosTatico)
			{
				String key = atendimento.getHorarioAula().getId() + "-" + atendimento.getSemana() + "-" + atendimento.getSala();
				if (!turmaEquivaleciaKey.contains(key))
				{
					atendimentosEquivalentes.addAll(AtendimentoTatico.findAllBy(getInstituicaoEnsinoUser(), atendimento.getHorarioAula(),
							atendimento.getSemana(), atendimento.getSala(), atendimento.getOferta().getCampus()));
					
					turmaEquivaleciaKey.add(key);
				}
			}
			atendimentosTatico.addAll(atendimentosEquivalentes);
			
			for (AtendimentoTatico atendimento : atendimentosTatico)
			{
				String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
						+ "-" + atendimento.getTurma();
				
				if (turmaKeyMapTurmaStatusDTO.get(key) == null)
				{
					TurmaStatusDTO newTurmaStatus = new TurmaStatusDTO();
					
					newTurmaStatus.setDisplayText(atendimento.getTurma() + " (" + (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo()
							: atendimento.getDisciplina().getCodigo()) + ")");
					newTurmaStatus.setNome(atendimento.getTurma());
					newTurmaStatus.setQtdeDiscSelecionada(0);
					if (atendimento.getDisciplina().equals(demanda.getDisciplina()))
						newTurmaStatus.setQtdeDiscSelecionada(newTurmaStatus.getQtdeDiscSelecionada() + atendimento.getQuantidadeAlunos());
					newTurmaStatus.setQtdeTotal(atendimento.getQuantidadeAlunos());
					newTurmaStatus.setStatus(atendimento.getConfirmada() ? "Planejada" : "Não Planejada");
					newTurmaStatus.setDisciplinaId(atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getId()
							: atendimento.getDisciplina().getId());
					newTurmaStatus.setTurma(atendimento.getTurma());
					
					turmaKeyMapTurmaStatusDTO.put(key, newTurmaStatus);
					Set<Oferta> ofertas = new HashSet<Oferta>();
					ofertas.add(atendimento.getOferta());
					turmaKeyMapOferta.put(key, ofertas);
				}
				if(!turmaKeyMapOferta.get(key).contains(atendimento.getOferta()))
				{
					TurmaStatusDTO newTurmaStatus = turmaKeyMapTurmaStatusDTO.get(key);
					if (atendimento.getDisciplina().equals(demanda.getDisciplina()))
						newTurmaStatus.setQtdeDiscSelecionada(newTurmaStatus.getQtdeDiscSelecionada() + atendimento.getQuantidadeAlunos());
					newTurmaStatus.setQtdeTotal(newTurmaStatus.getQtdeTotal() + atendimento.getQuantidadeAlunos());
					
					turmaKeyMapTurmaStatusDTO.put(key, newTurmaStatus);
					
					turmaKeyMapOferta.get(key).add(atendimento.getOferta());
				}
			}
		}
		else
		{
			atendimentosOperacional.addAll(AtendimentoOperacional.findAllByDemanda(getInstituicaoEnsinoUser(), demanda));
			
			Set<String> turmaEquivaleciaKey = new HashSet<String>();
			Set<AtendimentoOperacional> atendimentosEquivalentes = new HashSet<AtendimentoOperacional>();
			for (AtendimentoOperacional atendimento : atendimentosOperacional)
			{
				String key = atendimento.getHorarioDisponivelCenario().getId() + "-" + atendimento.getSala();
				if (!turmaEquivaleciaKey.contains(key))
				{
					atendimentosEquivalentes.addAll(AtendimentoOperacional.findAllBy(getInstituicaoEnsinoUser(), atendimento.getHorarioDisponivelCenario()
							, atendimento.getSala(), atendimento.getOferta().getCampus()));
					
					turmaEquivaleciaKey.add(key);
				}
			}
			atendimentosOperacional.addAll(atendimentosEquivalentes);
			
			for (AtendimentoOperacional atendimento : atendimentosOperacional)
			{
				String key = (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : atendimento.getDisciplina().getCodigo())
						+ "-" + atendimento.getTurma();
				
				if (turmaKeyMapTurmaStatusDTO.get(key) == null)
				{
					TurmaStatusDTO newTurmaStatus = new TurmaStatusDTO();
					
					newTurmaStatus.setDisplayText(atendimento.getTurma() + " (" + (atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo()
							: atendimento.getDisciplina().getCodigo()) + ")");
					newTurmaStatus.setNome(atendimento.getTurma());
					newTurmaStatus.setQtdeDiscSelecionada(0);
					if (atendimento.getDisciplina().equals(demanda.getDisciplina()))
						newTurmaStatus.setQtdeDiscSelecionada(newTurmaStatus.getQtdeDiscSelecionada() + atendimento.getQuantidadeAlunos());
					newTurmaStatus.setQtdeTotal(atendimento.getQuantidadeAlunos());
					newTurmaStatus.setStatus(atendimento.getConfirmada() ? "Planejada" : "Não Planejada");
					newTurmaStatus.setDisciplinaId(atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getId()
							: atendimento.getDisciplina().getId());
					newTurmaStatus.setTurma(atendimento.getTurma());
					
					turmaKeyMapTurmaStatusDTO.put(key, newTurmaStatus);
					Set<Oferta> ofertas = new HashSet<Oferta>();
					ofertas.add(atendimento.getOferta());
					turmaKeyMapOferta.put(key, ofertas);
				}
				if(!turmaKeyMapOferta.get(key).contains(atendimento.getOferta()))
				{
					TurmaStatusDTO newTurmaStatus = turmaKeyMapTurmaStatusDTO.get(key);
					if (atendimento.getDisciplina().equals(demanda.getDisciplina()))
						newTurmaStatus.setQtdeDiscSelecionada(newTurmaStatus.getQtdeDiscSelecionada() + atendimento.getQuantidadeAlunos());
					newTurmaStatus.setQtdeTotal(newTurmaStatus.getQtdeTotal() + atendimento.getQuantidadeAlunos());
					
					turmaKeyMapTurmaStatusDTO.put(key, newTurmaStatus);
					
					turmaKeyMapOferta.get(key).add(atendimento.getOferta());
				}
			}
		}
		Set<Turma> turmas = new HashSet<Turma>();
		turmas.addAll(Turma.findBy(getInstituicaoEnsinoUser(), demanda.getDisciplina().getCenario(), demanda.getDisciplina(), null));
		turmas.addAll(Turma.findByEquivalencia(getInstituicaoEnsinoUser(), demanda.getDisciplina().getCenario(), demanda.getDisciplina(), demanda.getOferta().getCampus()));
		for (Turma turma : turmas)
		{
			TurmaStatusDTO newTurmaStatus = new TurmaStatusDTO();

			newTurmaStatus.setId(turma.getId());
			newTurmaStatus.setDisplayText(turma.getNome() + " (" + turma.getDisciplina().getCodigo() + ")");
			newTurmaStatus.setNome(turma.getNome());
			int alunosDiscSelecionada = turma.getAlunos().size();
			for (AlunoDemanda alunoDemanda : turma.getAlunos())
			{
				if (!alunoDemanda.getDemanda().getDisciplina().equals(demanda.getDisciplina()))
					alunosDiscSelecionada--;
			}
			newTurmaStatus.setQtdeDiscSelecionada(alunosDiscSelecionada);
			newTurmaStatus.setQtdeTotal(turma.getAlunos().size());
			newTurmaStatus.setStatus("Parcial");
			newTurmaStatus.setDisciplinaId(turma.getDisciplina().getId());
			
			result.add(newTurmaStatus);
		}
		
		
		result.addAll(turmaKeyMapTurmaStatusDTO.values());
		return new BaseListLoadResult< TurmaStatusDTO >( result );
	}
	
	@Override
	@Transactional
	public void deleteTurmasStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, List<TurmaStatusDTO> turmasStatusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Demanda demanda = ConvertBeans.toDemanda(demandaDTO);
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		for (TurmaStatusDTO turmaStatusDTO : turmasStatusDTO)
		{
			Disciplina disciplina = Disciplina.find(turmaStatusDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
			if (turmaStatusDTO.getTurma() != null)
			{
				if (ehTatico)
				{
					List<AtendimentoTatico> atendimentos = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, demanda.getOferta().getCampus(), turmaStatusDTO.getTurma());
					for (AtendimentoTatico atendimento : atendimentos)
					{
						for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemanda())
						{
							alunoDemanda.setAtendido(false);
						}
						atendimento.remove();				
					}
				}
				else
				{
					List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, demanda.getOferta().getCampus(), turmaStatusDTO.getTurma(), null);
					for (AtendimentoOperacional atendimento : atendimentos)
					{
						for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemanda())
						{
							alunoDemanda.setAtendido(false);
						}
						atendimento.remove();
					}
				}
			}
			else
			{
				Turma turma = Turma.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, turmaStatusDTO.getNome()).get(0);
				
				turma.remove();
			}
		}
	}
	
	@Override
	@Transactional
	public void deleteTurmaSelecionada(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, TurmaDTO turmaDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Demanda demanda = ConvertBeans.toDemanda(demandaDTO);
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		Disciplina disciplina = Disciplina.find(turmaDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
		if (turmaDTO.getId() == null)
		{
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, demanda.getOferta().getCampus(), turmaDTO.getNome());
				for (AtendimentoTatico atendimento : atendimentos)
				{
					for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemanda())
					{
						alunoDemanda.setAtendido(false);
					}
					atendimento.remove();				
				}
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, demanda.getOferta().getCampus(), turmaDTO.getNome(), null);
				for (AtendimentoOperacional atendimento : atendimentos)
				{
					for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemanda())
					{
						alunoDemanda.setAtendido(false);
					}
					atendimento.remove();
				}
			}
		}
		else
		{
			Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
			
			turma.remove();
		}
	}
	
	@Override
	@Transactional
	public TurmaDTO saveTurma(TurmaDTO turmaDTO) throws TriedaException
	{
		Turma turma = ConvertBeans.toTurma(turmaDTO);
		Campus campus = Campus.find(turmaDTO.getCampusId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> turmasOperacional = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), turma.getCenario(), turma.getDisciplina(), campus, null, turma.getNome());
		List<AtendimentoTatico> turmasTatico = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), turma.getCenario(), turma.getDisciplina(), campus, null, turma.getNome());
		List<Turma> turmas = Turma.findBy(getInstituicaoEnsinoUser(), turma.getCenario(), turma.getDisciplina(), turma.getNome());
		
		if (!turmasOperacional.isEmpty() || !turmasTatico.isEmpty() || !turmas.isEmpty())
		{
			throw new TriedaException("Já existe uma turma com esse nome");
		}
		
		
		turma.persist();
		
		return ConvertBeans.toTurmaDTO(turma);
	}
	
	@Override
	@Transactional
	public void editTurma(TurmaDTO turmaDTO, String turmaEditadaNome)
	{
		Campus campus = Campus.find(turmaDTO.getCampusId(), getInstituicaoEnsinoUser());
		
		if (turmaDTO.getId() != null
			&& turmaDTO.getId() > 0)
		{
			Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
			turma.setNome(turmaDTO.getNome());
			turma.merge();
		}
		else
		{
			Turma turma = ConvertBeans.toTurma(turmaDTO);
			
			List<AtendimentoOperacional> turmasOperacional = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), turma.getCenario(), turma.getDisciplina(), campus, null, turmaEditadaNome);
			List<AtendimentoTatico> turmasTatico = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), turma.getCenario(), turma.getDisciplina(), campus, null, turmaEditadaNome);
			
			boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			
			if (ehTatico)
			{
				for (AtendimentoTatico atendimento : turmasTatico)
				{
					atendimento.setTurma(turmaDTO.getNome());
					atendimento.merge();
				}
			}
			else
			{
				for (AtendimentoOperacional atendimento : turmasOperacional)
				{
					atendimento.setTurma(turmaDTO.getNome());
					atendimento.merge();
				}
			}
		}
	}
	
	@Override
	public ParDTO<TurmaDTO, List<AulaDTO>> selecionarTurma(TurmaStatusDTO turmaStatusDTO, CenarioDTO cenarioDTO, DemandaDTO demandaDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Demanda demanda = ConvertBeans.toDemanda(demandaDTO);
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());

		Disciplina disciplina = Disciplina.find(turmaStatusDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
		if (turmaStatusDTO.getTurma() != null)
		{
			if (ehTatico)
			{
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, demanda.getOferta().getCampus(), turmaStatusDTO.getTurma());
				
				return  createTurmaTatico(atendimentos);
			}
			else
			{
				List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, demanda.getOferta().getCampus(), turmaStatusDTO.getTurma(), null);

				return  createTurmaOperacional(atendimentos);
			}
		}
		else
		{
			Turma turma = Turma.findBy(getInstituicaoEnsinoUser(), cenario, disciplina, turmaStatusDTO.getNome()).get(0);
			
			
			List<Aula> aulas = new ArrayList<Aula>(turma.getAulas());
			return ParDTO.create(ConvertBeans.toTurmaDTO(turma), ConvertBeans.toListAulasDTO(aulas));
		}
	}

	private ParDTO<TurmaDTO, List<AulaDTO>> createTurmaOperacional(List<AtendimentoOperacional> atendimentos) {
		Turma turma = new Turma();
		
		turma.setDisciplina(atendimentos.get(0).getDisciplinaSubstituta() == null ? atendimentos.get(0).getDisciplina() : atendimentos.get(0).getDisciplinaSubstituta());
		turma.setCenario(atendimentos.get(0).getCenario());
		turma.setNome(atendimentos.get(0).getTurma());
		turma.setParcial(false);
		
		List<AulaDTO> aulasDTO = new ArrayList<AulaDTO>();
		List<AtendimentoOperacionalDTO> aulas = extraiAulas(ConvertBeans.toListAtendimentoOperacionalDTO(atendimentos));
		int credAlocados = 0;
		int noAlunos = 0;
		Set<Long> ofertas = new HashSet<Long>();
		Set<String> horarioSemanaKey = new HashSet<String>();
		for (AtendimentoOperacionalDTO aula : aulas)
		{
			if (!ofertas.contains(aula.getOfertaId()))
			{
				noAlunos += aula.getQuantidadeAlunos();
				ofertas.add(aula.getOfertaId());
			}
			if (!horarioSemanaKey.contains(aula.getHorarioAulaId() + "-" + aula.getSemana()))
			{
				aulasDTO.add(ConvertBeans.toAulaDTO(aula));
				credAlocados += aula.getTotalCreditos();
				horarioSemanaKey.add(aula.getHorarioAulaId() + "-" + aula.getSemana());
			}
		}
		
		TurmaDTO turmaDTO = ConvertBeans.toTurmaDTO(turma);
		turmaDTO.setCredAlocados(credAlocados);
		turmaDTO.setNoAlunos(noAlunos);
		
		return ParDTO.create(turmaDTO, aulasDTO);
	}
	
	private ParDTO<TurmaDTO, List<AulaDTO>> createTurmaTatico(List<AtendimentoTatico> atendimentos) {
		Turma turma = new Turma();
		
		turma.setDisciplina(atendimentos.get(0).getDisciplinaSubstituta() == null ? atendimentos.get(0).getDisciplina() : atendimentos.get(0).getDisciplinaSubstituta());
		turma.setCenario(atendimentos.get(0).getCenario());
		turma.setNome(atendimentos.get(0).getTurma());
		turma.setParcial(false);
		
		List<AulaDTO> aulasDTO = new ArrayList<AulaDTO>();
		int credAlocados = 0;
		int noAlunos = 0;
		Set<Long> ofertas = new HashSet<Long>();
		Set<String> horarioSemanaKey = new HashSet<String>();
		for (AtendimentoTatico aula : atendimentos)
		{
			if (!ofertas.contains(aula.getOferta().getId()))
			{
				noAlunos += aula.getQuantidadeAlunos();
				ofertas.add(aula.getOferta().getId());
			}
			if (!horarioSemanaKey.contains(aula.getHorarioAula() + "-" + aula.getSemana()))
			{
				aulasDTO.add(ConvertBeans.toAulaDTO(aula));
				credAlocados += aula.getCreditosPratico() + aula.getCreditosTeorico();
				horarioSemanaKey.add(aula.getHorarioAula() + "-" + aula.getSemana());
			}
		}
		
		TurmaDTO turmaDTO = ConvertBeans.toTurmaDTO(turma);
		turmaDTO.setCredAlocados(credAlocados);
		turmaDTO.setNoAlunos(noAlunos);
		
		return ParDTO.create(turmaDTO, aulasDTO);
	}

	@Override
	public ListLoadResult<AlunoStatusDTO> getAlunosStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, TurmaDTO turmaDTO, List<AulaDTO> aulasDTO)
	{
		List<AlunoStatusDTO> result = new ArrayList<AlunoStatusDTO>();
		if (turmaDTO == null)
		{
			return new BaseListLoadResult<AlunoStatusDTO>(result);
		}
		Disciplina disciplina = Disciplina.find(turmaDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
		if (turmaDTO.getCredAlocados() < disciplina.getTotalCreditos())
		{
			return new BaseListLoadResult<AlunoStatusDTO>(result);
		}
		
		Demanda demanda = Demanda.find(demandaDTO.getId(), getInstituicaoEnsinoUser());
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		List<AlunoDemanda> alunosDemanda = new ArrayList<AlunoDemanda>();
		alunosDemanda.addAll(AlunoDemanda.findByDisciplinaCampusAndTurma(getInstituicaoEnsinoUser(), disciplina, 
				demanda.getOferta().getCampus(), turmaDTO.getNome(), turmaDTO.getId()));
		List<AlunoDemanda> alunosDiscEquivalentes =
				AlunoDemanda.findAlunosEquivalentes(getInstituicaoEnsinoUser(), cenario, disciplina, turmaDTO.getNome(), turmaDTO.getId() );
		alunosDemanda.addAll(alunosDiscEquivalentes);
		for (AlunoDemanda alunoDemanda : alunosDemanda)
		{
			AlunoStatusDTO alunoStatusDTO = new AlunoStatusDTO();
			
			alunoStatusDTO.setAlunoDemandaId(alunoDemanda.getId());
			alunoStatusDTO.setAlunoId(alunoDemanda.getAluno().getId());
			alunoStatusDTO.setMarcado(alunoDemanda.getAtendido() || !alunoDemanda.getTurmas().isEmpty());
			alunoStatusDTO.setMatricula(alunoDemanda.getAluno().getMatricula());
			alunoStatusDTO.setNome(alunoDemanda.getAluno().getNome());
			alunoStatusDTO.setStatus(alunoDemanda.getAluno().getNome());
			String status = "Hor. Livre";
			if (alunoDemanda.getAtendido() || !alunoDemanda.getTurmas().isEmpty())
			{
				status = "Alocado";
			}
			else {
				Set<HorarioDisponivelCenario> horariosDisponiveisAluno = findHorariosDisponiveis(alunoDemanda.getAluno(), ehTatico);
				for (AulaDTO aula : aulasDTO)
				{
					if (isHorarioConflitante(horariosDisponiveisAluno, HorarioDisponivelCenario.find(aula.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser())))
					{
						status = "Conflito";
					}
				}
			}
			alunoStatusDTO.setStatus(status);
			alunoStatusDTO.setFormando(alunoDemanda.getAluno().getFormando());
			if (!alunoDemanda.getDemanda().getDisciplina().equals(disciplina))
			{
				alunoStatusDTO.setEquivalenciaId(alunoDemanda.getDemanda().getDisciplina().getId());
				alunoStatusDTO.setEquivalenciaSring(alunoDemanda.getDemanda().getDisciplina().getCodigo());
			}
			result.add(alunoStatusDTO);
		}
		return new BaseListLoadResult<AlunoStatusDTO>(result);
	}

	private Set<HorarioDisponivelCenario> findHorariosDisponiveis(Aluno aluno, boolean ehTatico)
	{
		Set<HorarioDisponivelCenario> horariosDisponiveis = new HashSet<HorarioDisponivelCenario>();
		if (ehTatico)
		{
			List<AtendimentoTatico> atendimentos = AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), aluno, null, null);
			//TODO pensar em uma forma de adicionar num creditos tambem
			for(AtendimentoTatico atendimento : atendimentos)
			{
				horariosDisponiveis.add(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), atendimento.getHorarioAula(), atendimento.getSemana()));
			}
		}
		else
		{
			List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findBy(aluno, null, null, getInstituicaoEnsinoUser());
			
			for(AtendimentoOperacional atendimento : atendimentos)
			{
				horariosDisponiveis.add(atendimento.getHorarioDisponivelCenario());
			}
		}
		
		return horariosDisponiveis;
	}
	
	@Override
	public ListLoadResult<SalaStatusDTO> getAmbientesTurma(CenarioDTO cenarioDTO, TurmaDTO turmaDTO, List<AulaDTO> aulas, CampusDTO campusDTO)
	{
		List<SalaStatusDTO> result = new ArrayList<SalaStatusDTO>();
		if (turmaDTO == null)
		{
			return new BaseListLoadResult<SalaStatusDTO>(result);
		}
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		Disciplina disciplina = Disciplina.find(turmaDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
		Collection<Sala> salas = disciplina.getSalas().isEmpty() ? Sala.findByCampus(getInstituicaoEnsinoUser(), cenario, campus) : disciplina.getSalas();
		for (Sala sala : salas)
		{
			SalaStatusDTO salaStatus = new SalaStatusDTO();
			salaStatus.setId(sala.getId());
			salaStatus.setVersion(sala.getVersion());
			salaStatus.setCodigo(sala.getCodigo());
			salaStatus.setCapacidadeInstalada(sala.getCapacidadeInstalada());
			salaStatus.setTipoString(sala.getTipoSala().getNome());
			
			String status = "";
			
			for (AulaDTO aulaDTO : aulas)
			{
				HorarioAula horarioAula = HorarioAula.find(aulaDTO.getHorarioAulaId(), getInstituicaoEnsinoUser());
				if (aulaDTO.getSalaId() != null && sala.getId().equals(aulaDTO.getSalaId()))
				{
						status = "Alocado";
				}
				
				else if (!sala.estaDisponivelNoHorario(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), horarioAula, Semanas.get(aulaDTO.getSemana()))))
				{
					status = "Indisponível";
				}
				else {
					Set<HorarioDisponivelCenario> horariosDisponiveisAluno =
							findHorariosDisponiveis(sala.getUnidade().getCampus().getCenario(), sala, sala.getUnidade().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser()));
					if (isHorarioConflitante(horariosDisponiveisAluno, HorarioDisponivelCenario.find(aulaDTO.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser())))
					{
						status = "Conflito";
					}
					else
					{
						status = "Disponível";
					}
				}
			}
			salaStatus.setStatus(status);
			
			result.add(salaStatus);
		}
		return new BaseListLoadResult<SalaStatusDTO>(result);
	}
	
	private boolean isHorarioConflitante(Collection<HorarioDisponivelCenario> horarios, HorarioDisponivelCenario horarioAula)
	{
		for (HorarioDisponivelCenario horario : horarios)
		{
			if(horario.getHorarioAula().compareTo(horarioAula.getHorarioAula()) == 0 && horario.getDiaSemana().equals(horarioAula.getDiaSemana()))
			{
				return true;
			}
		}
		
		return false;
	}
	
	private Set<HorarioDisponivelCenario> findHorariosDisponiveis(Cenario cenario, Sala sala, boolean ehTatico)
	{
		Set<HorarioDisponivelCenario> horariosDisponiveis = new HashSet<HorarioDisponivelCenario>();
		if (!ehTatico)
		{
			List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findBySalaAndTurno(sala, null, null, getInstituicaoEnsinoUser(), cenario);
			for(AtendimentoOperacional atendimento : atendimentos)
			{
				horariosDisponiveis.add(atendimento.getHorarioDisponivelCenario());
			}
		}
		
		return horariosDisponiveis;
	}
	
	@Override
	public ListLoadResult<ProfessorStatusDTO> getProfessoresStatus(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO)
	{
		List<ProfessorStatusDTO> result = new ArrayList<ProfessorStatusDTO>();
		if (turmaDTO == null || aulaDTO == null)
		{
			return new BaseListLoadResult<ProfessorStatusDTO>(result);
		}
		Disciplina disciplina = Disciplina.find(turmaDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
		if (turmaDTO.getCredAlocados() < disciplina.getTotalCreditos())
		{
			return new BaseListLoadResult<ProfessorStatusDTO>(result);
		}
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Demanda demanda = Demanda.find(demandaDTO.getId(), getInstituicaoEnsinoUser());
		HorarioAula horarioAula = HorarioAula.find(aulaDTO.getHorarioAulaId(), getInstituicaoEnsinoUser());
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		List<ProfessorDisciplina> professoresDisciplinas = ProfessorDisciplina.findBy(getInstituicaoEnsinoUser(), cenario, disciplina);

		for (ProfessorDisciplina professorDisciplina :  professoresDisciplinas)
		{
			ProfessorStatusDTO professorStatusDTO = new ProfessorStatusDTO();
			
			professorStatusDTO.setProfessorId(professorDisciplina.getProfessor().getId());
			professorStatusDTO.setCpf(professorDisciplina.getProfessor().getCpf());
			professorStatusDTO.setTitulacao(professorDisciplina.getProfessor().getTitulacao().getNome());
			professorStatusDTO.setCusto(0);
			professorStatusDTO.setMarcado(false);
			professorStatusDTO.setNota(professorDisciplina.getNota());
			professorStatusDTO.setPreferencia(professorDisciplina.getPreferencia());
			professorStatusDTO.setNome(professorDisciplina.getProfessor().getNome());

			String status = "";
			if (aulaDTO.getProfessorId() != null && professorDisciplina.getProfessor().getId().equals(aulaDTO.getProfessorId()))
			{
					status = "Alocado";
					professorStatusDTO.setMarcado(true);
			}
			else if (!professorDisciplina.getProfessor().estaDisponivelNoHorario(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), horarioAula, Semanas.get(aulaDTO.getSemana()))))
			{
				status = "Indisponível";
			}
			else {
				Set<HorarioDisponivelCenario> horariosDisponiveisAluno = findHorariosDisponiveis(cenario, professorDisciplina.getProfessor(), ehTatico);
				if (isHorarioConflitante(horariosDisponiveisAluno, HorarioDisponivelCenario.find(aulaDTO.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser())))
				{
					status = "Conflito";
				}
				else
				{
					status = "Disponível";
				}
			}
			professorStatusDTO.setStatus(status);
			
			result.add(professorStatusDTO);
		}
		
		List<ProfessorVirtual> professoresVirtuais = ProfessorVirtual.findBy(getInstituicaoEnsinoUser(), cenario);
		for (ProfessorVirtual professorVirtual : professoresVirtuais)
		{
			if (professorVirtual.getDisciplinas().contains(disciplina))
			{
				ProfessorStatusDTO professorStatusDTO = new ProfessorStatusDTO();
				ProfessorVirtualDTO professorDTO = ConvertBeans.toProfessorVirtualDTO(professorVirtual);
				
				professorStatusDTO.setProfessorVirtualId(professorDTO.getId());
				professorStatusDTO.setCpf("");
				professorStatusDTO.setTitulacao(professorDTO.getTitulacaoString());
				professorStatusDTO.setCusto(0);
				professorStatusDTO.setMarcado(false);
				professorStatusDTO.setNota(null);
				professorStatusDTO.setPreferencia(null);
				professorStatusDTO.setNome(professorDTO.getNome());
				
				String status = "";
				Set<HorarioDisponivelCenario> horariosProfessor = new HashSet<HorarioDisponivelCenario>();
				for (AtendimentoOperacional atendimento : AtendimentoOperacional.findAllBy(demanda.getOferta().getCampus(), cenario, professorVirtual, getInstituicaoEnsinoUser()))
				{
					horariosProfessor.add(atendimento.getHorarioDisponivelCenario());
				}

				if (aulaDTO.getProfessorVirtualId() != null)
				{
					if (professorDTO.getId().equals(aulaDTO.getProfessorId()))
					{
						status = "Alocado";
						professorStatusDTO.setMarcado(true);
					}
				}
				else if (!horariosProfessor.contains(HorarioDisponivelCenario.find(aulaDTO.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser())))
				{
					status = "Conflito";
				}
				else
				{
					status = "Disponível";
				}
				
				professorStatusDTO.setStatus(status);
				
				result.add(professorStatusDTO);
			}
		}
		
		
		return new BaseListLoadResult<ProfessorStatusDTO>(result);
	}
	
	private Set<HorarioDisponivelCenario> findHorariosDisponiveis(Cenario cenario, Professor professor, boolean ehTatico)
	{
		Set<HorarioDisponivelCenario> horariosDisponiveis = new HashSet<HorarioDisponivelCenario>();
		if (!ehTatico)
		{
			List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(getInstituicaoEnsinoUser(), cenario, professor);
			
			for(AtendimentoOperacional atendimento : atendimentos)
			{
				horariosDisponiveis.add(atendimento.getHorarioDisponivelCenario());
			}
		}
		
		return horariosDisponiveis;
	}
	
	@Override
	public ListLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveisAula(CenarioDTO cenarioDTO, SalaDTO salaDTO, DisciplinaDTO disciplinaDTO, SemanaLetivaDTO semanaLetivaDTO, String semana)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Sala sala = Sala.find(salaDTO.getId(), getInstituicaoEnsinoUser());
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(), getInstituicaoEnsinoUser());
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId(), getInstituicaoEnsinoUser());
		Semanas diaSemana = Semanas.get(semana);
		
		List<HorarioDisponivelCenario> hdc = getHorariosDisponiveis(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), cenario, semanaLetiva, diaSemana),
				disciplina.getDisponibilidades(), sala.getDisponibilidades());
		
		List<HorarioDisponivelCenarioDTO> result = ConvertBeans.toHorarioDisponivelCenarioDTO(hdc);
		Collections.sort(result,  new Comparator<HorarioDisponivelCenarioDTO>() {
			@Override
			public int compare(HorarioDisponivelCenarioDTO arg1, HorarioDisponivelCenarioDTO arg2) {
				return arg1.getHorarioInicioString().compareTo(arg2.getHorarioInicioString());
			}
		});
		return new BaseListLoadResult<HorarioDisponivelCenarioDTO>(result);
	}
	
	private List<HorarioDisponivelCenario> getHorariosDisponiveis( List<HorarioDisponivelCenario> hdc,
			Set<DisponibilidadeDisciplina> disponibilidadesDisc,
			Set<DisponibilidadeSala> disponibilidadesSala) {
		List<HorarioDisponivelCenario> horariosDisponiveis = new ArrayList<HorarioDisponivelCenario>();
		for (HorarioDisponivelCenario horarioDisponivel : hdc)
		{
			boolean disponivelDisc = false;
			boolean disponivelSala = false;
			for (Disponibilidade dispDisc : disponibilidadesDisc)
			{
				if (checkHorarioDisponivelCenarioDisponvivel(horarioDisponivel, dispDisc))
					disponivelDisc = true;
			}
			for (Disponibilidade dispSala : disponibilidadesSala)
			{
				if (checkHorarioDisponivelCenarioDisponvivel(horarioDisponivel, dispSala))
					disponivelSala = true;
			}
			if (disponivelDisc && disponivelSala)
				horariosDisponiveis.add(horarioDisponivel);
		}
		
		return horariosDisponiveis;
	}
	
	private boolean checkHorarioDisponivelCenarioDisponvivel(
			HorarioDisponivelCenario hdc, Disponibilidade disponibilidade) {

		Calendar horaInicio = Calendar.getInstance();
		horaInicio.setTime(disponibilidade.getHorarioInicio());
		horaInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(disponibilidade.getHorarioFim());
		horaFim.set(1979,Calendar.NOVEMBER,6);
		
		Calendar oHoraInicio = Calendar.getInstance();
		oHoraInicio.setTime(hdc.getHorarioAula().getHorario());
		oHoraInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar oHoraFim = Calendar.getInstance();
		oHoraFim.setTime(hdc.getHorarioAula().getHorario());
		oHoraFim.set(1979,Calendar.NOVEMBER,6);
		oHoraFim.add(Calendar.MINUTE,hdc.getHorarioAula().getSemanaLetiva().getTempo());

		return (horaInicio.compareTo(oHoraInicio) <= 0 && horaFim.compareTo(oHoraFim) >= 0 );
	}

	@Override
	public TrioDTO<Boolean, List<String>, List<String>> verificaViabilidadeAula(CenarioDTO cenarioDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO, List<AulaDTO> aulasTurma)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Turma turma = ConvertBeans.toTurma(turmaDTO);
		Aula aula = ConvertBeans.toAula(aulaDTO);
		
		Map<String, Long> deslocamentoUnidadesMapTempo = DeslocamentoUnidade.buildDeslocamentoUnidadesMapTempo(getInstituicaoEnsinoUser(), cenario);
		
		for (AulaDTO aulaTurma : aulasTurma)
		{
			if(aulaDTO.getSemana().equals(aulaTurma.getSemana()))
			{
				List<String> motivosIncompatibilidadeFortes = new ArrayList<String>();
				List<String> motivosIncompatibilidadeFracos = new ArrayList<String>();
				String motivoIncompatibilidade = "A aula a ser criada ocorre no mesmo dia (" + aulaDTO.getSemanaString() + 
						") de uma aula ja existente para essa turma, edite a aula ja existente caso queira adicionar créditos a mesma.";
				motivosIncompatibilidadeFortes.add(motivoIncompatibilidade);
				return TrioDTO.create(false, motivosIncompatibilidadeFortes, motivosIncompatibilidadeFracos);
			}
		}
		
		TriedaTrio<Boolean, List<String>, List<String>> trio = aula.ehViavel(deslocamentoUnidadesMapTempo, turma, criaAulasAPartirDePossiveisAtendmentosConflitantes(aula, turma), cenario.getUltimoParametro(getInstituicaoEnsinoUser()));
		
		return TrioDTO.create(trio.getPrimeiro(), trio.getSegundo(), trio.getTerceiro());
	}
	
	@Override
	public TrioDTO<Boolean, List<String>, List<String>> verificaViabilidadeAulasNovosAlunos(CenarioDTO cenarioDTO, TurmaDTO turmaDTO, List<AulaDTO> aulasDTO, List<AlunoStatusDTO> alunos)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Turma turma = ConvertBeans.toTurma(turmaDTO);
		Map<String, Long> deslocamentoUnidadesMapTempo = DeslocamentoUnidade.buildDeslocamentoUnidadesMapTempo(getInstituicaoEnsinoUser(), cenario);
		for (AlunoStatusDTO aluno : alunos)
		{
			turma.getAlunos().add(AlunoDemanda.find(aluno.getAlunoDemandaId(), getInstituicaoEnsinoUser()));
		}
		
		List<String> errors = new ArrayList<String>();
		List<String> warnings = new ArrayList<String>();
		TriedaTrio<Boolean, List<String>, List<String>> result = TriedaTrio.create(true, errors, warnings);
		for (AulaDTO aulaDTO : aulasDTO)
		{
			Aula aula = ConvertBeans.toAula(aulaDTO);
			TriedaTrio<Boolean, List<String>, List<String>> trio = aula.ehViavel(deslocamentoUnidadesMapTempo, turma,
					criaAulasAPartirDePossiveisAtendmentosConflitantes(aula, turma), cenario.getUltimoParametro(getInstituicaoEnsinoUser()));
			if (!trio.getPrimeiro())
				result.setPrimeiro(trio.getPrimeiro());
			result.getSegundo().addAll(trio.getSegundo());
			result.getTerceiro().addAll(trio.getTerceiro());
		}
		
		return TrioDTO.create(result.getPrimeiro(), result.getSegundo(), result.getTerceiro());
	}
	
	@Override
	public TrioDTO<Boolean, List<String>, List<String>> verificaViabilidadeAulasNovoProfessor(CenarioDTO cenarioDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO, ProfessorStatusDTO professorStatusDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Turma turma = ConvertBeans.toTurma(turmaDTO);
		Aula aula = ConvertBeans.toAula(aulaDTO);
		Map<String, Long> deslocamentoUnidadesMapTempo = DeslocamentoUnidade.buildDeslocamentoUnidadesMapTempo(getInstituicaoEnsinoUser(), cenario);
		if (professorStatusDTO.getProfessorId() != null)
		{
			aula.setProfessor(Professor.find(professorStatusDTO.getProfessorId(), getInstituicaoEnsinoUser()));
		}
		else
		{
			aula.setProfessorVirtual(ProfessorVirtual.find(professorStatusDTO.getProfessorVirtualId(), getInstituicaoEnsinoUser()));
		}

		TriedaTrio<Boolean, List<String>, List<String>> trio = aula.ehViavel(deslocamentoUnidadesMapTempo, turma,
				criaAulasAPartirDePossiveisAtendmentosConflitantes(aula, turma), cenario.getUltimoParametro(getInstituicaoEnsinoUser()));
		
		return TrioDTO.create(trio.getPrimeiro(), trio.getSegundo(), trio.getTerceiro());
	}
	
	private Map<String, Aula> criaAulasAPartirDePossiveisAtendmentosConflitantes(Aula aula, Turma turma) 
	{
		Set<AtendimentoOperacional> atendimentosSet = new HashSet<AtendimentoOperacional>();
		Set<AtendimentoTatico> atendimentosTaticoSet = new HashSet<AtendimentoTatico>();
		Map<String, Aula> result = new HashMap<String, Aula>();
		
		if (aula.getProfessor() == null && aula.getProfessorVirtual() == null)
		{
			for(AtendimentoTatico atendimento : AtendimentoTatico.findByCenario(getInstituicaoEnsinoUser(), aula.getCenario(), aula.getSala(), null))
			{
				Long disciplinaId = atendimento.getDisciplinaSubstituta() != null ?
						atendimento.getDisciplinaSubstituta().getId() :  atendimento.getDisciplina().getId();
				if (atendimento.getSemana().equals(aula.getHorarioDisponivelCenario().getDiaSemana())
						&& (!atendimento.getTurma().equals(turma.getNome()) || !disciplinaId.equals(turma.getDisciplina().getId())))
				{
					atendimentosTaticoSet.add(atendimento);
				}
			}
			
			for (AtendimentoTatico atendimento : atendimentosTaticoSet)
			{
				Aula newAula = new Aula();
				
				newAula.setCenario(aula.getCenario());
				newAula.setSala(atendimento.getSala());
				newAula.setHorarioDisponivelCenario(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), atendimento.getHorarioAula(), atendimento.getSemana()));
				newAula.setCreditosPraticos(atendimento.getCreditosPratico());
				newAula.setCreditosTeoricos(atendimento.getCreditosTeorico());
				String key = newAula.getSala().getId() + "-" + newAula.getHorarioDisponivelCenario().getId();
				
				result.put(key, newAula);
			}
		}
		else
		{
			if (aula.getProfessor() != null)
			{
				for(AtendimentoOperacional atendimento : AtendimentoOperacional.findAllBy(getInstituicaoEnsinoUser(), aula.getCenario(), aula.getProfessor()))
				{
					Long disciplinaId = atendimento.getDisciplinaSubstituta() != null ?
							atendimento.getDisciplinaSubstituta().getId() :  atendimento.getDisciplina().getId();
					if (atendimento.getHorarioDisponivelCenario().getDiaSemana().equals(aula.getHorarioDisponivelCenario().getDiaSemana())
							&& (!atendimento.getTurma().equals(turma.getNome()) || !disciplinaId.equals(turma.getDisciplina().getId())))
					{
						atendimentosSet.add(atendimento);
					}
				}
			}
			else
			{
				for(AtendimentoOperacional atendimento : AtendimentoOperacional.findAllBy(getInstituicaoEnsinoUser(), aula.getCenario(), aula.getProfessorVirtual()))
				{
					Long disciplinaId = atendimento.getDisciplinaSubstituta() != null ?
							atendimento.getDisciplinaSubstituta().getId() :  atendimento.getDisciplina().getId();
					if (atendimento.getHorarioDisponivelCenario().getDiaSemana().equals(aula.getHorarioDisponivelCenario().getDiaSemana())
							&& (!atendimento.getTurma().equals(turma.getNome()) || !disciplinaId.equals(turma.getDisciplina().getId())))
					{
						atendimentosSet.add(atendimento);
					}
				}
			}
			for(AtendimentoOperacional atendimento : AtendimentoOperacional.findByCenario(getInstituicaoEnsinoUser(), aula.getCenario(), aula.getSala(), null))
			{
				Long disciplinaId = atendimento.getDisciplinaSubstituta() != null ?
						atendimento.getDisciplinaSubstituta().getId() :  atendimento.getDisciplina().getId();
				if (atendimento.getHorarioDisponivelCenario().getDiaSemana().equals(aula.getHorarioDisponivelCenario().getDiaSemana())
						&& (!atendimento.getTurma().equals(turma.getNome()) || !disciplinaId.equals(turma.getDisciplina().getId())))
				{
					atendimentosSet.add(atendimento);
				}
			}
			
			List<AtendimentoOperacional> atendimentos = new ArrayList<AtendimentoOperacional>(atendimentosSet);
			List<AtendimentoOperacionalDTO> aulas = extraiAulas(ConvertBeans.toListAtendimentoOperacionalDTO(atendimentos));
			
			for (AtendimentoOperacionalDTO aulaAtendimento : aulas)
			{
				Aula newAula = new Aula();
				
				newAula.setCenario(aula.getCenario());
				newAula.setSala(Sala.find(aulaAtendimento.getSalaId(), getInstituicaoEnsinoUser()));
				newAula.setHorarioDisponivelCenario(HorarioDisponivelCenario.find(aulaAtendimento.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser()));
				newAula.setCreditosPraticos(aulaAtendimento.getCreditoTeoricoBoolean() ? 0 : aulaAtendimento.getTotalCreditos());
				newAula.setCreditosTeoricos(aulaAtendimento.getCreditoTeoricoBoolean() ? aulaAtendimento.getTotalCreditos() : 0);
				String key = newAula.getSala().getId() + "-" + newAula.getHorarioDisponivelCenario().getId();
				
				result.put(key, newAula);
			}
		}
		return result;
	}
	
	@Override
	@Transactional
	public TurmaDTO saveAula(DisciplinaDTO disciplinaDTO, CampusDTO campusDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO)
	{
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(), getInstituicaoEnsinoUser());
		if (turmaDTO.getId() != null)
		{
			Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
			Aula aula = ConvertBeans.toAula(aulaDTO);
			if (aulaDTO.getId() != null)
			{
				aula.getTurmas().add(turma);
				aula.merge();
			}
			else
			{
				turma.getAulas().add(aula);
				
				aula.persist();
			}
			
			return turmaDTO;
		}
		else
		{
			Set<HorarioDisponivelCenario> horarios = new HashSet<HorarioDisponivelCenario>();
			Set<HorarioAula> horariosTatico = new HashSet<HorarioAula>();
			List<AtendimentoOperacional> atendimentos = new ArrayList<AtendimentoOperacional>();
			List<AtendimentoTatico> atendimentosTatico = new ArrayList<AtendimentoTatico>();
			for (Long id : aulaDTO.getAtendimentosIds())
			{
					
				atendimentos.add(AtendimentoOperacional.find(id, getInstituicaoEnsinoUser()));
				horarios.add(AtendimentoOperacional.find(id, getInstituicaoEnsinoUser()).getHorarioDisponivelCenario());
			}
			for (Long id : aulaDTO.getAtendimentosIds())
			{
					
				atendimentosTatico.add(AtendimentoTatico.find(id, getInstituicaoEnsinoUser()));
				horariosTatico.add(AtendimentoTatico.find(id, getInstituicaoEnsinoUser()).getHorarioAula());
			}
			
			if (!atendimentos.isEmpty())
			{
				Collections.sort(atendimentos,  new Comparator<AtendimentoOperacional>() {
					@Override
					public int compare(AtendimentoOperacional arg1, AtendimentoOperacional arg2) {
						return arg1.getHorarioDisponivelCenario().getHorarioAula().compareTo(arg2.getHorarioDisponivelCenario().getHorarioAula());
					}
				});
				Aula aula = ConvertBeans.toAula(aulaDTO);
				List<HorarioDisponivelCenario> novosHorarios = aula.getHDCs();
				HorarioDisponivelCenario primeiroHorario = atendimentos.get(0).getHorarioDisponivelCenario();
				int horarioIndex = 0;
				for (int i = 0; i < atendimentos.size(); i++)
				{
					if (horarioIndex >= (aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()))
					{
						atendimentos.get(i).remove();
					}
					else
					{
						if (atendimentos.get(i).getHorarioDisponivelCenario().equals(primeiroHorario))
						{
							atendimentos.get(i).setHorarioDisponivelCenario(novosHorarios.get(horarioIndex));
						}
						else
						{
							horarioIndex++;
							if (horarioIndex >= (aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()))
							{
								atendimentos.get(i).remove();
							}
							else
							{
								primeiroHorario = atendimentos.get(i).getHorarioDisponivelCenario();
								atendimentos.get(i).setHorarioDisponivelCenario(novosHorarios.get(horarioIndex));
								atendimentos.get(i).setSala(aula.getSala());
								atendimentos.get(i).merge();
							}
						}
					}
				}
				AtendimentoOperacional.entityManager().flush();
				
				if ((aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()) < horarios.size())
				{
					List<AtendimentoOperacional> atendimentosTurma = 
							AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), disciplina.getCenario(), disciplina,
									campus, null, turmaDTO.getNome());
					
					return ConvertBeans.toTurmaDTO(converteTurmaDeNaoParcialParaParcial(atendimentosTurma));
				}
				
				return turmaDTO;
			}
			else
			{
				Collections.sort(atendimentosTatico,  new Comparator<AtendimentoTatico>() {
					@Override
					public int compare(AtendimentoTatico arg1, AtendimentoTatico arg2) {
						return arg1.getHorarioAula().compareTo(arg2.getHorarioAula());
					}
				});
				Aula aula = ConvertBeans.toAula(aulaDTO);
				List<HorarioDisponivelCenario> novosHorarios = aula.getHDCs();
				HorarioAula primeiroHorario = atendimentosTatico.get(0).getHorarioAula();
				int horarioIndex = 0;
				for (int i = 0; i < atendimentosTatico.size(); i++)
				{
					if (horarioIndex >= (aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()))
					{
						atendimentosTatico.get(i).remove();
					}
					else
					{
						if (atendimentosTatico.get(i).getHorarioAula().equals(primeiroHorario) )
						{
							atendimentosTatico.get(i).setHorarioAula(novosHorarios.get(horarioIndex).getHorarioAula());
							atendimentosTatico.get(i).setSemana(novosHorarios.get(horarioIndex).getDiaSemana());
						}
						else
						{
							horarioIndex++;
							if (horarioIndex >= (aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()))
							{
								atendimentosTatico.get(i).remove();
							}
							else
							{
								primeiroHorario = atendimentosTatico.get(i).getHorarioAula();
								atendimentosTatico.get(i).setHorarioAula(novosHorarios.get(horarioIndex).getHorarioAula());
								atendimentosTatico.get(i).setSemana(novosHorarios.get(horarioIndex).getDiaSemana());
								atendimentosTatico.get(i).setSala(aula.getSala());
								atendimentosTatico.get(i).merge();
							}
						}
					}
				}
				AtendimentoTatico.entityManager().flush();
				
				if ((aulaDTO.getCreditosPraticos() + aulaDTO.getCreditosTeoricos()) < horarios.size())
				{
					List<AtendimentoTatico> atendimentosTurma = 
							AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), disciplina.getCenario(), disciplina,
									campus, null, turmaDTO.getNome());
					
					return ConvertBeans.toTurmaDTO(converteTurmaDeNaoParcialParaParcialTatico(atendimentosTurma));
				}
				
				return turmaDTO;
			}
		}
	}
	
	@Override
	@Transactional
	public void removeAula(TurmaDTO turmaDTO, AulaDTO aulaDTO)
	{
		if (turmaDTO.getId() != null)
		{
			if (aulaDTO.getId() != null)
			{
				Aula aula = Aula.find(aulaDTO.getId(), getInstituicaoEnsinoUser());
				aula.remove();
			}
		}
		else
		{
			if (aulaDTO.getProfessorId() != null) {
				for (Long id : aulaDTO.getAtendimentosIds()) {
					AtendimentoOperacional.find(id, getInstituicaoEnsinoUser()).remove();
				}
			} else {
				for (Long id : aulaDTO.getAtendimentosIds()) {
					AtendimentoTatico.find(id, getInstituicaoEnsinoUser()).remove();
				}
			}
		}
	}
	
	@Override
	@Transactional
	public void alocaAlunosTurma(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, TurmaDTO turmaDTO, List<AlunoStatusDTO> alunos)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Demanda demanda = Demanda.find(demandaDTO.getId(), getInstituicaoEnsinoUser());
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		Map<Long, Boolean> alunosDemandaIdMapMarcado = new HashMap<Long, Boolean>();
		for (AlunoStatusDTO aluno : alunos)
		{
			alunosDemandaIdMapMarcado.put(aluno.getAlunoDemandaId(), aluno.getMarcado());
		}
		
		Map<Long, AlunoDemanda> alunoDemandaIdMapAlunoDemanda;
		if (turmaDTO.getId() == null)
		{
			if (ehTatico)
			{
				alunoDemandaIdMapAlunoDemanda = AlunoDemanda.buildIdMapAlunoDemandaFetchAtendimentoTatico(
						getInstituicaoEnsinoUser(), alunosDemandaIdMapMarcado.keySet());
				for (AtendimentoTatico atendimentos : AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), cenario, demanda.getDisciplina(),
						demanda.getOferta().getCampus(), demanda.getOferta(), turmaDTO.getNome()))
				{
					for (Entry<Long, Boolean> alunoMarcado : alunosDemandaIdMapMarcado.entrySet())
					{
						if (alunoMarcado.getValue())
						{
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).setAtendido(true);
							atendimentos.setQuantidadeAlunos(atendimentos.getQuantidadeAlunos() + 1);
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).getAtendimentosTatico().add(atendimentos);
						}
						else
						{
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).setAtendido(false);
							atendimentos.setQuantidadeAlunos(atendimentos.getQuantidadeAlunos() - 1);
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).getAtendimentosTatico().remove(atendimentos);
						}
					}
				}
			}
			else
			{
				alunoDemandaIdMapAlunoDemanda = AlunoDemanda.buildIdMapAlunoDemandaFetchAtendimentoOperacional(
						getInstituicaoEnsinoUser(), alunosDemandaIdMapMarcado.keySet());
				for (AtendimentoOperacional atendimentos : AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, demanda.getDisciplina(),
						demanda.getOferta().getCampus(), demanda.getOferta(), turmaDTO.getNome()))
				{
					for (Entry<Long, Boolean> alunoMarcado : alunosDemandaIdMapMarcado.entrySet())
					{
						if (alunoMarcado.getValue())
						{
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).setAtendido(true);
							atendimentos.setQuantidadeAlunos(atendimentos.getQuantidadeAlunos() + 1);
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).getAtendimentosOperacional().add(atendimentos);
						}
						else
						{
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).setAtendido(false);
							atendimentos.setQuantidadeAlunos(atendimentos.getQuantidadeAlunos() - 1);
							alunoDemandaIdMapAlunoDemanda.get(alunoMarcado.getKey()).getAtendimentosOperacional().remove(atendimentos);
						}
					}
				}
			}
		}
		else
		{
			Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
			for (Entry<Long, Boolean> alunoMarcado : alunosDemandaIdMapMarcado.entrySet())
			{
				if (alunoMarcado.getValue())
				{
					turma.getAlunos().add(AlunoDemanda.find(alunoMarcado.getKey(), getInstituicaoEnsinoUser()));
				}
				else
				{
					turma.getAlunos().remove(AlunoDemanda.find(alunoMarcado.getKey(), getInstituicaoEnsinoUser()));
				}
			}
		}
	}
	
	@Override
	@Transactional
	public void alocaProfessoresAula(CenarioDTO cenarioDTO, DemandaDTO demandaDTO, TurmaDTO turmaDTO, AulaDTO aulaDTO, ProfessorStatusDTO professorStatusDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Demanda demanda = Demanda.find(demandaDTO.getId(), getInstituicaoEnsinoUser());
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		if (turmaDTO.getId() == null)
		{
			if (!ehTatico)
			{
				List<AtendimentoOperacional> atendimentosTurma = AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), cenario, demanda.getDisciplina(),
						demanda.getOferta().getCampus(), demanda.getOferta(), turmaDTO.getNome());
				for (AtendimentoOperacional atendimentos : extraiAtendimentosSequenciais(atendimentosTurma,
						HorarioDisponivelCenario.find(aulaDTO.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser())))
				{
					if (professorStatusDTO.getMarcado())
					{
						if (professorStatusDTO.getProfessorId() != null)
						{
							atendimentos.setProfessor(Professor.find(professorStatusDTO.getProfessorId(), getInstituicaoEnsinoUser()));
						}
						else
						{
							atendimentos.setProfessorVirtual(ProfessorVirtual.find(professorStatusDTO.getProfessorVirtualId(), getInstituicaoEnsinoUser()));
						}
					}
				}
			}

		}
		else
		{
			Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
			for (Aula aula : turma.getAulas())
			{
				if (aula.getSala().getId().equals(aulaDTO.getSalaId())
						&& aula.getHorarioDisponivelCenario().getHorarioAula().getId().equals(aulaDTO.getHorarioAulaId()))
				{
					if (professorStatusDTO.getProfessorId() != null)
					{
						aula.setProfessor(Professor.find(professorStatusDTO.getProfessorId(), getInstituicaoEnsinoUser()));
					}
					else
					{
						aula.setProfessorVirtual(ProfessorVirtual.find(professorStatusDTO.getProfessorVirtualId(), getInstituicaoEnsinoUser()));
					}
				}
			}
		}
	}

	private List<AtendimentoOperacional> extraiAtendimentosSequenciais(
			List<AtendimentoOperacional> atendimentosTurma,
			HorarioDisponivelCenario hdc)
	{
		List<AtendimentoOperacionalDTO> grupoAtendimentosOrdenadosPorHorario = ordenaPorHorarioAula(ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosTurma));
		
		List<List<AtendimentoOperacionalDTO>> subgruposDeAtendimentosConsecutivos = separaAtendimentosNaoConsecutivos(grupoAtendimentosOrdenadosPorHorario);
		
		List<AtendimentoOperacional> result = new ArrayList<AtendimentoOperacional>();
		for (List<AtendimentoOperacionalDTO> atendimentosDTO : subgruposDeAtendimentosConsecutivos)
		{
			if (atendimentosDTO.get(0).getHorarioDisponivelCenarioId().equals(hdc.getId()))
			{
				for (AtendimentoOperacionalDTO atedimentoDTO : atendimentosDTO)
				{
					result.add(AtendimentoOperacional.find(atedimentoDTO.getId(), getInstituicaoEnsinoUser()));
				}
			}
		}
		
		return result;
	}
	
	@Override
	public TrioDTO<Boolean, List<String>, List<String>> verificaViabilidadeSalvarTurma(CenarioDTO cenarioDTO, TurmaDTO turmaDTO, List<AulaDTO> aulasDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
		Map<String, Long> deslocamentoUnidadesMapTempo = DeslocamentoUnidade.buildDeslocamentoUnidadesMapTempo(getInstituicaoEnsinoUser(), cenario);
		
		List<String> errors = new ArrayList<String>();
		List<String> warnings = new ArrayList<String>();
		TriedaTrio<Boolean, List<String>, List<String>> result = TriedaTrio.create(true, errors, warnings);
		for (AulaDTO aulaDTO : aulasDTO)
		{
			Aula aula = Aula.find(aulaDTO.getId(), getInstituicaoEnsinoUser());
			TriedaTrio<Boolean, List<String>, List<String>> trio = aula.ehViavel(deslocamentoUnidadesMapTempo, turma,
					criaAulasAPartirDePossiveisAtendmentosConflitantes(aula, turma), cenario.getUltimoParametro(getInstituicaoEnsinoUser()));
			if (!trio.getPrimeiro())
				result.setPrimeiro(trio.getPrimeiro());
			result.getSegundo().addAll(trio.getSegundo());
			result.getTerceiro().addAll(trio.getTerceiro());
		}
		
		return TrioDTO.create(result.getPrimeiro(), result.getSegundo(), result.getTerceiro());
	}
	
	@Override
	public void salvarTurma(TurmaDTO turmaDTO)
	{
		Turma turma = Turma.find(turmaDTO.getId(), getInstituicaoEnsinoUser());
		
		converteTurmaDeParcialParaNaoParcial(turma);
	}
	
	@Transactional
	boolean converteTurmaDeParcialParaNaoParcial(Turma turma) {	
		// organiza os alunos_demanda da turma em questão por oferta e disciplina
		Map<Oferta,Map<Disciplina,Set<AlunoDemanda>>> ofeToDiscSubstToAlnDemMap = new HashMap<Oferta,Map<Disciplina,Set<AlunoDemanda>>>();
		for (AlunoDemanda ald : turma.getAlunos())
		{
			Oferta ald_ofe = ald.getDemanda().getOferta();
			Disciplina ald_dis = ald.getDemanda().getDisciplina();
			
			Map<Disciplina,Set<AlunoDemanda>> discSubstToAlnDemMap = ofeToDiscSubstToAlnDemMap.get(ald_ofe);
			if (discSubstToAlnDemMap == null) {
				discSubstToAlnDemMap = new HashMap<Disciplina,Set<AlunoDemanda>>();
				ofeToDiscSubstToAlnDemMap.put(ald_ofe,discSubstToAlnDemMap);
			}		
			
			Set<AlunoDemanda> alnDem = discSubstToAlnDemMap.get(ald_dis);
			if (alnDem == null) {
				alnDem = new HashSet<AlunoDemanda>();
				discSubstToAlnDemMap.put(ald_dis,alnDem);
			}
			alnDem.add(ald);
		}
		
		// cria os atendimentos operacionais a partir das aulas
		Set<AtendimentoOperacional> atendimentosOp = new HashSet<AtendimentoOperacional>();
		Set<AtendimentoTatico> atendimentosTc = new HashSet<AtendimentoTatico>();
		Cenario cen = turma.getCenario();
		Disciplina discDaTurma = turma.getDisciplina();
		String turmaNome = turma.getNome();
		for (Aula aul : turma.getAulas())
		{
			// obtém os horários da aula
			List<HorarioDisponivelCenario> horariosHDC = aul.getHDCs();
			// para cada oferta atendida pela aula
			for (Oferta ofe : ofeToDiscSubstToAlnDemMap.keySet()) {
				// para cada disciplina da oferta atendida pela aula
				for (Disciplina dis : ofeToDiscSubstToAlnDemMap.get(ofe).keySet())
				{
					// para cada horário da aula
					for (HorarioDisponivelCenario hdc : horariosHDC){
						if (aul.getProfessorVirtual() != null || aul.getProfessor() != null)
						{
							AtendimentoOperacional atp = new AtendimentoOperacional();
							atp.setCreditoTeorico(aul.isTeorica());
							atp.setQuantidadeAlunos(ofeToDiscSubstToAlnDemMap.get(ofe).get(dis).size());
							atp.setTurma(turmaNome);
							atp.setHorarioDisponivelCenario(hdc);
							atp.setCenario(cen);            
							atp.setOferta(ofe);
							atp.setProfessor(aul.getProfessor());
							atp.setProfessorVirtual(aul.getProfessorVirtual());
							atp.setSala(aul.getSala());
							atp.setInstituicaoEnsino(cen.getInstituicaoEnsino());
							if (!discDaTurma.equals(dis)) {
								atp.setDisciplina(dis);
							   	atp.setDisciplinaSubstituta(discDaTurma);
							} else {
								atp.setDisciplina(discDaTurma);
								atp.setDisciplinaSubstituta(null);
							}
							atp.setConfirmada(false);
	
							atendimentosOp.add(atp);
						}
					}
					if (aul.getProfessorVirtual() == null && aul.getProfessor() == null)
					{
						AtendimentoTatico atp = new AtendimentoTatico();
						atp.setCreditosTeorico(aul.getCreditosTeoricos());
						atp.setCreditosPratico(aul.getCreditosPraticos());
						atp.setQuantidadeAlunos(ofeToDiscSubstToAlnDemMap.get(ofe).get(dis).size());
						atp.setTurma(turmaNome);
						atp.setHorarioAula(aul.getHorarioDisponivelCenario().getHorarioAula());
						atp.setSemana(aul.getHorarioDisponivelCenario().getDiaSemana());
						atp.setCenario(cen);            
						atp.setOferta(ofe);
						atp.setSala(aul.getSala());
						atp.setInstituicaoEnsino(cen.getInstituicaoEnsino());
						if (!discDaTurma.equals(dis)) {
							atp.setDisciplina(dis);
						   	atp.setDisciplinaSubstituta(discDaTurma);
						} else {
							atp.setDisciplina(discDaTurma);
							atp.setDisciplinaSubstituta(null);
						}
						atp.setConfirmada(false);

						atendimentosTc.add(atp);
					}
				}
			}
		}
		
		// insere os atendimentos operacionais no BD
		for (AtendimentoOperacional atendimento : atendimentosOp)
		{
			atendimento.getAlunosDemanda().addAll(ofeToDiscSubstToAlnDemMap.get(atendimento.getOferta()).get(atendimento.getDisciplina()));
			atendimento.persist();
			for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemanda())
			{
				alunoDemanda.setAtendido(true);
				alunoDemanda.getAtendimentosOperacional().add(atendimento);
			}
		}
		for (AtendimentoTatico atendimento : atendimentosTc)
		{
			atendimento.getAlunosDemanda().addAll(ofeToDiscSubstToAlnDemMap.get(atendimento.getOferta()).get(atendimento.getDisciplina()));
			atendimento.persist();
			for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemanda())
			{
				alunoDemanda.setAtendido(true);
				alunoDemanda.getAtendimentosTatico().add(atendimento);
			}
		}
		turma.remove();
		
	    return true;
	}
	
	@Transactional
	Turma converteTurmaDeNaoParcialParaParcial(List<AtendimentoOperacional> atendimentos)
	{	
		Turma turma = new Turma();
		turma.setCenario(atendimentos.get(0).getCenario());
		turma.setDisciplina(atendimentos.get(0).getDisciplina() != null
				? atendimentos.get(0).getDisciplina() : atendimentos.get(0).getDisciplinaSubstituta());
		turma.setNome(atendimentos.get(0).getTurma());
		turma.setParcial(true);

		List<AtendimentoOperacionalDTO> aulas = extraiAulas(ConvertBeans.toListAtendimentoOperacionalDTO(atendimentos));
		for (AtendimentoOperacionalDTO aula : aulas)
		{
			Aula novaAula = new Aula();
			novaAula.setCenario(atendimentos.get(0).getCenario());
			novaAula.setCreditosPraticos(aula.getCreditoTeoricoBoolean() ? 0 : aula.getTotalCreditos());
			novaAula.setCreditosTeoricos(aula.getCreditoTeoricoBoolean() ? aula.getTotalCreditos() : 0);
			novaAula.setHorarioDisponivelCenario(HorarioDisponivelCenario.find(aula.getHorarioDisponivelCenarioId(), getInstituicaoEnsinoUser()));
			novaAula.setSala(Sala.find(aula.getSalaId(), getInstituicaoEnsinoUser()));
			if (aula.getProfessorId() != null)
			{
				novaAula.setProfessor(Professor.find(aula.getProfessorId(), getInstituicaoEnsinoUser()));
			}
			else if (aula.getProfessorVirtualId() != null)
			{
				novaAula.setProfessorVirtual(ProfessorVirtual.find(aula.getProfessorVirtualId(), getInstituicaoEnsinoUser()));
			}
			turma.getAulas().add(novaAula);
		}
		
		for (AtendimentoOperacional atendimento : atendimentos)
		{
			turma.getAlunos().addAll(atendimento.getAlunosDemanda());
			atendimento.remove();
		}
		
		turma.persist();
		
		return turma;
	}
	
	@Transactional
	Turma converteTurmaDeNaoParcialParaParcialTatico(List<AtendimentoTatico> atendimentos)
	{	
		Turma turma = new Turma();
		turma.setCenario(atendimentos.get(0).getCenario());
		turma.setDisciplina(atendimentos.get(0).getDisciplina() != null
				? atendimentos.get(0).getDisciplina() : atendimentos.get(0).getDisciplinaSubstituta());
		turma.setNome(atendimentos.get(0).getTurma());
		turma.setParcial(true);

		for (AtendimentoTatico aula : atendimentos)
		{
			Aula novaAula = new Aula();
			novaAula.setCenario(atendimentos.get(0).getCenario());
			novaAula.setCreditosPraticos(aula.getCreditosPratico());
			novaAula.setCreditosTeoricos(aula.getCreditosTeorico());
			novaAula.setHorarioDisponivelCenario(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), aula.getHorarioAula(), aula.getSemana()));
			novaAula.setSala(aula.getSala());

			turma.getAulas().add(novaAula);
		}
		
		for (AtendimentoTatico atendimento : atendimentos)
		{
			turma.getAlunos().addAll(atendimento.getAlunosDemanda());
			atendimento.remove();
		}
		
		turma.persist();
		
		return turma;
	}
	
	@Override
	@Transactional
	public void confirmarTurmaSelecionada(DemandaDTO demandaDTO, TurmaDTO turmaDTO)
	{
		Demanda demanda = ConvertBeans.toDemanda(demandaDTO);
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		if (ehTatico)
		{
			List<AtendimentoTatico> atendimentos = 
					AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), demanda.getDisciplina().getCenario(), demanda.getDisciplina(),
							demanda.getOferta().getCampus(), null, turmaDTO.getNome());
			for (AtendimentoTatico atendimento : atendimentos)
			{
				atendimento.setConfirmada(true);
			}
		}
		else
		{
			List<AtendimentoOperacional> atendimentos = 
					AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), demanda.getDisciplina().getCenario(), demanda.getDisciplina(),
							demanda.getOferta().getCampus(), null, turmaDTO.getNome());
			for (AtendimentoOperacional atendimento : atendimentos)
			{
				atendimento.setConfirmada(true);
			}
		}
	}
	
	@Override
	@Transactional
	public void desconfirmarTurmaSelecionada(DemandaDTO demandaDTO, TurmaDTO turmaDTO)
	{
		Demanda demanda = ConvertBeans.toDemanda(demandaDTO);
		
		boolean ehTatico = demanda.getOferta().getCampus().isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		if (ehTatico)
		{
			List<AtendimentoTatico> atendimentos = 
					AtendimentoTatico.findBy(getInstituicaoEnsinoUser(), demanda.getDisciplina().getCenario(), demanda.getDisciplina(),
							demanda.getOferta().getCampus(), null, turmaDTO.getNome());
			for (AtendimentoTatico atendimento : atendimentos)
			{
				atendimento.setConfirmada(false);
			}
		}
		else
		{
			List<AtendimentoOperacional> atendimentos = 
					AtendimentoOperacional.findBy(getInstituicaoEnsinoUser(), demanda.getDisciplina().getCenario(), demanda.getDisciplina(),
							demanda.getOferta().getCampus(), null, turmaDTO.getNome());
			for (AtendimentoOperacional atendimento : atendimentos)
			{
				atendimento.setConfirmada(false);
			}
		}
	}


}
