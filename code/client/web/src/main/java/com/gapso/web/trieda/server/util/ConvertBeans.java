package com.gapso.web.trieda.server.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Dificuldades;
import com.gapso.trieda.misc.Estados;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.AtendimentoTaticoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.DivisaoCreditoDTO;
import com.gapso.web.trieda.client.mvp.model.EquivalenciaDTO;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorCampusDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoContratoDTO;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;

public class ConvertBeans {
	
	
	// CENÁRIO
	public static Cenario toCenario(CenarioDTO dto) {
		Cenario domain = new Cenario();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setMasterData(dto.getMasterData());
		domain.setOficial(dto.getOficial());
		domain.setAno(dto.getAno());
		domain.setSemestre(dto.getSemestre());
		// TODO Criar ligação com o usuário (Edição e criação)
		// TODO Criar ligação com data de edição e criação
		domain.setComentario(dto.getComentario());
		SemanaLetiva semanaLetiva = SemanaLetiva.find(dto.getSemanaLetivaId());
		domain.setSemanaLetiva(semanaLetiva);
		return domain;
	}
	
	public static CenarioDTO toCenarioDTO(Cenario domain) {
		CenarioDTO dto = new CenarioDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setMasterData(domain.getMasterData());
		dto.setOficial(domain.getOficial());
		dto.setAno(domain.getAno());
		dto.setSemestre(domain.getSemestre());
		// TODO Criar ligação com o usuário (Edição e criação)
		// TODO Criar ligação com data de edição e criação
		dto.setComentario(domain.getComentario());
		if(!domain.getMasterData()) {
			dto.setSemanaLetivaId(domain.getSemanaLetiva().getId());
			dto.setSemanaLetivaString(domain.getSemanaLetiva().getCodigo());
		}
		return dto;
	}
	
	// CAMPUS
	public static Campus toCampus(CampusDTO dto) {
		Campus domain = new Campus();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Cenario cenario = Cenario.find(dto.getCenarioId());
		domain.setCenario(cenario);
		domain.setNome(dto.getNome());
		domain.setCodigo(dto.getCodigo());
		for(Estados estadoEnum : Estados.values()) {
			if(estadoEnum.name().equals(dto.getEstado())) {
				domain.setEstado(estadoEnum);
				break;
			}
		}
		domain.setMunicipio(dto.getMunicipio());
		domain.setBairro(dto.getBairro());
		domain.setValorCredito(dto.getValorCredito());
		return domain;
	}
	
	public static CampusDTO toCampusDTO(Campus domain) {
		CampusDTO dto = new CampusDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setCodigo(domain.getCodigo());
		dto.setValorCredito(domain.getValorCredito());
		if(domain.getEstado() != null) dto.setEstado(domain.getEstado().name());
		if(domain.getMunicipio() != null) dto.setMunicipio(domain.getMunicipio());
		if(domain.getBairro() != null) dto.setBairro(domain.getBairro());
		
		return dto;
	}
	
	// UNIDADE
	public static Unidade toUnidade(UnidadeDTO dto) {
		Unidade domain = new Unidade();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setCodigo(dto.getCodigo());
		Campus campus = Campus.find(dto.getCampusId());
		domain.setCampus(campus);
		return domain;
	}
	
	public static UnidadeDTO toUnidadeDTO(Unidade domain) {
		UnidadeDTO dto = new UnidadeDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setCodigo(domain.getCodigo());
		
		Campus campus = domain.getCampus();
		dto.setCampusId(campus.getId());
		dto.setCampusString(campus.getCodigo());
		dto.setDisplay(domain.getNome() + " (" + domain.getCodigo() +")");
		return dto;
	}
	
	// PROFESSOR CAMPUS
	public static List<ProfessorCampusDTO> toProfessorCampusDTO(Campus campus) {
		Set<Professor> professores = campus.getProfessores();
		List<ProfessorCampusDTO> list = new ArrayList<ProfessorCampusDTO>(professores.size());
		for(Professor professor : professores) {
			ProfessorCampusDTO dto = new ProfessorCampusDTO();
			dto.setProfessorId(professor.getId());
			dto.setProfessorString(professor.getNome());
			dto.setProfessorCpf(professor.getCpf());
			dto.setCampusId(campus.getId());
			dto.setCampusString(campus.getCodigo());
			list.add(dto);
		}
		return list;
	}
	
	public static List<ProfessorCampusDTO> toProfessorCampusDTO(Professor professor) {
		Set<Campus> campi = professor.getCampi();
		List<ProfessorCampusDTO> list = new ArrayList<ProfessorCampusDTO>(campi.size());
		for(Campus campus : campi) {
			ProfessorCampusDTO dto = new ProfessorCampusDTO();
			dto.setProfessorId(professor.getId());
			dto.setProfessorString(professor.getNome());
			dto.setProfessorCpf(professor.getCpf());
			dto.setCampusId(campus.getId());
			dto.setCampusString(campus.getCodigo());
			list.add(dto);
		}
		return list;
	}
	
	// TIPO SALA
	public static TipoSala toTipoSala(TipoSalaDTO dto) {
		TipoSala domain = new TipoSala();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setDescricao(dto.getDescricao());
		return domain;
	}
	
	public static TipoSalaDTO toTipoSalaDTO(TipoSala domain) {
		TipoSalaDTO dto = new TipoSalaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setDescricao(domain.getDescricao());
		return dto;
	}
	
