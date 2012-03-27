package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.dev.util.Pair;

public class AtendimentosServiceImpl extends RemoteService implements AtendimentosService {
	private static final long serialVersionUID = -1505176338607927637L;
	
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
		final Map<Long,HorarioAula> idToHorarioAulaMap = HorarioAula.buildHorarioAulaIdToHorarioAulaMap(HorarioAula.findAll(getInstituicaoEnsinoUser()));

		Collections.sort(atendimentosOrdenados, new Comparator<AtendimentoOperacionalDTO>() {
			@Override
			public int compare(AtendimentoOperacionalDTO arg1, AtendimentoOperacionalDTO arg2) {
				HorarioAula h1 = idToHorarioAulaMap.get(arg1.getHorarioId());
				HorarioAula h2 = idToHorarioAulaMap.get(arg2.getHorarioId());

				return h1.getHorario().compareTo( h2.getHorario() );
			}
		});

		return atendimentosOrdenados;
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.AtendimentosService#getAtendimentosParaGradeHorariaVisaoSala(com.gapso.web.trieda.shared.dtos.SalaDTO, com.gapso.web.trieda.shared.dtos.TurnoDTO, com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO)
	 */
	@Override
	public List<AtendimentoRelatorioDTO> getAtendimentosParaGradeHorariaVisaoSala(SalaDTO salaDTO, TurnoDTO turnoDTO, SemanaLetivaDTO semanaLetivaDTO) {
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();

		// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
		List<AtendimentoTaticoDTO> atendimentosTaticoDTO = buscaNoBancoDadosDTOsDeAtendimentoTatico(salaDTO,turnoDTO,semanaLetivaDTO);
		if (!atendimentosTaticoDTO.isEmpty()) {
			// insere os atendimentos do modo tático na lista de atendimentos
			aulas.addAll(atendimentosTaticoDTO);
		}
		else {
			// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
			List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = buscaNoBancoDadosDTOsDeAtendimentoOperacional(salaDTO,turnoDTO,semanaLetivaDTO);
			// processa os atendimentos do operacional e os transforma em aulas
			List<AtendimentoOperacionalDTO> aulasOperacional = extraiAulas(atendimentosOperacionalDTO);
			// insere as aulas do modo operacional na lista de atendimentos
			aulas.addAll(aulasOperacional);
		}
 
		return uneAulasQuePodemSerCompartilhadas(aulas);
	}

