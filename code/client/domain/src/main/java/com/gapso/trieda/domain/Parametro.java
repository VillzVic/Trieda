package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "PAR_ID")
@Table(name = "PARAMETROS")
public class Parametro implements Serializable {

	@NotNull
    @OneToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;
	
	//////////////////////////////////////////////
	// PREFERENCIAS DO ALUNO
	//////////////////////////////////////////////
	
	//Distribuição da carga horária semanal do aluno
    @Column(name = "PAR_CARGAHORARIAALUNO")
    private Boolean cargaHorariaAluno;
    @Size(min = 1, max = 20)
    @Column(name = "PAR_CARGAHORARIAALUNOSEL")
    private String cargaHorariaAlunoSel;
    
    //Manter alunos do mesmo curso-período na mesma sala
    @Column(name = "PAR_ALUNOPERIODOSALA")
    private Boolean alunoDePeriodoMesmaSala;
    
    //Permitir que o aluno estude em mais de um Campus
    @Column(name = "PAR_ALUNOMUITOSCAMPI")
    private Boolean alunoEmMuitosCampi;
    
    //Minimizar Deslocamento de Alunos entre Campi
    @Column(name = "PAR_MINDESLALUNO")
    private Boolean minimizarDeslocamentoAluno;

    //////////////////////////////////////////////
	// PREFERENCIAS DO PROFESSOR
    //////////////////////////////////////////////
    
    //Distribuição da carga horária semanal do professor
    @Column(name = "PAR_CARGAHORARIAPROF")
    private Boolean cargaHorariaProfessor;
    @Size(min = 1, max = 20)
    @Column(name = "PAR_CARGAHORARIAPROFSEL")
    private String cargaHorariaProfessorSel;
    
    //Permitir que o professor ministre aulas em mais de um Campus
    @Column(name = "PAR_PROFMUITOSCAMPI")
    private Boolean professorEmMuitosCampi;
    
    //Minimizar Deslocamentos de Professores entre Campi
    @Column(name = "PAR_MINDESLPROF")
    private Boolean minimizarDeslocamentoProfessor;
    @Column(name = "PAR_MINDESLPROFVALUE")
    private Integer minimizarDeslocamentoProfessorValue;
    
    //Minimizar Gaps nos Horários dos Professores
    @Column(name = "PAR_MINGAPPROF")
    private Boolean minimizarGapProfessor;
    
    //Evitar Redução de Carga Horária de Professor
    @Column(name = "PAR_EVITARREDCARGAHORPROF")
    private Boolean evitarReducaoCargaHorariaProfessor;
    @Column(name = "PAR_EVITARREDCARGAHORPROFVALUE")
    private Integer evitarReducaoCargaHorariaProfessorValue;
    
    //Evitar alocação de professores no último horário do dia e primeiro do dia seguinte
    @Column(name = "PAR_EDITARULTIMOPRIHORPROF")
    private Boolean editarUltimoEPrimeiroHorarioProfessor;
    
    //Considerar preferência de professores por disciplinas
    @Column(name = "PAR_PREFPROF")
    private Boolean preferenciaDeProfessores;
    
    //Considerar avaliação de desempenho de professores
    @Column(name = "PAR_AVALIACAODESEMPROF")
    private Boolean avaliacaoDesempenhoProfessor;

    //////////////////////////////////////////////
	// PREFERENCIAS DA INSTITUIÇÃO
    //////////////////////////////////////////////
    
    //Número mínimo de alunos para abrir uma turma
    @Column(name = "PAR_MINALUNTURMA")
    private Boolean minAlunosParaAbrirTurma;
    @Column(name = "PAR_MINALUNTURMAVALUE")
    private Integer minAlunosParaAbrirTurmaValue;

    //Considerar nível de dificuldade de disciplinas
    @Column(name = "PAR_NIVELDIFDISCI")
    private Boolean nivelDificuldadeDisciplina;

    //Considerar compatibilidade de disciplinas no mesmo dia
    @Column(name = "PAR_COMPATDISCDIA")
    private Boolean compatibilidadeDisciplinasMesmoDia;

    //Considerar regras genéricas de divisão de créditos
    @Column(name = "PAR_REGRASGENDIVCRE")
    private Boolean regrasGenericasDivisaoCredito;

