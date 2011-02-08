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
@RooEntity(identifierColumn = "INC_ID")
@Table(name = "INCOMPATIBILIDADES")
public class Incompatibilidade implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS1_ID")
    private Disciplina disciplina1;

    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS2_ID")
    private Disciplina disciplina2;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "INC_ID")
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
            Incompatibilidade attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Incompatibilidade merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Incompatibilidade merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Incompatibilidade().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int count() {
        return ((Number) entityManager().createQuery("SELECT COUNT(o) FROM Incompatibilidade o").getSingleResult()).intValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Incompatibilidade> findAll() {
        return entityManager().createQuery("SELECT o FROM Incompatibilidade o").getResultList();
    }

	public static Incompatibilidade find(Long id) {
        if (id == null) return null;
        return entityManager().find(Incompatibilidade.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Incompatibilidade> find(int firstResult, int maxResults) {
        return entityManager().createQuery("SELECT o FROM Incompatibilidade o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Disciplina1: ").append(getDisciplina1()).append(", ");
        sb.append("Disciplina2: ").append(getDisciplina2());
        return sb.toString();
    }

	private static final long serialVersionUID = -6796413707742091455L;

	public Disciplina getDisciplina1() {
        return this.disciplina1;
    }

	public void setDisciplina1(Disciplina disciplina1) {
        this.disciplina1 = disciplina1;
    }

	public Disciplina getDisciplina2() {
        return this.disciplina2;
    }

	public void setDisciplina2(Disciplina disciplina2) {
        this.disciplina2 = disciplina2;
    }
}
