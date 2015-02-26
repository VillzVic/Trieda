package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;

public class AtendimentoImportExcelBean extends AbstractImportExcelBean implements Comparable<AtendimentoImportExcelBean> {
	private String aula_campusCodigoStr;
	private String aula_AmbienteCodigoStr;
	private String aula_diaSemanaStr;
	private String aula_horarioInicialStr;
	private String aula_horarioFinalStr;
	private String aula_disciplinaAulaCodigoStr;
	private String aula_turmaStr;
	private String aula_cpfProfessorStr;
	private String aula_qtdCreditosPraticosStr;
	private String aula_qtdCreditosTeoricosStr;
	private String dem_codigoTurnoStr;
	private String dem_codigoCursoStr;
	private String dem_codigoCurriculoStr;
	private String dem_periodoStr;
	private String dem_disciplinaCodigoStr;
	private String dem_matriculaAlunoStr;
	private String dem_prioridadeAlunoStr;

	private Campus campus;
	private Sala ambiente;
	private Semanas diaSemana;
	private Calendar horarioInicial;
	private Calendar horarioFinal;
	private Disciplina disciplinaAula;
	private Professor professor;
	private Integer qtdCreditosTeoricos;
	private Integer qtdCreditosPraticos;
	private Turno turno;
	private Curso curso;
	private Curriculo curriculo;
	private Integer periodo;
	private Disciplina disciplinaDemanda;
	private Aluno aluno;
	private Integer prioridade;
	private AlunoDemanda alunoDemanda;