    //Considerar regras específicas de divisão de créditos
    @Column(name = "PAR_REGRASESPDIVCRED")
    private Boolean regrasEspecificasDivisaoCredito;

    //Maximizar nota da avaliação do corpo docente de cursos específicos
    @Column(name = "PAR_MAXNOTAAVALCORDOC")
    private Boolean maximizarNotaAvaliacaoCorpoDocente;
    @OneToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH})
    private Set<Curso> cursosMaxNotaAval = new HashSet<Curso>();

    //Minimizar custo docente de cursos específicos
    @Column(name = "PAR_MINCUSTDOCCUR")
    private Boolean minimizarCustoDocenteCursos;
    @OneToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH})
    private Set<Curso> cursosMinCust = new HashSet<Curso>();

    //Permitir compartilhamento de disciplinas entre cursos
    @Column(name = "PAR_COMPDISCCAMPI")
    private Boolean compartilharDisciplinasCampi;
    @OneToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH})
    private Set<Curso> cursosCompartDiscCampi = new HashSet<Curso>();

    //Considerar percentuais mínimos de mestres
    @Column(name = "PAR_PERCMINMEST")
    private Boolean percentuaisMinimosMestres;

    //Considerar percentuais mínimos de doutores
    @Column(name = "PAR_PERCMINDOUT")
    private Boolean percentuaisMinimosDoutores;

    //Considerar áreas de titulação dos professores e cursos
    @Column(name = "PAR_AREATITPROFCUR")
    private Boolean areaTitulacaoProfessoresECursos;

    //Limitar máximo de disciplinas que um professor pode ministrar por curso
    @Column(name = "PAR_LIMMAXDISCPRO")
    private Boolean limitarMaximoDisciplinaProfessor;

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TUR_ID")
    private Long id;

    @Version
    @Column(name = "version")
    private Integer version;

    public Long getId() {
        return this.id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Integer getVersion() {
        return this.version;
    }

    public void setVersion(Integer version) {
        this.version = version;
    }

    @Transactional
    public void persist() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.persist(this);
    }

    @Transactional
    public void remove() {
        if (this.entityManager == null) this.entityManager = entityManager();
        if (this.entityManager.contains(this)) {
            this.entityManager.remove(this);
        } else {
            Parametro attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
	}
    
    @Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

    @Transactional
    public Parametro merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Parametro merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager() {
        EntityManager em = new Parametro().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

    @SuppressWarnings("unchecked")
    public static List<Parametro> findAll() {
        return entityManager().createQuery("SELECT o FROM Parametro o").getResultList();
    }

    public static Parametro find(Long id) {
        if (id == null) return null;
        return entityManager().find(Parametro.class, id);
    }
    
	public Cenario getCenario() {
        return this.cenario;
    }
	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
    }

	public Boolean getCargaHorariaAluno() {
		return cargaHorariaAluno;
	}
	public void setCargaHorariaAluno(Boolean cargaHorariaAluno) {
		this.cargaHorariaAluno = cargaHorariaAluno;
	}

	public Boolean getAlunoDePeriodoMesmaSala() {
		return alunoDePeriodoMesmaSala;
	}
	public void setAlunoDePeriodoMesmaSala(Boolean alunoDePeriodoMesmaSala) {
		this.alunoDePeriodoMesmaSala = alunoDePeriodoMesmaSala;
	}

	public Boolean getAlunoEmMuitosCampi() {
		return alunoEmMuitosCampi;
	}
	public void setAlunoEmMuitosCampi(Boolean alunoEmMuitosCampi) {
		this.alunoEmMuitosCampi = alunoEmMuitosCampi;
	}

	public Boolean getMinimizarDeslocamentoAluno() {
		return minimizarDeslocamentoAluno;
	}
	public void setMinimizarDeslocamentoAluno(Boolean minimizarDeslocamentoAluno) {
		this.minimizarDeslocamentoAluno = minimizarDeslocamentoAluno;
	}

	public Boolean getCargaHorariaProfessor() {
		return cargaHorariaProfessor;
	}
	public void setCargaHorariaProfessor(Boolean cargaHorariaProfessor) {
		this.cargaHorariaProfessor = cargaHorariaProfessor;
	}

	public Boolean getProfessorEmMuitosCampi() {
		return professorEmMuitosCampi;
	}
	public void setProfessorEmMuitosCampi(Boolean professorEmMuitosCampi) {
		this.professorEmMuitosCampi = professorEmMuitosCampi;
	}

	public Boolean getMinimizarDeslocamentoProfessor() {
		return minimizarDeslocamentoProfessor;
	}
	public void setMinimizarDeslocamentoProfessor(
			Boolean minimizarDeslocamentoProfessor) {
		this.minimizarDeslocamentoProfessor = minimizarDeslocamentoProfessor;
	}

	public Boolean getMinimizarGapProfessor() {
		return minimizarGapProfessor;
	}
	public void setMinimizarGapProfessor(Boolean minimizarGapProfessor) {
		this.minimizarGapProfessor = minimizarGapProfessor;
	}

	public Boolean getEvitarReducaoCargaHorariaProfessor() {
		return evitarReducaoCargaHorariaProfessor;
	}
	public void setEvitarReducaoCargaHorariaProfessor(
			Boolean evitarReducaoCargaHorariaProfessor) {
		this.evitarReducaoCargaHorariaProfessor = evitarReducaoCargaHorariaProfessor;
	}

	public Boolean getEditarUltimoEPrimeiroHorarioProfessor() {
		return editarUltimoEPrimeiroHorarioProfessor;
	}
	public void setEditarUltimoEPrimeiroHorarioProfessor(
			Boolean editarUltimoEPrimeiroHorarioProfessor) {
		this.editarUltimoEPrimeiroHorarioProfessor = editarUltimoEPrimeiroHorarioProfessor;
	}

	public Boolean getPreferenciaDeProfessores() {
		return preferenciaDeProfessores;
	}
	public void setPreferenciaDeProfessores(Boolean preferenciaDeProfessores) {
		this.preferenciaDeProfessores = preferenciaDeProfessores;
	}

	public Boolean getAvaliacaoDesempenhoProfessor() {
		return avaliacaoDesempenhoProfessor;
	}
	public void setAvaliacaoDesempenhoProfessor(Boolean avaliacaoDesempenhoProfessor) {
		this.avaliacaoDesempenhoProfessor = avaliacaoDesempenhoProfessor;
	}
	
	public Boolean getMinAlunosParaAbrirTurma() {
		return minAlunosParaAbrirTurma;
	}
	public void setMinAlunosParaAbrirTurma(Boolean minAlunosParaAbrirTurma) {
		this.minAlunosParaAbrirTurma = minAlunosParaAbrirTurma;
	}

	public Integer getMinAlunosParaAbrirTurmaValue() {
		return minAlunosParaAbrirTurmaValue;
	}
	public void setMinAlunosParaAbrirTurmaValue(Integer minAlunosParaAbrirTurmaValue) {
		this.minAlunosParaAbrirTurmaValue = minAlunosParaAbrirTurmaValue;
	}

	public Boolean getNivelDificuldadeDisciplina() {
		return nivelDificuldadeDisciplina;
	}
	public void setNivelDificuldadeDisciplina(Boolean nivelDificuldadeDisciplina) {
		this.nivelDificuldadeDisciplina = nivelDificuldadeDisciplina;
	}

	public Boolean getCompatibilidadeDisciplinasMesmoDia() {
		return compatibilidadeDisciplinasMesmoDia;
	}
	public void setCompatibilidadeDisciplinasMesmoDia(
			Boolean compatibilidadeDisciplinasMesmoDia) {
		this.compatibilidadeDisciplinasMesmoDia = compatibilidadeDisciplinasMesmoDia;
	}

	public Boolean getRegrasGenericasDivisaoCredito() {
		return regrasGenericasDivisaoCredito;
	}
	public void setRegrasGenericasDivisaoCredito(
			Boolean regrasGenericasDivisaoCredito) {
		this.regrasGenericasDivisaoCredito = regrasGenericasDivisaoCredito;
	}

	public Boolean getRegrasEspecificasDivisaoCredito() {
		return regrasEspecificasDivisaoCredito;
	}
	public void setRegrasEspecificasDivisaoCredito(
			Boolean regrasEspecificasDivisaoCredito) {
		this.regrasEspecificasDivisaoCredito = regrasEspecificasDivisaoCredito;
	}

	public Boolean getMaximizarNotaAvaliacaoCorpoDocente() {
		return maximizarNotaAvaliacaoCorpoDocente;
	}
	public void setMaximizarNotaAvaliacaoCorpoDocente(
			Boolean maximizarNotaAvaliacaoCorpoDocente) {
		this.maximizarNotaAvaliacaoCorpoDocente = maximizarNotaAvaliacaoCorpoDocente;
	}

	public Boolean getMinimizarCustoDocenteCursos() {
		return minimizarCustoDocenteCursos;
	}
	public void setMinimizarCustoDocenteCursos(Boolean minimizarCustoDocenteCursos) {
		this.minimizarCustoDocenteCursos = minimizarCustoDocenteCursos;
	}

	public Boolean getCompartilharDisciplinasCampi() {
		return compartilharDisciplinasCampi;
	}
	public void setCompartilharDisciplinasCampi(Boolean compartilharDisciplinasCampi) {
		this.compartilharDisciplinasCampi = compartilharDisciplinasCampi;
	}

	public Boolean getPercentuaisMinimosMestres() {
		return percentuaisMinimosMestres;
	}
	public void setPercentuaisMinimosMestres(Boolean percentuaisMinimosMestres) {
		this.percentuaisMinimosMestres = percentuaisMinimosMestres;
	}

	public Boolean getPercentuaisMinimosDoutores() {
		return percentuaisMinimosDoutores;
	}
	public void setPercentuaisMinimosDoutores(Boolean percentuaisMinimosDoutores) {
		this.percentuaisMinimosDoutores = percentuaisMinimosDoutores;
	}

	public Boolean getAreaTitulacaoProfessoresECursos() {
		return areaTitulacaoProfessoresECursos;
	}
	public void setAreaTitulacaoProfessoresECursos(
			Boolean areaTitulacaoProfessoresECursos) {
		this.areaTitulacaoProfessoresECursos = areaTitulacaoProfessoresECursos;
	}

	public Boolean getLimitarMaximoDisciplinaProfessor() {
		return limitarMaximoDisciplinaProfessor;
	}
	public void setLimitarMaximoDisciplinaProfessor(
			Boolean limitarMaximoDisciplinaProfessor) {
		this.limitarMaximoDisciplinaProfessor = limitarMaximoDisciplinaProfessor;
	}

	public String getCargaHorariaAlunoSel() {
		return cargaHorariaAlunoSel;
	}

	public void setCargaHorariaAlunoSel(String cargaHorariaAlunoSel) {
		this.cargaHorariaAlunoSel = cargaHorariaAlunoSel;
	}

	public String getCargaHorariaProfessorSel() {
		return cargaHorariaProfessorSel;
	}
	public void setCargaHorariaProfessorSel(String cargaHorariaProfessorSel) {
		this.cargaHorariaProfessorSel = cargaHorariaProfessorSel;
	}

	public Integer getMinimizarDeslocamentoProfessorValue() {
		return minimizarDeslocamentoProfessorValue;
	}
	public void setMinimizarDeslocamentoProfessorValue(
			Integer minimizarDeslocamentoProfessorValue) {
		this.minimizarDeslocamentoProfessorValue = minimizarDeslocamentoProfessorValue;
	}

	public Integer getEvitarReducaoCargaHorariaProfessorValue() {
		return evitarReducaoCargaHorariaProfessorValue;
	}
	public void setEvitarReducaoCargaHorariaProfessorValue(
			Integer evitarReducaoCargaHorariaProfessorValue) {
		this.evitarReducaoCargaHorariaProfessorValue = evitarReducaoCargaHorariaProfessorValue;
	}

	public Set<Curso> getCursosMaxNotaAval() {
		return cursosMaxNotaAval;
	}
	public void setCursosMaxNotaAval(Set<Curso> cursosMaxNotaAval) {
		this.cursosMaxNotaAval = cursosMaxNotaAval;
	}

	public Set<Curso> getCursosMinCust() {
		return cursosMinCust;
	}
	public void setCursosMinCust(Set<Curso> cursosMinCust) {
		this.cursosMinCust = cursosMinCust;
	}

	public Set<Curso> getCursosCompartDiscCampi() {
		return cursosCompartDiscCampi;
	}
	public void setCursosCompartDiscCampi(Set<Curso> cursosCompartDiscCampi) {
		this.cursosCompartDiscCampi = cursosCompartDiscCampi;
	}

    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("CargaHorariaAluno: ").append(getCargaHorariaAluno()).append(", ");
        sb.append("CargaHorariaAlunoSel: ").append(getCargaHorariaAlunoSel()).append(", ");
        sb.append("AlunoDePeriodoMesmaSala: ").append(getAlunoDePeriodoMesmaSala()).append(", ");
        sb.append("AlunoEmMuitosCampi: ").append(getAlunoEmMuitosCampi()).append(", ");
        sb.append("MinimizarDeslocamentoAluno: ").append(getMinimizarDeslocamentoAluno()).append(", ");
        sb.append("CargaHorariaProfessor: ").append(getCargaHorariaProfessor()).append(", ");
        sb.append("CargaHorariaProfessorSel: ").append(getCargaHorariaProfessorSel()).append(", ");
        sb.append("ProfessorEmMuitosCampi: ").append(getProfessorEmMuitosCampi()).append(", ");
        sb.append("MinimizarDeslocamentoProfessor: ").append(getMinimizarDeslocamentoProfessor()).append(", ");
        sb.append("MinimizarDeslocamentoProfessorValue: ").append(getMinimizarDeslocamentoProfessorValue()).append(", ");
        sb.append("MinimizarGapProfessor: ").append(getMinimizarGapProfessor()).append(", ");
        sb.append("EvitarReducaoCargaHorariaProfessor: ").append(getEvitarReducaoCargaHorariaProfessor()).append(", ");
        sb.append("EvitarReducaoCargaHorariaProfessorValue: ").append(getEvitarReducaoCargaHorariaProfessorValue()).append(", ");
        sb.append("EditarUltimoEPrimeiroHorarioProfessor: ").append(getEditarUltimoEPrimeiroHorarioProfessor()).append(", ");
        sb.append("PreferenciaDeProfessores: ").append(getPreferenciaDeProfessores()).append(", ");
        sb.append("AvaliacaoDesempenhoProfessor: ").append(getAvaliacaoDesempenhoProfessor()).append(", ");
        sb.append("NivelDificuldadeDisciplina: ").append(getNivelDificuldadeDisciplina()).append(", ");
        sb.append("CompatibilidadeDisciplinasMesmoDia: ").append(getCompatibilidadeDisciplinasMesmoDia()).append(", ");
        sb.append("RegrasGenericasDivisaoCredito: ").append(getRegrasGenericasDivisaoCredito()).append(", ");
        sb.append("RegrasEspecificasDivisaoCredito: ").append(getRegrasEspecificasDivisaoCredito()).append(", ");
        sb.append("MaximizarNotaAvaliacaoCorpoDocente: ").append(getMaximizarNotaAvaliacaoCorpoDocente()).append(", ");
        sb.append("CursosMaxNotaAval: ").append(getCursosMaxNotaAval() == null ? "null" : getCursosMaxNotaAval().size());
        sb.append("MinimizarCustoDocenteCursos: ").append(getMinimizarCustoDocenteCursos()).append(", ");
        sb.append("MinAlunosParaAbrirTurma: ").append(getMinAlunosParaAbrirTurma()).append(", ");
        sb.append("MinAlunosParaAbrirTurmaValue: ").append(getMinAlunosParaAbrirTurmaValue()).append(", ");
        sb.append("CursosMinCust: ").append(getCursosMinCust() == null ? "null" : getCursosMinCust().size());
        sb.append("CompartilharDisciplinasCampi: ").append(getCompartilharDisciplinasCampi()).append(", ");
        sb.append("CursosCompartDiscCampi: ").append(getCursosCompartDiscCampi() == null ? "null" : getCursosCompartDiscCampi().size());
        sb.append("PercentuaisMinimosMestres: ").append(getPercentuaisMinimosMestres()).append(", ");
        sb.append("PercentuaisMinimosDoutores: ").append(getPercentuaisMinimosDoutores()).append(", ");
        sb.append("AreaTitulacaoProfessoresECursos: ").append(getAreaTitulacaoProfessoresECursos()).append(", ");
        sb.append("LimitarMaximoDisciplinaProfessor: ").append(getLimitarMaximoDisciplinaProfessor()).append(", ");
        return sb.toString();
    }
}
