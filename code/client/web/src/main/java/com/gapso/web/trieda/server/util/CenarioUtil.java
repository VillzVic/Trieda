package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Compatibilidade;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;

public class CenarioUtil {
	
	public void criaCenario(Cenario cenario, List<Campus> campi) {
		criaCenarioCopiaCalendario(cenario);
		criaCenarioCopiaCampi(cenario, campi);
	}
	
	private void criaCenarioCopiaCalendario(Cenario cenario) {
		SemanaLetiva semanaLetiva = cenario.getSemanaLetiva();
		semanaLetiva.setId(null);
		semanaLetiva.detach();
		cenario.setId(null);
		cenario.detach();
		List<Turno> turnos = new ArrayList<Turno>();
		
		Set<HorarioAula> horariosAula = cenario.getSemanaLetiva().getHorariosAula();
		for(HorarioAula horarioAula : horariosAula) {
			Turno turno = horarioAula.getTurno();
			if(!turnos.contains(turno)) {
				turno.setId(null);
				turno.setCenario(cenario);
				turno.detach();
				turnos.add(turno);
			}
			horarioAula.setId(null);
			horarioAula.detach();
			horarioAula.setSemanaLetiva(cenario.getSemanaLetiva());
			Set<HorarioDisponivelCenario> horariosDisponivelCenario = horarioAula.getHorariosDisponiveisCenario();
			for(HorarioDisponivelCenario horarioDisponivelCenario : horariosDisponivelCenario) {
				horarioDisponivelCenario.setId(null);
				horarioDisponivelCenario.detach();
				horarioDisponivelCenario.setHorarioAula(horarioAula);
			}
		}
		
		semanaLetiva.persist();
		cenario.persist();
		for(Turno turno : turnos) {
			turno.persist();
		}
		for(HorarioAula horarioAula : horariosAula) {
			horarioAula.persist();
			Set<HorarioDisponivelCenario> horariosDisponivelCenario = horarioAula.getHorariosDisponiveisCenario();
			for(HorarioDisponivelCenario horarioDisponivelCenario : horariosDisponivelCenario) {
				horarioDisponivelCenario.persist();
			}
		}
	}
	
	static private void criaCenarioCopiaCampi(Cenario cenario, List<Campus> campi) {
	}
	
	/* ******************************************* */
	
	private List<SemanaLetiva> semanaLetivaList = new ArrayList<SemanaLetiva>();
	private Cenario cenario;
	private List<Turno> turnoList = new ArrayList<Turno>();
	private List<HorarioAula> horarioAulaList = new ArrayList<HorarioAula>();
	private List<Curso> cursoList = new ArrayList<Curso>();
	private List<Campus> campusList = new ArrayList<Campus>();
	private List<DeslocamentoCampus> deslocamentoCampusList = new ArrayList<DeslocamentoCampus>();
	private List<Unidade> unidadeList = new ArrayList<Unidade>();
	private List<DeslocamentoUnidade> deslocamentoUnidadeList = new ArrayList<DeslocamentoUnidade>();
	private List<Sala> salaList = new ArrayList<Sala>();
	private List<GrupoSala> grupoSalaList = new ArrayList<GrupoSala>();
	private List<Disciplina> disciplinaList = new ArrayList<Disciplina>();
	private List<Compatibilidade> compatibilidadeList = new ArrayList<Compatibilidade>();
	private List<Equivalencia> equivalenciaList = new ArrayList<Equivalencia>();
	private List<DivisaoCredito> divisaoCreditoList = new ArrayList<DivisaoCredito>();
	private List<Professor> professorList = new ArrayList<Professor>();
	private List<ProfessorDisciplina> professorDisciplinaList = new ArrayList<ProfessorDisciplina>();
	private List<Curriculo> curriculoList = new ArrayList<Curriculo>();
	private List<CurriculoDisciplina> curriculoDisciplinaList = new ArrayList<CurriculoDisciplina>();
	private List<Oferta> ofertaList = new ArrayList<Oferta>();
	private List<Demanda> demandaList = new ArrayList<Demanda>();
	private List<HorarioDisponivelCenario> horarioDisponivelCenarioList = new ArrayList<HorarioDisponivelCenario>();
	
	public void clonarCenario(Cenario cenario) {
		this.cenario = cenario; 
		popularListas();
		detachList();
		saveList();
	}
	
