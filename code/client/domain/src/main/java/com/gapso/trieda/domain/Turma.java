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
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "TUR_ID" )
@Table( name = "TURMAS" )
public class Turma 
	implements Serializable
{

	private static final long serialVersionUID = 2635105670922575311L;

	@PersistenceContext
	transient EntityManager entityManager;
	
	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "TUR_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;
	
	@NotNull
	@Column( name = "TUR_NOME" )
	@Size( min = 3, max = 255 )
	private String nome;
	
	@Column( name = "TUR_PARCIAL" )
	private Boolean parcial;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Disciplina.class )
	@JoinColumn( name = "DIS_ID" )
	private Disciplina disciplina;
	
	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE})
	@JoinTable(name="TURMAS_ALUNOS",
	joinColumns=@JoinColumn(name="TUR_ID"),
	inverseJoinColumns=@JoinColumn(name="ALN_ID"))
	private Set<Aluno> alunos = new HashSet<Aluno>();
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE})
	@JoinTable(name="AULAS_TURMAS",
	joinColumns=@JoinColumn(name="TUR_ID"),
	inverseJoinColumns=@JoinColumn(name="AUL_ID"))
	private Set<Aula> aulas = new HashSet<Aula>();

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

	public String getNome() {
		return nome;
	}

	public void setNome(String nome) {
		this.nome = nome;
	}

	public Boolean getParcial() {
		return parcial;
	}

	public void setParcial(Boolean parcial) {
		this.parcial = parcial;
	}

	public Disciplina getDisciplina() {
		return disciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.disciplina = disciplina;
	}

	public Cenario getCenario() {
		return cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public Set<Aluno> getAlunos() {
		return alunos;
	}

	public void setAlunos(Set<Aluno> alunos) {
		this.alunos = alunos;
	}

	public Set<Aula> getAulas() {
		return aulas;
	}

	public void setAulas(Set<Aula> aulas) {
		this.aulas = aulas;
	}
}
