package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
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
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "AUL_ID" )
@Table( name = "AULAS" )
public class Aula
	implements Serializable
{
	private static final long serialVersionUID = -8810399104404941276L;

	@PersistenceContext
	transient EntityManager entityManager;
	
	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "MOT_PRV_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;
	
	@Column( name = "AUL_QTD_CRED_TEORICOS" )
	private Integer creditosTeoricos;
	
	@Column( name = "AUL_QTD_CRED_PRATICOS" )
	private Integer parcial;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = HorarioDisponivelCenario.class )
	@JoinColumn( name = "HDC_ID" )
	private HorarioDisponivelCenario horarioDisponivelCenario;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Sala.class )
	@JoinColumn( name = "SAL_ID" )
	private Sala sala;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Professor.class )
	@JoinColumn( name = "PRF_ID" )
	private Professor professor;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = ProfessorVirtual.class )
	@JoinColumn( name = "PRV_ID" )
	private ProfessorVirtual professorVirtual;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;
	
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
    private Set<Turma> turmas = new HashSet<Turma>();

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Integer getVersion() {
		return version;
	}

	public void setVersion(Integer version) {
		this.version = version;
	}

	public Integer getCreditosTeoricos() {
		return creditosTeoricos;
	}

	public void setCreditosTeoricos(Integer creditosTeoricos) {
		this.creditosTeoricos = creditosTeoricos;
	}

	public Integer getParcial() {
		return parcial;
	}

	public void setParcial(Integer parcial) {
		this.parcial = parcial;
	}

	public HorarioDisponivelCenario getHorarioDisponivelCenario() {
		return horarioDisponivelCenario;
	}

	public void setHorarioDisponivelCenario(HorarioDisponivelCenario horarioDisponivelCenario) {
		this.horarioDisponivelCenario = horarioDisponivelCenario;
	}

	public Sala getSala() {
		return sala;
	}

	public void setSala(Sala sala) {
		this.sala = sala;
	}

	public Professor getProfessor() {
		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	public ProfessorVirtual getProfessorVirtual() {
		return professorVirtual;
	}

	public void setProfessorVirtual(ProfessorVirtual professorVirtual) {
		this.professorVirtual = professorVirtual;
	}

	public Cenario getCenario() {
		return cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public Set<Turma> getTurmas() {
		return turmas;
	}

	public void setTurmas(Set<Turma> turmas) {
		this.turmas = turmas;
	}
}
