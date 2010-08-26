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
@RooEntity(identifierColumn = "EQV_ID")
@Table(name = "EQUIVALENCIAS")
public class Equivalencia implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS_CURSOU_ID")
    private Disciplina cursou;

    @NotNull
    @ManyToOne(targetEntity = Disciplina.class)
    @JoinColumn(name = "DIS_ELIMINA_ID")
    private Disciplina elimina;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Cursou: ").append(getCursou()).append(", ");
        sb.append("Elimina: ").append(getElimina());
        return sb.toString();
    }

	public Disciplina getCursou() {
        return this.cursou;
    }

	public void setCursou(Disciplina cursou) {
        this.cursou = cursou;
    }

	public Disciplina getElimina() {
        return this.elimina;
    }

	public void setElimina(Disciplina elimina) {
        this.elimina = elimina;
    }

	private static final long serialVersionUID = -8632323368932009356L;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "EQV_ID")
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
            Equivalencia attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Equivalencia merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Equivalencia merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Equivalencia().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countEquivalencias() {
        return ((Number) entityManager().createQuery("select count(o) from Equivalencia o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Equivalencia> findAllEquivalencias() {
        return entityManager().createQuery("select o from Equivalencia o").getResultList();
    }

	public static Equivalencia findEquivalencia(Long id) {
        if (id == null) return null;
        return entityManager().find(Equivalencia.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Equivalencia> findEquivalenciaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Equivalencia o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
