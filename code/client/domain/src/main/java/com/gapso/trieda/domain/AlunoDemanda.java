package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
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
@RooEntity( identifierColumn = "ALD_ID" )
@Table( name = "ALUNOS_DEMANDA", uniqueConstraints =
@UniqueConstraint( columnNames = { "ALN_ID", "DEM_ID" } ) )
public class AlunoDemanda
	implements Serializable, Comparable< AlunoDemanda >
{
	private static final long serialVersionUID = 5574796519360717359L;

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "ALD_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Aluno.class, fetch = FetchType.LAZY )
    @JoinColumn( name = "ALN_ID" )
    private Aluno aluno;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Demanda.class , fetch = FetchType.LAZY )
    @JoinColumn( name = "DEM_ID" )
    private Demanda demanda;
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
    private Set<AtendimentoTatico> atendimentosTatico = new HashSet<AtendimentoTatico>();
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
    private Set<AtendimentoOperacional> atendimentosOperacional = new HashSet<AtendimentoOperacional>();

	@Column( name = "ALD_ATENDIDO" )
	private Boolean atendido;

	@NotNull
	@Column( name = "ALD_PRIORIDADE" )
	@Min( 0L )
	@Max( 10L )
	private Integer prioridade;

	@NotNull
	@Column( name = "ALD_PERIODO" )
	@Min( 0L )
	@Max( 10L )
	private Integer periodo;

	public Long getId()
	{
		return this.id;
	}

	public void setId( Long id )
	{
		this.id = id;
	}

	public Integer getVersion()
	{
		return this.version;
	}

	public void setVersion( Integer version )
	{
		this.version = version;
	}

	public Aluno getAluno()
	{
		return this.aluno;
	}

	public void setAluno( Aluno aluno )
	{
		this.aluno = aluno;
	}

	public Demanda getDemanda()
	{
		return this.demanda;
	}

	public void setDemanda( Demanda demanda )
	{
		this.demanda = demanda;
	}

	public Boolean getAtendido()
	{
		return this.atendido;
	}

	public void setAtendido( Boolean atendido )
	{
		this.atendido = atendido;
	}

	public Integer getPrioridade()
	{
		return this.prioridade;
	}

	public void setPrioridade( Integer prioridade )
	{
		this.prioridade = prioridade;
	}

	public Integer getPeriodo()
	{
		return this.periodo;
	}

	public void setPeriodo( Integer periodo )
	{
		this.periodo = periodo;
	}
	
	public void setAtendimentosTatico(Set<AtendimentoTatico> atendimentosTatico){
		this.atendimentosTatico = atendimentosTatico;
	}
	
	public Set<AtendimentoTatico> getAtendimentosTatico(){
		return this.atendimentosTatico;
	}

	public void setAtendimentosOperacional(Set<AtendimentoOperacional> atendimentosOperacional){
		this.atendimentosOperacional = atendimentosOperacional;
	}
	
	public Set<AtendimentoOperacional> getAtendimentosOperacional(){
		return this.atendimentosOperacional;
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
			AlunoDemanda attached = this.entityManager.find(
					this.getClass(), this.id );

			this.entityManager.remove( attached );
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
	public AlunoDemanda merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		AlunoDemanda merged = this.entityManager.merge( this );
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new AlunoDemanda().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				"Entity manager has not been injected (is the Spring " +
				"Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByDemanda(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda = :demanda " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "demanda", demanda );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByCampusAndTurno(
		InstituicaoEnsino instituicaoEnsino, Collection<Campus> campi, Turno turno )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus IN ( :campi ) " +
			" AND o.demanda.oferta.turno = :turno " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campi", campi );
		q.setParameter( "turno", turno );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static AlunoDemanda findByDemandaAndAluno(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda, Aluno aluno )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.aluno.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda = :demanda " +
			" AND o.aluno = :aluno " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "demanda", demanda );
		q.setParameter( "aluno", aluno );

		List< AlunoDemanda > alunosDemanda = q.getResultList();
		return ( alunosDemanda.size() == 0 ? null : alunosDemanda.get( 0 ) );
	}

	public static AlunoDemanda find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		AlunoDemanda alunoDemanda = entityManager().find(
			AlunoDemanda.class, id );

		if ( alunoDemanda != null
			&& alunoDemanda.getAluno() != null
			&& alunoDemanda.getAluno().getInstituicaoEnsino() != null
			&& alunoDemanda.getAluno().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return alunoDemanda;
		}

		return null;
	}

	public String getNaturalKey()
	{
		return this.getDemanda() + "-" + this.getAluno();
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Aluno: " ).append( getAluno() ).append( ", " );
		sb.append( "Periodo: " ).append( getPeriodo() ).append( ", " );
		sb.append( "Demanda: " ).append( getDemanda() );

		return sb.toString();
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.getId() == null ) ? 0 : this.getId().hashCode() ) );
		result = ( prime * result + ( ( this.getVersion() == null ) ? 0 : this.getVersion().hashCode() ) );
		result = ( prime * result + ( ( this.getDemanda() == null ) ? 0 : this.getDemanda().hashCode() ) );
		result = ( prime * result + ( ( this.getAluno() == null ) ? 0 : this.getAluno().hashCode() ) );

		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null
			|| !( obj instanceof AlunoDemanda ) )
		{
			return false;
		}

		AlunoDemanda other = (AlunoDemanda) obj;

		// Comparando as demandas
		if ( getDemanda() == null )
		{
			if ( other.getDemanda() != null )
			{
				return false;
			}
		}
		else if ( !getDemanda().equals( other.getDemanda() ) )
		{
			return false;
		}

		// Comparando os alunos
		if ( getAluno() == null )
		{
			if ( other.getAluno() != null )
			{
				return false;
			}
		}
		else if ( !getAluno().equals( other.getAluno() ) )
		{
			return false;
		}

		return true;
	}

	@Override
	public int compareTo( AlunoDemanda o )
	{
		int compare = this.getDemanda().getId().compareTo( o.getDemanda().getId() );

		if ( compare == 0 )
		{
			compare = this.getAluno().getId().compareTo( o.getAluno().getId() );
		}

		return compare;
	}

	public static Map< String, AlunoDemanda > buildCodAlunoCodDemandaToAlunosDemandasMap(
		List< AlunoDemanda > alunosDemanda )
	{
		Map< String, AlunoDemanda > alunosDemandaMap
			= new HashMap< String, AlunoDemanda >();

		for ( AlunoDemanda alunoDemanda : alunosDemanda )
		{
			String codigo = "";
			codigo += alunoDemanda.getAluno().getMatricula();
			codigo += "-";
			codigo += alunoDemanda.getDemanda().getId();

			alunosDemandaMap.put( codigo, alunoDemanda );
		}

		return alunosDemandaMap;
	}
}
