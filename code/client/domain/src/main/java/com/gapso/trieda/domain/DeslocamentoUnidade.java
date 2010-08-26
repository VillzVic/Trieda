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
@RooEntity(identifierColumn = "DEU_ID")
@Table(name = "DESLOCAMENTOS_UNIDADES")
public class DeslocamentoUnidade implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ORIG_ID")
    private Unidade origem;

    @NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_DEST_ID")
    private Unidade destino;

    @NotNull
    @Column(name = "DEC_TEMPO")
    @Min(1L)
    @Max(999L)
    private Integer tempo;

	public Unidade getOrigem() {
        return this.origem;
    }

	public void setOrigem(Unidade origem) {
        this.origem = origem;
    }

	public Unidade getDestino() {
        return this.destino;
    }

	public void setDestino(Unidade destino) {
        this.destino = destino;
    }

	public Integer getTempo() {
        return this.tempo;
    }

	public void setTempo(Integer tempo) {
        this.tempo = tempo;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DEU_ID")
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
            DeslocamentoUnidade attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public DeslocamentoUnidade merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        DeslocamentoUnidade merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new DeslocamentoUnidade().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countDeslocamentoUnidades() {
        return ((Number) entityManager().createQuery("select count(o) from DeslocamentoUnidade o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<DeslocamentoUnidade> findAllDeslocamentoUnidades() {
        return entityManager().createQuery("select o from DeslocamentoUnidade o").getResultList();
    }

	public static DeslocamentoUnidade findDeslocamentoUnidade(Long id) {
        if (id == null) return null;
        return entityManager().find(DeslocamentoUnidade.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<DeslocamentoUnidade> findDeslocamentoUnidadeEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from DeslocamentoUnidade o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	private static final long serialVersionUID = -7515490800522918430L;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Origem: ").append(getOrigem()).append(", ");
        sb.append("Destino: ").append(getDestino()).append(", ");
        sb.append("Tempo: ").append(getTempo());
        return sb.toString();
    }
}