	// SALA
	public static Collection<SalaDTO> toSalaDTO(Collection<Sala> domains) {
		Collection<SalaDTO> list = new ArrayList<SalaDTO>(domains.size());
		for(Sala domain : domains) {
			list.add(toSalaDTO(domain));
		}
		return list;
	}
	public static Sala toSala(SalaDTO dto) {
		Sala domain = new Sala();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setNumero(dto.getNumero());
		domain.setAndar(dto.getAndar());
		domain.setCapacidade(dto.getCapacidade());
		
		TipoSala tipoSala = TipoSala.find(dto.getTipoId());
		domain.setTipoSala(tipoSala);
		
		Unidade unidade = Unidade.find(dto.getUnidadeId());
		domain.setUnidade(unidade);
		
		return domain;
	}
	public static SalaDTO toSalaDTO(Sala domain) {
		SalaDTO dto = new SalaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setNumero(domain.getNumero());
		dto.setAndar(domain.getAndar());
		dto.setCapacidade(domain.getCapacidade());
		dto.setTipoId(domain.getTipoSala().getId());
		dto.setTipoString(domain.getTipoSala().getNome());
		dto.setCampusId(domain.getUnidade().getCampus().getId());
		dto.setUnidadeId(domain.getUnidade().getId());
		dto.setUnidadeString(domain.getUnidade().getCodigo());
		return dto;
	}

	// GRUPO DE SALA
	public static GrupoSala toGrupoSala(GrupoSalaDTO dto) {
		GrupoSala domain = new GrupoSala();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setNome(dto.getNome());
		
		Unidade unidade = Unidade.find(dto.getUnidadeId());
		domain.setUnidade(unidade);
		
		return domain;
	}
	
	public static GrupoSalaDTO toGrupoSalaDTO(GrupoSala domain) {
		GrupoSalaDTO dto = new GrupoSalaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setNome(domain.getNome());
		dto.setUnidadeId(domain.getUnidade().getId());
		dto.setUnidadeString(domain.getUnidade().getCodigo());
		dto.setCampusId(domain.getUnidade().getCampus().getId());
		return dto;
	}
	
	// TURNO
	public static Collection<TurnoDTO> toTurnoDTO(Collection<Turno> domains) {
		Collection<TurnoDTO> list = new ArrayList<TurnoDTO>(domains.size());
		for(Turno domain : domains) {
			list.add(toTurnoDTO(domain));
		}
		return list;
	}
	public static Turno toTurno(TurnoDTO dto) {
		Turno domain = new Turno();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Cenario cenario = Cenario.find(dto.getCenarioId());
		domain.setCenario(cenario);
		domain.setNome(dto.getNome());
		domain.setTempo(dto.getTempo());
		return domain;
	}
	public static TurnoDTO toTurnoDTO(Turno domain) {
		TurnoDTO dto = new TurnoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setNome(domain.getNome());
		dto.setTempo(domain.getTempo());
		dto.setMaxCreditos(domain.getHorariosAula().size());
		return dto;
	}

	// CALENDÁRIO
	public static SemanaLetiva toSemanaLetiva(SemanaLetivaDTO dto) {
		SemanaLetiva domain = new SemanaLetiva();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setDescricao(dto.getDescricao());
		return domain;
	}

	public static SemanaLetivaDTO toSemanaLetivaDTO(SemanaLetiva domain) {
		SemanaLetivaDTO dto = new SemanaLetivaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setDescricao(domain.getDescricao());
		return dto;
	}

	// HORARIO DE AULA
	public static HorarioAula toHorarioAula(HorarioAulaDTO dto) {
		HorarioAula domain = new HorarioAula();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		SemanaLetiva calendario = SemanaLetiva.find(dto.getSemanaLetivaId());
		domain.setSemanaLetiva(calendario);
		Turno turno = Turno.find(dto.getTurnoId());
		domain.setTurno(turno);
		domain.setHorario(dto.getInicio());
		return domain;
	}

	public static HorarioAulaDTO toHorarioAulaDTO(HorarioAula domain) {
		HorarioAulaDTO dto = new HorarioAulaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setSemanaLetivaId(domain.getSemanaLetiva().getId());
		dto.setSemanaLetivaString(domain.getSemanaLetiva().getCodigo());
		Turno turno = domain.getTurno();
		dto.setTurnoId(turno.getId());
		dto.setTurnoString(turno.getNome());

		dto.setInicio(domain.getHorario());

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime(domain.getHorario());
		fimCal.add(Calendar.MINUTE, turno.getTempo());
		dto.setFim(fimCal.getTime());
		return dto;
	}


