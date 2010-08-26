package com.gapso.trieda.domain;

import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
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
@RooEntity(identifierColumn = "TCU_ID")
@Table(name = "TIPOS_CURSO")
public class TipoCurso implements java.io.Serializable {

    @NotNull
    @Column(name = "TCU_NOME")
    @Size(min = 3, max = 255)
    private String nome;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Nome: ").append(getNome());
        return sb.toString();
    }

	private static final long serialVersionUID = 8519531461330290008L;

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TCU_ID")
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
            TipoCurso attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public TipoCurso merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        TipoCurso merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new TipoCurso().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countTipoCursoes() {
        return ((Number) entityManager().createQuery("select count(o) from TipoCurso o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<TipoCurso> findAllTipoCursoes() {
        return entityManager().createQuery("select o from TipoCurso o").getResultList();
    }

	public static TipoCurso findTipoCurso(Long id) {
        if (id == null) return null;
        return entityManager().find(TipoCurso.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<TipoCurso> findTipoCursoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from TipoCurso o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
