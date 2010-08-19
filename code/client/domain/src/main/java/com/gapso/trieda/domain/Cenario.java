package com.gapso.trieda.domain;

import javax.persistence.Entity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.serializable.RooSerializable;
import org.springframework.transaction.annotation.Transactional;
import javax.validation.constraints.Size;
import javax.validation.constraints.Min;
import javax.validation.constraints.Max;
import java.io.Serializable;
import java.util.Date;
import java.util.List;
import javax.validation.constraints.NotNull;
import javax.persistence.Column;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.PersistenceContext;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.format.annotation.DateTimeFormat;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity
@RooSerializable
public class Cenario implements java.io.Serializable {

    @Size(max = 255)
    private String codigo;

    @Size(max = 255)
    private String descricao;

    @Min(2000L)
    @Max(3000L)
    private int ano;

    @Min(1L)
    @Max(12L)
    private int periodo;

    @NotNull
    @Temporal(TemporalType.DATE)
    @DateTimeFormat(style = "S-")
    private Date criacao;

    @NotNull
    @Temporal(TemporalType.DATE)
    @DateTimeFormat(style = "S-")
    private Date atualizacao;

    @Size(max = 1024)
    private String comentario;

    private boolean oficial;

	private static final long serialVersionUID = -5852010979062684997L;

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Descricao: ").append(getDescricao()).append(", ");
        sb.append("Ano: ").append(getAno()).append(", ");
        sb.append("Periodo: ").append(getPeriodo()).append(", ");
        sb.append("Criacao: ").append(getCriacao()).append(", ");
        sb.append("Atualizacao: ").append(getAtualizacao()).append(", ");
        sb.append("Comentario: ").append(getComentario()).append(", ");
        sb.append("Oficial: ").append(isOficial());
        return sb.toString();
    }

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
            Cenario attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Cenario merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Cenario merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Cenario().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countCenarios() {
        return ((Number) entityManager().createQuery("select count(o) from Cenario o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Cenario> findAllCenarios() {
        return entityManager().createQuery("select o from Cenario o").getResultList();
    }

	public static Cenario findCenario(Long id) {
        if (id == null) return null;
        return entityManager().find(Cenario.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Cenario> findCenarioEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Cenario o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public String getCodigo() {
        return this.codigo;
    }

	public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

	public String getDescricao() {
        return this.descricao;
    }

	public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

	public int getAno() {
        return this.ano;
    }

	public void setAno(int ano) {
        this.ano = ano;
    }

	public int getPeriodo() {
        return this.periodo;
    }

	public void setPeriodo(int periodo) {
        this.periodo = periodo;
    }

	public Date getCriacao() {
        return this.criacao;
    }

	public void setCriacao(Date criacao) {
        this.criacao = criacao;
    }

	public Date getAtualizacao() {
        return this.atualizacao;
    }

	public void setAtualizacao(Date atualizacao) {
        this.atualizacao = atualizacao;
    }

	public String getComentario() {
        return this.comentario;
    }

	public void setComentario(String comentario) {
        this.comentario = comentario;
    }

	public boolean isOficial() {
        return this.oficial;
    }

	public void setOficial(boolean oficial) {
        this.oficial = oficial;
    }
}