	// HORARIO DISPONÍVEL CENÁRIO
	public static List<HorarioDisponivelCenario> toHorarioDisponivelCenario(List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listDomain = new ArrayList<HorarioDisponivelCenario>();
		for(HorarioDisponivelCenarioDTO dto : listDTO) {
			HorarioAula horarioAula = HorarioAula.find(dto.getHorarioDeAulaId());
			HorarioDisponivelCenario domain = null;
			if(dto.getSegunda()) {
				domain = null;
				if(dto.getSegundaId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.SEG);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getSegundaId());
				}
				if(domain != null) { listDomain.add(domain); }
			}
			if(dto.getTerca()) {
				domain = null;
				if(dto.getTercaId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.TER);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getTercaId());
				}
				if(domain != null) { listDomain.add(domain); }
			}
			if(dto.getQuarta()) {
				domain = null;
				if(dto.getQuartaId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.QUA);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getQuartaId());
				}
				if(domain != null) { listDomain.add(domain); }
			}
			if(dto.getQuinta()) {
				domain = null;
				if(dto.getQuintaId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.QUI);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getQuintaId());
				}
				if(domain != null) { listDomain.add(domain); }
			} 
			if(dto.getSexta()) {
				domain = null;
				if(dto.getSextaId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.SEX);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getSextaId());
				}
				if(domain != null) { listDomain.add(domain); }
			}
			if(dto.getSabado()) {
				domain = null;
				if(dto.getSabadoId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.SAB);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getSabadoId());
				}
				if(domain != null) { listDomain.add(domain); }
			}
			if(dto.getDomingo()) {
				domain = null;
				if(dto.getDomingoId() == null) {
					domain = new HorarioDisponivelCenario();
					domain.setSemana(Semanas.DOM);
					domain.setHorarioAula(horarioAula);
				} else {
					domain = HorarioDisponivelCenario.findHorarioDisponivelCenario(dto.getDomingoId());
				}
				if(domain != null) { listDomain.add(domain); }
 			}
		}
		
		return listDomain;
	}

	public static HorarioDisponivelCenarioDTO toHorarioDisponivelCenarioDTO(HorarioAula domain) {
		
		HorarioDisponivelCenarioDTO dto = new HorarioDisponivelCenarioDTO();
		dto.setHorarioDeAulaId(domain.getId());
		dto.setHorarioDeAulaVersion(domain.getVersion());
		dto.setTurnoString(domain.getTurno().getNome());
		
		dto.setSegunda(false);
		dto.setTerca(false);
		dto.setQuarta(false);
		dto.setQuinta(false);
		dto.setSexta(false);
		dto.setSabado(false);
		dto.setDomingo(false);
		
		for(HorarioDisponivelCenario o : domain.getHorariosDisponiveisCenario()) {
			switch (o.getSemana()) {
			case SEG:
				dto.setSegunda(true);
				dto.setSegundaId(o.getId());
				break;
			case TER:
				dto.setTerca(true);
				dto.setTercaId(o.getId());
			break;
			case QUA:
				dto.setQuarta(true);
				dto.setQuartaId(o.getId());
			break;
			case QUI:
				dto.setQuinta(true);
				dto.setQuintaId(o.getId());
			break;
			case SEX:
				dto.setSexta(true);
				dto.setSextaId(o.getId());
			break;
			case SAB:
				dto.setSabado(true);
				dto.setSabadoId(o.getId());
			break;
			case DOM:
				dto.setDomingo(true);
				dto.setDomingoId(o.getId());
			break;
			default:
				break;
			}
		}

		DateFormat df = new SimpleDateFormat("HH:mm");
		String inicio = df.format(domain.getHorario());
		
		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime(domain.getHorario());
		fimCal.add(Calendar.MINUTE, domain.getTurno().getTempo());
		String fim = df.format(fimCal.getTime());
		
		dto.setHorarioString(inicio + " / " + fim);
		
		dto.setHorario(domain.getHorario());
		
		return dto;
	}
	
	public static List<HorarioDisponivelCenarioDTO> toHorarioDisponivelCenarioDTO(List<HorarioDisponivelCenario> domains) {
		Map<HorarioAula, List<HorarioDisponivelCenario>> horariosAula = new HashMap<HorarioAula, List<HorarioDisponivelCenario>>();
		for(HorarioDisponivelCenario domain : domains) {
			if(!horariosAula.containsKey(domain.getHorarioAula())) {
				horariosAula.put(domain.getHorarioAula(), new ArrayList<HorarioDisponivelCenario>());
			}
			horariosAula.get(domain.getHorarioAula()).add(domain);
		}
		
		List<HorarioDisponivelCenarioDTO> dtos = new ArrayList<HorarioDisponivelCenarioDTO>(horariosAula.keySet().size());
		
		for(HorarioAula horarioAula : horariosAula.keySet()) {
			HorarioDisponivelCenarioDTO dto = new HorarioDisponivelCenarioDTO();
			dto.setHorarioDeAulaId(horarioAula.getId());
			dto.setHorarioDeAulaVersion(horarioAula.getVersion());
			dto.setTurnoString(horarioAula.getTurno().getNome());
			
			dto.setSegunda(false);
			dto.setTerca(false);
			dto.setQuarta(false);
			dto.setQuinta(false);
			dto.setSexta(false);
			dto.setSabado(false);
			dto.setDomingo(false);
			
			for(HorarioDisponivelCenario o : horariosAula.get(horarioAula)) {
				switch (o.getSemana()) {
				case SEG:
					dto.setSegunda(true);
					dto.setSegundaId(o.getId());
					break;
				case TER:
					dto.setTerca(true);
					dto.setTercaId(o.getId());
				break;
				case QUA:
					dto.setQuarta(true);
					dto.setQuartaId(o.getId());
				break;
				case QUI:
					dto.setQuinta(true);
					dto.setQuintaId(o.getId());
				break;
				case SEX:
					dto.setSexta(true);
					dto.setSextaId(o.getId());
				break;
				case SAB:
					dto.setSabado(true);
					dto.setSabadoId(o.getId());
				break;
				case DOM:
					dto.setDomingo(true);
					dto.setDomingoId(o.getId());
				break;
				default:
					break;
				}
			}
			
			DateFormat df = new SimpleDateFormat("HH:mm");
			String inicio = df.format(horarioAula.getHorario());
			
			Calendar fimCal = Calendar.getInstance();
			fimCal.setTime(horarioAula.getHorario());
			fimCal.add(Calendar.MINUTE, horarioAula.getTurno().getTempo());
			String fim = df.format(fimCal.getTime());
			
			dto.setHorarioString(inicio + " / " + fim);
			
			dto.setHorario(horarioAula.getHorario());
			
			dtos.add(dto);
		}
		
		return dtos;
	}
	
	// TIPO CURSO
	public static TipoCurso toTipoCurso(TipoCursoDTO dto) {
		TipoCurso domain = new TipoCurso();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setDescricao(dto.getDescricao());
		return domain;
	}

	public static TipoCursoDTO toTipoCursoDTO(TipoCurso domain) {
		TipoCursoDTO dto = new TipoCursoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setDescricao(domain.getDescricao());
		return dto;
	}
	
	// CURSO
	public static Curso toCurso(CursoDTO dto) {
		Curso domain = new Curso();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Cenario cenario = Cenario.find(dto.getCenarioId());
		domain.setCenario(cenario);
		domain.setCodigo(dto.getCodigo());
		domain.setNome(dto.getNome());
		domain.setNumMinDoutores(dto.getNumMinDoutores());
		domain.setNumMinMestres(dto.getNumMinMestres());
		domain.setMaxDisciplinasPeloProfessor(dto.getMaxDisciplinasPeloProfessor());
		domain.setAdmMaisDeUmDisciplina(dto.getAdmMaisDeUmDisciplina());
		
		TipoCurso tipoCurso = TipoCurso.find(dto.getTipoId());
		domain.setTipoCurso(tipoCurso);
		
		return domain;
	}
	
	public static CursoDTO toCursoDTO(Curso domain) {
		CursoDTO dto = new CursoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setCodigo(domain.getCodigo());
		dto.setNome(domain.getNome());
		dto.setNumMinDoutores(domain.getNumMinDoutores());
		dto.setNumMinMestres(domain.getNumMinMestres());
		dto.setMaxDisciplinasPeloProfessor(domain.getMaxDisciplinasPeloProfessor());
		dto.setAdmMaisDeUmDisciplina(domain.getAdmMaisDeUmDisciplina());
		dto.setTipoId(domain.getTipoCurso().getId());
		dto.setTipoString(domain.getTipoCurso().getCodigo());
		dto.setName(domain.getCodigo());
		dto.setDisplay(domain.getNome() + " (" + domain.getCodigo() +")");
		return dto;
	}
	
	// ÁREA DE TITULAÇÂO
	public static AreaTitulacao toAreaTitulacao(AreaTitulacaoDTO dto) {
		AreaTitulacao domain = new AreaTitulacao();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setDescricao(dto.getDescricao());
		return domain;
	}
	
	public static AreaTitulacaoDTO toAreaTitulacaoDTO(AreaTitulacao domain) {
		AreaTitulacaoDTO dto = new AreaTitulacaoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setDescricao(domain.getDescricao());
		return dto;
	}
	
	// TITULAÇÃO
	public static Titulacao toTitulacao(TitulacaoDTO dto) {
		Titulacao domain = new Titulacao();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		return domain;
	}
	
	public static TitulacaoDTO toTitulacaoDTO(Titulacao domain) {
		TitulacaoDTO dto = new TitulacaoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		return dto;
	}
	
	// TIPO DE CONTRATO
	public static TipoContrato toTipoContrato(TipoContratoDTO dto) {
		TipoContrato domain = new TipoContrato();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		return domain;
	}
	
	public static TipoContratoDTO toTipoContratoDTO(TipoContrato domain) {
		TipoContratoDTO dto = new TipoContratoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		return dto;
	}
	
	// DESLOCAMENTO UNIDADES
	public static List<DeslocamentoUnidade> toDeslocamentoUnidade(DeslocamentoUnidadeDTO deslocamentoUnidadeDTO) {
		List<Long> listIds = new ArrayList<Long>();
		for(String keyString : deslocamentoUnidadeDTO.getPropertyNames()) {
			if(keyString.startsWith("destinoTempo")) {
				Long id = Long.valueOf(keyString.replace("destinoTempo", ""));
				if(!listIds.contains(id)) listIds.add(id);
			} else if(keyString.startsWith("destinoCusto")) {
//				Long id = Long.valueOf(keyString.replace("destinoCusto", ""));
				Long id = 0L;
				if(!listIds.contains(id)) listIds.add(id);
			}
		}
		List<DeslocamentoUnidade> list = new ArrayList<DeslocamentoUnidade>();
		Unidade unidade = Unidade.find(deslocamentoUnidadeDTO.getOrigemId());
		for(Long idUnidade : listIds) {
			Integer tempo = deslocamentoUnidadeDTO.getDestinoTempo(idUnidade);
			Double custo = deslocamentoUnidadeDTO.getDestinoCusto(idUnidade);
			if(tempo <= 0 && custo <= 0.0) continue;
			DeslocamentoUnidade du = new DeslocamentoUnidade();
			du.setDestino(Unidade.find(idUnidade));
			du.setOrigem(unidade);
			du.setTempo(tempo);
			du.setCusto(custo);
			list.add(du);
		}
		return list;
	}
	
	public static DeslocamentoUnidadeDTO toDeslocamentoUnidadeDTO(Unidade unidade, Set<Unidade> unidadesDestinos) {
		DeslocamentoUnidadeDTO deslocamentoUnidadeDTO = new DeslocamentoUnidadeDTO();
		deslocamentoUnidadeDTO.setOrigemId(unidade.getId());
		deslocamentoUnidadeDTO.setOrigemString(unidade.getCodigo());
		Set<DeslocamentoUnidade> set = unidade.getDeslocamentos();
		for(Unidade u : unidadesDestinos) {
			deslocamentoUnidadeDTO.addDestino(u.getId(), u.getCodigo(), 0, 0.0);
		}
		for(DeslocamentoUnidade du : set) {
			deslocamentoUnidadeDTO.addDestino(du.getDestino().getId(), du.getDestino().getCodigo(), du.getTempo(), du.getCusto());
		}
		return deslocamentoUnidadeDTO;
	}
	
	// DESLOCAMENTO CAMPUS
	public static List<DeslocamentoCampus> toDeslocamentoCampus(DeslocamentoCampusDTO deslocamentoCampusDTO) {
		List<Long> listIds = new ArrayList<Long>();
		for(String keyString : deslocamentoCampusDTO.getPropertyNames()) {
			if(keyString.startsWith("destinoTempo")) {
				Long id = Long.valueOf(keyString.replace("destinoTempo", ""));
				if(!listIds.contains(id)) listIds.add(id);
			} else if(keyString.startsWith("destinoCusto")) {
				Long id = Long.valueOf(keyString.replace("destinoCusto", ""));
				if(!listIds.contains(id)) listIds.add(id);
			}
		}
		List<DeslocamentoCampus> list = new ArrayList<DeslocamentoCampus>();
		Campus campus = Campus.find(deslocamentoCampusDTO.getOrigemId());
		for(Long idCampus : listIds) {
			Integer tempo = deslocamentoCampusDTO.getDestinoTempo(idCampus);
			Double custo = deslocamentoCampusDTO.getDestinoCusto(idCampus);
			if(tempo <= 0 && custo <= 0.0) continue;
			DeslocamentoCampus du = new DeslocamentoCampus();
			du.setDestino(Campus.find(idCampus));
			du.setOrigem(campus);
			du.setTempo(tempo);
			du.setCusto(custo);
			list.add(du);
		}
		return list;
	}
	
	public static DeslocamentoCampusDTO toDeslocamentoCampusDTO(Campus campus, List<Campus> campusDestinos) {
		DeslocamentoCampusDTO deslocamentoCampusDTO = new DeslocamentoCampusDTO();
		deslocamentoCampusDTO.setOrigemId(campus.getId());
		deslocamentoCampusDTO.setOrigemString(campus.getCodigo());
		Set<DeslocamentoCampus> set = campus.getDeslocamentos();
		for(Campus u : campusDestinos) {
			deslocamentoCampusDTO.addDestino(u.getId(), u.getCodigo(), 0, 0.0);
		}
		for(DeslocamentoCampus du : set) {
			deslocamentoCampusDTO.addDestino(du.getDestino().getId(), du.getDestino().getCodigo(), du.getTempo(), du.getCusto());
		}
		return deslocamentoCampusDTO;
	}
	
	// TIPO DISCIPLINA
	public static TipoDisciplina toTipoDisciplina(TipoDisciplinaDTO dto) {
		TipoDisciplina domain = new TipoDisciplina();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		return domain;
	}
	
	public static TipoDisciplinaDTO toTipoDisciplinaDTO(TipoDisciplina domain) {
		TipoDisciplinaDTO dto = new TipoDisciplinaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		return dto;
	}
	
	// DISCIPLINA
	public static Disciplina toDisciplina(DisciplinaDTO dto) {
		Disciplina domain = new Disciplina();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Cenario cenario = Cenario.find(dto.getCenarioId());
		domain.setCenario(cenario);
		domain.setCodigo(dto.getCodigo());
		domain.setNome(dto.getNome());
		domain.setCreditosPratico(dto.getCreditosPratico());
		domain.setCreditosTeorico(dto.getCreditosTeorico());
		domain.setLaboratorio(dto.getLaboratorio());
		domain.setMaxAlunosTeorico(dto.getMaxAlunosTeorico());
		domain.setMaxAlunosPratico(dto.getMaxAlunosPratico());
		domain.setDificuldade(Dificuldades.get(dto.getDificuldade()));
		domain.setTipoDisciplina(TipoDisciplina.find(dto.getTipoId()));
		return domain;
	}
	
	public static DisciplinaDTO toDisciplinaDTO(Disciplina domain) {
		DisciplinaDTO dto = new DisciplinaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setCodigo(domain.getCodigo());
		dto.setNome(domain.getNome());
		dto.setCreditosPratico(domain.getCreditosPratico());
		dto.setCreditosTeorico(domain.getCreditosTeorico());
		dto.setLaboratorio(domain.getLaboratorio());
		dto.setMaxAlunosTeorico(domain.getMaxAlunosTeorico());
		dto.setMaxAlunosPratico(domain.getMaxAlunosPratico());
		dto.setDificuldade(domain.getDificuldade().name());
		dto.setTipoId(domain.getTipoDisciplina().getId());
		dto.setTipoString(domain.getTipoDisciplina().getNome());
		return dto;
	}
	
	// CURRICULO (MATRIZ CURRICULAR)
	public static Curriculo toCurriculo(CurriculoDTO dto) {
		Curriculo domain = new Curriculo();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Cenario cenario = Cenario.find(dto.getCenarioId());
		domain.setCenario(cenario);
		domain.setCodigo(dto.getCodigo());
		domain.setDescricao(dto.getDescricao());
		Curso curso = Curso.find(dto.getCursoId());
		domain.setCurso(curso);
		return domain;
	}
	
	public static CurriculoDTO toCurriculoDTO(Curriculo domain) {
		CurriculoDTO dto = new CurriculoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getId());
		dto.setCodigo(domain.getCodigo());
		dto.setDescricao(domain.getDescricao());
		String periodos = "";
		Set<Integer> periodosSet = new HashSet<Integer>();
		for(CurriculoDisciplina cd : domain.getDisciplinas()) {
			if(periodosSet.add(cd.getPeriodo())) periodos += cd.getPeriodo() + ", ";
		}
		dto.setPeriodos(periodos.substring(0, periodos.length() - 2));
		Curso curso = domain.getCurso();
		dto.setCursoId(curso.getId());
		dto.setCursoString(curso.getCodigo() + " (" +curso.getNome()+ ")");
		dto.setDisplay(domain.getCodigo() + " ("+ curso.getNome() +")");
		return dto;
	}
	
	// CURRICULO DISCIPLINA (DISCIPLINA NA MATRIZ CURRICULAR)
	public static CurriculoDisciplina toCurriculoDisciplina(CurriculoDisciplinaDTO dto) {
		CurriculoDisciplina domain = new CurriculoDisciplina();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setPeriodo(dto.getPeriodo());
		Disciplina disciplina = Disciplina.find(dto.getDisciplinaId());
		domain.setDisciplina(disciplina);
		Curriculo curriculo = Curriculo.find(dto.getCurriculoId());
		domain.setCurriculo(curriculo);
		return domain;
	}
	
	public static CurriculoDisciplinaDTO toCurriculoDisciplinaDTO(CurriculoDisciplina domain) {
		CurriculoDisciplinaDTO dto = new CurriculoDisciplinaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		Disciplina disciplina = domain.getDisciplina();
		dto.setDisciplinaId(disciplina.getId());
		dto.setDisciplinaString(disciplina.getCodigo());
		dto.setPeriodo(domain.getPeriodo());
		Integer crTeorico = disciplina.getCreditosTeorico();
		Integer crPratico = disciplina.getCreditosPratico();
		dto.setCreditosTeorico(crTeorico);
		dto.setCreditosPratico(crPratico);
		dto.setCreditosTotal(crTeorico + crPratico);
		dto.setCurriculoId(domain.getCurriculo().getId());
		dto.setDisciplinaCodigoNomeString(disciplina.getCodigo()+" ("+disciplina.getNome()+")");
		return dto;
	}

	// OFERTA DE CURSO EM UM CAMPUS
	public static Oferta toOferta(OfertaDTO dto) {
		Oferta domain = new Oferta();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCampus(Campus.find(dto.getCampusId()));
		domain.setCurriculo(Curriculo.find(dto.getMatrizCurricularId()));
		domain.setTurno(Turno.find(dto.getTurnoId()));
		return domain;
	}
	
	public static OfertaDTO toOfertaDTO(Oferta domain) {
		OfertaDTO dto = new OfertaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCampusString(domain.getCampus().getCodigo());
		dto.setCampusId(domain.getCampus().getId());
		Curso curso = domain.getCurriculo().getCurso();
		dto.setCursoString(curso.getCodigo() +" ("+ curso.getNome() +")");
		dto.setMatrizCurricularString(domain.getCurriculo().getCodigo());
		dto.setMatrizCurricularId(domain.getCurriculo().getId());
		dto.setTurnoString(domain.getTurno().getNome());
		dto.setTurnoId(domain.getTurno().getId());
		
		return dto;
	}
	
	// DEMANDA
	public static Demanda toDemanda(DemandaDTO dto) {
		Demanda domain = new Demanda();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Oferta oferta = Oferta.find(dto.getOfertaId());
		if(oferta != null) {
			domain.setOferta(oferta);
		} else {
			Campus campus = Campus.find(dto.getCampusId());
			Curso curso = Curso.find(dto.getCursoId());
			Curriculo curriculo = Curriculo.find(dto.getCurriculoId());
			Turno turno = Turno.find(dto.getTurnoId());
			List<Oferta> ofertas = Oferta.findByTurnoAndCampusAndCursoAndCurriculo(turno, campus, curso, curriculo, 0, 1, null);
			domain.setOferta(ofertas.get(0));
		}
		domain.setDisciplina(Disciplina.find(dto.getDisciplinaId()));
		domain.setQuantidade(dto.getDemanda());
		return domain;
	}
	
	public static DemandaDTO toDemandaDTO(Demanda domain) {
		DemandaDTO dto = new DemandaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		
		Oferta oferta = domain.getOferta();
		
		Campus campus = oferta.getCampus();
		dto.setCampusString(campus.getCodigo());
		dto.setCampusId(campus.getId());
		
		Curriculo curriculo = oferta.getCurriculo();
		dto.setCurriculoString(curriculo.getCodigo());
		dto.setCurriculoId(curriculo.getId());
		
		Curso curso = curriculo.getCurso();
		dto.setCursoString(curso.getCodigo() +" (" +curso.getNome()+ ")");
		dto.setCursoId(curso.getId());
		
		Turno turno = oferta.getTurno();
		dto.setTurnoString(turno.getNome());
		dto.setTurnoId(turno.getId());
		
		Disciplina disciplina = domain.getDisciplina();
		dto.setDisciplinaString(disciplina.getCodigo());
		dto.setDisciplinaId(disciplina.getId());
		
		dto.setCenarioId(disciplina.getCenario().getId());
		
		dto.setDemanda(domain.getQuantidade());
		
		return dto;
	}
	
	// ATENDIMENTO TÁTICO
	public static AtendimentoTatico toAtendimentoTatico(AtendimentoTaticoDTO dto) {
		AtendimentoTatico domain = new AtendimentoTatico();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCenario(Cenario.find(dto.getCenarioId()));
		domain.setTurma(dto.getTurma());
		domain.setSala(Sala.find(dto.getSalaId()));
		domain.setSemana(Semanas.get(dto.getSemana()));
		domain.setOferta(Oferta.find(dto.getOfertaId()));
		domain.setDisciplina(Disciplina.find(dto.getDisciplinaId()));
		domain.setQuantidadeAlunos(dto.getQuantidadeAlunos());
		domain.setCreditosTeorico(dto.getCreditosTeorico());
		domain.setCreditosPratico(dto.getCreditosPratico());
		return domain;
	}

	public static AtendimentoTaticoDTO toAtendimentoTaticoDTO(AtendimentoTatico domain) {
		AtendimentoTaticoDTO dto = new AtendimentoTaticoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setTurma(domain.getTurma());
		dto.setCampusId(domain.getSala().getUnidade().getCampus().getId());
		dto.setCampusString(domain.getSala().getUnidade().getCampus().getCodigo());
		dto.setUnidadeId(domain.getSala().getUnidade().getId());
		dto.setUnidadeString(domain.getSala().getUnidade().getCodigo());
		dto.setSalaId(domain.getSala().getId());
		dto.setSalaString(domain.getSala().getCodigo());
		dto.setSemana(Semanas.toInt(domain.getSemana()));
		dto.setOfertaId(domain.getOferta().getId());
		dto.setDisciplinaId(domain.getDisciplina().getId());
		dto.setDisciplinaString(domain.getDisciplina().getCodigo());
		dto.setQuantidadeAlunos(domain.getQuantidadeAlunos());
		dto.setQuantidadeAlunosString(domain.getQuantidadeAlunos().toString());
		dto.setCreditosTeorico(domain.getCreditosTeorico());
		dto.setCreditosPratico(domain.getCreditosPratico());
		dto.setCursoString(domain.getOferta().getCurriculo().getCurso().getCodigo());
		dto.setCurricularString(domain.getOferta().getCurriculo().getCodigo());
		dto.setPeriodo(domain.getOferta().getCurriculo().getPeriodo(domain.getDisciplina()));
		dto.setPeriodoString(String.valueOf(domain.getOferta().getCurriculo().getPeriodo(domain.getDisciplina())));
		dto.setTotalCreditoDisciplina(domain.getDisciplina().getTotalCreditos());
		
		return dto;
	}
	
	// DIVISÃO DE CREDITO
	public static DivisaoCredito toDivisaoCredito(DivisaoCreditoDTO dto) {
		DivisaoCredito domain = new DivisaoCredito();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		if(dto.getDisciplinaId() != null) {
			domain.getCenario().add(Cenario.find(dto.getCenarioId()));
		} else {
			domain.setDisciplina(Disciplina.find(dto.getDisciplinaId()));
		}
		domain.setCreditos(dto.getDia1() + dto.getDia2() + dto.getDia3() + dto.getDia4() + dto.getDia5() + dto.getDia6() + dto.getDia7());
		domain.setDia1(dto.getDia1());
		domain.setDia2(dto.getDia2());
		domain.setDia3(dto.getDia3());
		domain.setDia4(dto.getDia4());
		domain.setDia5(dto.getDia5());
		domain.setDia6(dto.getDia6());
		domain.setDia7(dto.getDia7());
		return domain;
	}

	public static DivisaoCreditoDTO toDivisaoCreditoDTO(DivisaoCredito domain) {
		DivisaoCreditoDTO dto = new DivisaoCreditoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		if(domain.getCenario().size() > 0) {
			dto.setCenarioId((new ArrayList<Cenario>(domain.getCenario())).get(0).getId());
		}
		Disciplina disciplina = domain.getDisciplina();
		if(disciplina != null) {
			dto.setDisciplinaId(disciplina.getId());
			dto.setDisciplinaString(disciplina.getCodigo() + " (" +disciplina.getNome()+ ")");
		}
		dto.setDia1(domain.getDia1());
		dto.setDia2(domain.getDia2());
		dto.setDia3(domain.getDia3());
		dto.setDia4(domain.getDia4());
		dto.setDia5(domain.getDia5());
		dto.setDia6(domain.getDia6());
		dto.setDia7(domain.getDia7());
		dto.setTotalCreditos(dto.getDia1() + dto.getDia2() + dto.getDia3() + dto.getDia4() + dto.getDia5() + dto.getDia6() + dto.getDia7());
		return dto;
	}
	
	// PROFESSOR
	public static Professor toProfessor(ProfessorDTO dto) {
		Professor domain = new Professor();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCenario(Cenario.find(dto.getCenarioId()));
		domain.setNome(dto.getNome());
		domain.setCpf(dto.getCpf());
		domain.setTipoContrato(TipoContrato.find(dto.getTipoContratoId()));
		domain.setCargaHorariaMax(dto.getCargaHorariaMax());
		domain.setCargaHorariaMin(dto.getCargaHorariaMin());
		domain.setTitulacao(Titulacao.find(dto.getTitulacaoId()));
		domain.setAreaTitulacao(AreaTitulacao.find(dto.getAreaTitulacaoId()));
		domain.setCreditoAnterior(dto.getCreditoAnterior());
		domain.setValorCredito(dto.getValorCredito());
		return domain;
	}
	
	public static ProfessorDTO toProfessorDTO(Professor domain) {
		ProfessorDTO dto = new ProfessorDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCenarioId(domain.getCenario().getId());
		dto.setNome(domain.getNome());
		dto.setCpf(domain.getCpf());
		dto.setTipoContratoId(domain.getTipoContrato().getId());
		dto.setTipoContratoString(domain.getTipoContrato().getNome());
		dto.setCargaHorariaMax(domain.getCargaHorariaMax());
		dto.setCargaHorariaMin(domain.getCargaHorariaMin());
		dto.setTitulacaoId(domain.getTitulacao().getId());
		dto.setTitulacaoString(domain.getTitulacao().getNome());
		dto.setAreaTitulacaoId(domain.getAreaTitulacao().getId());
		dto.setAreaTitulacaoString(domain.getAreaTitulacao().getCodigo());
		dto.setCreditoAnterior(domain.getCreditoAnterior());
		dto.setValorCredito(domain.getValorCredito());
		return dto;
	}
	
	// PROFESSOR-DISCIPLINA
	public static ProfessorDisciplina toProfessorDisciplina(ProfessorDisciplinaDTO dto) {
		ProfessorDisciplina domain = new ProfessorDisciplina();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setProfessor(Professor.find(dto.getProfessorId()));
		domain.setDisciplina(Disciplina.find(dto.getDisciplinaId()));
		domain.setPreferencia(dto.getPreferencia());
		domain.setNota(dto.getNotaDesempenho());
		return domain;
	}
	
	public static ProfessorDisciplinaDTO toProfessorDisciplinaDTO(ProfessorDisciplina domain) {
		ProfessorDisciplinaDTO dto = new ProfessorDisciplinaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setProfessorId(domain.getProfessor().getId());
		dto.setProfessorString(domain.getProfessor().getNome());
		dto.setProfessorCpf(domain.getProfessor().getCpf());
		dto.setDisciplinaId(domain.getDisciplina().getId());
		dto.setDisciplinaString(domain.getDisciplina().getCodigo());
		dto.setPreferencia(domain.getPreferencia());
		dto.setNotaDesempenho(domain.getNota());
		return dto;
	}
	
	// TURNO
	public static Equivalencia toEquivalencia(EquivalenciaDTO dto) {
		Equivalencia domain = new Equivalencia();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCursou(Disciplina.find(dto.getCursouId()));
		// TODO Necessário pegar a lista de quem cursou?
		return domain;
	}

	public static EquivalenciaDTO toEquivalenciaDTO(Equivalencia domain) {
		EquivalenciaDTO dto = new EquivalenciaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCursouId(domain.getCursou().getId());
		dto.setCursouString(domain.getCursou().getCodigo());
		Set<Disciplina> eliminaList = domain.getElimina();
		String eliminaString = "";
		for(Disciplina d : eliminaList) {
			eliminaString += d.getCodigo() + ", ";
		}
		if(eliminaString.length() > 0) eliminaString = eliminaString.substring(0, eliminaString.length()-2);
		dto.setEliminaString(eliminaString);
		return dto;
	}
	
	// FIXACAO
	public static Fixacao toFixacao(FixacaoDTO dto) {
		Fixacao domain = new Fixacao();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setDescricao(dto.getDescricao());
		domain.setDisciplina(Disciplina.find(dto.getDisciplinaId()));
		domain.setCampus(Campus.find(dto.getCampusId()));
		domain.setUnidade(Unidade.find(dto.getUnidadeId()));
		domain.setSala(Sala.find(dto.getSalaId()));
		return domain;
	}

	public static FixacaoDTO toFixacaoDTO(Fixacao domain) {
		FixacaoDTO dto = new FixacaoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setDescricao(domain.getDescricao());
		Disciplina disciplina = domain.getDisciplina();
		if(disciplina != null) {
			dto.setDisciplinaId(disciplina.getId());
			dto.setDisciplinaString(disciplina.getCodigo());
		}
		Campus campus = domain.getCampus();
		if(campus != null) {
			dto.setCampusId(campus.getId());
			dto.setCampusString(campus.getCodigo());
		}
		Unidade unidade = domain.getUnidade();
		if(unidade != null) {
			dto.setUnidadeId(unidade.getId());
			dto.setUnidadeString(unidade.getCodigo());
		}
		Sala sala = domain.getSala();
		if(sala != null) {
			dto.setSalaId(sala.getId());
			dto.setSalaString(sala.getCodigo());
		}
		return dto;
	}
}
