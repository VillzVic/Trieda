package com.gapso.trieda.domain;

import java.util.Date;
import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

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
@RooEntity(identifierColumn = "CEN_ID")
@Table(name = "CENARIOS")
public class Cenario implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USU_CRIACAO_ID")
    private Usuario criadoPor;

    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USU_ATUALIZACAO_ID")
    private Usuario atualizadoPor;

    @NotNull
    @Column(name = "CEN_NOME")
    @Size(min = 5, max = 20)
    private String nome;

    @Column(name = "CEN_ANO")
    @Min(1000L)
    @Max(1000L)
    private Integer ano;

    @Column(name = "CEN_PERIODO")
    @Min(1L)
    @Max(12L)
    private Integer periodo;

    @NotNull
    @Column(name = "CEN_DT_CRIACAO")
    @Temporal(TemporalType.TIMESTAMP)
    @DateTimeFormat(style = "S-")
    private Date dataCriacao;

    @NotNull
    @Column(name = "CEN_DT_ATUALIZACAO")
    @Temporal(TemporalType.TIMESTAMP)
    @DateTimeFormat(style = "S-")
    private Date dataAtualizacao;

    @Column(name = "CEN_COMENTARIO")
    private String comentario;

    @Column(name = "CEN_OFICIAL")
    private Boolean oficial;

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "cenario")
    private Set<com.gapso.trieda.domain.DivisaoCredito> divisoesCredito = new java.util.HashSet<com.gapso.trieda.domain.DivisaoCredito>();

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("CriadoPor: ").append(getCriadoPor()).append(", ");
        sb.append("AtualizadoPor: ").append(getAtualizadoPor()).append(", ");
        sb.append("Nome: ").append(getNome()).append(", ");
        sb.append("Ano: ").append(getAno()).append(", ");
        sb.append("Periodo: ").append(getPeriodo()).append(", ");
        sb.append("DataCriacao: ").append(getDataCriacao()).append(", ");
        sb.append("DataAtualizacao: ").append(getDataAtualizacao()).append(", ");
        sb.append("Comentario: ").append(getComentario()).append(", ");
        sb.append("Oficial: ").append(getOficial()).append(", ");
        sb.append("DivisoesCredito: ").append(getDivisoesCredito() == null ? "null" : getDivisoesCredito().size());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CEN_ID")
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

	public Usuario getCriadoPor() {
        return this.criadoPor;
    }

	public void setCriadoPor(Usuario criadoPor) {
        this.criadoPor = criadoPor;
    }

	public Usuario getAtualizadoPor() {
        return this.atualizadoPor;
    }

	public void setAtualizadoPor(Usuario atualizadoPor) {
        this.atualizadoPor = atualizadoPor;
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public Integer getAno() {
        return this.ano;
    }

	public void setAno(Integer ano) {
        this.ano = ano;
    }

	public Integer getPeriodo() {
        return this.periodo;
    }

	public void setPeriodo(Integer periodo) {
        this.periodo = periodo;
    }

	public Date getDataCriacao() {
        return this.dataCriacao;
    }

	public void setDataCriacao(Date dataCriacao) {
        this.dataCriacao = dataCriacao;
    }

	public Date getDataAtualizacao() {
        return this.dataAtualizacao;
    }

	public void setDataAtualizacao(Date dataAtualizacao) {
        this.dataAtualizacao = dataAtualizacao;
    }

	public String getComentario() {
        return this.comentario;
    }

	public void setComentario(String comentario) {
        this.comentario = comentario;
    }

	public Boolean getOficial() {
        return this.oficial;
    }

	public void setOficial(Boolean oficial) {
        this.oficial = oficial;
    }

	public Set<DivisaoCredito> getDivisoesCredito() {
        return this.divisoesCredito;
    }

	public void setDivisoesCredito(Set<DivisaoCredito> divisoesCredito) {
        this.divisoesCredito = divisoesCredito;
    }

	private static final long serialVersionUID = 3453500164175470562L;
}
