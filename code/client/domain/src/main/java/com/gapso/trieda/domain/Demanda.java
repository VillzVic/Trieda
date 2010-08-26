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
@RooEntity(identifierColumn = "DEM_ID")
@Table(name = "DEMANDAS")
public class Demanda implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = CampusCurriculo.class)
    @JoinColumn(name = "CAC_ID")
    private CampusCurriculo campusCurriculo;

    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS_ID")
    private Disciplina disciplina;

    @NotNull
    @Column(name = "DEM_QUANTIDADE")
    @Min(1L)
    @Max(999L)
    private Integer quantidade;

	public CampusCurriculo getCampusCurriculo() {
        return this.campusCurriculo;
    }

	public void setCampusCurriculo(CampusCurriculo campusCurriculo) {
        this.campusCurriculo = campusCurriculo;
    }

	public Disciplina getDisciplina() {
        return this.disciplina;
    }

	public void setDisciplina(Disciplina disciplina) {
        this.disciplina = disciplina;
    }

	public Integer getQuantidade() {
        return this.quantidade;
    }

	public void setQuantidade(Integer quantidade) {
        this.quantidade = quantidade;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DEM_ID")
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
            Demanda attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Demanda merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Demanda merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Demanda().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countDemandas() {
        return ((Number) entityManager().createQuery("select count(o) from Demanda o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Demanda> findAllDemandas() {
        return entityManager().createQuery("select o from Demanda o").getResultList();
    }

	public static Demanda findDemanda(Long id) {
        if (id == null) return null;
        return entityManager().find(Demanda.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Demanda> findDemandaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Demanda o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("CampusCurriculo: ").append(getCampusCurriculo()).append(", ");
        sb.append("Disciplina: ").append(getDisciplina()).append(", ");
        sb.append("Quantidade: ").append(getQuantidade());
        return sb.toString();
    }

	private static final long serialVersionUID = -3935898184270072639L;
}
