package com.gapso.trieda.domain;

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
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
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
@RooEntity(identifierColumn = "SAL_ID")
@Table(name = "SALAS")
public class Sala implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = TipoSala.class)
    @JoinColumn(name = "TSA_ID")
    private TipoSala tipoSala;

    @NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ID")
    private Unidade unidade;

    @NotNull
    @Column(name = "SAL_CODIGO")
    @Size(min = 3, max = 20)
    private String codigo;

    @NotNull
    @Column(name = "SAL_NUMERO")
    @Size(min = 1, max = 20)
    private String numero;

    @NotNull
    @Column(name = "SAL_ANDAR")
    @Size(min = 1, max = 20)
    private String andar;

    @NotNull
    @Column(name = "SAL_CAPACIDADE")
    @Min(1L)
    @Max(9999L)
    private Integer capacidade;

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "salas")
    private Set<com.gapso.trieda.domain.HorarioDisponivelCenario> horarios = new java.util.HashSet<com.gapso.trieda.domain.HorarioDisponivelCenario>();

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "salas")
    private Set<com.gapso.trieda.domain.Disciplina> disciplinas = new java.util.HashSet<com.gapso.trieda.domain.Disciplina>();

    @ManyToMany(cascade = CascadeType.ALL, mappedBy = "salas")
    private Set<com.gapso.trieda.domain.GrupoSala> gruposSala = new java.util.HashSet<com.gapso.trieda.domain.GrupoSala>();

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("TipoSala: ").append(getTipoSala()).append(", ");
        sb.append("Unidade: ").append(getUnidade()).append(", ");
        sb.append("Codigo: ").append(getCodigo()).append(", ");
        sb.append("Numero: ").append(getNumero()).append(", ");
        sb.append("Andar: ").append(getAndar()).append(", ");
        sb.append("Capacidade: ").append(getCapacidade()).append(", ");
        sb.append("Horarios: ").append(getHorarios() == null ? "null" : getHorarios().size()).append(", ");
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size()).append(", ");
        sb.append("GruposSala: ").append(getGruposSala() == null ? "null" : getGruposSala().size());
        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "SAL_ID")
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
            Sala attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public Sala merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        Sala merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new Sala().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countSalas() {
        return ((Number) entityManager().createQuery("select count(o) from Sala o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<Sala> findAllSalas() {
        return entityManager().createQuery("select o from Sala o").getResultList();
    }

	public static Sala findSala(Long id) {
        if (id == null) return null;
        return entityManager().find(Sala.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<Sala> findSalaEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from Sala o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public TipoSala getTipoSala() {
        return this.tipoSala;
    }

	public void setTipoSala(TipoSala tipoSala) {
        this.tipoSala = tipoSala;
    }

	public Unidade getUnidade() {
        return this.unidade;
    }

	public void setUnidade(Unidade unidade) {
        this.unidade = unidade;
    }

	public String getCodigo() {
        return this.codigo;
    }

	public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

	public String getNumero() {
        return this.numero;
    }

	public void setNumero(String numero) {
        this.numero = numero;
    }

	public String getAndar() {
        return this.andar;
    }

	public void setAndar(String andar) {
        this.andar = andar;
    }

	public Integer getCapacidade() {
        return this.capacidade;
    }

	public void setCapacidade(Integer capacidade) {
        this.capacidade = capacidade;
    }

	public Set<HorarioDisponivelCenario> getHorarios() {
        return this.horarios;
    }

	public void setHorarios(Set<HorarioDisponivelCenario> horarios) {
        this.horarios = horarios;
    }

	public Set<Disciplina> getDisciplinas() {
        return this.disciplinas;
    }

	public void setDisciplinas(Set<Disciplina> disciplinas) {
        this.disciplinas = disciplinas;
    }

	public Set<GrupoSala> getGruposSala() {
        return this.gruposSala;
    }

	public void setGruposSala(Set<GrupoSala> gruposSala) {
        this.gruposSala = gruposSala;
    }

	private static final long serialVersionUID = -2533999449644229682L;
}
