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
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
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
public class CurriculoDisciplina implements Serializable {

    @NotNull
    @ManyToOne(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH}, targetEntity = Curriculo.class)
    @JoinColumn(name = "CRC_ID")
    private Curriculo curriculo;

    @NotNull
    @ManyToOne(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH}, targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS_ID")
    private Disciplina disciplina;

    @NotNull
    @Column(name = "CDI_PERIODO")
    @Min(1L)
    @Max(99L)
    private Integer periodo;

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH})
    private Set<Sala> salas = new HashSet<Sala>();
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH}, mappedBy = "curriculoDisciplinas")
    private Set<GrupoSala> gruposSala = new HashSet<GrupoSala>();
    
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Curriculo: ").append(getCurriculo()).append(", ");
        sb.append("Disciplina: ").append(getDisciplina()).append(", ");
        sb.append("Periodo: ").append(getPeriodo());
        sb.append("Salas: ").append(getSalas() == null ? "null" : getSalas().size()).append(", ");
        sb.append("GruposSala: ").append(getGruposSala() == null ? "null" : getGruposSala().size()).append(", ");
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
	
	private static final long serialVersionUID = -5429743673577487971L;

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
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
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

	public static int count() {
        return ((Number) entityManager().createQuery("SELECT COUNT(o) FROM CurriculoDisciplina o").getSingleResult()).intValue();
    }
	
	@SuppressWarnings("unchecked")
	public static List<CurriculoDisciplina> findAllByCurriculoAndPeriodo(Curriculo curriculo, Integer periodo) {
		Query q = entityManager().createQuery("SELECT o FROM CurriculoDisciplina o WHERE o.curriculo = :curriculo AND o.periodo = :periodo");
		q.setParameter("curriculo", curriculo);
		q.setParameter("periodo", periodo);
		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
    public static List<CurriculoDisciplina> findAll() {
        return entityManager().createQuery("SELECT o FROM CurriculoDisciplina o").getResultList();
    }

	public static CurriculoDisciplina find(Long id) {
        if (id == null) return null;
        return entityManager().find(CurriculoDisciplina.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<CurriculoDisciplina> find(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from CurriculoDisciplina o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
