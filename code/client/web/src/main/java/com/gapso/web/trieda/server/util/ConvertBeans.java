package com.gapso.web.trieda.server.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Set;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;

public class ConvertBeans {
	
	// CAMPUS
	public static Campus toCampus(CampusDTO dto) {
		Campus domain = new Campus();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
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
		return domain;
	}
	
	public static CampusDTO toCampusDTO(Campus domain) {
		CampusDTO dto = new CampusDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setCodigo(domain.getCodigo());
		
		dto.setEstado(domain.getEstado().name());
		dto.setMunicipio(domain.getMunicipio());
		dto.setBairro(domain.getBairro());
		
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
		return dto;
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
		return dto;
	}
	
	// TURNO
	public static Turno toTurno(TurnoDTO dto) {
		Turno domain = new Turno();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setTempo(dto.getTempo());
		return domain;
	}

	public static TurnoDTO toTurnoDTO(Turno domain) {
		TurnoDTO dto = new TurnoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setTempo(domain.getTempo());
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
		dto.setCodigo(domain.getCodigo());
		dto.setNome(domain.getNome());
		dto.setNumMinDoutores(domain.getNumMinDoutores());
		dto.setNumMinMestres(domain.getNumMinMestres());
		dto.setMaxDisciplinasPeloProfessor(domain.getMaxDisciplinasPeloProfessor());
		dto.setAdmMaisDeUmDisciplina(domain.getAdmMaisDeUmDisciplina());
		dto.setTipoId(domain.getTipoCurso().getId());
		dto.setTipoString(domain.getTipoCurso().getCodigo());
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
	
	// DESLOCAMENTO UNIDADES
	public static Unidade toDeslocamentoUnidade(DeslocamentoUnidadeDTO deslocamentoUnidadeDTO) {
		Unidade unidade = Unidade.find(deslocamentoUnidadeDTO.getOrigemId());
		// TODO O correto é remover somente os inexistentes e editar os alterados, atualmente está removendo todo mundo e adicionando tudo
		unidade.getDeslocamentos().removeAll(unidade.getDeslocamentos());
		
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
		for(Long idUnidade : listIds) {
			Integer tempo = deslocamentoUnidadeDTO.getDestinoTempo(idUnidade);
			Double custo = deslocamentoUnidadeDTO.getDestinoCusto(idUnidade);
			if(tempo <= 0 && custo <= 0.0) continue;
			DeslocamentoUnidade du = new DeslocamentoUnidade();
			du.setDestino(Unidade.find(idUnidade));
			du.setOrigem(unidade);
			du.setTempo(tempo);
			du.setCusto(custo);
			unidade.getDeslocamentos().add(du);
		}
		return unidade;
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
	public static Campus toDeslocamentoCampus(DeslocamentoCampusDTO deslocamentoCampusDTO) {
		Campus campus = Campus.find(deslocamentoCampusDTO.getOrigemId());
		// TODO O correto é remover somente os inexistentes e editar os alterados, atualmente está removendo todo mundo e adicionando tudo
		campus.getDeslocamentos().removeAll(campus.getDeslocamentos());
		
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
		for(Long idCampus : listIds) {
			Integer tempo = deslocamentoCampusDTO.getDestinoTempo(idCampus);
			Double custo = deslocamentoCampusDTO.getDestinoCusto(idCampus);
			if(tempo <= 0 && custo <= 0.0) continue;
			DeslocamentoCampus du = new DeslocamentoCampus();
			du.setDestino(Campus.find(idCampus));
			du.setOrigem(campus);
			du.setTempo(tempo);
			du.setCusto(custo);
			campus.getDeslocamentos().add(du);
		}
		return campus;
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
}
