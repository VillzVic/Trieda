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

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "CAC_ID")
@Table(name = "CAMPI_CURRICULOS")
public class CampusCurriculo implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Curriculo.class)
    @JoinColumn(name = "CRC_ID")
    private Curriculo curriculo;

    @NotNull
    @ManyToOne(targetEntity = Campus.class)
    @JoinColumn(name = "CAM_ID")
    private Campus campus;

    @NotNull
    @ManyToOne(targetEntity = Turno.class)
    @JoinColumn(name = "TUR_ID")
    private Turno turno;

    @NotNull
    @OneToMany(cascade = CascadeType.ALL, mappedBy = "campusCurriculo")
    private Set<com.gapso.trieda.domain.Demanda> demandas = new java.util.HashSet<com.gapso.trieda.domain.Demanda>();

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CAC_ID")
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
            CampusCurriculo attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public CampusCurriculo merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        CampusCurriculo merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new CampusCurriculo().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countCampusCurriculoes() {
        return ((Number) entityManager().createQuery("select count(o) from CampusCurriculo o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<CampusCurriculo> findAllCampusCurriculoes() {
        return entityManager().createQuery("select o from CampusCurriculo o").getResultList();
    }

	public static CampusCurriculo findCampusCurriculo(Long id) {
        if (id == null) return null;
        return entityManager().find(CampusCurriculo.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<CampusCurriculo> findCampusCurriculoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from CampusCurriculo o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public Curriculo getCurriculo() {
        return this.curriculo;
    }

	public void setCurriculo(Curriculo curriculo) {
        this.curriculo = curriculo;
    }

	public Campus getCampus() {
        return this.campus;
    }

	public void setCampus(Campus campus) {
        this.campus = campus;
    }

	public Turno getTurno() {
        return this.turno;
    }

	public void setTurno(Turno turno) {
        this.turno = turno;
    }

	public Set<Demanda> getDemandas() {
        return this.demandas;
    }

	public void setDemandas(Set<Demanda> demandas) {
        this.demandas = demandas;
    }

	private static final long serialVersionUID = -976299446108675926L;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Curriculo: ").append(getCurriculo()).append(", ");
        sb.append("Campus: ").append(getCampus()).append(", ");
        sb.append("Turno: ").append(getTurno()).append(", ");
        sb.append("Demandas: ").append(getDemandas() == null ? "null" : getDemandas().size());
        return sb.toString();
    }
}