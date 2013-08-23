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
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
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
@RooEntity( identifierColumn = "CUR_ID" )
@Table( name = "CURSOS", uniqueConstraints =
@UniqueConstraint( columnNames = { "CEN_ID", "CUR_CODIGO" } ) )
public class Curso
	implements Serializable, Comparable< Curso >
{
	private static final long serialVersionUID = 2645879541329424105L;

	@NotNull
	@ManyToOne( targetEntity = TipoCurso.class )
	@JoinColumn( name = "TCU_ID" )
	private TipoCurso tipoCurso;

	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	@NotNull
	@Column( name = "CUR_CODIGO" )
	@Size( min = 1, max = 50 )
	private String codigo;

	@NotNull
	@Column( name = "CUR_NOME" )
	@Size( min = 1, max = 255 )
	private String nome;

	@NotNull
	@Column( name = "CUR_MIN_DOUTORES" )
	@Min( 0L )
	@Max( 100L )
	private Integer numMinDoutores;

	@NotNull
	@Column( name = "CUR_MIN_MESTRES" )
	@Min( 0L )
	@Max( 100L )
	private Integer numMinMestres;

	@NotNull
	@Column( name = "CUR_TEM_INTPARC" )
	@Min( 0L )
	@Max( 100L )
	private Integer minTempoIntegralParcial;

	@NotNull
	@Column( name = "CUR_TEM_INT" )
	@Min( 0L )
	@Max( 100L )
	private Integer minTempoIntegral;

	@NotNull
	@Column( name = "CUR_MAX_DISC_PROF" )
	@Min( 1L )
	@Max( 99L )
	private Integer maxDisciplinasPeloProfessor;

	@Column( name = "CUR_ADM_MAIS_DE_UMA_DISC" )
	private Boolean admMaisDeUmDisciplina;

	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "cursos" )
	private Set< AreaTitulacao > areasTitulacao = new HashSet< AreaTitulacao >();
	
	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "cursos" )
	private Set< Equivalencia > equivalencias = new HashSet< Equivalencia >();

	@OneToMany( mappedBy = "curso" )
	private Set< Curriculo > curriculos = new HashSet< Curriculo >();

    @OneToMany( mappedBy = "curso" )
    private Set< Oferta > ofertas = new HashSet< Oferta >();
    
    

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "TipoCurso: " ).append( getTipoCurso() ).append( ", " );
		sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
		sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
		sb.append( "Nome: " ).append( getNome() ).append( ", " );
		sb.append( "NumMinDoutores: " ).append( getNumMinDoutores() ).append( ", " );
		sb.append( "NumMinMestres: " ).append( getNumMinMestres() ).append( ", " );
		sb.append( "MinTempoIntegralParcial: " ).append( getMinTempoIntegralParcial() ).append( ", " );
		sb.append( "MinTempoIntegral: " ).append( getMinTempoIntegral() ).append( ", " );
		sb.append( "MaxDisciplinasPeloProfessor: " ).append( getMaxDisciplinasPeloProfessor() ).append( ", " );
		sb.append( "AdmMaisDeUmDisciplina: " ).append( getAdmMaisDeUmDisciplina() ).append( ", " );
		sb.append( "AreasTitulacao: " ).append( getAreasTitulacao() == null ? "null" : getAreasTitulacao().size() );

		return sb.toString();
	}

	public TipoCurso getTipoCurso()
	{
		return this.tipoCurso;
	}

	public void setTipoCurso( TipoCurso tipoCurso )
	{
		this.tipoCurso = tipoCurso;
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

	public String getNome()
	{
		return this.nome;
	}

	public void setNome( String nome )
	{
		this.nome = nome;
	}

	public Integer getNumMinDoutores()
	{
		return this.numMinDoutores;
	}

	public void setNumMinDoutores( Integer numMinDoutores )
	{
		this.numMinDoutores = numMinDoutores;
	}

	public Integer getNumMinMestres()
	{
		return this.numMinMestres;
	}

	public void setNumMinMestres( Integer numMinMestres )
	{
		this.numMinMestres = numMinMestres;
	}

	public Integer getMinTempoIntegralParcial()
	{
		return minTempoIntegralParcial;
	}

	public void setMinTempoIntegralParcial( Integer minTempoIntegralParcial )
	{
		this.minTempoIntegralParcial = minTempoIntegralParcial;
	}

	public Integer getMinTempoIntegral()
	{
		return minTempoIntegral;
	}

	public void setMinTempoIntegral( Integer minTempoIntegral )
	{
		this.minTempoIntegral = minTempoIntegral;
	}

	public Integer getMaxDisciplinasPeloProfessor()
	{
		return this.maxDisciplinasPeloProfessor;
	}

	public void setMaxDisciplinasPeloProfessor( Integer maxDisciplinasPeloProfessor )
	{
		this.maxDisciplinasPeloProfessor = maxDisciplinasPeloProfessor;
	}

	public Boolean getAdmMaisDeUmDisciplina()
	{
		return this.admMaisDeUmDisciplina;
	}

	public void setAdmMaisDeUmDisciplina( Boolean admMaisDeUmDisciplina )
	{
		this.admMaisDeUmDisciplina = admMaisDeUmDisciplina;
	}

	public Set< AreaTitulacao > getAreasTitulacao()
	{
		return this.areasTitulacao;
	}

	public void setAreasTitulacao( Set< AreaTitulacao > areasTitulacao )
	{
		this.areasTitulacao = areasTitulacao;
	}

	public Set< Curriculo > getCurriculos()
	{
		return this.curriculos;
	}

	public void setCurriculos( Set< Curriculo > curriculos )
	{
		this.curriculos = curriculos;
	}

	public Set< Oferta > getOfertas()
	{
		return this.ofertas;
	}

	public void setOfertas( Set< Oferta > ofertas )
	{
		this.ofertas = ofertas;
	}
	
	public Set< Equivalencia > getEquivalencias()
	{
		return this.equivalencias;
	}

	public void setEquivalencias( Set< Equivalencia > equivalencias )
	{
		this.equivalencias = equivalencias;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "CUR_ID" )
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
			this.removeAreasTitulacao();
			this.removeMatrizesCurriculares();

			this.entityManager.remove( this );
		}
		else
		{
			Curso attached = this.entityManager.find(
				this.getClass(), this.id );

			attached.removeAreasTitulacao();
			attached.removeMatrizesCurriculares();

			this.entityManager.remove( attached );
		}
	}

	@Transactional
	public void removeMatrizesCurriculares()
	{
		Set< Curriculo > curriculos = this.getCurriculos();

		for ( Curriculo curriculo : curriculos )
		{
			if ( curriculo != null )
			{
				curriculo.remove();
			}
		}
	}

	@Transactional
	public void removeAreasTitulacao()
	{
		Set< AreaTitulacao > areas = this.getAreasTitulacao();

		for ( AreaTitulacao area : areas )
		{
			area.getCursos().remove( this );
			area.merge();
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
	public Curso merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Curso merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Curso().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?) " );
		}

		return em;
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Curso o " +
			" WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Curso > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Curso o " +
			" WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static Map< String, Curso > buildCursoCodigoToCursoMap(
		List< Curso > cursos )
	{
		Map< String, Curso > cursosMap
			= new HashMap< String, Curso >();

		for ( Curso curso : cursos )
		{
			cursosMap.put( curso.getCodigo(), curso );
		}

		return cursosMap;
	}

	public static Curso find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Curso curso = entityManager().find( Curso.class, id );

		if ( curso != null
			&& curso.getTipoCurso() != null
			&& curso.getTipoCurso().getInstituicaoEnsino() != null
			&& curso.getTipoCurso().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return curso;
		}

		return null;
	}

	public static List< Curso > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return Curso.find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< Curso > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM Curso o " +
			" WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " + orderBy );

		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Curso > findByCampus(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Curso o " +
			" WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o IN ( SELECT cc.curriculo.curso FROM Oferta cc WHERE cc.campus = :campus )" );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<Curso> findByCampi(InstituicaoEnsino instituicaoEnsino, List<Campus> campi) {
		Query q = entityManager().createQuery(
			" SELECT o FROM Curso o "
			+ " WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino "
			+ " AND o IN ( SELECT cc.curriculo.curso FROM Oferta cc WHERE cc.campus IN ( :campi ) )");

		q.setParameter("campi",campi);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Curso > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Curso o " +
			" WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Curso > findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo,
		String nome, TipoCurso tipoCurso, int firstResult, int maxResults, String orderBy )
	{
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy.replace("String", "") : "" );

		String queryCampus = "";
		if ( tipoCurso != null )
		{
			queryCampus = ( " o.tipoCurso = :tipoCurso AND " );
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM Curso o WHERE "	+ queryCampus +
			" LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " + orderBy );

		if ( tipoCurso != null )
		{
			q.setParameter( "tipoCurso", tipoCurso );
		}

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "nome", nome );
		q.setParameter( "codigo", codigo );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		String codigo, String nome, TipoCurso tipoCurso )
	{
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

		String queryCampus = "";
		if ( tipoCurso != null )
		{
			queryCampus = ( " o.tipoCurso = :tipoCurso AND " );
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Curso o " +
			" WHERE " + queryCampus	+ " LOWER ( o.nome ) LIKE LOWER ( :nome ) " +
			" AND o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " );

		if ( tipoCurso != null )
		{
			q.setParameter( "tipoCurso", tipoCurso );
		}

		q.setParameter( "nome", nome );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Curso o " +
			" WHERE o.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND o.codigo = :codigo" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.id == null ) ? 0 : this.id.hashCode() ) );
		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( this == obj )
		{
			return true;
		}

		if ( obj == null )
		{
			return false;
		}

		if ( getClass() != obj.getClass() )
		{
			return false;
		}

		Curso other = (Curso) obj;

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

	//@Override
	public int compareTo( Curso c )
	{
		return this.getCodigo().compareTo( c.getCodigo() );
	}
}
