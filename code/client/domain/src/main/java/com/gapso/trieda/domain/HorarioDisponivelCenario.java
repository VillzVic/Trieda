package com.gapso.trieda.domain;

import java.util.List;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "HDC_ID")
@Table(name = "HORARIO_DISPONIVEL_CENARIO")
public class HorarioDisponivelCenario implements java.io.Serializable {

    @NotNull
    @ManyToOne(targetEntity = HorarioAula.class)
    @JoinColumn(name = "HOR_ID")
    private HorarioAula horarioAula;

    @Enumerated
    private Semanas semana;

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Campus> campi = new java.util.HashSet<com.gapso.trieda.domain.Campus>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Unidade> unidades = new java.util.HashSet<com.gapso.trieda.domain.Unidade>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Sala> salas = new java.util.HashSet<com.gapso.trieda.domain.Sala>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Disciplina> disciplinas = new java.util.HashSet<com.gapso.trieda.domain.Disciplina>();

    @ManyToMany(cascade = CascadeType.ALL)
    private Set<com.gapso.trieda.domain.Professor> professores = new java.util.HashSet<com.gapso.trieda.domain.Professor>();

	private static final long serialVersionUID = -6325702360524336129L;

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "HDC_ID")
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
            HorarioDisponivelCenario attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public HorarioDisponivelCenario merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        HorarioDisponivelCenario merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager() {
        EntityManager em = new HorarioDisponivelCenario().entityManager;
        if (em == null) throw new IllegalStateException("Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
        return em;
    }

	public static long countHorarioDisponivelCenarios() {
        return ((Number) entityManager().createQuery("select count(o) from HorarioDisponivelCenario o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<HorarioDisponivelCenario> findAllHorarioDisponivelCenarios() {
        return entityManager().createQuery("select o from HorarioDisponivelCenario o").getResultList();
    }

	public static HorarioDisponivelCenario findHorarioDisponivelCenario(Long id) {
        if (id == null) return null;
        return entityManager().find(HorarioDisponivelCenario.class, id);
    }

	@SuppressWarnings("unchecked")
    public static List<HorarioDisponivelCenario> findHorarioDisponivelCenarioEntries(int firstResult, int maxResults) {
        return entityManager().createQuery("select o from HorarioDisponivelCenario o").setFirstResult(firstResult).setMaxResults(maxResults).getResultList();
    }

	public HorarioAula getHorarioAula() {
        return this.horarioAula;
    }

	public void setHorarioAula(HorarioAula horarioAula) {
        this.horarioAula = horarioAula;
    }

	public Semanas getSemana() {
        return this.semana;
    }

	public void setSemana(Semanas semana) {
        this.semana = semana;
    }

	public Set<Campus> getCampi() {
        return this.campi;
    }

	public void setCampi(Set<Campus> campi) {
        this.campi = campi;
    }

	public Set<Unidade> getUnidades() {
        return this.unidades;
    }

	public void setUnidades(Set<Unidade> unidades) {
        this.unidades = unidades;
    }

	public Set<Sala> getSalas() {
        return this.salas;
    }

	public void setSalas(Set<Sala> salas) {
        this.salas = salas;
    }

	public Set<Disciplina> getDisciplinas() {
        return this.disciplinas;
    }

	public void setDisciplinas(Set<Disciplina> disciplinas) {
        this.disciplinas = disciplinas;
    }

	public Set<Professor> getProfessores() {
        return this.professores;
    }

	public void setProfessores(Set<Professor> professores) {
        this.professores = professores;
    }

	public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Id: ").append(getId()).append(", ");
        sb.append("Version: ").append(getVersion()).append(", ");
        sb.append("HorarioAula: ").append(getHorarioAula()).append(", ");
        sb.append("Semana: ").append(getSemana()).append(", ");
        sb.append("Campi: ").append(getCampi() == null ? "null" : getCampi().size()).append(", ");
        sb.append("Unidades: ").append(getUnidades() == null ? "null" : getUnidades().size()).append(", ");
        sb.append("Salas: ").append(getSalas() == null ? "null" : getSalas().size()).append(", ");
        sb.append("Disciplinas: ").append(getDisciplinas() == null ? "null" : getDisciplinas().size()).append(", ");
        sb.append("Professores: ").append(getProfessores() == null ? "null" : getProfessores().size());
        return sb.toString();
    }
}
