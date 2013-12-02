package com.gapso.web.trieda.server;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import org.springframework.dao.EmptyResultDataAccessException;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaCreditoDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaTurmaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.PercentMestresDoutoresDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuartetoDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TipoProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
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
		List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(String alunoNome, String alunoMatricula);
		List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO);
		List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(String alunoNome, String alunoMatricula);
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
			public List<AtendimentoTaticoDTO> buscaDTOsDeAtendimentoTatico(String alunoNome, String alunoMatricula) {
				return buscaNoBancoDadosDTOsDeAtendimentoTatico(alunoNome,alunoMatricula);
			}

			@Override
			public List<AtendimentoOperacionalDTO> buscaDTOsDeAtendimentoOperacional(String alunoNome, String alunoMatricula) {
				return buscaNoBancoDadosDTOsDeAtendimentoOperacional(alunoNome,alunoMatricula);
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
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoSala(RelatorioVisaoSalaFiltro filtro) throws TriedaException{
		QuintetoDTO<List<AtendimentoRelatorioDTO>, ParDTO<Integer, Boolean>, List<String>, List<String>, List<String>> q;
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		boolean temInfoDeHorario = true;

		// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
		try {
			List<AtendimentoTaticoDTO> atendimentosTaticoDTO = buscaNoBancoDadosDTOsDeAtendimentoTatico(filtro.getSalaCodigo());
			if (!atendimentosTaticoDTO.isEmpty()) {
				temInfoDeHorario = (atendimentosTaticoDTO.iterator().next().getHorarioAulaId() != null);
				// insere os atendimentos do modo tático na lista de atendimentos
				aulas.addAll(atendimentosTaticoDTO);
			}
			else {
				temInfoDeHorario = true;
				// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
				List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = buscaNoBancoDadosDTOsDeAtendimentoOperacional(filtro.getSalaCodigo());
				// processa os atendimentos do operacional e os transforma em aulas
				List<AtendimentoOperacionalDTO> aulasOperacional = extraiAulas(atendimentosOperacionalDTO);
				// insere as aulas do modo operacional na lista de atendimentos
				aulas.addAll(aulasOperacional);
			}
			
			if (!aulas.isEmpty()) {
		 		// trata compartilhamento de turmas entre cursos
				List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = uneAulasQuePodemSerCompartilhadas(aulas);
				
				// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão
				// juntamente com o numero de semanas letivas utilizadas e o máximo de 
				// créditos em um dia das semanas letivas relacionadas com as aulas em questão
				Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(aulasComCompartilhamentos);
				Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = buscaNoBancoDadosSemanasLetivas();
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, aulas.get(0).getTurnoId());
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
				List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
				List<String> horariosDeFimDeAula = quarteto.getQuarto();
				
				q = QuintetoDTO.create(aulasComCompartilhamentos,mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			}
			else q = QuintetoDTO.create(aulas,ParDTO.create(0, false),Collections.<String>emptyList(),Collections.<String>emptyList(),Collections.<String>emptyList());
		}
		catch (EmptyResultDataAccessException ex){
			throw new TriedaException("Os campos digitados no filtro não foram encontrados");
		}
		
		return AtendimentoServiceRelatorioResponse.create(q);
	}
	
	public QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(Set<Long> semanasLetivasIDsDasAulas, Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap, boolean temInfoDeHorario, Long turnoId) {
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
	
	private void calculaLabelsDasLinhasDaGradeHoraria(boolean temInfoDeHorario, Long turnoId, List<SemanaLetiva> semanasLetivasDasAulas, int mdcTemposAula,
			List<String> labelsDasLinhasDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {		
		// coleta todos os pares (HoraInicio,HoraFim) dos horários de aula das semanas letivas
		Set<Pair<Calendar,Calendar>> horarios = new HashSet<Pair<Calendar,Calendar>>();
		for (SemanaLetiva semanaLetiva : semanasLetivasDasAulas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				if (horarioAula.getTurno().getId().equals(turnoId)) {
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
			while (!h.equals(hf)) {
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
			QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala,semanaLetivaIdToSemanaLetivaMap,temInfoDeHorario,filtro.getTurnoDTO().getId());
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
	
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoProfessor(RelatorioVisaoProfessorFiltro filtro, boolean isVisaoProfessor) throws TriedaException{
		QuintetoDTO<List<AtendimentoRelatorioDTO>,ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> q;
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		boolean temInfoDeHorario = true;
		boolean isAdmin = isAdministrador();
		
		String professorNome = (filtro.getProfessorNome() == null ? "" : filtro.getProfessorNome());
		String professorCpf = (filtro.getProfessorCpf() == null ? "" : filtro.getProfessorCpf());

		try {
			Professor professor = (professorCpf.isEmpty() && professorNome.isEmpty()) ? null : Professor.findByNomeCpf(getInstituicaoEnsinoUser(), professorNome, professorCpf);
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
				
				Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = buscaNoBancoDadosSemanasLetivas();
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, aulas.get(0).getTurnoId());
				ParDTO<Integer, Boolean> mdcTemposAulaNumSemanasLetivas = quarteto.getPrimeiro();
				List<String> labelsDasLinhasDaGradeHoraria = quarteto.getSegundo();
				List<String> horariosDeInicioDeAula = quarteto.getTerceiro();
				List<String> horariosDeFimDeAula = quarteto.getQuarto();
				
				q = QuintetoDTO.create(atendimentosParaEscrita,mdcTemposAulaNumSemanasLetivas,labelsDasLinhasDaGradeHoraria,horariosDeInicioDeAula,horariosDeFimDeAula);
			}
			else q = QuintetoDTO.create(aulas,ParDTO.create(0, false),Collections.<String>emptyList(),Collections.<String>emptyList(),Collections.<String>emptyList());
		}
		catch (EmptyResultDataAccessException ex){
			throw new TriedaException("Os campos do digitados no filtro não foram encontrados");
		}
		
		return AtendimentoServiceRelatorioResponse.create(q);
	}
	
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoAluno(RelatorioVisaoAlunoFiltro filtro, IAtendimentosServiceDAO dao) throws TriedaException{
		List<AtendimentoRelatorioDTO> atendimentos = new ArrayList<AtendimentoRelatorioDTO>();
		QuintetoDTO<List<AtendimentoRelatorioDTO>,ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> q;
		boolean temInfoDeHorario = true;

		// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
		try {
			List<AtendimentoTaticoDTO> atendimentosTaticoDTO = dao.buscaDTOsDeAtendimentoTatico(filtro.getAlunoNome(), filtro.getAlunoMatricula());
			if(!atendimentosTaticoDTO.isEmpty()){
				temInfoDeHorario = (atendimentosTaticoDTO.iterator().next().getHorarioAulaId() != null);
				// insere os atendimentos do modo tático na lista de atendimentos
				atendimentos.addAll(atendimentosTaticoDTO);
			}
			else{
				temInfoDeHorario = true;
				// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
				List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = dao.buscaDTOsDeAtendimentoOperacional(filtro.getAlunoNome(), filtro.getAlunoMatricula());
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
				QuartetoDTO<ParDTO<Integer, Boolean>,List<String>,List<String>,List<String>> quarteto = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala, semanaLetivaIdToSemanaLetivaMap, temInfoDeHorario, atendimentos.get(0).getTurnoId());
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
	
	public AtendimentoServiceRelatorioResponse getAtendimentosParaGradeHorariaVisaoAluno(RelatorioVisaoAlunoFiltro filtro) throws TriedaException{
		return getAtendimentosParaGradeHorariaVisaoAluno(filtro,getDAO());
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
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(String salaCodigo) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.findByCodigo(getInstituicaoEnsinoUser(), salaCodigo);

		// busca no BD os atendimentos do modo tático
		List<AtendimentoTatico> atendimentosTaticoBD = AtendimentoTatico.findBySalaAndTurno(getInstituicaoEnsinoUser(),sala,null,null);
		
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
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(String alunoNome, String alunoMatricula) {
		// obtém os beans de Banco de Dados
		Aluno aluno = Aluno.findOneByNomeMatricula(getInstituicaoEnsinoUser(), alunoNome, alunoMatricula);

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
	public List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(String salaCodigo) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.findByCodigo(getInstituicaoEnsinoUser(), salaCodigo);

		// busca no BD os atendimentos do modo operacional
		List<AtendimentoOperacional> atendimentosOperacionalBD = AtendimentoOperacional.findBySalaAndTurno(sala,null,null,getInstituicaoEnsinoUser());

		// transforma os atendimentos obtidos do BD em DTOs
		return ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacionalBD);		
	}
	
	/**
	 * Busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs.
	 * @param alunoDTO sala
	 * @param turnoDTO turno
	 * @return uma lista com DTOs que representam atendimentos do modo operacional
	 */
	public List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(String alunoNome, String alunoMatricula){
		// obtém os beans de Banco de Dados
		Aluno aluno = Aluno.findOneByNomeMatricula(getInstituicaoEnsinoUser(), alunoNome, alunoMatricula);

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
		// [Curso-Curriculo-Disciplina-Turma-DiaSemana-Sala -> List<AtendimentoOperacionalDTO>]
		Map<String,List<AtendimentoOperacionalDTO>> atendimentosAgrupadosMap = new HashMap<String,List<AtendimentoOperacionalDTO>>();
		// Agrupa os DTOS pela chave Curso-Curriculo-Disciplina-Turma-DiaSemana-Sala
		for (AtendimentoOperacionalDTO atendimento : atendimentosDTO) {
			String key = atendimento.getCursoString() + "-" + atendimento.getCurriculoString() + "-" + atendimento.getDisciplinaString() + "-" + atendimento.getTurma() + "-" + atendimento.getSemana() + "-" + atendimento.getSalaId();

			List<AtendimentoOperacionalDTO> grupoAtendimentos = atendimentosAgrupadosMap.get(key);
			if (grupoAtendimentos == null) {
				grupoAtendimentos = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentosAgrupadosMap.put(key,grupoAtendimentos);
			}
			grupoAtendimentos.add(atendimento);
		}
		
		// Quando há mais de um DTO por chave [Curso-Curriculo-Disciplina-Turma-DiaSemana-Sala], concatena as informações de todos em um único DTO.
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
	public ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais()
	{
		List< ProfessorVirtual > professoresVirtuais
			= ProfessorVirtual.findAll( getInstituicaoEnsinoUser() );

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
			PagingLoadConfig config ) throws TriedaException
	{
		Titulacao titulacao = titulacaoDTO == null ? null : Titulacao.find(titulacaoDTO.getId(), getInstituicaoEnsinoUser());
		
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
			= ProfessorVirtual.findBy( getInstituicaoEnsinoUser(), cenario, titulacao, orderBy );

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
				getInstituicaoEnsinoUser(), cenario, titulacao, orderBy ).size() );
		
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
			for (Campus campiOtimizados : Campus.findAllOtimized(getInstituicaoEnsinoUser()) )
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
				atendimentoOperacionalList = AtendimentoOperacional.findAllByCampi(getInstituicaoEnsinoUser(), listCampus);
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
			percentMestresDoutoresDTO.setCursoString(curso.getNome());
			
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
		Campus campus = Campus.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
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
		Campus campus = Campus.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
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
						titulo = "Todas as disciplina atendidas";
					}
					else
					{
						titulo = i + " disciplina" + ((i != 1) ? "s " : "") + " nao atendida" + ((i != 1) ? "s" : "");
					}
					AtendimentoFaixaCreditoDTO novaFaixa = new AtendimentoFaixaCreditoDTO(titulo);
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
	public List<RelatorioQuantidadeDTO> getProfessoresJanelasGrade(CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentosList = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
		
		Map<String, Set<HorarioDisponivelCenario>> professorToListHorarioDisponivelCenario = new HashMap<String, Set<HorarioDisponivelCenario>>();
		
		for (AtendimentoOperacional atendimento : atendimentosList)
		{
			if (tipoProfessorDTO == null || (atendimento.getProfessor().getCpf() != null && tipoProfessorDTO.getInstitucional())
					|| (atendimento.getProfessorVirtual() != null && tipoProfessorDTO.getVirtual()))
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
		}
		
		List<RelatorioQuantidadeDTO> janelasGradeHorariaDocente = new ArrayList<RelatorioQuantidadeDTO>();
		
		for (Entry<String, Set<HorarioDisponivelCenario>> professor : professorToListHorarioDisponivelCenario.entrySet() )
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
					Calendar h1 = Calendar.getInstance();
					h1.setTime(horarioProfessorOrdenado.get(0).getHorarioAula().getHorario());
					for (int i = 1; i < horarioProfessorOrdenado.size(); i++)
					{
						h1.add(Calendar.MINUTE,horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo());
						Calendar h2 = Calendar.getInstance();
						h2.setTime(horarioProfessorOrdenado.get(i).getHorarioAula().getHorario());
						
						if (h2.getTimeInMillis() - h1.getTimeInMillis() > (horarioProfessorOrdenado.get(i-1).getHorarioAula().getSemanaLetiva().getTempo() * 60000))
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
			janelasGradeHorariaDocente.get(janelas).setQuantidade(janelasGradeHorariaDocente.get(janelas).getQuantidade() + 1);
		}
		return janelasGradeHorariaDocente;
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getProfessoresDisciplinasLecionadas(CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		
		List<AtendimentoOperacional> atendimentosList = AtendimentoOperacional.findAll(campus, cenario, getInstituicaoEnsinoUser());
		
		Map<String, Set<String>> professorToQtdeDisciplinasLecionadas = new HashMap<String, Set<String>>();
		
		for (AtendimentoOperacional atendimento : atendimentosList)
		{
			if (tipoProfessorDTO == null || (atendimento.getProfessor().getCpf() != null && tipoProfessorDTO.getInstitucional())
					|| (atendimento.getProfessorVirtual() != null && tipoProfessorDTO.getVirtual()))
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
					turmas.add((atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta() : atendimento.getDisciplina())
							+ "-" + atendimento.getTurma());
					professorToQtdeDisciplinasLecionadas.put(professor, turmas);
				}
				else
				{
					professorToQtdeDisciplinasLecionadas.get(professor).add(
							(atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta() : atendimento.getDisciplina())
							+ "-" + atendimento.getTurma());
				}
			}
		}
		
		List<RelatorioQuantidadeDTO> professoresDisciplinasLecionadas = new ArrayList<RelatorioQuantidadeDTO>();
		
		List<String> todosProfessoresList = new ArrayList<String>();
		if (tipoProfessorDTO == null || tipoProfessorDTO.getInstitucional())
		{
			for (Professor professorInstitucional : Professor.findByCampus(getInstituicaoEnsinoUser(), cenario, campus))
			{
				todosProfessoresList.add(professorInstitucional.getNome());
			}
		}
		if (tipoProfessorDTO == null || tipoProfessorDTO.getVirtual())
		{
			for (ProfessorVirtual professorVirtual : ProfessorVirtual.findBy(getInstituicaoEnsinoUser(), cenario, campus))
			{
				todosProfessoresList.add(professorVirtual.getNome());
			}
		}
		
		for (String professor : todosProfessoresList )
		{
			int qtdeDisciplinasLecionadas = professorToQtdeDisciplinasLecionadas.get(professor) == null
					? 0 : professorToQtdeDisciplinasLecionadas.get(professor).size() ;
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
						titulo = i + " disciplina" + ((i != 1) ? "s " : "") + " lecionada" + ((i != 1) ? "s " : "");
					}
					RelatorioQuantidadeDTO novaFaixa = new RelatorioQuantidadeDTO(titulo);
					professoresDisciplinasLecionadas.add(novaFaixa);
				}
			}
			professoresDisciplinasLecionadas.get(qtdeDisciplinasLecionadas).setQuantidade(professoresDisciplinasLecionadas.get(qtdeDisciplinasLecionadas).getQuantidade() + 1);
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
		Set<Sala> salasUtilizadas = new HashSet<Sala>();
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
					
					salasUtilizadas.add(aula.getSala());
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				for (AtendimentoOperacional atendimento : oferta.getAtendimentosOperacionais()) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimento));
					
					salasUtilizadas.add(atendimento.getSala());
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
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			Map<Long,Integer> salaIdToTempoUsoSemanalEmMinutosMap = new HashMap<Long,Integer>();
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Turno turno : turnosConsiderados) {
				for (Sala sala : salasUtilizadas) {
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
							salaIdToTempoUsoSemanalEmMinutosMap.put(aula.getSalaId(), tempoUsoSemanalEmMinutos + aula.getTotalCreditos()*aula.getSemanaLetivaTempoAula());
						}
					}
				}
			}
			
			//calculo do indicador de taxa de uso dos horarios das salas de aula
			SemanaLetiva maiorSemanaLetiva = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivasUtilizadas);
			Map<Integer, Integer> countHorariosAula = new HashMap<Integer, Integer>();
			for(HorarioAula ha : maiorSemanaLetiva.getHorariosAula()){
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
				else if (mediaUtilizacaoHorarioSalas <= 0.8 && mediaUtilizacaoHorarioSalas > 0.6)
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
				else if (mediaUtilizacaoHorarioSalas <= 0.6 && mediaUtilizacaoHorarioSalas > 0.4)
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
				else if (mediaUtilizacaoHorarioSalas <= 0.4 && mediaUtilizacaoHorarioSalas > 0.2)
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
				else if (mediaUtilizacaoHorarioSalas <= 0.2)
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
			}
		}
		return faixasOcupacaoHorario;		
	}
	
	@Override
	public List<RelatorioQuantidadeDTO> getAmbientesFaixaUtilizacaoCapacidade(CenarioDTO cenarioDTO, CampusDTO campusDTO)
	{
		Campus campus = Campus.find(campusDTO.getId(), getInstituicaoEnsinoUser());
		boolean ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
		
		// cálculo dos indicadores de utilização das salas de aula e laboratórios
		Set<Turno> turnosConsiderados = new HashSet<Turno>();
		Set<Sala> salasUtilizadas = new HashSet<Sala>();
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
					
					salasUtilizadas.add(aula.getSala());
					semanasLetivasUtilizadas.add(aula.getOferta().getCurriculo().getSemanaLetiva());
				}
			} else {
				// atendimentos operacionais
				for (AtendimentoOperacional atendimento : oferta.getAtendimentosOperacionais()) {
					String key = atendimento.getSala().getId() + "-" + oferta.getTurno().getId();
					List<AtendimentoRelatorioDTO> atendimetosPorSalaTurno = salaIdTurnoIdToAtendimentosMap.get(key);
					if (atendimetosPorSalaTurno == null) {
						atendimetosPorSalaTurno = new ArrayList<AtendimentoRelatorioDTO>();
						salaIdTurnoIdToAtendimentosMap.put(key,atendimetosPorSalaTurno);
					}
					atendimetosPorSalaTurno.add(ConvertBeans.toAtendimentoOperacionalDTO(atendimento));
					
					salasUtilizadas.add(atendimento.getSala());
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
		Map<Sala, List<Integer>> salasToQtdeAlunosPorAulaMap = new HashMap<Sala, List<Integer>>();
		if (!salaIdTurnoIdToAtendimentosMap.isEmpty()) {
			// [SalaId -> Tempo de uso (min) semanal]
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for (Turno turno : turnosConsiderados) {
				for (Sala sala : salasUtilizadas) {
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
				else if (mediaUtilizacaoCapacidadeSala <= 0.8 && mediaUtilizacaoCapacidadeSala > 0.6)
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
				else if (mediaUtilizacaoCapacidadeSala <= 0.6 && mediaUtilizacaoCapacidadeSala > 0.4)
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
				else if (mediaUtilizacaoCapacidadeSala <= 0.4 && mediaUtilizacaoCapacidadeSala > 0.2)
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
				else if (mediaUtilizacaoCapacidadeSala <= 0.2)
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
			}
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
			String turmaKey = (atendimento.getDisciplina() == null ? atendimento.getDisciplinaSubstituta() : atendimento.getDisciplina())
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
		int atendimentos = 0;
		for (Entry<String, List<AtendimentoOperacional>> turma : turmaMapAtendimentos.entrySet())
		{
			Long key = turma.getValue().get(0).getHorarioDisponivelCenario().getId();
			for (AtendimentoOperacional atendimento : turma.getValue())
			{
				if (atendimento.getHorarioDisponivelCenario().getId() == key)
				{
					atendimentos += atendimento.getQuantidadeAlunos();
				}
			}
		}
		System.out.println("num " + atendimentos);
		return atendimentosFaixaTurma;
	}

	private List<AtendimentoFaixaTurmaDTO> getAtendimentosFaixaTurmaTatico(
			List<AtendimentoTatico> atendimentosTatico) {
		// TODO Auto-generated method stub
		return null;
	}
}
