package com.gapso.trieda.domain;

import java.util.Date;
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
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "SES_ID")
@Table(name = "SESSOES")
public class Sessao implements java.io.Serializable {

    @NotNull
    @Column(name = "SES_HORARIO")
    @Temporal(TemporalType.TIMESTAMP)
    @DateTimeFormat(style = "S-")
    private Date horario;

    @NotNull
    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USU_ID")
    private Usuario usuario;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Horario: ").append(getHorario()).append(", ");
        sb.append("Usuario: ").append(getUsuario());
        return sb.toString();
    }

	public Date getHorario() {
        return this.horario;
    }

	public void setHorario(Date horario) {
        this.horario = horario;
    }

	public Usuario getUsuario() {
        return this.usuario;
    }

	public void setUsuario(Usuario usuario) {
        this.usuario = usuario;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "SES_ID")
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
            Sessao attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Sessao merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Sessao merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Sessao().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countSessaos() {
        return ((Number) entityManager().createQuery("select count(o) from Sessao o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Sessao> findAllSessaos() {
        return entityManager().createQuery("select o from Sessao o").getResultList();
    }

	public static Sessao findSessao(Long id) {
        if (id == null) return null;
        return entityManager().find(Sessao.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Sessao> findSessaoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Sessao o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = -1164221895650416996L;
}
