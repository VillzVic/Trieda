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
@RooEntity(identifierColumn = "TIT_ID")
@Table(name = "TITULACOES")
public class Titulacao implements java.io.Serializable {

    @NotNull
    @Column(name = "TIT_NOME")
    @Size(min = 3, max = 20)
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
    @Column(name = "TIT_ID")
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
            Titulacao attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Titulacao merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Titulacao merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Titulacao().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countTitulacaos() {
        return ((Number) entityManager().createQuery("select count(o) from Titulacao o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Titulacao> findAllTitulacaos() {
        return entityManager().createQuery("select o from Titulacao o").getResultList();
    }

	public static Titulacao findTitulacao(Long id) {
        if (id == null) return null;
        return entityManager().find(Titulacao.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Titulacao> findTitulacaoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Titulacao o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = -4640366191671276965L;

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }
}
