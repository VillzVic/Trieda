package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashSet;
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
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

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
@RooEntity( identifierColumn = "TUR_ID" )
@Table( name = "TURMAS" )
public class Turma 
	implements Serializable, Clonable<Turma>
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
	@Size( min = 1, max = 255 )
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
	@JoinTable(name="TURMAS_ALUNOS_DEMANDA",
	joinColumns=@JoinColumn(name="TUR_ID"),
	inverseJoinColumns=@JoinColumn(name="ALD_ID"))
	private Set<AlunoDemanda> alunos = new HashSet<AlunoDemanda>();
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REMOVE})
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

	public Set<AlunoDemanda> getAlunos() {
		return alunos;
	}

	public void setAlunos(Set<AlunoDemanda> alunos) {
		this.alunos = alunos;
	}

	public Set<Aula> getAulas() {
		return aulas;
	}
	
	public Set<Aula> getAulas(Semanas semana) {
		Set<Aula> aulas = new HashSet<Aula>();
		for (Aula aula : getAulas())
		{
			if(aula.getHorarioDisponivelCenario().getDiaSemana().equals(semana))
			{
				aulas.add(aula);
			}
		}
		return aulas;
	}

	public void setAulas(Set<Aula> aulas) {
		this.aulas = aulas;
	}
	
	@Transactional
	public void detach()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.detach( this );
	}

	@Transactional
	public void persist()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.persist( this );
	}

	@Transactional
	public void remove()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		if ( this.entityManager.contains( this ) )
		{
			this.entityManager.remove( this );
		}
		else
		{
			Turma attached = this.entityManager.find(
				this.getClass(), this.id );

			if ( attached != null )
			{
				this.entityManager.remove( attached );
			}
		}
	}

	@Transactional
	public void flush()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		this.entityManager.flush();
	}

	@Transactional
	public Turma merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Turma merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Turma().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}
	
	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof InstituicaoEnsino ) )
		{
			return false;
		}

		Turma other = (Turma) obj;

		if ( id == null )
		{
			if ( other.id != null )
			{
				return false;
			}
		}
		else if ( !id.equals( other.id ) )
		{
			return false;
		}

		return true;
	}
	
	public static Turma find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Turma turma = entityManager().find( Turma.class, id );
		
		if (turma != null && turma.getCenario().getInstituicaoEnsino().equals(instituicaoEnsino))
		{
			return turma;
		}

		return null;
	}

	@SuppressWarnings("unchecked")
	public static List<Turma> findBy(InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Disciplina disciplina,
		String nome)
	{
		String nomeQuery = "";
		if (nome != null)
		{
			nomeQuery = " AND o.nome = :nome";
		}
		
		Query q = entityManager().createQuery(
				" SELECT o FROM Turma o " +
				" WHERE o.cenario.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				nomeQuery +
				" AND o.disciplina = :disciplina ");

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "disciplina", disciplina );
		if (nome != null)
		{
			q.setParameter( "nome", nome );
		}
		
		
		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<Turma> findByEquivalencia(InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Disciplina disciplina,
		Campus campus)
	{
		
		Query q = entityManager().createQuery(
				" SELECT DISTINCT (o) FROM Turma o, IN (o.alunos) alunos " +
				" WHERE o.cenario.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.cenario = :cenario " +
				" AND alunos.demanda.disciplina = :disciplina " +
				" AND alunos.demanda.oferta.campus = :campus ");

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "disciplina", disciplina );
		q.setParameter( "campus", campus );
		
		return q.getResultList();
	}

	public Turma clone(CenarioClone novoCenario) {
		Turma clone = new Turma();
		clone.setCenario(novoCenario.getCenario());
		clone.setDisciplina(novoCenario.getEntidadeClonada(this.getDisciplina()));
		clone.setNome(this.getNome());
		clone.setParcial(this.getParcial());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Turma entidadeClone) {
		for (AlunoDemanda alunoDemanda : this.getAlunos())
		{
			entidadeClone.getAlunos().add(novoCenario.getEntidadeClonada(alunoDemanda));
			
		}
	}
}
