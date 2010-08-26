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
@RooEntity(identifierColumn = "TDI_ID")
@Table(name = "TIPOS_DISCIPLINA")
public class TipoDisciplina implements java.io.Serializable {

    @NotNull
    @Column(name = "TDI_NOME")
    @Size(min = 3, max = 255)
    private String nome;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Nome: ").append(getNome());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TDI_ID")
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
            TipoDisciplina attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public TipoDisciplina merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        TipoDisciplina merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new TipoDisciplina().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countTipoDisciplinas() {
        return ((Number) entityManager().createQuery("select count(o) from TipoDisciplina o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<TipoDisciplina> findAllTipoDisciplinas() {
        return entityManager().createQuery("select o from TipoDisciplina o").getResultList();
    }

	public static TipoDisciplina findTipoDisciplina(Long id) {
        if (id == null) return null;
        return entityManager().find(TipoDisciplina.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<TipoDisciplina> findTipoDisciplinaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from TipoDisciplina o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	private static final long serialVersionUID = 3145587865770084666L;
}
