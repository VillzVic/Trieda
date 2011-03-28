package com.gapso.trieda.domain;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
@RooEntity(identifierColumn = "TSA_ID")
@Table(name = "TIPOS_SALA")
public class TipoSala implements java.io.Serializable {

    @NotNull
    @Column(name = "TSA_NOME")
    @Size(min = 1, max = 255)
    private String nome;

    @Column(name = "TSA_DESCRICAO")
    @Size(max = 255)
    private String descricao;

	private static final long serialVersionUID = -1633461518380764117L;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Descricao: ").append(getDescricao());
        return sb.toString();
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public String getDescricao() {
        return this.descricao;
    }

	public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TSA_ID")
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
            TipoSala attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public TipoSala merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        TipoSala merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new TipoSala().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long count() {
        return ((Number) entityManager().createQuery("select count(o) from TipoSala o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<TipoSala> findAll() {
        return entityManager().createQuery("select o from TipoSala o").getResultList();
    }
	
	public static Map<String,TipoSala> buildTipoSalaNomeToTipoSalaMap(List<TipoSala> tiposSala) {
		Map<String,TipoSala> tiposSalaMap = new HashMap<String,TipoSala>();
		for (TipoSala tipoSala : tiposSala) {
			tiposSalaMap.put(tipoSala.getNome(),tipoSala);
		}
		return tiposSalaMap;
	}

	public static TipoSala find(Long id) {
        if (id == null) return null;
        return entityManager().find(TipoSala.class, id);
    }

	public static List<TipoSala> find(int firstResult, int maxResults) {
		return find(firstResult, maxResults, null);
	}
	@SuppressWarnings("unchecked")
    public static List<TipoSala> find(int firstResult, int maxResults, String orderBy) {
		orderBy = (orderBy != null)? "ORDER BY o."+orderBy : "";
        return entityManager().createQuery("select o from TipoSala o "+orderBy).setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }
}
