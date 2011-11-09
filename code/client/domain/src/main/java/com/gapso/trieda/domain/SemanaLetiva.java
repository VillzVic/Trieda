package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
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
@RooEntity( identifierColumn = "SLE_ID" )
@Table( name = "SEMANA_LETIVA", uniqueConstraints =
@UniqueConstraint( columnNames = { "SLE_CODIGO" } ) )
public class SemanaLetiva
	implements Serializable, Comparable< SemanaLetiva >
{
	private static final long serialVersionUID = 6807360646327130208L;

	@OneToOne( mappedBy = "semanaLetiva" )
	private Cenario cenario;

	@NotNull
	@Column( name = "SLE_CODIGO" )
	@Size( min = 1, max = 500 )
	private String codigo;

	@NotNull
	@Column( name = "SLE_DESCRICAO" )
	@Size( max = 500 )
	private String descricao;

    @NotNull
    @Column( name = "SLE_TEMPO" )
    @Min( 1L )
    @Max( 1000L )
    private Integer tempo;

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "semanaLetiva" )
	private Set< HorarioAula > horariosAula = new HashSet< HorarioAula >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "semanaLetiva" )
	private Set< Parametro > parametros = new HashSet< Parametro >();

	@OneToMany( mappedBy = "semanaLetiva" )
	private Set< Curriculo > curriculos = new HashSet< Curriculo >();

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST,
		CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	public Set< Curriculo > getCurriculos()
	{
		return this.curriculos;
	}

	public void setCurriculos(
		Set< Curriculo > curriculos )
	{
		this.curriculos = curriculos;
	}

	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
	}

	public String getCodigo()
	{
		return this.codigo;
	}

	public void setCodigo( String codigo )
	{
		this.codigo = codigo;
	}

	public String getDescricao()
	{
		return this.descricao;
	}

	public void setDescricao( String descricao )
	{
		this.descricao = descricao;
	}

	public Set< HorarioAula > getHorariosAula()
	{
		return this.horariosAula;
	}

	public void setHorariosAula(
		Set< HorarioAula > horariosAula )
	{
		this.horariosAula = horariosAula;
	}

	public Set< Parametro > getParametros()
	{
		return this.parametros;
	}

	public void setParametros(
		Set< Parametro > parametros )
	{
		this.parametros = parametros;
	}

	public Integer getTempo()
	{
		return this.tempo;
	}

	public void setTempo( Integer tempo )
	{
		this.tempo = tempo;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "SLE_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;

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
			SemanaLetiva attached = this.entityManager.find(
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
	public SemanaLetiva merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		SemanaLetiva merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new SemanaLetiva().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	public static Map< String, SemanaLetiva > buildSemanaLetivaCodigoToSemanaLetivaMap(
		List< SemanaLetiva > semanasLetivas )
	{
		Map< String, SemanaLetiva> semanasLetivasMap
			= new HashMap< String, SemanaLetiva >();

		for ( SemanaLetiva semanaLetiva : semanasLetivas )
		{
			semanasLetivasMap.put( semanaLetiva.getCodigo(), semanaLetiva );
		}

		return semanasLetivasMap;
	}

	@SuppressWarnings( "unchecked" )
	public static List< SemanaLetiva > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		List< SemanaLetiva > result = null;

		try
		{
			result = q.getResultList();
		}
		catch( Exception e )
		{
			result = null;
		}

		return result;
	}

	public static SemanaLetiva find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		SemanaLetiva semanaLetiva
			= entityManager().find( SemanaLetiva.class, id );

		if ( semanaLetiva != null
			&& semanaLetiva.getInstituicaoEnsino() != null
			&& semanaLetiva.getInstituicaoEnsino() == instituicaoEnsino )
		{
			return semanaLetiva;
		}

		return null;
	}

	public static List< SemanaLetiva > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< SemanaLetiva > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " + orderBy );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino,
		String codigo, String descricao )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		descricao = ( ( descricao == null ) ? "" : descricao );
		descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );

		EntityManager em = Turno.entityManager();
		Query q = em.createQuery(
			" SELECT COUNT ( o ) FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " +
			" AND LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " );

		q.setParameter( "codigo", codigo );
		q.setParameter( "descricao", descricao );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< SemanaLetiva > findBy(
		InstituicaoEnsino instituicaoEnsino, String codigo,
		String descricao, int firstResult, int maxResults, String orderBy )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		descricao = ( ( descricao == null ) ? "" : descricao );
		descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );

		EntityManager em = Turno.entityManager();
		orderBy = ( ( orderBy != null ) ? "ORDER BY o." + orderBy : "" );

		Query q = em.createQuery(
			" SELECT o FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.codigo ) LIKE  LOWER ( :codigo ) " +
			" AND LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " + orderBy );

		q.setParameter( "codigo", codigo );
		q.setParameter( "descricao", descricao );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.codigo = :codigo " );

		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = ( (Number) ( q.setMaxResults( 1 ).getSingleResult() ) );
		return ( size.intValue() > 0 );
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicoes de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
		sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
		sb.append( "Tempo: " ).append( getTempo() ).append( ", " );
		sb.append( "HorariosAula: " ).append(
			getHorariosAula() == null ? "null" : getHorariosAula().size() ).append( ", " );
		sb.append( "Descricao: " ).append( getDescricao() ).append( ", " );
		sb.append( "Parametros: " ).append(
			getParametros() == null ? "null" : getParametros().size() );

		return sb.toString();
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof SemanaLetiva ) )
		{
			return false;
		}

		SemanaLetiva other = (SemanaLetiva) obj;

		if ( this.id == null )
		{
			if ( other.id != null )
			{
				return false;
			}
		}
		else if ( !this.id.equals( other.id ) )
		{
			return false;
		}

		return true;
	}

	@Override
	public int compareTo( SemanaLetiva o )
	{
		int result = getInstituicaoEnsino().compareTo( o.getInstituicaoEnsino() );
		
		if ( result == 0 )
		{
			result = getCodigo().compareTo( o.getCodigo() );
		}

		return result;
	}
}