	public AtendimentoImportExcelBean( int row ) {
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors() {
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();

		if ( !tudoVazio() ) {
			checkMandatoryField( this.aula_campusCodigoStr, ImportExcelError.ATENDIMENTO_CAMPUS_VAZIO, erros );
			checkMandatoryField( this.aula_AmbienteCodigoStr, ImportExcelError.ATENDIMENTO_AMBIENTE_VAZIO, erros );
			checkMandatoryField( this.aula_diaSemanaStr, ImportExcelError.ATENDIMENTO_DIA_SEMANA_VAZIO, erros );
			checkMandatoryField( this.aula_horarioInicialStr, ImportExcelError.ATENDIMENTO_HORARIO_INICIAL_VAZIO, erros );
			checkMandatoryField( this.aula_horarioFinalStr, ImportExcelError.ATENDIMENTO_HORARIO_FINAL_VAZIO, erros );
			checkMandatoryField( this.aula_disciplinaAulaCodigoStr, ImportExcelError.ATENDIMENTO_DISCIPLINA_AULA_VAZIO, erros );
			checkMandatoryField( this.aula_turmaStr, ImportExcelError.ATENDIMENTO_TURMA_VAZIO, erros );
			checkMandatoryField( this.aula_cpfProfessorStr, ImportExcelError.ATENDIMENTO_PROFESSOR_CPF_VAZIO, erros );
			checkMandatoryField( this.aula_qtdCreditosPraticosStr, ImportExcelError.ATENDIMENTO_CRED_PRATICOS_VAZIO, erros );
			checkMandatoryField( this.aula_qtdCreditosTeoricosStr, ImportExcelError.ATENDIMENTO_CRED_TEORICOS_VAZIO, erros );
			checkMandatoryField( this.dem_codigoTurnoStr, ImportExcelError.ATENDIMENTO_TURNO_VAZIO, erros );
			checkMandatoryField( this.dem_codigoCursoStr, ImportExcelError.ATENDIMENTO_CURSO_VAZIO, erros );
			checkMandatoryField( this.dem_codigoCurriculoStr, ImportExcelError.ATENDIMENTO_CURRICULO_VAZIO, erros );
			checkMandatoryField( this.dem_matriculaAlunoStr, ImportExcelError.ATENDIMENTO_MATRICULA_ALUNO_VAZIO, erros );
			checkMandatoryField( this.dem_disciplinaCodigoStr, ImportExcelError.ATENDIMENTO_DISCIPLINA_DEMANDA_VAZIO, erros );
			
			this.diaSemana = checkEnumField(this.aula_diaSemanaStr,Semanas.class,ImportExcelError.ATENDIMENTO_DIA_SEMANA_VALOR_INVALIDO,erros);
			this.horarioInicial = checkTimeField(this.aula_horarioInicialStr,ImportExcelError.ATENDIMENTO_HORARIO_INICIAL_FORMATO_INVALIDO,erros);
			this.horarioFinal = checkTimeField(this.aula_horarioFinalStr,ImportExcelError.ATENDIMENTO_HORARIO_FINAL_FORMATO_INVALIDO,erros);
			
			this.qtdCreditosPraticos = checkNonNegativeIntegerField( this.aula_qtdCreditosPraticosStr, ImportExcelError.ATENDIMENTO_CRED_PRATICOS_FORMATO_INVALIDO, ImportExcelError.ATENDIMENTO_CRED_PRATICOS_VALOR_NEGATIVO, erros );
			this.qtdCreditosTeoricos = checkNonNegativeIntegerField( this.aula_qtdCreditosTeoricosStr, ImportExcelError.ATENDIMENTO_CRED_TEORICOS_FORMATO_INVALIDO, ImportExcelError.ATENDIMENTO_CRED_TEORICOS_VALOR_NEGATIVO, erros );
			
			this.periodo = checkNonNegativeIntegerField( this.dem_periodoStr, ImportExcelError.ATENDIMENTO_PERIODO_FORMATO_INVALIDO, ImportExcelError.ATENDIMENTO_PERIODO_NEGATIVO, erros );
			
			this.prioridade = checkNonNegativeIntegerField( this.dem_prioridadeAlunoStr, ImportExcelError.ATENDIMENTO_PRIORIDADE_FORMATO_INVALIDO, ImportExcelError.ATENDIMENTO_PRIORIDADE_NEGATIVO, erros );
		} else {
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	public boolean tudoVazio() {
		return ( isEmptyField( this.aula_campusCodigoStr)
			&& isEmptyField( this.aula_AmbienteCodigoStr)
			&& isEmptyField( this.aula_diaSemanaStr)
			&& isEmptyField( this.aula_horarioInicialStr)
			&& isEmptyField( this.aula_horarioFinalStr)
			&& isEmptyField( this.aula_disciplinaAulaCodigoStr)
			&& isEmptyField( this.aula_turmaStr)
			&& isEmptyField( this.aula_cpfProfessorStr)
			&& isEmptyField( this.aula_qtdCreditosPraticosStr)
			&& isEmptyField( this.aula_qtdCreditosTeoricosStr)
			&& isEmptyField( this.dem_codigoTurnoStr)
			&& isEmptyField( this.dem_codigoCursoStr)
			&& isEmptyField( this.dem_codigoCurriculoStr)
			&& isEmptyField( this.dem_matriculaAlunoStr)
			&& isEmptyField( this.dem_disciplinaCodigoStr));
	}

	public String getNaturalKeyString() {
		return this.dem_matriculaAlunoStr +
			"-" + this.dem_disciplinaCodigoStr +
			"-" + this.aula_campusCodigoStr +
			"-" + this.aula_AmbienteCodigoStr +
			"-" + this.aula_diaSemanaStr +
			"-" + this.aula_horarioInicialStr +
			"-" + this.aula_horarioFinalStr;
	}
	
	public String getAlunoDemandaNaturalKeyString() {
		return this.aula_campusCodigoStr +
			"-" + this.dem_codigoTurnoStr +
			"-" + this.dem_codigoCursoStr +
			"-" + this.dem_codigoCurriculoStr +
			"-" + this.dem_disciplinaCodigoStr +
			"-" + this.dem_matriculaAlunoStr;
	}

	@Override
	public int compareTo( AtendimentoImportExcelBean o )
	{
		int result = getCampus().compareTo( o.getCampus() );
		if ( result == 0 ) {
			result = getAmbiente().compareTo( o.getAmbiente() );
			if ( result == 0 ) {
				result = getDiaSemana().compareTo( o.getDiaSemana() );
				if ( result == 0 ) {
					result = getHorarioInicial().compareTo( o.getHorarioInicial() );
					if ( result == 0 ) {
						result = getHorarioFinal().compareTo( o.getHorarioFinal() );
						if ( result == 0 ) {
							result = getDisciplinaAula().compareTo( o.getDisciplinaAula() );
							if ( result == 0 ) {
								result = getAluno().compareTo( o.getAluno() );
								if ( result == 0 ) {
									result = getDisciplinaDemanda().compareTo( o.getDisciplinaDemanda() );
								}
							}
						}
					}
				}
			}
		}

		return result;
	}
	
	public boolean podeSerProfessorVirtual() {
		return !isEmptyField( this.aula_cpfProfessorStr) && (!this.aula_cpfProfessorStr.contains(".") || !this.aula_cpfProfessorStr.contains("-"));
	}
	
	public Oferta getOferta() {
		return this.getAlunoDemanda().getDemanda().getOferta();
	}
	
	public Disciplina getDisciplinaSubstituta() {
		if (!this.getDisciplinaAula().equals(this.getDisciplinaDemanda())) {
			return this.getDisciplinaAula();
		}
		return null;
	}
	
	public int getTotalCreditos() {
		return this.qtdCreditosTeoricos + this.qtdCreditosPraticos;
	}

	public String getAula_campusCodigoStr() {
		return aula_campusCodigoStr;
	}

	public void setAula_campusCodigoStr(String aula_campusCodigoStr) {
		this.aula_campusCodigoStr = aula_campusCodigoStr;
	}

	public String getAula_AmbienteCodigoStr() {
		return aula_AmbienteCodigoStr;
	}

	public void setAula_AmbienteCodigoStr(String aula_AmbienteCodigoStr) {
		this.aula_AmbienteCodigoStr = aula_AmbienteCodigoStr;
	}

	public String getAula_diaSemanaStr() {
		return aula_diaSemanaStr;
	}

	public void setAula_diaSemanaStr(String aula_diaSemanaStr) {
		this.aula_diaSemanaStr = aula_diaSemanaStr;
	}

	public String getAula_horarioInicialStr() {
		return aula_horarioInicialStr;
	}

	public void setAula_horarioInicialStr(String aula_horarioInicialStr) {
		this.aula_horarioInicialStr = aula_horarioInicialStr;
	}

	public String getAula_horarioFinalStr() {
		return aula_horarioFinalStr;
	}

	public void setAula_horarioFinalStr(String aula_horarioFinalStr) {
		this.aula_horarioFinalStr = aula_horarioFinalStr;
	}

	public String getAula_disciplinaAulaCodigoStr() {
		return aula_disciplinaAulaCodigoStr;
	}

	public void setAula_disciplinaAulaCodigoStr(String aula_disciplinaAulaCodigoStr) {
		this.aula_disciplinaAulaCodigoStr = aula_disciplinaAulaCodigoStr;
	}

	public String getAula_turmaStr() {
		return aula_turmaStr;
	}

	public void setAula_turmaStr(String aula_turmaStr) {
		this.aula_turmaStr = aula_turmaStr;
	}

	public String getAula_cpfProfessorStr() {
		return aula_cpfProfessorStr;
	}

	public void setAula_cpfProfessorStr(String aula_cpfProfessorStr) {
		this.aula_cpfProfessorStr = aula_cpfProfessorStr;
	}

	public String getAula_qtdCreditosPraticosStr() {
		return aula_qtdCreditosPraticosStr;
	}

	public void setAula_qtdCreditosPraticosStr(String aula_qtdCreditosPraticosStr) {
		this.aula_qtdCreditosPraticosStr = aula_qtdCreditosPraticosStr;
	}

	public String getAula_qtdCreditosTeoricosStr() {
		return aula_qtdCreditosTeoricosStr;
	}

	public void setAula_qtdCreditosTeoricosStr(String aula_qtdCreditosTeoricosStr) {
		this.aula_qtdCreditosTeoricosStr = aula_qtdCreditosTeoricosStr;
	}

	public String getDem_codigoTurnoStr() {
		return dem_codigoTurnoStr;
	}

	public void setDem_codigoTurnoStr(String dem_codigoTurnoStr) {
		this.dem_codigoTurnoStr = dem_codigoTurnoStr;
	}

	public String getDem_codigoCursoStr() {
		return dem_codigoCursoStr;
	}

	public void setDem_codigoCursoStr(String dem_codigoCursoStr) {
		this.dem_codigoCursoStr = dem_codigoCursoStr;
	}

	public String getDem_codigoCurriculoStr() {
		return dem_codigoCurriculoStr;
	}

	public void setDem_codigoCurriculoStr(String dem_codigoCurriculoStr) {
		this.dem_codigoCurriculoStr = dem_codigoCurriculoStr;
	}

	public String getDem_periodoStr() {
		return dem_periodoStr;
	}

	public void setDem_periodoStr(String dem_periodoStr) {
		this.dem_periodoStr = dem_periodoStr;
	}

	public String getDem_disciplinaCodigoStr() {
		return dem_disciplinaCodigoStr;
	}

	public void setDem_disciplinaCodigoStr(String dem_disciplinaCodigoStr) {
		this.dem_disciplinaCodigoStr = dem_disciplinaCodigoStr;
	}

	public String getDem_matriculaAlunoStr() {
		return dem_matriculaAlunoStr;
	}

	public void setDem_matriculaAlunoStr(String dem_matriculaAlunoStr) {
		this.dem_matriculaAlunoStr = dem_matriculaAlunoStr;
	}

	public String getDem_prioridadeAlunoStr() {
		return dem_prioridadeAlunoStr;
	}

	public void setDem_prioridadeAlunoStr(String dem_prioridadeAlunoStr) {
		this.dem_prioridadeAlunoStr = dem_prioridadeAlunoStr;
	}

	public Campus getCampus() {
		return campus;
	}

	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public Sala getAmbiente() {
		return ambiente;
	}

	public void setAmbiente(Sala ambiente) {
		this.ambiente = ambiente;
	}

	public Semanas getDiaSemana() {
		return diaSemana;
	}

	public void setDiaSemana(Semanas diaSemana) {
		this.diaSemana = diaSemana;
	}

	public Calendar getHorarioInicial() {
		return horarioInicial;
	}

	public void setHorarioInicial(Calendar horarioInicial) {
		this.horarioInicial = horarioInicial;
	}

	public Calendar getHorarioFinal() {
		return horarioFinal;
	}

	public void setHorarioFinal(Calendar horarioFinal) {
		this.horarioFinal = horarioFinal;
	}

	public Disciplina getDisciplinaAula() {
		return disciplinaAula;
	}

	public void setDisciplinaAula(Disciplina disciplinaAula) {
		this.disciplinaAula = disciplinaAula;
	}

	public Professor getProfessor() {
		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	public Integer getQtdCreditosTeoricos() {
		return qtdCreditosTeoricos;
	}

	public void setQtdCreditosTeoricos(Integer qtdCreditosTeoricos) {
		this.qtdCreditosTeoricos = qtdCreditosTeoricos;
	}

	public Integer getQtdCreditosPraticos() {
		return qtdCreditosPraticos;
	}

	public void setQtdCreditosPraticos(Integer qtdCreditosPraticos) {
		this.qtdCreditosPraticos = qtdCreditosPraticos;
	}

	public Turno getTurno() {
		return turno;
	}

	public void setTurno(Turno turno) {
		this.turno = turno;
	}

	public Curso getCurso() {
		return curso;
	}

	public void setCurso(Curso curso) {
		this.curso = curso;
	}

	public Curriculo getCurriculo() {
		return curriculo;
	}

	public void setCurriculo(Curriculo curriculo) {
		this.curriculo = curriculo;
	}

	public Integer getPeriodo() {
		return periodo;
	}

	public void setPeriodo(Integer periodo) {
		this.periodo = periodo;
	}

	public Disciplina getDisciplinaDemanda() {
		return disciplinaDemanda;
	}

	public void setDisciplinaDemanda(Disciplina disciplinaDemanda) {
		this.disciplinaDemanda = disciplinaDemanda;
	}

	public Aluno getAluno() {
		return aluno;
	}

	public void setAluno(Aluno aluno) {
		this.aluno = aluno;
	}

	public Integer getPrioridade() {
		return prioridade;
	}

	public void setPrioridade(Integer prioridade) {
		this.prioridade = prioridade;
	}

	public AlunoDemanda getAlunoDemanda() {
		return alunoDemanda;
	}

	public void setAlunoDemanda(AlunoDemanda alunoDemanda) {
		this.alunoDemanda = alunoDemanda;
	}
}

