package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
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
import javax.persistence.OneToMany;
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
public class HorarioDisponivelCenario implements Serializable {

    @NotNull
    @ManyToOne(targetEntity = HorarioAula.class)
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
		result = prime
				* result
				+ ((atendimentosOperacionais == null) ? 0
						: atendimentosOperacionais.hashCode());
		result = prime * result + ((campi == null) ? 0 : campi.hashCode());
		result = prime * result
				+ ((disciplinas == null) ? 0 : disciplinas.hashCode());
		result = prime * result
				+ ((fixacoes == null) ? 0 : fixacoes.hashCode());
		result = prime * result
				+ ((horarioAula == null) ? 0 : horarioAula.hashCode());
		result = prime * result + ((id == null) ? 0 : id.hashCode());
		result = prime * result
				+ ((professores == null) ? 0 : professores.hashCode());
		result = prime * result + ((salas == null) ? 0 : salas.hashCode());
		result = prime * result + ((semana == null) ? 0 : semana.hashCode());
		result = prime * result
				+ ((unidades == null) ? 0 : unidades.hashCode());
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
		if (atendimentosOperacionais == null) {
			if (other.atendimentosOperacionais != null)
				return false;
		} else if (!atendimentosOperacionais
				.equals(other.atendimentosOperacionais))
			return false;
		if (campi == null) {
			if (other.campi != null)
				return false;
		} else if (!campi.equals(other.campi))
			return false;
		if (disciplinas == null) {
			if (other.disciplinas != null)
				return false;
		} else if (!disciplinas.equals(other.disciplinas))
			return false;
		if (fixacoes == null) {
			if (other.fixacoes != null)
				return false;
		} else if (!fixacoes.equals(other.fixacoes))
			return false;
		if (horarioAula == null) {
			if (other.horarioAula != null)
				return false;
		} else if (!horarioAula.equals(other.horarioAula))
			return false;
		if (id == null) {
			if (other.id != null)
				return false;
		} else if (!id.equals(other.id))
			return false;
		if (professores == null) {
			if (other.professores != null)
				return false;
		} else if (!professores.equals(other.professores))
			return false;
		if (salas == null) {
			if (other.salas != null)
				return false;
		} else if (!salas.equals(other.salas))
			return false;
		if (semana != other.semana)
			return false;
		if (unidades == null) {
			if (other.unidades != null)
				return false;
		} else if (!unidades.equals(other.unidades))
			return false;
		if (version == null) {
			if (other.version != null)
				return false;
		} else if (!version.equals(other.version))
			return false;
		return true;
	}
	
	
}
