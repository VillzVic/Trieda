package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;
import javax.persistence.Entity;
import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.serializable.RooSerializable;
import org.springframework.transaction.annotation.Transactional;
import com.gapso.trieda.domain.Campus;
import javax.validation.constraints.NotNull;
import javax.persistence.Column;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.JoinColumn;
import javax.persistence.PersistenceContext;
import javax.persistence.Version;
import javax.validation.constraints.Min;
import javax.validation.constraints.Max;
import javax.validation.constraints.Digits;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity
@RooSerializable
public class Deslocamento implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Campus.class)
    @JoinColumn
    private Campus origem;

    @NotNull
    @ManyToOne(targetEntity = Campus.class)
    @JoinColumn
    private Campus destino;

    @Min(0L)
    @Max(1000L)
    @Digits(integer = 3, fraction = 2)
    private float horas;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "id")
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
            Deslocamento attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Deslocamento merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Deslocamento merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Deslocamento().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countDeslocamentoes() {
        return ((Number) entityManager().createQuery("select count(o) from Deslocamento o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Deslocamento> findAllDeslocamentoes() {
        return entityManager().createQuery("select o from Deslocamento o").getResultList();
    }

	public static Deslocamento findDeslocamento(Long id) {
        if (id == null) return null;
        return entityManager().find(Deslocamento.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Deslocamento> findDeslocamentoEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Deslocamento o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = -681388717725381478L;

	public Campus getOrigem() {
        return this.origem;
    }

	public void setOrigem(Campus origem) {
        this.origem = origem;
    }

	public Campus getDestino() {
        return this.destino;
    }

	public void setDestino(Campus destino) {
        this.destino = destino;
    }

	public float getHoras() {
        return this.horas;
    }

	public void setHoras(float horas) {
        this.horas = horas;
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Origem: ").append(getOrigem()).append(", ");
        sb.append("Destino: ").append(getDestino()).append(", ");
        sb.append("Horas: ").append(getHoras());
        return sb.toString();
    }
}
