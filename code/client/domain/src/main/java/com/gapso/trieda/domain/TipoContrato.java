package com.gapso.trieda.domain;

import java.io.Serializable;
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
@RooEntity(identifierColumn = "TCO_ID")
@Table(name = "TIPOS_CONTRATO")
public class TipoContrato implements Serializable {

    @NotNull
    @Column(name = "TCO_NOME")
    @Size(min = 1, max = 50)
    private String nome;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TCO_ID")
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
            TipoContrato attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public TipoContrato merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        TipoContrato merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new TipoContrato().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static int countTipoContratoes() {
        return ((Number) entityManager().createQuery("SELECT COUNT(o) FROM TipoContrato o").getSingleResult()).intValue();
    }

	@SuppressWarnings("unchecked")
    public static List<TipoContrato> findAll() {
        return entityManager().createQuery("SELECT o FROM TipoContrato o").getResultList();
    }

	public static TipoContrato find(Long id) {
        if (id == null) return null;
        return entityManager().find(TipoContrato.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<TipoContrato> find(int firstResult, int maxResults) {
        return entityManager().createQuery("SELECT o FROM TipoContrato o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public static Map<String,TipoContrato> buildTipoContratoNomeToTipoContratoMap(List<TipoContrato> tiposContrato) {
		Map<String,TipoContrato> tiposContratoMap = new HashMap<String,TipoContrato>();
		for (TipoContrato tipoContrato : tiposContrato) {
			tiposContratoMap.put(tipoContrato.getNome(),tipoContrato);
		}
		return tiposContratoMap;
	}
	
	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Nome: ").append(getNome());
        return sb.toString();
    }

	private static final long serialVersionUID = -6475305286240446247L;

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }
}
