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
@RooEntity(identifierColumn = "CRC_ID")
@Table(name = "CURRICULOS")
public class Curriculo implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Curso.class)
    @JoinColumn(name = "CUR_ID")
    private Curso curso;

    @NotNull
    @ManyToOne(targetEntity = Cenario.class)
    @JoinColumn(name = "CEN_ID")
    private Cenario cenario;

    @NotNull
    @Column(name = "CRC_COD")
    @Size(min = 3, max = 20)
    private String codigo;

    @Column(name = "CRC_DESCRICAO")
    @Size(max = 255)
    private String descricao;

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "disciplina")
    private Set<com.gapso.trieda.domain.CurriculoDisciplina> disciplinas = new java.util.HashSet<com.gapso.trieda.domain.CurriculoDisciplina>();

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "curriculo")
    private Set<com.gapso.trieda.domain.CampusCurriculo> campusCurriculo = new java.util.HashSet<com.gapso.trieda.domain.CampusCurriculo>();

	public Curso getCurso() {
        return this.curso;
    }

	public void setCurso(Curso curso) {
        this.curso = curso;
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

	public String getDescricao() {
        return this.descricao;
    }

	public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

	public Set<CurriculoDisciplina> getDisciplinas() {
        return this.disciplinas;
    }

	public void setDisciplinas(Set<CurriculoDisciplina> disciplinas) {
        this.disciplinas = disciplinas;
    }

	public Set<CampusCurriculo> getCampusCurriculo() {
        return this.campusCurriculo;
    }

	public void setCampusCurriculo(Set<CampusCurriculo> campusCurriculo) {
        this.campusCurriculo = campusCurriculo;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CRC_ID")
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
            Curriculo attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Curriculo merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Curriculo merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Curriculo().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countCurriculoes() {
        return ((Number) entityManager().createQuery("select count(o) from Curriculo o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Curriculo> findAllCurriculoes() {
        return entityManager().createQuery("select o from Curriculo o").getResultList();
    }

	public static Curriculo findCurriculo(Long id) {
        if (id == null) return null;
        return entityManager().find(Curriculo.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Curriculo> findCurriculoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Curriculo o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Curso: ").append(getCurso()).append(", ");
        sb.append("Cenario: ").append(getCenario()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Descricao: ").append(getDescricao()).append(", ");
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size()).append(", ");
        sb.append("CampusCurriculo: ").append(getCampusCurriculo() == null ? "null" : getCampusCurriculo().size());
        return sb.toString();
    }

	private static final long serialVersionUID = -9204016994046445376L;
}