	private void popularListas() {
		SemanaLetiva semanaLetiva = cenario.getSemanaLetiva();
		if(!semanaLetivaList.contains(semanaLetiva)) { semanaLetivaList.add(semanaLetiva); }
		
		Set<HorarioAula> horariosAula = semanaLetiva.getHorariosAula();
		for(HorarioAula horarioAula : horariosAula) {
			horarioAulaList.add(horarioAula);
			Turno turno = horarioAula.getTurno();
			if(!turnoList.contains(turno)) {
				turnoList.add(turno);
			}
			horarioDisponivelCenarioList.addAll(horarioAula.getHorariosDisponiveisCenario());
		}
		cursoList.addAll(cenario.getCursos());
		campusList.addAll(cenario.getCampi());
		for(Campus c : campusList) {
			deslocamentoCampusList.addAll(c.getDeslocamentos());
			unidadeList.addAll(c.getUnidades());
			ofertaList.addAll(c.getOfertas());
		}
		for(Unidade u : unidadeList) {
			deslocamentoUnidadeList.addAll(u.getDeslocamentos());
			salaList.addAll(u.getSalas());
			grupoSalaList.addAll(u.getGruposSalas());
		}
		divisaoCreditoList.addAll(cenario.getDivisoesCredito()); 
		disciplinaList.addAll(cenario.getDisciplinas());
		for(Disciplina d : disciplinaList) {
			Set<Compatibilidade> compatibilidades = d.getCompatibilidades(); 
			for(Compatibilidade c : compatibilidades) {
				if(!compatibilidadeList.contains(c)) compatibilidadeList.add(c);
			}
			Set<Equivalencia> equivalencias = d.getEquivalencias(); 
			for(Equivalencia e : equivalencias) {
				if(!equivalenciaList.contains(e)) equivalenciaList.add(e);
			}
			if(!divisaoCreditoList.contains(d.getDivisaoCreditos())) divisaoCreditoList.add(d.getDivisaoCreditos());
			demandaList.addAll(d.getDemandas());
		}
		professorList.addAll(cenario.getProfessores());
		for(Professor p : professorList) {
			professorDisciplinaList.addAll(p.getDisciplinas());
		}
		curriculoList.addAll(cenario.getCurriculos());
		for(Curriculo c : curriculoList) {
			curriculoDisciplinaList.addAll(c.getDisciplinas());
		}
	}
	
	private void detachList() {
		for(SemanaLetiva o : semanaLetivaList) { o.setId(null); o.detach(); }
		cenario.setId(null); cenario.detach();
		for(Turno o : turnoList) { o.setId(null); o.detach(); }
		for(HorarioAula o : horarioAulaList) { o.setId(null); o.detach(); }
		for(Curso o : cursoList) { o.setId(null); o.detach(); }
		for(Campus o : campusList) { o.setId(null); o.detach(); }
		for(DeslocamentoCampus o : deslocamentoCampusList) { o.setId(null); o.detach(); }
		for(Unidade o : unidadeList) { o.setId(null); o.detach(); }
		for(DeslocamentoUnidade o : deslocamentoUnidadeList) { o.setId(null); o.detach(); }
		for(Sala o : salaList) { o.setId(null); o.detach(); }
		for(GrupoSala o : grupoSalaList) { o.setId(null); o.detach(); }
		for(Disciplina o : disciplinaList) { o.setId(null); o.detach(); }
		for(Compatibilidade o : compatibilidadeList) { o.setId(null); o.detach(); }
		for(Equivalencia o : equivalenciaList) { o.setId(null); o.detach(); }
		for(DivisaoCredito o : divisaoCreditoList) { o.setId(null); o.detach(); }
		for(Professor o : professorList) { o.setId(null); o.detach(); }
		for(ProfessorDisciplina o : professorDisciplinaList) { o.setId(null); o.detach(); }
		for(Curriculo o : curriculoList) { o.setId(null); o.detach(); }
		for(CurriculoDisciplina o : curriculoDisciplinaList) { o.setId(null); o.detach(); }
		for(Oferta o : ofertaList) { o.setId(null); o.detach(); }
		for(Demanda o : demandaList) { o.setId(null); o.detach(); }
		for(HorarioDisponivelCenario o : horarioDisponivelCenarioList) { o.setId(null); o.detach(); }
	}
	
	private void saveList() {
		for(SemanaLetiva o : semanaLetivaList) { o.persist(); }
		cenario.persist();
		for(Turno o : turnoList) { o.persist(); }
		for(HorarioAula o : horarioAulaList) { o.persist(); }
		for(Curso o : cursoList) { o.persist(); }
		for(Campus o : campusList) { o.persist(); }
		for(DeslocamentoCampus o : deslocamentoCampusList) { o.persist(); }
		for(Unidade o : unidadeList) { o.persist(); }
		for(DeslocamentoUnidade o : deslocamentoUnidadeList) { o.persist(); }
		for(Sala o : salaList) { o.persist(); }
		for(GrupoSala o : grupoSalaList) { o.persist(); }
		for(Disciplina o : disciplinaList) { o.persist(); }
		for(Compatibilidade o : compatibilidadeList) { o.persist(); }
		for(Equivalencia o : equivalenciaList) { o.persist(); }
		for(DivisaoCredito o : divisaoCreditoList) { o.persist(); }
		for(Professor o : professorList) { o.persist(); }
		for(ProfessorDisciplina o : professorDisciplinaList) { o.persist(); }
		for(Curriculo o : curriculoList) { o.persist(); }
		for(CurriculoDisciplina o : curriculoDisciplinaList) { o.persist(); }
		for(Oferta o : ofertaList) { o.persist(); }
		for(Demanda o : demandaList) { o.persist(); }
		for(HorarioDisponivelCenario o : horarioDisponivelCenarioList) { o.persist(); }
	}
	
}