	/**
	 * @see com.gapso.web.trieda.shared.services.AtendimentosService#getAtendimentosParaGradeHorariaVisaoSala(com.gapso.web.trieda.shared.dtos.SalaDTO, com.gapso.web.trieda.shared.dtos.TurnoDTO)
	 */
	@Override
	public QuintetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<String>> getAtendimentosParaGradeHorariaVisaoSala(SalaDTO salaDTO, TurnoDTO turnoDTO) {
		List<AtendimentoRelatorioDTO> aulas = new ArrayList<AtendimentoRelatorioDTO>();
		boolean ehTatico = true;

		// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
		List<AtendimentoTaticoDTO> atendimentosTaticoDTO = buscaNoBancoDadosDTOsDeAtendimentoTatico(salaDTO,turnoDTO);
		if (!atendimentosTaticoDTO.isEmpty()) {
			ehTatico = true;
			// insere os atendimentos do modo tático na lista de atendimentos
			aulas.addAll(atendimentosTaticoDTO);
		}
		else {
			ehTatico = false;
			// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
			List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = buscaNoBancoDadosDTOsDeAtendimentoOperacional(salaDTO,turnoDTO);
			// processa os atendimentos do operacional e os transforma em aulas
			List<AtendimentoOperacionalDTO> aulasOperacional = extraiAulas(atendimentosOperacionalDTO);
			// insere as aulas do modo operacional na lista de atendimentos
			aulas.addAll(aulasOperacional);
		}
		
		if (!aulas.isEmpty()) {
	 		// trata compartilhamento de turmas entre cursos
			List<AtendimentoRelatorioDTO> aulasComCompartilhamentos = uneAulasQuePodemSerCompartilhadas(aulas);
			
			// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão e o máximo de 
			// créditos em um dia das semanas letivas relacionadas com as aulas em questão
			Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(aulasComCompartilhamentos);
			TrioDTO<Integer,SemanaLetiva,List<String>> trio = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala,ehTatico,turnoDTO.getId());
			int mdcTemposAula = trio.getPrimeiro();
			SemanaLetiva semanaLetivaComMaiorCargaHoraria = trio.getSegundo();
			List<String> labelsDasLinhasDaGradeHoraria = trio.getTerceiro();
			
			return QuintetoDTO.create(mdcTemposAula,semanaLetivaComMaiorCargaHoraria.calculaMaxCreditos(),semanaLetivaComMaiorCargaHoraria.getTempo(),aulasComCompartilhamentos,labelsDasLinhasDaGradeHoraria);
		} else {
			return QuintetoDTO.create(0,0,0,aulas,Collections.<String>emptyList());
		}
	}
	
	private TrioDTO<Integer,SemanaLetiva,List<String>> calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(Set<Long> semanasLetivasIDsDasAulasNaSala, boolean ehTatico, Long turnoId) {
		List<String> labelsDasLinhasDaGradeHoraria = new ArrayList<String>();
		
		if (!semanasLetivasIDsDasAulasNaSala.isEmpty()) {
			List<SemanaLetiva> todasSemanasLetivas = SemanaLetiva.findAll(getInstituicaoEnsinoUser());
			Map<Long,SemanaLetiva> semanaLetivaIdToSemanaLetivaMap = SemanaLetiva.buildSemanaLetivaIDToSemanaLetivaMap(todasSemanasLetivas);
			List<SemanaLetiva> semanasLetivasDasAulasNaSala = new ArrayList<SemanaLetiva>();
			for (Long semanaLetivaId : semanasLetivasIDsDasAulasNaSala) {
				semanasLetivasDasAulasNaSala.add(semanaLetivaIdToSemanaLetivaMap.get(semanaLetivaId));
			}
			
			SemanaLetiva semanaLetivaComMaiorCargaHoraria = SemanaLetiva.getSemanaLetivaComMaiorCargaHoraria(semanasLetivasDasAulasNaSala); 
			int mdcTemposAula = SemanaLetiva.caculaMaximoDivisorComumParaTemposDeAulaDasSemanasLetivas(semanasLetivasDasAulasNaSala);
			
			if (ehTatico) {
				Integer cargaHorariaMaximaEmMinutos = semanaLetivaComMaiorCargaHoraria.calculaMaxCreditos()*semanaLetivaComMaiorCargaHoraria.getTempo();
				int qtdLinhasDaGradeHoraria = cargaHorariaMaximaEmMinutos/mdcTemposAula;
				Integer cargaHorariaAcumuladaEmMinutos = mdcTemposAula;
				for (int linha = 0; linha < qtdLinhasDaGradeHoraria; linha++) {
					labelsDasLinhasDaGradeHoraria.add(cargaHorariaAcumuladaEmMinutos.toString());
					cargaHorariaAcumuladaEmMinutos += mdcTemposAula;
				}
			} else {
				// coleta todos os pares (HoraInicio,HoraFim) dos horários de aula das semanas letivas
				Set<Pair<Calendar,Calendar>> horarios = new HashSet<Pair<Calendar,Calendar>>();
				for (SemanaLetiva semanaLetiva : semanasLetivasDasAulasNaSala) {
					for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
						if (horarioAula.getTurno().getId().equals(turnoId)) {
							Calendar hi = dateToCalendar(horarioAula.getHorario());
							
							Calendar hf = Calendar.getInstance();
							hf.clear();
							hf.setTime(hi.getTime());
							hf.add(Calendar.MINUTE,semanaLetiva.getTempo());
							
							horarios.add(Pair.create(hi,hf));
						}
					}
				}
				
				// ordena os pares (HoraInicio,HoraFim)
				List<Pair<Calendar,Calendar>> horariosOrdenados = new ArrayList<Pair<Calendar,Calendar>>(horarios);
				Collections.sort(horariosOrdenados,new Comparator<Pair<Calendar,Calendar>>() {
					@Override
					public int compare(Pair<Calendar,Calendar> o1, Pair<Calendar,Calendar> o2) {
						Calendar horarioInicial1 = o1.getLeft();
						Calendar horarioInicial2 = o2.getLeft();
						int ret = horarioInicial1.compareTo(horarioInicial2);
						if (ret == 0) {
							Calendar horarioFinal1 = o1.getRight();
							Calendar horarioFinal2 = o2.getRight();
							return horarioFinal1.compareTo(horarioFinal2);
						}
						return ret;
					}
				});
				
				// processa lista de pares de horários e une aqueles que possuem interseção em um único par (HorarioInicio,HorarioFim)
				List<Pair<Calendar,Calendar>> horariosProcessados = new ArrayList<Pair<Calendar,Calendar>>();
				for (Pair<Calendar,Calendar> parAtual : horariosOrdenados) {
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
				for (Pair<Calendar,Calendar> par : horariosProcessados) {
					Calendar h = par.getLeft();
					Calendar hf = par.getRight();
					while (!h.equals(hf)) {
						labelsDasLinhasDaGradeHoraria.add(TriedaUtil.shortTimeString(h.getTime()));
						h.add(Calendar.MINUTE,mdcTemposAula);
					}
					labelsDasLinhasDaGradeHoraria.add(TriedaUtil.shortTimeString(hf.getTime()));
				}
			} 
			
			return TrioDTO.create(mdcTemposAula,semanaLetivaComMaiorCargaHoraria,labelsDasLinhasDaGradeHoraria);
		}
		return TrioDTO.create(0,null,labelsDasLinhasDaGradeHoraria);
	}

	@SuppressWarnings("deprecation")
	private Calendar dateToCalendar(Date date) {
		Calendar horario = Calendar.getInstance();
		
		horario.clear();
		horario.set(2012,Calendar.FEBRUARY,1,date.getHours(),date.getMinutes(),0);
		horario.set(Calendar.MILLISECOND,0);
		
		return horario;
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.AtendimentosService#getAtendimentosParaGradeHorariaVisaoCurso(com.gapso.web.trieda.shared.dtos.CurriculoDTO, java.lang.Integer, com.gapso.web.trieda.shared.dtos.TurnoDTO, com.gapso.web.trieda.shared.dtos.CampusDTO, com.gapso.web.trieda.shared.dtos.CursoDTO)
	 */
	@Override
	public SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>> getAtendimentosParaGradeHorariaVisaoCurso(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO) {
		// Par<Aulas, Qtd de Colunas para cada Dia da Semana da Grade Horária>
		ParDTO<List<AtendimentoRelatorioDTO>,List<Integer>> parResultante = ParDTO.<List<AtendimentoRelatorioDTO>,List<Integer>>create(new ArrayList<AtendimentoRelatorioDTO>(),null);
		
		// verifica se o campus foi otimizado no modo tático ou no operacional
		if (campusDTO.getOtimizadoTatico()) {
			// Otimização no modo Tático
			
			// busca no BD os atendimentos do modo tático e transforma os mesmos em DTOs
			List<AtendimentoTaticoDTO> aulas = buscaNoBancoDadosDTOsDeAtendimentoTatico(curriculoDTO,periodo,turnoDTO,campusDTO,cursoDTO);
			// Par<Aulas, Qtd de Colunas para cada Dia da Semana da Grade Horária>
			ParDTO<List<AtendimentoTaticoDTO>,List<Integer>> parDTO = montaEstruturaParaGradeHorariaVisaoCursoTatico(turnoDTO,aulas);
			// preenche o par resultante
			parResultante.getPrimeiro().addAll(parDTO.getPrimeiro());
			parResultante.setSegundo(parDTO.getSegundo());
		} else {
			// Otimização no modo Operacional
			
			// busca no BD os atendimentos do modo operacional e transforma os mesmos em DTOs
			List<AtendimentoOperacionalDTO> atendimentosOperacionalDTO = buscaNoBancoDadosDTOsDeAtendimentoOperacional(curriculoDTO,periodo,turnoDTO,campusDTO,cursoDTO);
			// processa os atendimentos do operacional e os transforma em aulas
			List<AtendimentoOperacionalDTO> aulas = extraiAulas(atendimentosOperacionalDTO);
			// Par<Aulas, Qtd de Colunas para cada Dia da Semana da Grade Horária>
			ParDTO<List<AtendimentoOperacionalDTO>,List<Integer>> parDTO = montaEstruturaParaGradeHorariaVisaoCursoOperacional(aulas);
			// preenche o par resultante
			parResultante.getPrimeiro().addAll(parDTO.getPrimeiro());
			parResultante.setSegundo(parDTO.getSegundo());
		}
		
		List<AtendimentoRelatorioDTO> aulas = parResultante.getPrimeiro();
		if (!aulas.isEmpty()) {
			// calcula MDC dos tempos de aula das semanas letivas relacionadas com as aulas em questão e o máximo de 
			// créditos em um dia das semanas letivas relacionadas com as aulas em questão
			Set<Long> semanasLetivasIDsDasAulasNaSala = obtemIDsDasSemanasLetivasAssociadasComAsAulas(parResultante.getPrimeiro());
			TrioDTO<Integer,SemanaLetiva,List<String>> trio = calcula_MDCTemposDeAula_SemanaLetivaComMaiorCargaHoraria_LabelsLinhasGradeHoraria(semanasLetivasIDsDasAulasNaSala,campusDTO.getOtimizadoTatico(),turnoDTO.getId());
			int mdcTemposAula = trio.getPrimeiro();
			SemanaLetiva semanaLetivaComMaiorCargaHoraria = trio.getSegundo();
			List<String> labelsDasLinhasDaGradeHoraria = trio.getTerceiro();
			
			return SextetoDTO.create(mdcTemposAula,semanaLetivaComMaiorCargaHoraria.calculaMaxCreditos(),semanaLetivaComMaiorCargaHoraria.getTempo(),parResultante.getPrimeiro(),parResultante.getSegundo(),labelsDasLinhasDaGradeHoraria);
		} else {
			return SextetoDTO.create(0,0,0,parResultante.getPrimeiro(),parResultante.getSegundo(),Collections.<String>emptyList());
		}
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
	public List<AtendimentoTaticoDTO> buscaNoBancoDadosDTOsDeAtendimentoTatico(SalaDTO salaDTO, TurnoDTO turnoDTO) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.find(salaDTO.getId(),getInstituicaoEnsinoUser());
		Turno turno = Turno.find(turnoDTO.getId(),getInstituicaoEnsinoUser());

		// busca no BD os atendimentos do modo tático
		List<AtendimentoTatico> atendimentosTaticoBD = AtendimentoTatico.findBySalaAndTurno(getInstituicaoEnsinoUser(),sala,turno,null);
		
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
	public List<AtendimentoOperacionalDTO> buscaNoBancoDadosDTOsDeAtendimentoOperacional(SalaDTO salaDTO, TurnoDTO turnoDTO) {
		// obtém os beans de Banco de Dados
		Sala sala = Sala.find(salaDTO.getId(),getInstituicaoEnsinoUser());
		Turno turno = Turno.find(turnoDTO.getId(),getInstituicaoEnsinoUser());

		// busca no BD os atendimentos do modo operacional
		List<AtendimentoOperacional> atendimentosOperacionalBD = AtendimentoOperacional.findBySalaAndTurno(sala,turno,null,getInstituicaoEnsinoUser());

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

				if ( !h0.getHorarioId().equals( h1.getHorarioId() ) )
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
		// [Curso-Disciplina-Turma-DiaSemana-Sala -> List<AtendimentoOperacionalDTO>]
		Map<String,List<AtendimentoOperacionalDTO>> atendimentosAgrupadosMap = new HashMap<String,List<AtendimentoOperacionalDTO>>();
		// Agrupa os DTOS pela chave Curso-Disciplina-Turma-DiaSemana-Sala
		for (AtendimentoOperacionalDTO atendimento : atendimentosDTO) {
			String key = atendimento.getCursoString() + "-" + atendimento.getDisciplinaString() + "-" + atendimento.getTurma() + "-" + atendimento.getSemana() + "-" + atendimento.getSalaId();

			List<AtendimentoOperacionalDTO> grupoAtendimentos = atendimentosAgrupadosMap.get(key);
			if (grupoAtendimentos == null) {
				grupoAtendimentos = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentosAgrupadosMap.put(key,grupoAtendimentos);
			}
			grupoAtendimentos.add(atendimento);
		}
		
		// Quando há mais de um DTO por chave [Curso-Disciplina-Turma-DiaSemana-Sala], concatena as informações de todos em um único DTO.
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
					AtendimentoOperacionalDTO aula = subgrupoDeAtendimentosConsecutivos.get(0) ;
	
					// procura pelo horário correspondente ao início da aula
					// TODO: CREIO QUE AS 3 LINHAS ABAIXO SEJAM DESNECESSÁRIAS UMA VEZ QUE OS ATENDIMENTOS JÁ FORAM ORDENADOS PREVIAMENTE
					HorarioAula menorHorario = AtendimentoOperacional.retornaAtendimentoMenorHorarioAula(ConvertBeans.toListAtendimentoOperacional(subgrupoDeAtendimentosConsecutivos));
					aula.setHorarioId( menorHorario.getId() );
					aula.setHorarioString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
					
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
	
				if (!saoConsecutivos(horariosAulaAnterior,horariosAulaAtual)) {
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
		// Agrupa os DTOS pela chave [ Disciplina - Turma - DiaSemana - HorárioInício (caso seja o operacional) ]
		Map<String,List<AtendimentoRelatorioDTO>> atendimentoTaticoDTOMap = new HashMap<String,List<AtendimentoRelatorioDTO>>();

		for (AtendimentoRelatorioDTO aula : aulas) {
			String disciplinaInfo = (aula.getDisciplinaSubstitutaId() != null) ? aula.getDisciplinaSubstitutaString() : aula.getDisciplinaString();
			String key = disciplinaInfo + "-" + aula.getTurma() + "-" + aula.getSemana() + (aula.getHorarioId() != null ? ("-" + aula.getHorarioId()) : "");
			List<AtendimentoRelatorioDTO> aulasASeremCompartilhadas = atendimentoTaticoDTOMap.get(key);
			if (aulasASeremCompartilhadas == null) {
				aulasASeremCompartilhadas = new ArrayList<AtendimentoRelatorioDTO>();
				atendimentoTaticoDTOMap.put(key,aulasASeremCompartilhadas);
			}
			aulasASeremCompartilhadas.add(aula);
		}

		// Quando há mais de um DTO por chave [Disciplina-Turma-DiaSemana],
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
	
	private ParDTO<List<AtendimentoTaticoDTO>,List<Integer>> montaEstruturaParaGradeHorariaVisaoCursoTatico(TurnoDTO turnoDTO, List<AtendimentoTaticoDTO> aulas) {
		// [DiaSemana -> Aulas]
		Map<Integer,List<AtendimentoTaticoDTO>> diaSemanaToAulasMap = new TreeMap<Integer,List<AtendimentoTaticoDTO>>();
		// mapeia as aulas com o dia da semana em questão
		for (AtendimentoTaticoDTO aula : aulas) {
			List<AtendimentoTaticoDTO> aulasNoMesmoDiaSemana = diaSemanaToAulasMap.get(aula.getSemana());
			if (aulasNoMesmoDiaSemana == null) {
				aulasNoMesmoDiaSemana = new ArrayList<AtendimentoTaticoDTO>();
				diaSemanaToAulasMap.put(aula.getSemana(),aulasNoMesmoDiaSemana);
			}

			aulasNoMesmoDiaSemana.add(aula);
		}
		
		// preenche as entradas nulas do mapa diaSemanaToAulasMap com uma lista vazia.
		// 1=DOM, 2=SEG, 3=TER, 4=QUA, 5=QUI, 6=SEX, 7=SAB
		for (int i = 2; i <= 7; i++) {
			if (diaSemanaToAulasMap.get(i) == null) {
				diaSemanaToAulasMap.put(i,Collections.<AtendimentoTaticoDTO>emptyList());
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
		
		List<AtendimentoTaticoDTO> listaResultanteComAulas = new ArrayList<AtendimentoTaticoDTO>();
		// para cada dia da semana, agrupa as aulas que ocorrerão em paralelo, atualiza a coluna em que cada aula
		// será desenhada na grade horária e atualiza a quantidade de colunas que serão necessárias em cada dia
		// da semana
		int colunaNaGradeHoraria = 2;
		for (Entry<Integer,List<AtendimentoTaticoDTO>> entry : diaSemanaToAulasMap.entrySet()) {
			Integer diaSemana = entry.getKey();
			List<AtendimentoTaticoDTO> aulasMesmoDiaSemana = entry.getValue();
			
			List<List<AtendimentoTaticoDTO>> gruposAulasParalelas = agrupaAulasQueOcorreraoEmParalelo(turnoDTO,diaSemana,aulasMesmoDiaSemana);

			int maiorQuantidadeAulasEmParalelo = 1;
			for (List<AtendimentoTaticoDTO> aulasParalelas : gruposAulasParalelas) {
				if (aulasParalelas.size() > maiorQuantidadeAulasEmParalelo) {
					maiorQuantidadeAulasEmParalelo = aulasParalelas.size();
				}

				AtendimentoTaticoDTO primeiraAulaParalela = aulasParalelas.get(0);
				primeiraAulaParalela.setSemana(colunaNaGradeHoraria);
				for (int i = 1; i < aulasParalelas.size(); i++) {
					AtendimentoTaticoDTO proximaAulaParalela = aulasParalelas.get(i);
					proximaAulaParalela.setSemana(primeiraAulaParalela.getSemana()+i);
				}

				listaResultanteComAulas.addAll(aulasParalelas);
			}
			
			if (gruposAulasParalelas.isEmpty()) {
				for (AtendimentoTaticoDTO aula : aulasMesmoDiaSemana) {
					aula.setSemana(colunaNaGradeHoraria);
				}
				listaResultanteComAulas.addAll(aulasMesmoDiaSemana);
			}

			qtdColunasGradeHorariaPorDiaSemana.add(diaSemana,maiorQuantidadeAulasEmParalelo);
			colunaNaGradeHoraria += maiorQuantidadeAulasEmParalelo;
		}

		adicionaDadosCompartilhamentoSalaCursoTatico(listaResultanteComAulas);

		return ParDTO.create(listaResultanteComAulas,qtdColunasGradeHorariaPorDiaSemana);
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
	private ParDTO<List<AtendimentoOperacionalDTO>,List<Integer>> montaEstruturaParaGradeHorariaVisaoCursoOperacional(List<AtendimentoOperacionalDTO> aulas) {
		// [DiaSemana -> Aulas]
		Map<Integer,List<AtendimentoOperacionalDTO>> diaSemanaToAulasMap = new TreeMap<Integer,List<AtendimentoOperacionalDTO>>();
		// mapeia as aulas com o dia da semana em questão
		for (AtendimentoOperacionalDTO aula : aulas) {
			List<AtendimentoOperacionalDTO> aulasNoMesmoDiaSemana = diaSemanaToAulasMap.get(aula.getSemana());
			if (aulasNoMesmoDiaSemana == null) {
				aulasNoMesmoDiaSemana = new ArrayList<AtendimentoOperacionalDTO>();
				diaSemanaToAulasMap.put(aula.getSemana(),aulasNoMesmoDiaSemana);
			}

			aulasNoMesmoDiaSemana.add(aula);
		}

		// preenche as entradas nulas do mapa diaSemanaToAulasMap com uma lista vazia.
		// 1=DOM, 2=SEG, 3=TER, 4=QUA, 5=QUI, 6=SEX, 7=SAB
		for (int i = 2; i <= 7; i++) {
			if (diaSemanaToAulasMap.get(i) == null) {
				diaSemanaToAulasMap.put(i,Collections.<AtendimentoOperacionalDTO>emptyList());
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

		List<AtendimentoOperacionalDTO> listaResultanteComAulas = new ArrayList<AtendimentoOperacionalDTO>();

		// para cada dia da semana, agrupa as aulas que ocorrerão em paralelo, atualiza a coluna em que cada aula
		// será desenhada na grade horária e atualiza a quantidade de colunas que serão necessárias em cada dia
		// da semana
		int colunaNaGradeHoraria = 2;
		for (Entry<Integer,List<AtendimentoOperacionalDTO>> entry : diaSemanaToAulasMap.entrySet()) {
			Integer diaSemana = entry.getKey();
			List<AtendimentoOperacionalDTO> aulasMesmoDiaSemana = entry.getValue();
			
			List<List<AtendimentoOperacionalDTO>> gruposAulasParalelas = agrupaAulasQueOcorreraoEmParalelo(aulasMesmoDiaSemana);

			int maiorQuantidadeAulasEmParalelo = 1;
			for (List<AtendimentoOperacionalDTO> aulasParalelas : gruposAulasParalelas) {
				if (aulasParalelas.size() > maiorQuantidadeAulasEmParalelo) {
					maiorQuantidadeAulasEmParalelo = aulasParalelas.size();
				}

				AtendimentoOperacionalDTO primeiraAulaParalela = aulasParalelas.get(0);
				primeiraAulaParalela.setSemana(colunaNaGradeHoraria);
				for (int i = 1; i < aulasParalelas.size(); i++) {
					AtendimentoOperacionalDTO proximaAulaParalela = aulasParalelas.get(i);
					proximaAulaParalela.setSemana(primeiraAulaParalela.getSemana()+i);
				}

				// ordenando por horário de início da aula
				// TODO: CREIO QUE NÃO SEJA NECESSÁRIO ORDENAR AS AULAS AQUI
				aulasParalelas = ordenaPorHorarioAula(aulasParalelas);

				listaResultanteComAulas.addAll(aulasParalelas);
			}

			qtdColunasGradeHorariaPorDiaSemana.add(diaSemana,maiorQuantidadeAulasEmParalelo);
			colunaNaGradeHoraria += maiorQuantidadeAulasEmParalelo;
		}

		//adicionaDadosCompartilhamentoSalaCursoOperacional( finalProcessedList );

		return ParDTO.create(listaResultanteComAulas,qtdColunasGradeHorariaPorDiaSemana);
	}
	
	private List<List<AtendimentoTaticoDTO>> agrupaAulasQueOcorreraoEmParalelo(TurnoDTO turnoDTO, Integer diaSemana, List<AtendimentoTaticoDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoTaticoDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoTaticoDTO>>();
		
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
				gruposAulasParalelas = agrupaAulasQueOcorreraoEmParaleloAbordagem2(aulasMesmoDiaSemana);
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
	private List<List<AtendimentoTaticoDTO>> agrupaAulasQueOcorreraoEmParaleloAbordagem1(List<AtendimentoTaticoDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoTaticoDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoTaticoDTO>>();

		for (AtendimentoTaticoDTO aulaAtual : aulasMesmoDiaSemana) {
			if (gruposAulasParalelas.isEmpty()) {
				gruposAulasParalelas.add(new ArrayList<AtendimentoTaticoDTO>());
				gruposAulasParalelas.get(0).add(aulaAtual);
			} else {
				boolean paralelismoIdentificadoComAulaAtual = false;

				// verifica se há paralelismo entre a aula atual e alguma aula processada anteriormente
				for (List<AtendimentoTaticoDTO> aulasParalelas : gruposAulasParalelas) {
					boolean naoSaoCandidatasAOcorreremEmParelelo = false;
					for (AtendimentoTaticoDTO aulaProcessadaAnteriormente : aulasParalelas) {
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
					gruposAulasParalelas.add(new ArrayList<AtendimentoTaticoDTO>());
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
	private List<List<AtendimentoTaticoDTO>> agrupaAulasQueOcorreraoEmParaleloAbordagem2(List<AtendimentoTaticoDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoTaticoDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoTaticoDTO>>();

		// ordena as aulas pelo número da turma
		List<AtendimentoTaticoDTO> aulasOrdenadasPelaTurma = new ArrayList<AtendimentoTaticoDTO>(aulasMesmoDiaSemana);
		Collections.sort(aulasOrdenadasPelaTurma, new Comparator<AtendimentoTaticoDTO>() {
			@Override
			public int compare(AtendimentoTaticoDTO o1, AtendimentoTaticoDTO o2) {
				return o1.getTurma().compareTo(o2.getTurma());
			}
		});

		for (AtendimentoTaticoDTO aulaAtual : aulasOrdenadasPelaTurma) {
			if (gruposAulasParalelas.isEmpty()) {
				gruposAulasParalelas.add(new ArrayList<AtendimentoTaticoDTO>());
				gruposAulasParalelas.get(0).add(aulaAtual);
			} else {
				boolean paralelismoIdentificadoComAulaAtual = false;
				
				// verifica se há paralelismo entre a aula atual e alguma aula processada anteriormente
				for (List<AtendimentoTaticoDTO> aulasParalelas : gruposAulasParalelas) {
					boolean naoSaoCandidatasAOcorreremEmParelelo = false;
					for (AtendimentoTaticoDTO aulaProcessadaAnteriormente : aulasParalelas) {
						if (!AtendimentoTaticoDTO.podemOcorrerEmParaleloAbordagem2(aulaAtual,aulaProcessadaAnteriormente)) {
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
					gruposAulasParalelas.add(new ArrayList<AtendimentoTaticoDTO>());
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
	private List<List<AtendimentoOperacionalDTO>> agrupaAulasQueOcorreraoEmParalelo(List<AtendimentoOperacionalDTO> aulasMesmoDiaSemana) {
		List<List<AtendimentoOperacionalDTO>> gruposAulasParalelas = new ArrayList<List<AtendimentoOperacionalDTO>>();
		
		// ordena as aulas pela turma para garantir que o paralelismo viável seja construído corretamente
		Collections.sort(aulasMesmoDiaSemana,new Comparator<AtendimentoOperacionalDTO>() {
			@Override
			public int compare(AtendimentoOperacionalDTO o1, AtendimentoOperacionalDTO o2) {
				return o1.getTurma().compareTo(o2.getTurma());
			}
		});

		for (AtendimentoOperacionalDTO aulaAtual : aulasMesmoDiaSemana) {
			if (gruposAulasParalelas.isEmpty()) {
				gruposAulasParalelas.add(new ArrayList<AtendimentoOperacionalDTO>());
				gruposAulasParalelas.get(0).add(aulaAtual);
			}
			else {
				boolean paralelismoIdentificadoComAulaAtual = false;

				// verifica se há paralelismo entre a aula atual e alguma aula processada anteriormente
				for (List<AtendimentoOperacionalDTO> aulasParalelas : gruposAulasParalelas) {
					boolean naoHaIntersecaoEntreAulas = false;
					// verifica se há ou não interseção entre a aula atual e as aulas previamente processadas
					for (AtendimentoOperacionalDTO aulaProcessadaAnteriormente : aulasParalelas) {
						if (!temIntersecao(aulaAtual,aulaProcessadaAnteriormente)) {
							naoHaIntersecaoEntreAulas = true;
							break;
						}
					}

					if (!naoHaIntersecaoEntreAulas) {
						aulasParalelas.add(aulaAtual);
						paralelismoIdentificadoComAulaAtual = true;
						break;
					}
				}

				if (!paralelismoIdentificadoComAulaAtual) {
					gruposAulasParalelas.add(new ArrayList<AtendimentoOperacionalDTO>());
					gruposAulasParalelas.get(gruposAulasParalelas.size()-1).add(aulaAtual);
				}
			}
		}

		return gruposAulasParalelas;
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

//	@Override
//	public ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > getBusca(
//		CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO )
//	{
//		return this.getAtendimentosParaGradeHorariaVisaoCurso( curriculoDTO, periodo, turnoDTO, campusDTO, null );
//	}

	// Implementaçao da verifição relacionada com a issue
	// http://jira.gapso.com.br/browse/TRIEDA-979
	private void adicionaDadosCompartilhamentoSalaCursoOperacional(
		List< AtendimentoOperacionalDTO > atendimentos )
	{
		Map< ParDTO< Sala, ParDTO< Integer, Long > >, List< AtendimentoOperacionalDTO > > mapSalaAtendimentos
			= new HashMap< ParDTO< Sala, ParDTO< Integer, Long > >, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento : atendimentos )
		{
			Sala sala = Sala.find( atendimento.getSalaId(), getInstituicaoEnsinoUser() );
			Integer dia = atendimento.getSemana();
			Long horario = atendimento.getHorarioId();

			ParDTO< Sala, ParDTO< Integer, Long > > key = ParDTO.create(sala,ParDTO.create(dia,horario));

			List< AtendimentoOperacionalDTO > list = mapSalaAtendimentos.get( key );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoOperacionalDTO >();
				mapSalaAtendimentos.put( key, list );
			}

			list.add( atendimento );
		}

		// Informa que essa aula é compartilhada por mais de um curso
		for ( Entry< ParDTO< Sala, ParDTO< Integer, Long > >,
				List< AtendimentoOperacionalDTO > > entry
				: mapSalaAtendimentos.entrySet() )
		{
			List< AtendimentoOperacionalDTO > list = entry.getValue();
			montaStringCompartilhamentoSalaCursosOperacional( list );
		}
	}

	private void montaStringCompartilhamentoSalaCursosOperacional(
		List< AtendimentoOperacionalDTO > list )
	{
		if ( list != null && list.size() > 0 )
		{
			Set< Long > idsCursos = new HashSet< Long >();
			for ( AtendimentoOperacionalDTO atendimento : list )
			{
				idsCursos.add( atendimento.getCursoId() );
			}

			List< Long > listIds = new ArrayList< Long >( idsCursos );

			Curso curso = Curso.find( listIds.get( 0 ),
				getInstituicaoEnsinoUser() );

			String nomeCursos = "";

			if ( curso != null )
			{
				nomeCursos = curso.getNome();
			}

			for ( int i = 1; i < listIds.size(); i++ )
			{
				curso = Curso.find( listIds.get( i ),
					getInstituicaoEnsinoUser() );

				if ( curso != null )
				{			
					nomeCursos += ( ", " + curso.getNome() );
				}
			}

			for ( AtendimentoOperacionalDTO atendimento : list )
			{
				atendimento.setCompartilhamentoCursosString( nomeCursos );
			}
		}
	}

	// Implementaçao da verifição relacionada com a issue
	// http://jira.gapso.com.br/browse/TRIEDA-979
	private void adicionaDadosCompartilhamentoSalaCursoTatico(
		List< AtendimentoTaticoDTO > atendimentos )
	{
		Map< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > > mapSalaAtendimentos
			= new HashMap< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > >();

		for ( AtendimentoTaticoDTO atendimento : atendimentos )
		{
			Sala sala = Sala.find( atendimento.getSalaId(), getInstituicaoEnsinoUser() );
			Integer dia = atendimento.getSemana();

			// No modelo tático, consideramos apenas a
			// sala e o dia da aula, para agrupar os atendimentos
			// de acordo com os cursos que compartilham essa sala
			// Long horario = atendimento.getHorario();

			ParDTO< Sala, Integer > key = ParDTO.create(sala,dia);
			List< AtendimentoTaticoDTO > list = mapSalaAtendimentos.get( key );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoTaticoDTO >();
				mapSalaAtendimentos.put( key, list );
			}

			list.add( atendimento );
		}

		// Informa que essa aula é compartilhada por mais de um curso
		for ( Entry< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > > entry
			: mapSalaAtendimentos.entrySet() )
		{
			List< AtendimentoTaticoDTO > list = entry.getValue();
			montaStringCompartilhamentoSalaCursosTatico( list );
		}
	}

	private void montaStringCompartilhamentoSalaCursosTatico(
		List< AtendimentoTaticoDTO > list )
	{
		if ( list != null && list.size() > 0 )
		{
			Set< Long > idsCursos = new HashSet< Long >();
			for ( AtendimentoTaticoDTO atendimento : list )
			{
				idsCursos.add( atendimento.getCursoId() );
			}

			List< Long > listIds = new ArrayList< Long >( idsCursos );

			Curso curso = Curso.find( listIds.get( 0 ),
				getInstituicaoEnsinoUser() );

			String nomeCursos = "";
			
			if ( curso != null )
			{
				nomeCursos = curso.getNome();
			}

			for ( int i = 1; i < listIds.size(); i++ )
			{
				curso = Curso.find( listIds.get( i ),
					this.getInstituicaoEnsinoUser() );

				if ( curso != null )
				{
					nomeCursos += ( ", " + curso.getNome() );
				}
			}

			for ( AtendimentoTaticoDTO atendimento : list )
			{
				atendimento.setCompartilhamentoCursosString( nomeCursos );
			}
		}
	}

	private boolean temIntersecao(AtendimentoOperacionalDTO dto1, AtendimentoOperacionalDTO dto2) {
		if (dto1.getSemana().equals(dto2.getSemana())) {
			if (dto1.getHorarioId().equals(dto2.getHorarioId())) {
				return true;
			} else {
				Pair<Calendar,Calendar> horarios1 = extraiHorariosInicial_e_Final(dto1);
				Pair<Calendar,Calendar> horarios2 = extraiHorariosInicial_e_Final(dto2);
				return temIntersecao(horarios1,horarios2);
			}
		}
		return false;
	}
	
	private Pair<Calendar,Calendar> extraiHorariosInicial_e_Final(AtendimentoOperacionalDTO dto) {
		Calendar horarioInicial = Calendar.getInstance();
		String[] horarioInicialArray = dto.getHorarioString().split(":");
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

	@Override
	public List< AtendimentoOperacionalDTO > getAtendimentosOperacional(
		ProfessorDTO professorDTO, ProfessorVirtualDTO professorVirtualDTO,
		TurnoDTO turnoDTO, boolean isVisaoProfessor, SemanaLetivaDTO semanaLetivaDTO )
	{
		boolean isAdmin = isAdministrador();
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );

		Professor professor = ( professorDTO == null ? null : Professor.find( professorDTO.getId(), getInstituicaoEnsinoUser() ) );
		ProfessorVirtual professorVirtual = ( professorVirtualDTO == null ? null : ProfessorVirtual.find( professorVirtualDTO.getId(), getInstituicaoEnsinoUser() ) );
		SemanaLetiva semanaLetiva = SemanaLetiva.find( semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );
		List< AtendimentoOperacional > atendimentosOperacional = AtendimentoOperacional.getAtendimentosOperacional( getInstituicaoEnsinoUser(), isAdmin, professor, professorVirtual, turno, isVisaoProfessor, semanaLetiva );
		List< AtendimentoOperacionalDTO > atendimentosOperacionalDTO = ConvertBeans.toListAtendimentoOperacionalDTO(atendimentosOperacional);
		List< AtendimentoRelatorioDTO > aulas = new ArrayList< AtendimentoRelatorioDTO >(extraiAulas(atendimentosOperacionalDTO));
		
		List< AtendimentoRelatorioDTO > tempDTOList = uneAulasQuePodemSerCompartilhadas( aulas );
		List<AtendimentoOperacionalDTO> resultDTOList = new ArrayList<AtendimentoOperacionalDTO>();
		for (AtendimentoRelatorioDTO ar : tempDTOList) {
			resultDTOList.add((AtendimentoOperacionalDTO)ar);
		}
		
		return resultDTOList;
	}

	private List< AtendimentoOperacionalDTO > montaListaParaVisaoProfessor1(
		List< AtendimentoOperacionalDTO > list )
	{
		// Agrupa os DTOS pela chave [ Curso - Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List< AtendimentoOperacionalDTO > > atendimentoOperacionalDTOMap
			= new HashMap< String, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO dto : list )
		{
			String key = dto.getCursoString()
				+ "-" + dto.getDisciplinaString() + "-" + dto.getTurma()
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
		// [ Curso - Disciplina - Turma - DiaSemana - Sala ],
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
				AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 ) ;

				// Procura pelo horário correspondente ao início da aula
				HorarioAula menorHorario = AtendimentoOperacional.retornaAtendimentoMenorHorarioAula(
				    ConvertBeans.toListAtendimentoOperacional( ordenadoPorHorario ) );

				for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
				{
					AtendimentoOperacionalDTO dtoCurrent = ordenadoPorHorario.get( i );
					dtoMain.concatenateVisaoProfessor( dtoCurrent );
				}

				dtoMain.setHorarioId( menorHorario.getId() );
				dtoMain.setHorarioString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
				dtoMain.setTotalCreditos( entry.getValue().size() );

				processedList.add( dtoMain );
			}
		}

		return processedList;
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

				dtoMain.setHorarioId( menorHorario.getId() );
				dtoMain.setHorarioString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
				dtoMain.setTotalCreditos( ordenadoPorHorario.size() );

				processedList.add( dtoMain );
			}
		}

		return processedList;
	}

	@Override
	public ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CampusDTO campusDTO )
	{
		Campus campus = Campus.find(
			campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< ProfessorVirtual > professoresVirtuais
			= ProfessorVirtual.findBy( getInstituicaoEnsinoUser(), campus );

		List< ProfessorVirtualDTO > professoresVirtuaisDTO
			= new ArrayList< ProfessorVirtualDTO >();

		for ( ProfessorVirtual professorVirtual : professoresVirtuais )
		{
			professoresVirtuaisDTO.add(
				ConvertBeans.toProfessorVirtualDTO( professorVirtual ) );
		}

		return new BaseListLoadResult< ProfessorVirtualDTO >( professoresVirtuaisDTO );
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
			atendimentosDia.get( 0 ).getHorarioId() );

		for ( int i = 1; i < atendimentosDia.size(); i++ )
		{
			AtendimentoOperacionalDTO atDTO = atendimentosDia.get( i );
			HorarioAula ha = mapHorarios.get( atDTO.getHorarioId() );

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
}
