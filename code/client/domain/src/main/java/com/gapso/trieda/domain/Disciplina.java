package com.gapso.trieda.domain;

import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
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
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Dificuldades;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "DIS_ID")
@Table(name = "DISCIPLINAS")
public class Disciplina implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @ManyToOne(targetEntity = TipoDisciplina.class)
    @JoinColumn(name = "TDI_ID")
    private TipoDisciplina tipoDisciplina;

    @ManyToOne(targetEntity = DivisaoCredito.class)
    @JoinColumn(name = "DCR_ID")
    private DivisaoCredito divisaoCreditos;

    @NotNull
    @Column(name = "DIS_CODIGO")
    @Size(min = 3, max = 20)
    private String codigo;

    @NotNull
    @Column(name = "DIS_NOME")
    @Size(min = 5, max = 20)
    private String nome;

    @NotNull
    @Column(name = "DIS_CRED_TEORICO")
    @Min(1L)
    @Max(99L)
    private Integer creditosTeorico;

    @NotNull
    @Column(name = "DIS_CRED_PRATICO")
    @Min(1L)
    @Max(99L)
    private Integer creditosPratico;

    @Column(name = "DIS_LABORATORIO")
    private Boolean laboratorio;

    @Enumerated
    private Dificuldades dificuldade;

    @NotNull
    @Column(name = "DIS_MAX_ALUN_TEORICO")
    @Min(1L)
    @Max(99L)
    private Integer maxAlunosTeorico;

    @NotNull
    @Column(name = "DIS_MAX_ALUN_PRATICO")
    @Min(1L)
    @Max(999L)
    private Integer maxAlunosPratico;

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "disciplinas")
    private Set<com.gapso.trieda.domain.HorarioDisponivelCenario> horarios = new java.util.HashSet<com.gapso.trieda.domain.HorarioDisponivelCenario>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "professor")
    private Set<com.gapso.trieda.domain.ProfessorDisciplina> professores = new java.util.HashSet<com.gapso.trieda.domain.ProfessorDisciplina>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina1")
    private Set<com.gapso.trieda.domain.Compatibilidade> compatibilidades = new java.util.HashSet<com.gapso.trieda.domain.Compatibilidade>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "cursou")
    private Set<com.gapso.trieda.domain.Equivalencia> equivalencias = new java.util.HashSet<com.gapso.trieda.domain.Equivalencia>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Sala> salas = new java.util.HashSet<com.gapso.trieda.domain.Sala>();

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "disciplinas")
    private Set<com.gapso.trieda.domain.GrupoSala> gruposSala = new java.util.HashSet<com.gapso.trieda.domain.GrupoSala>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "curriculo")
    private Set<com.gapso.trieda.domain.CurriculoDisciplina> curriculos = new java.util.HashSet<com.gapso.trieda.domain.CurriculoDisciplina>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
    private Set<com.gapso.trieda.domain.Demanda> demandas = new java.util.HashSet<com.gapso.trieda.domain.Demanda>();

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("TipoDisciplina: ").append(getTipoDisciplina()).append(", ");
        sb.append("DivisaoCreditos: ").append(getDivisaoCreditos()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("CreditosTeorico: ").append(getCreditosTeorico()).append(", ");
        sb.append("CreditosPratico: ").append(getCreditosPratico()).append(", ");
        sb.append("Laboratorio: ").append(getLaboratorio()).append(", ");
        sb.append("Dificuldade: ").append(getDificuldade()).append(", ");
        sb.append("MaxAlunosTeorico: ").append(getMaxAlunosTeorico()).append(", ");
        sb.append("MaxAlunosPratico: ").append(getMaxAlunosPratico()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        sb.append("Professores: ").append(getProfessores() == null ? "null" : getProfessores().size()).append(", ");
        sb.append("Compatibilidades: ").append(getCompatibilidades() == null ? "null" : getCompatibilidades().size()).append(", ");
        sb.append("Equivalencias: ").append(getEquivalencias() == null ? "null" : getEquivalencias().size()).append(", ");
        sb.append("Salas: ").append(getSalas() == null ? "null" : getSalas().size()).append(", ");
        sb.append("GruposSala: ").append(getGruposSala() == null ? "null" : getGruposSala().size()).append(", ");
        sb.append("Curriculos: ").append(getCurriculos() == null ? "null" : getCurriculos().size()).append(", ");
        sb.append("Demandas: ").append(getDemandas() == null ? "null" : getDemandas().size());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DIS_ID")
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
            Disciplina attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Disciplina merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Disciplina merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Disciplina().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countDisciplinas() {
        return ((Number) entityManager().createQuery("select count(o) from Disciplina o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Disciplina> findAllDisciplinas() {
        return entityManager().createQuery("select o from Disciplina o").getResultList();
    }

	public static Disciplina findDisciplina(Long id) {
        if (id == null) return null;
        return entityManager().find(Disciplina.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Disciplina> findDisciplinaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Disciplina o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = 7980821696468062987L;

	public Cenario getCenario() {
        return this.cenario;
    }

	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
    }

	public TipoDisciplina getTipoDisciplina() {
        return this.tipoDisciplina;
    }

	public void setTipoDisciplina(TipoDisciplina tipoDisciplina) {
        this.tipoDisciplina = tipoDisciplina;
    }

	public DivisaoCredito getDivisaoCreditos() {
        return this.divisaoCreditos;
    }

	public void setDivisaoCreditos(DivisaoCredito divisaoCreditos) {
        this.divisaoCreditos = divisaoCreditos;
    }

	public String getCodigo() {
        return this.codigo;
    }

	public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public Integer getCreditosTeorico() {
        return this.creditosTeorico;
    }

	public void setCreditosTeorico(Integer creditosTeorico) {
        this.creditosTeorico = creditosTeorico;
    }

	public Integer getCreditosPratico() {
        return this.creditosPratico;
    }

	public void setCreditosPratico(Integer creditosPratico) {
        this.creditosPratico = creditosPratico;
    }

	public Boolean getLaboratorio() {
        return this.laboratorio;
    }

	public void setLaboratorio(Boolean laboratorio) {
        this.laboratorio = laboratorio;
    }

	public Dificuldades getDificuldade() {
        return this.dificuldade;
    }

	public void setDificuldade(Dificuldades dificuldade) {
        this.dificuldade = dificuldade;
    }

	public Integer getMaxAlunosTeorico() {
        return this.maxAlunosTeorico;
    }

	public void setMaxAlunosTeorico(Integer maxAlunosTeorico) {
        this.maxAlunosTeorico = maxAlunosTeorico;
    }

	public Integer getMaxAlunosPratico() {
        return this.maxAlunosPratico;
    }

	public void setMaxAlunosPratico(Integer maxAlunosPratico) {
        this.maxAlunosPratico = maxAlunosPratico;
    }

	public Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }

	public Set<ProfessorDisciplina> getProfessores() {
        return this.professores;
    }

	public void setProfessores(Set<ProfessorDisciplina> professores) {
        this.professores = professores;
    }

	public Set<Compatibilidade> getCompatibilidades() {
        return this.compatibilidades;
    }

	public void setCompatibilidades(Set<Compatibilidade> compatibilidades) {
        this.compatibilidades = compatibilidades;
    }

	public Set<Equivalencia> getEquivalencias() {
        return this.equivalencias;
    }

	public void setEquivalencias(Set<Equivalencia> equivalencias) {
        this.equivalencias = equivalencias;
    }

	public Set<Sala> getSalas() {
        return this.salas;
    }

	public void setSalas(Set<Sala> salas) {
        this.salas = salas;
    }

	public Set<GrupoSala> getGruposSala() {
        return this.gruposSala;
    }

	public void setGruposSala(Set<GrupoSala> gruposSala) {
        this.gruposSala = gruposSala;
    }

	public Set<CurriculoDisciplina> getCurriculos() {
        return this.curriculos;
    }

	public void setCurriculos(Set<CurriculoDisciplina> curriculos) {
        this.curriculos = curriculos;
    }

	public Set<Demanda> getDemandas() {
        return this.demandas;
    }

	public void setDemandas(Set<Demanda> demandas) {
        this.demandas = demandas;
    }
}