package com.gapso.trieda.domain;

import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "CDI_ID")
@Table(name = "CURRICULOS_DISCIPLINAS")
public class CurriculoDisciplina implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Curriculo.class)
    @JoinColumn(name = "CRC_ID")
    private Curriculo curriculo;

    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS_ID")
    private Disciplina disciplina;

    @NotNull
    @Column(name = "CDI_PERIODO")
    @Min(1L)
    @Max(99L)
    private Integer periodo;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Curriculo: ").append(getCurriculo()).append(", ");
        sb.append("Disciplina: ").append(getDisciplina()).append(", ");
        sb.append("Periodo: ").append(getPeriodo());
        return sb.toString();
    }

	public Curriculo getCurriculo() {
        return this.curriculo;
    }

	public void setCurriculo(Curriculo curriculo) {
        this.curriculo = curriculo;
    }

	public Disciplina getDisciplina() {
        return this.disciplina;
    }

	public void setDisciplina(Disciplina disciplina) {
        this.disciplina = disciplina;
    }

	public Integer getPeriodo() {
        return this.periodo;
    }

	public void setPeriodo(Integer periodo) {
        this.periodo = periodo;
    }

	private static final long serialVersionUID = -1212851224869372713L;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CDI_ID")
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
            CurriculoDisciplina attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public CurriculoDisciplina merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        CurriculoDisciplina merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new CurriculoDisciplina().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countCurriculoDisciplinas() {
        return ((Number) entityManager().createQuery("select count(o) from CurriculoDisciplina o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<CurriculoDisciplina> findAllCurriculoDisciplinas() {
        return entityManager().createQuery("select o from CurriculoDisciplina o").getResultList();
    }

	public static CurriculoDisciplina findCurriculoDisciplina(Long id) {
        if (id == null) return null;
        return entityManager().find(CurriculoDisciplina.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<CurriculoDisciplina> findCurriculoDisciplinaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from CurriculoDisciplina o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
