package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
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
public class HorarioDisponivelCenario implements Serializable {

    @NotNull
    @ManyToOne(targetEntity = HorarioAula.class, fetch=FetchType.LAZY)
    @JoinColumn(name = "HOR_ID")
    private HorarioAula horarioAula;

    @Enumerated
    private Semanas semana;

    @ManyToMany
    private Set<Campus> campi = new HashSet<Campus>();

    @ManyToMany
    private Set<Unidade> unidades = new HashSet<Unidade>();

    @ManyToMany
    private Set<Sala> salas = new HashSet<Sala>();

    @ManyToMany
    private Set<Disciplina> disciplinas = new HashSet<Disciplina>();

    @ManyToMany
    private Set<Professor> professores = new HashSet<Professor>();
    
    @ManyToMany
    private Set<Fixacao> fixacoes = new HashSet<Fixacao>();

    @OneToMany(mappedBy="HorarioDisponivelCenario")
    private Set<AtendimentoOperacional> atendimentosOperacionais =  new HashSet<AtendimentoOperacional>();
    
	private static final long serialVersionUID = 9128639869205918403L;

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
	public void detach() {
		if (this.entityManager == null) this.entityManager = entityManager();
		this.entityManager.detach(this);
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
        return ((Number) entityManager().createQuery("SELECT COUNT(o) FROM HorarioDisponivelCenario o").getSingleResult()).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List<HorarioDisponivelCenario> findAll() {
        return entityManager().createQuery("SELECT o FROM HorarioDisponivelCenario o").getResultList();
    }

	public static HorarioDisponivelCenario find(Long id) {
        if (id == null) return null;
        return entityManager().find(HorarioDisponivelCenario.class, id);
    }

    public static HorarioDisponivelCenario findBy(HorarioAula horarioAula, Semanas semana) {
		Query q = entityManager().createQuery("SELECT o FROM HorarioDisponivelCenario o WHERE o.horarioAula = :horarioAula AND o.semana = :semana");
		q.setParameter("horarioAula", horarioAula);
		q.setParameter("semana", semana);
		return (HorarioDisponivelCenario) q.getSingleResult();
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
	
	public Set<Fixacao> getFixacoes() {
		return this.fixacoes;
	}
	
	public void setFixacoes(Set<Fixacao> fixacoes) {
		this.fixacoes = fixacoes;
	}
	
	public Set<AtendimentoOperacional> getAtendimentosOperacionais() {
		return this.atendimentosOperacionais;
	}
	
	public void setAtendimentosOperacionais(Set<AtendimentoOperacional> atendimentosOperacionais) {
		this.atendimentosOperacionais = atendimentosOperacionais;
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
        sb.append("Fixacoes: ").append(getFixacoes() == null ? "null" : getFixacoes().size());
        sb.append("Atendimentos Operacionais: ").append(getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size());
        return sb.toString();
    }

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((id == null) ? 0 : id.hashCode());
		result = prime * result + ((version == null) ? 0 : version.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		HorarioDisponivelCenario other = (HorarioDisponivelCenario) obj;
		if (id == null) {
			if (other.id != null)
				return false;
		} else if (!id.equals(other.id))
			return false;
		if (version == null) {
			if (other.version != null)
				return false;
		} else if (!version.equals(other.version))
			return false;
		return true;
	}
	
}
