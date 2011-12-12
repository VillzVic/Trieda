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
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
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
@RooEntity( identifierColumn = "PAR_ID" )
@Table( name = "PARAMETROS" )
public class Parametro
	implements Serializable
{
	private static final long serialVersionUID = -1310877837088078190L;

	public static final String TATICO = "TATICO";
	public static final String OPERACIONAL = "OPERACIONAL";

    @NotNull
    @ManyToOne
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;
	
	@Size(min = 1, max = 20)
	@Column(name = "PAR_MODOOTIMIZACAO")
	private String modoOtimizacao;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST,
		CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}
	
	@NotNull
	@ManyToOne
	@JoinColumn(name = "CAM_ID")
	private Campus campus;
	
	@NotNull
	@ManyToOne
	@JoinColumn(name = "TUR_ID")
	private Turno turno;
	
	//////////////////////////////////////////////
	// PREFERENCIAS DO ALUNO
	//////////////////////////////////////////////
	
	//Distribui��o da carga hor�ria semanal do aluno
    @Column(name = "PAR_CARGAHORARIAALUNO")
    private Boolean cargaHorariaAluno = false;
    @Size(min = 1, max = 20)
    @Column(name = "PAR_CARGAHORARIAALUNOSEL")
    private String cargaHorariaAlunoSel;
    
    //Manter alunos do mesmo curso-per�odo na mesma sala
    @Column(name = "PAR_ALUNOPERIODOSALA")
    private Boolean alunoDePeriodoMesmaSala = false;
    
    //Permitir que o aluno estude em mais de um Campus
    @Column(name = "PAR_ALUNOMUITOSCAMPI")
    private Boolean alunoEmMuitosCampi = false;
    
    //Minimizar Deslocamento de Alunos entre Campi
    @Column(name = "PAR_MINDESLALUNO")
    private Boolean minimizarDeslocamentoAluno = false;

    //////////////////////////////////////////////
	// PREFERENCIAS DO PROFESSOR
    //////////////////////////////////////////////
    
    //Distribui��o da carga hor�ria semanal do professor
    @Column(name = "PAR_CARGAHORARIAPROF")
    private Boolean cargaHorariaProfessor = false;
    @Size(min = 1, max = 20)
    @Column(name = "PAR_CARGAHORARIAPROFSEL")
    private String cargaHorariaProfessorSel;
    
    //Permitir que o professor ministre aulas em mais de um Campus
    @Column(name = "PAR_PROFMUITOSCAMPI")
    private Boolean professorEmMuitosCampi = false;
    
    //Minimizar Deslocamentos de Professores entre Campi
    @Column(name = "PAR_MINDESLPROF")
    private Boolean minimizarDeslocamentoProfessor = false;
    @Column(name = "PAR_MINDESLPROFVALUE")
    private Integer minimizarDeslocamentoProfessorValue;
    
    //Minimizar Gaps nos Hor�rios dos Professores
    @Column(name = "PAR_MINGAPPROF")
    private Boolean minimizarGapProfessor = false;
    
    //Evitar Redu��o de Carga Hor�ria de Professor
    @Column(name = "PAR_EVITARREDCARGAHORPROF")
    private Boolean evitarReducaoCargaHorariaProfessor = false;
    @Column(name = "PAR_EVITARREDCARGAHORPROFVALUE")
    private Integer evitarReducaoCargaHorariaProfessorValue;
    
    //Evitar aloca��o de professores no �ltimo hor�rio do dia e primeiro do dia seguinte
    @Column(name = "PAR_EVITARULTIMOPRIHORPROF")
    private Boolean evitarUltimoEPrimeiroHorarioProfessor = false;
    
    //Considerar prefer�ncia de professores por disciplinas
    @Column(name = "PAR_PREFPROF")
    private Boolean preferenciaDeProfessores = false;
    
    //Considerar avalia��o de desempenho de professores
    @Column(name = "PAR_AVALIACAODESEMPROF")
    private Boolean avaliacaoDesempenhoProfessor = false;

    //////////////////////////////////////////////
	// PREFERENCIAS DA INSTITUI��O
    //////////////////////////////////////////////
    
    // Func��o objetivo
    @Column(name = "PAR_FUNCAOOBJETIVO")
    private Integer funcaoObjetivo = 0;
    
    //Considerar Equival�ncias entre Disciplinas
    @Column(name = "PAR_CONSIDERAR_EQUIV")
    private Boolean considerarEquivalencia = false;
    
    //N�mero m�nimo de alunos para abrir uma turma
    @Column(name = "PAR_MINALUNTURMA")
    private Boolean minAlunosParaAbrirTurma = false;
    @Column(name = "PAR_MINALUNTURMAVALUE")
    private Integer minAlunosParaAbrirTurmaValue;

    //Considerar n�vel de dificuldade de disciplinas
    @Column(name = "PAR_NIVELDIFDISCI")
    private Boolean nivelDificuldadeDisciplina = false;

    //Considerar compatibilidade de disciplinas no mesmo dia
    @Column(name = "PAR_COMPATDISCDIA")
    private Boolean compatibilidadeDisciplinasMesmoDia = false;

    //Considerar regras gen�ricas de divis�o de cr�ditos
    @Column(name = "PAR_REGRASGENDIVCRE")
    private Boolean regrasGenericasDivisaoCredito = false;

    //Considerar regras espec�ficas de divis�o de cr�ditos
    @Column(name = "PAR_REGRASESPDIVCRED")
    private Boolean regrasEspecificasDivisaoCredito = false;

    //Maximizar nota da avalia��o do corpo docente de cursos espec�ficos
    @Column(name = "PAR_MAXNOTAAVALCORDOC")
    private Boolean maximizarNotaAvaliacaoCorpoDocente = false;
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE})
    private Set<Curso> cursosMaxNotaAval = new HashSet<Curso>();

    //Minimizar custo docente de cursos espec�ficos
    @Column(name = "PAR_MINCUSTDOCCUR")
    private Boolean minimizarCustoDocenteCursos = false;
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE})
    private Set<Curso> cursosMinCust = new HashSet<Curso>();

    //Permitir compartilhamento de disciplinas entre cursos
    @Column(name = "PAR_COMPDISCCAMPI")
    private Boolean compartilharDisciplinasCampi = false;
    @OneToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH}, mappedBy="parametro")
    private Set<CursoDescompartilha> cursosDescompartDiscCampi = new HashSet<CursoDescompartilha>();

    //Considerar percentuais m�nimos de mestres
    @Column(name = "PAR_PERCMINMEST")
    private Boolean percentuaisMinimosMestres = false;

    //Considerar percentuais m�nimos de doutores
    @Column(name = "PAR_PERCMINDOUT")
    private Boolean percentuaisMinimosDoutores = false;

    //Considerar �reas de titula��o dos professores e cursos
    @Column(name = "PAR_AREATITPROFCUR")
    private Boolean areaTitulacaoProfessoresECursos = false;

    //Limitar m�ximo de disciplinas que um professor pode ministrar por curso
    @Column(name = "PAR_LIMMAXDISCPRO")
    private Boolean limitarMaximoDisciplinaProfessor = false;

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "PAR_ID")
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

    public boolean isPersist() {
    	return (this.getId() != null && this.getId() > 0);
    }
    
    @Transactional
    public void save() {
    	if(isPersist()) {
    		merge();
    	} else {
    		persist();
    	}
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

    public static final EntityManager entityManager()
    {
        EntityManager em = new Parametro().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

    @SuppressWarnings("unchecked")
    public static List< Parametro > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Parametro o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.turno.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
    }

    public static Parametro find(
    	Long id, InstituicaoEnsino instituicaoEnsino )
    {
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Parametro parametro = entityManager().find( Parametro.class, id );

        if ( parametro != null
        	&& parametro.getInstituicaoEnsino() != null
        	&& parametro.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return parametro;
        }

        return null;
    }

    public boolean isTatico()
    {
    	return getModoOtimizacao().equals( Parametro.TATICO );
    }

    public boolean isOperacional()
    {
    	return getModoOtimizacao().equals( Parametro.OPERACIONAL );
    }

	public Cenario getCenario()
	{
        return this.cenario;
    }

	public void setCenario(Cenario cenario )
	{
        this.cenario = cenario;
    }

	public Boolean getCargaHorariaAluno()
	{
		return cargaHorariaAluno;
	}

	public void setCargaHorariaAluno( Boolean cargaHorariaAluno )
	{
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
	public void setMinimizarDeslocamentoProfessor(Boolean minimizarDeslocamentoProfessor) {
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
	public void setEvitarReducaoCargaHorariaProfessor(Boolean evitarReducaoCargaHorariaProfessor) {
		this.evitarReducaoCargaHorariaProfessor = evitarReducaoCargaHorariaProfessor;
	}

	public Boolean getEvitarUltimoEPrimeiroHorarioProfessor() {
		return evitarUltimoEPrimeiroHorarioProfessor;
	}
	public void setEvitarUltimoEPrimeiroHorarioProfessor(Boolean editarUltimoEPrimeiroHorarioProfessor) {
		this.evitarUltimoEPrimeiroHorarioProfessor = editarUltimoEPrimeiroHorarioProfessor;
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
	
	public Integer getFuncaoObjetivo() {
		return funcaoObjetivo;
	}
	public void setFuncaoObjetivo(Integer funcaoObjetivo) {
		this.funcaoObjetivo = funcaoObjetivo;
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
	
	public Boolean getConsiderarEquivalencia() {
		return considerarEquivalencia;
	}

	public void setConsiderarEquivalencia(Boolean considerarEquivalencia) {
		this.considerarEquivalencia = considerarEquivalencia;
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
	public void setCompatibilidadeDisciplinasMesmoDia(Boolean compatibilidadeDisciplinasMesmoDia) {
		this.compatibilidadeDisciplinasMesmoDia = compatibilidadeDisciplinasMesmoDia;
	}

	public Boolean getRegrasGenericasDivisaoCredito() {
		return regrasGenericasDivisaoCredito;
	}
	public void setRegrasGenericasDivisaoCredito(Boolean regrasGenericasDivisaoCredito) {
		this.regrasGenericasDivisaoCredito = regrasGenericasDivisaoCredito;
	}

	public Boolean getRegrasEspecificasDivisaoCredito() {
		return regrasEspecificasDivisaoCredito;
	}
	public void setRegrasEspecificasDivisaoCredito(Boolean regrasEspecificasDivisaoCredito) {
		this.regrasEspecificasDivisaoCredito = regrasEspecificasDivisaoCredito;
	}

	public Boolean getMaximizarNotaAvaliacaoCorpoDocente() {
		return maximizarNotaAvaliacaoCorpoDocente;
	}
	public void setMaximizarNotaAvaliacaoCorpoDocente(Boolean maximizarNotaAvaliacaoCorpoDocente) {
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
	public void setAreaTitulacaoProfessoresECursos(Boolean areaTitulacaoProfessoresECursos) {
		this.areaTitulacaoProfessoresECursos = areaTitulacaoProfessoresECursos;
	}

	public Boolean getLimitarMaximoDisciplinaProfessor() {
		return limitarMaximoDisciplinaProfessor;
	}
	public void setLimitarMaximoDisciplinaProfessor(Boolean limitarMaximoDisciplinaProfessor) {
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
	public void setMinimizarDeslocamentoProfessorValue(Integer minimizarDeslocamentoProfessorValue) {
		this.minimizarDeslocamentoProfessorValue = minimizarDeslocamentoProfessorValue;
	}

	public Integer getEvitarReducaoCargaHorariaProfessorValue() {
		return evitarReducaoCargaHorariaProfessorValue;
	}
	public void setEvitarReducaoCargaHorariaProfessorValue(Integer evitarReducaoCargaHorariaProfessorValue) {
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

	public Set<CursoDescompartilha> getCursosDescompartDiscCampi() {
		return cursosDescompartDiscCampi;
	}
	public void setCursosDescompartDiscCampi(Set<CursoDescompartilha> cursosDescompartDiscCampi) {
		this.cursosDescompartDiscCampi = cursosDescompartDiscCampi;
	}
	
	public Campus getCampus() {
		return campus;
	}
	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public Turno getTurno() {
		return turno;
	}
	public void setTurno(Turno turno) {
		this.turno = turno;
	}

	public String getModoOtimizacao() {
		return modoOtimizacao;
	}
	public void setModoOtimizacao(String modoOtimizacao) {
		this.modoOtimizacao = modoOtimizacao;
	}

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("ModoOtimizacao: ").append(getModoOtimizacao()).append(", ");
        sb.append("Campus: ").append(getCampus()).append(", ");
        sb.append("Turno: ").append(getTurno()).append(", ");
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
        sb.append("EditarUltimoEPrimeiroHorarioProfessor: ").append(getEvitarUltimoEPrimeiroHorarioProfessor()).append(", ");
        sb.append("PreferenciaDeProfessores: ").append(getPreferenciaDeProfessores()).append(", ");
        sb.append("AvaliacaoDesempenhoProfessor: ").append(getAvaliacaoDesempenhoProfessor()).append(", ");
        sb.append("ConsiderarEquivalencia: ").append(getConsiderarEquivalencia()).append(", ");
        sb.append("NivelDificuldadeDisciplina: ").append(getNivelDificuldadeDisciplina()).append(", ");
        sb.append("CompatibilidadeDisciplinasMesmoDia: ").append(getCompatibilidadeDisciplinasMesmoDia()).append(", ");
        sb.append("RegrasGenericasDivisaoCredito: ").append(getRegrasGenericasDivisaoCredito()).append(", ");
        sb.append("RegrasEspecificasDivisaoCredito: ").append(getRegrasEspecificasDivisaoCredito()).append(", ");
        sb.append("MaximizarNotaAvaliacaoCorpoDocente: ").append(getMaximizarNotaAvaliacaoCorpoDocente()).append(", ");
        sb.append("CursosMaxNotaAval: ").append(getCursosMaxNotaAval() == null ? "null" : getCursosMaxNotaAval().size());
        sb.append("MinimizarCustoDocenteCursos: ").append(getMinimizarCustoDocenteCursos()).append(", ");
        sb.append("FuncaoObjetivo: ").append(getFuncaoObjetivo()).append(", "); 
        sb.append("MinAlunosParaAbrirTurma: ").append(getMinAlunosParaAbrirTurma()).append(", "); 
        sb.append("MinAlunosParaAbrirTurmaValue: ").append(getMinAlunosParaAbrirTurmaValue()).append(", ");
        sb.append("CursosMinCust: ").append(getCursosMinCust() == null ? "null" : getCursosMinCust().size());
        sb.append("CompartilharDisciplinasCampi: ").append(getCompartilharDisciplinasCampi()).append(", ");
        sb.append("CursosDescompartDiscCampi: ").append(getCursosDescompartDiscCampi() == null ? "null" : getCursosDescompartDiscCampi().size());
        sb.append("PercentuaisMinimosMestres: ").append(getPercentuaisMinimosMestres()).append(", ");
        sb.append("PercentuaisMinimosDoutores: ").append(getPercentuaisMinimosDoutores()).append(", ");
        sb.append("AreaTitulacaoProfessoresECursos: ").append(getAreaTitulacaoProfessoresECursos()).append(", ");
        sb.append("LimitarMaximoDisciplinaProfessor: ").append(getLimitarMaximoDisciplinaProfessor()).append(", ");

        return sb.toString();
    }
}
