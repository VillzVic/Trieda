package com.gapso.trieda.domain;

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

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "CUR_ID")
@Table(name = "CURSOS")
public class Curso implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = TipoCurso.class)
    @JoinColumn(name = "TCU_ID")
    private TipoCurso tipoCurso;

    @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @Column(name = "CUR_CODIGO")
    @Size(min = 3, max = 255)
    private String codigo;

    @NotNull
    @Column(name = "CUR_MIN_DOUTORES")
    @Min(0L)
    @Max(99L)
    private Integer numMinDoutores;

    @NotNull
    @Column(name = "CUR_MIN_MESTRES")
    @Min(0L)
    @Max(99L)
    private Integer numMinMestres;

    @NotNull
    @Column(name = "CUR_MAX_DISC_PROF")
    @Min(1L)
    @Max(99L)
    private Integer maxDisciplinasPeloProfessor;

    @Column(name = "CUR_ADM_MAIS_DE_UMA_DISC")
    private Boolean admMaisDeUmDisciplina;

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "cursos")
    private Set<com.gapso.trieda.domain.AreaTitulacao> areasTitulacao = new java.util.HashSet<com.gapso.trieda.domain.AreaTitulacao>();

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("TipoCurso: ").append(getTipoCurso()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("NumMinDoutores: ").append(getNumMinDoutores()).append(", ");
        sb.append("NumMinMestres: ").append(getNumMinMestres()).append(", ");
        sb.append("MaxDisciplinasPeloProfessor: ").append(getMaxDisciplinasPeloProfessor()).append(", ");
        sb.append("AdmMaisDeUmDisciplina: ").append(getAdmMaisDeUmDisciplina()).append(", ");
        sb.append("AreasTitulacao: ").append(getAreasTitulacao() == null ? "null" : getAreasTitulacao().size());
        return sb.toString();
    }

	public TipoCurso getTipoCurso() {
        return this.tipoCurso;
    }

	public void setTipoCurso(TipoCurso tipoCurso) {
        this.tipoCurso = tipoCurso;
    }

	public Cenario getCenario() {
        return this.cenario;
    }

	public void setCenario(Cenario cenario) {
        this.cenario = cenario;
    }

	public String getCodigo() {
        return this.codigo;
    }

	public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

	public Integer getNumMinDoutores() {
        return this.numMinDoutores;
    }

	public void setNumMinDoutores(Integer numMinDoutores) {
        this.numMinDoutores = numMinDoutores;
    }

	public Integer getNumMinMestres() {
        return this.numMinMestres;
    }

	public void setNumMinMestres(Integer numMinMestres) {
        this.numMinMestres = numMinMestres;
    }

	public Integer getMaxDisciplinasPeloProfessor() {
        return this.maxDisciplinasPeloProfessor;
    }

	public void setMaxDisciplinasPeloProfessor(Integer maxDisciplinasPeloProfessor) {
        this.maxDisciplinasPeloProfessor = maxDisciplinasPeloProfessor;
    }

	public Boolean getAdmMaisDeUmDisciplina() {
        return this.admMaisDeUmDisciplina;
    }

	public void setAdmMaisDeUmDisciplina(Boolean admMaisDeUmDisciplina) {
        this.admMaisDeUmDisciplina = admMaisDeUmDisciplina;
    }

	public Set<AreaTitulacao> getAreasTitulacao() {
        return this.areasTitulacao;
    }

	public void setAreasTitulacao(Set<AreaTitulacao> areasTitulacao) {
        this.areasTitulacao = areasTitulacao;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CUR_ID")
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
            Curso attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Curso merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Curso merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Curso().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countCursoes() {
        return ((Number) entityManager().createQuery("select count(o) from Curso o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Curso> findAllCursoes() {
        return entityManager().createQuery("select o from Curso o").getResultList();
    }

	public static Curso findCurso(Long id) {
        if (id == null) return null;
        return entityManager().find(Curso.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Curso> findCursoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Curso o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = 2645879541329424105L;
}