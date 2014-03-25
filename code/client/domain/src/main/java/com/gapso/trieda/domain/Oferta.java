package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Collection;
import java.util.Collections;
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
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Digits;
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
@RooEntity( identifierColumn = "OFE_ID" )
@Table( name = "OFERTAS" )
public class Oferta
	implements Serializable, Comparable< Oferta >
{
	private static final long serialVersionUID = -976299446108675926L;

	@NotNull
	@ManyToOne( targetEntity = Curriculo.class )
	@JoinColumn( name = "CRC_ID" )
	private Curriculo curriculo;

	@NotNull
	@ManyToOne( targetEntity = Campus.class )
	@JoinColumn( name = "CAM_ID" )
	private Campus campus;

	@NotNull
	@ManyToOne( targetEntity = Turno.class )
	@JoinColumn( name = "TUR_ID" )
	private Turno turno;

	@NotNull
	@ManyToOne( targetEntity = Curso.class )
	@JoinColumn( name = "CUR_ID" )
	private Curso curso;

	@Column( name = "OFE_RECEITA" )
	@Digits( integer = 6, fraction = 2 )
	private Double receita;

	@NotNull
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "oferta" )
	private Set< Demanda > demandas = new HashSet< Demanda >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "oferta" )
	private Set< AtendimentoOperacional > atendimentosOperacionais = new HashSet< AtendimentoOperacional >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "oferta" )
	private Set< AtendimentoTatico > atendimentosTaticos = new HashSet< AtendimentoTatico >();

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "OFE_ID" )
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
	
	public String getNaturalKeyString() {
		return getCampus().getCodigo() + "-" + 
		       getTurno().getNome() + "-" + 
		       getCurso().getCodigo() + "-" + 
		       getCurriculo().getCodigo();
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
			Oferta attached = this.entityManager.find(
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
	public Oferta merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Oferta merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Oferta().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				"Entity manager has not been injected (is the Spring " +
				"Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Oferta > findAllBy(
		InstituicaoEnsino instituicaoEnsino, Sala sala )
	{
		Query q = entityManager().createQuery(
			"SELECT DISTINCT ( o ) FROM Oferta o, IN " +
			"( o.curriculo.disciplinas ) dis " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND dis.disciplina IN ( :disciplinas ) " );

		Set< Disciplina > disciplinas
			= sala.getDisciplinas();

		q.setParameter( "disciplinas", sala.getDisciplinas() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( disciplinas.isEmpty() )
		{
			return Collections.< Oferta > emptyList();
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Oferta > findAllBy(
		InstituicaoEnsino instituicaoEnsino, GrupoSala grupoSala )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o ) FROM Oferta o, " +
			" IN ( o.curriculo.disciplinas ) dis " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND dis.disciplina IN ( :disciplinas ) " );

		Set< Disciplina > curriculoDisciplinas
			= grupoSala.getDisciplinas();

		q.setParameter( "disciplinas", grupoSala.getDisciplinas() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( curriculoDisciplinas.isEmpty() )
		{
			return Collections.< Oferta > emptyList();
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Oferta > findByCampusAndTurno(
		InstituicaoEnsino instituicaoEnsino, Campus campus, Turno turno )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus = :campus " +
			" AND o.turno = :turno " );

		q.setParameter( "campus", campus );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< Oferta > findByCampus(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus = :campus " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Oferta > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Oferta > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		return entityManager().createQuery(
			" SELECT o FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino ")
			.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
	}
	
	public static Map< Long, Oferta > buildOfertaIdToOfertaMap( List< Oferta > ofertas )
	{
		Map< Long, Oferta > ofertasMap
			= new HashMap< Long, Oferta >();

		for ( Oferta oferta : ofertas )
		{
			ofertasMap.put( oferta.getId(), oferta );
		}

		return ofertasMap;
	}

	public static Oferta find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Oferta oferta = entityManager().find( Oferta.class, id );

		if ( oferta != null && oferta.getCampus() != null
			&& oferta.getCampus().getInstituicaoEnsino() != null
			&& oferta.getCampus().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return oferta;
		}

		return null;
	}

	public static List< Oferta > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< Oferta > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		return entityManager().createQuery(
			" SELECT o FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " + orderBy )
			.setParameter( "instituicaoEnsino", instituicaoEnsino )
			.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
	}

	public static int count( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Turno turno, Campus campus, Curso curso, Curriculo curriculo )
	{
		String queryTurno = ( ( turno != null ) ? " o.turno = :turno AND " : "" );
		String queryCampus = ( ( campus != null ) ? " o.campus = :campus AND " : "" );
		String queryCurso = ( ( curso != null ) ? " o.curriculo.curso = :curso AND " : "" );
		String queryCurriculo = ( ( curriculo != null ) ? " o.curriculo = :curriculo AND " : "" );

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus.cenario = :cenario " +
			" AND " + queryTurno + queryCampus + queryCurso + queryCurriculo + " 1=1 " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}

		if ( campus != null )
		{
			q.setParameter( "campus", campus );
		}

		if ( curso != null )
		{
			q.setParameter( "curso", curso );
		}

		if ( curriculo != null )
		{
			q.setParameter( "curriculo", curriculo );
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List<Oferta> findBy( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Turno turno, Campus campus, Curso curso,
		Curriculo curriculo, int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy.replace("String", "") : "" );

		String queryTurno = ( ( turno != null ) ? " o.turno = :turno AND " : "" );
		String queryCampus = ( ( campus != null ) ? " o.campus = :campus AND " : "" );
		String queryCurso = ( ( curso != null ) ? " o.curriculo.curso = :curso AND " : "" );
		String queryCurriculo = ( ( curriculo != null ) ? " o.curriculo = :curriculo AND "	: "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus.cenario = :cenario " +
			" AND " + queryTurno + queryCampus + queryCurso + queryCurriculo + " 1=1 " + orderBy);

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}

		if ( campus != null )
		{
			q.setParameter( "campus", campus );
		}

		if ( curso != null )
		{
			q.setParameter( "curso", curso );
		}

		if ( curriculo != null )
		{
			q.setParameter( "curriculo", curriculo );
		}

		return q.getResultList();
	}

	// [ Campus + Turno + Matriz Curricular ]
	private static String getCodeOferta( Oferta domain )
	{
		String codigo = domain.getCampus().getCodigo()
			+ "-" + domain.getTurno().getNome()
			+ "-" + domain.getCurriculo().getCodigo();

		return codigo;
	}
	
	public static Map< String, Oferta > buildCampusTurnoCurriculoToOfertaMap(
		List< Oferta > ofertas )
	{
		Map< String, Oferta > ofertasMap
			= new HashMap< String, Oferta >();

		for ( Oferta oferta : ofertas )
		{
			String codigo = Oferta.getCodeOferta( oferta );
			ofertasMap.put( codigo, oferta );
		}

		return ofertasMap;
	}

	public Curriculo getCurriculo()
	{
		return this.curriculo;
	}

	public void setCurriculo( Curriculo curriculo )
	{
		this.curriculo = curriculo;
	}

	public Campus getCampus()
	{
		return this.campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}

	public Turno getTurno()
	{
		return this.turno;
	}

	public void setTurno( Turno turno )
	{
		this.turno = turno;
	}

	public Curso getCurso()
	{
		return this.curso;
	}

	public void setCurso( Curso curso )
	{
		this.curso = curso;
	}

	public Set< Demanda > getDemandas()
	{
		return this.demandas;
	}

	public void setDemandas(
		Set< Demanda > demandas )
	{
		this.demandas = demandas;
	}

	public Double getReceita()
	{
		return this.receita;
	}

	public void setReceita( Double receita )
	{
		this.receita = receita;
	}

	public Set< AtendimentoOperacional > getAtendimentosOperacionais()
	{
		return this.atendimentosOperacionais;
	}

	public void setAtendimentosOperacionais(
			Set< AtendimentoOperacional > atendimentosOperacionais )
	{
		this.atendimentosOperacionais = atendimentosOperacionais;
	}

	public Set< AtendimentoTatico > getAtendimentosTaticos()
	{
		return this.atendimentosTaticos;
	}

	public void setAtendimentosTaticos(
		Set< AtendimentoTatico > atendimentosTaticos )
	{
		this.atendimentosTaticos = atendimentosTaticos;
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Curriculo: " ).append( getCurriculo().getCodigo() ).append( ", " );
		sb.append( "Campus: " ).append( getCampus().getCodigo() ).append( ", " );
		sb.append( "Turno: " ).append( getTurno().getNome() ).append( ", " );
		sb.append( "Demandas: " ).append( getDemandas() == null ?
			"null" : getDemandas().size() ).append( ", " );
		sb.append( "Receita: " ).append( getReceita() ).append( ", " );
		sb.append( "Atendimentos Operacionais: " ).append( getAtendimentosOperacionais() == null ?
			"null" : getAtendimentosOperacionais().size() ).append( ", " );
		sb.append( "Atendimentos Taticos: " ).append( getAtendimentosTaticos() == null ?
			"null" : getAtendimentosTaticos().size() );

		return sb.toString();
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null
			|| !( obj instanceof Oferta ) )
		{
			return false;
		}

		Oferta other = (Oferta) obj;

		if ( !this.getCampus().equals( other.getCampus() ) )
		{
			return false;
		}

		if ( !this.getCurriculo().equals( other.getCurriculo() ) )
		{
			return false;
		}

		if ( !this.getTurno().equals( other.getTurno() ) )
		{
			return false;
		}

		if ( !this.getCurso().equals( other.getCurso() ) )
		{
			return false;
		}

		return true;
	}

	//@Override
	public int compareTo( Oferta o )
	{
		return this.getCampus().getNome().compareTo( o.getCampus().getNome() );
	}
	
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.id == null ) ? 0 : this.id.hashCode() ) );
		result = ( prime * result + ( ( this.version == null ) ? 0 : this.version.hashCode() ) );

		return result;
	}
	
	static public Map<Long,Oferta> getOfertasMap(Collection<Oferta> ofertas) {
		Map<Long,Oferta> map = new HashMap<Long, Oferta>();
		for (Oferta oferta : ofertas) {
			map.put(oferta.getId(), oferta);
		}
		return map;
	}
	
	public void setReceitaWithPrecision (double d) {
		long factor = (long) Math.pow(10, 2);
		d = d*factor;
		long tmp = Math.round(d);
		
		this.setReceita( (double)tmp/factor );
	}
}