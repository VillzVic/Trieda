package com.gapso.web.trieda.server.util;

import java.util.Calendar;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
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

}
