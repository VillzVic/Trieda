package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
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
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "ATI_ID" )
@Table( name = "AREAS_TITULACAO" )
public class AreaTitulacao
	implements Serializable
{
	private static final long serialVersionUID = 8739246006672184100L;

    @NotNull
    @Column(name = "ATI_CODIGO")
    @Size(min = 1, max = 50)
    private String codigo;

    @Column(name = "ATI_DESCRICAO")
    @Size(min = 3, max = 255)
    private String descricao;

	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;
    
    @ManyToMany
    private Set< Curso > cursos = new java.util.HashSet< Curso >();

	@PersistenceContext
    transient EntityManager entityManager;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return instituicaoEnsino;
	}

	public void setInstituicaoEnsino( InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ATI_ID")
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
	
	public Cenario getCenario() {
		return this.cenario;
	}
	
	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public Integer getVersion() {
        return this.version;
    }

	public void setVersion( Integer version )
	{
        this.version = version;
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
    public boolean remove()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        if ( this.entityManager.contains( this ) )
        {
            List< Professor > verificaProfessres = this.getProfessores();
            if ( verificaProfessres != null && verificaProfessres.size() > 0 )
            {
            	// N�o posso remover a �rea de titula��o,
            	// pois h� professores vinculados a ela
            	return false;
            }

            this.entityManager.remove( this );
        }
        else
        {
            AreaTitulacao attached = this.entityManager.find( this.getClass(), this.id );

            List< Professor > verificaProfessres = attached.getProfessores();
            if ( verificaProfessres != null && verificaProfessres.size() > 0 )
            {
            	// N�o posso remover a �rea de titula��o,
            	// pois h� professores vinculados a ela
            	return false;
            }

            this.entityManager.remove( attached );
        }

        return true;
    }

	@SuppressWarnings("unchecked")
	@Transactional
	public List< Professor > getProfessores()
	{
		List< Professor > professores = null;

		Query q = entityManager().createQuery(
			" SELECT p FROM Professor p " +
			" WHERE p.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND p.areaTitulacao.instituicaoEnsino = :instituicaoEnsino " +
			" AND p.areaTitulacao = :areaT " );

		q.setParameter( "areaT", this );
		q.setParameter( "instituicaoEnsino", this.instituicaoEnsino );

		try
		{
			professores = q.getResultList();
		}
		catch ( EmptyResultDataAccessException e ) { }

		return professores;
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
    public AreaTitulacao merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        AreaTitulacao merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new AreaTitulacao().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	@SuppressWarnings("unchecked")
    public static List< AreaTitulacao > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
        return entityManager().createQuery(
        	" SELECT o FROM AreaTitulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.getResultList();
    }
	
	@SuppressWarnings("unchecked")
    public static List< AreaTitulacao > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
        return entityManager().createQuery(
        	" SELECT o FROM AreaTitulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.cenario = :cenario" )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setParameter("cenario", cenario)
        	.getResultList();
    }

	public static AreaTitulacao find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        AreaTitulacao at = entityManager().find( AreaTitulacao.class, id );

        if ( at != null
        	&& at.getInstituicaoEnsino() != null 
        	&& at.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return at;
        }

        return null;
    }

	public static List<AreaTitulacao> find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings("unchecked")
    public static List< AreaTitulacao > find( InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults, String orderBy )
    {
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        return entityManager().createQuery(
        	" SELECT o FROM AreaTitulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
    }

	public static int count( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo, String descricao )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

		if ( descricao != null )
		{
			descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );
		}

		String descricaoQuery = ( ( descricao == null ) ? "" :
			" AND LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " );

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM AreaTitulacao o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.cenario = :cenario " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " + descricaoQuery );

		q.setParameter( "codigo", codigo );
		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( descricao != null )
		{
			q.setParameter( "descricao", descricao );
		}

		return ( (Number)q.getSingleResult() ).intValue();
	}

    @SuppressWarnings("unchecked")
    public static List< AreaTitulacao > findBy(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo,
    	String descricao, int firstResult, int maxResults, String orderBy )
    {
    	codigo = ( ( codigo == null ) ? "" : codigo );
        codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

        if ( descricao != null )
        {
        	descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );
        }

        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        String descricaoQuery = ( ( descricao == null ) ? "" :
        	"AND LOWER ( o.descricao ) LIKE LOWER ( :descricao )" );

        Query q = entityManager().createQuery(
        	" SELECT o FROM AreaTitulacao o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.cenario = :cenario " +
        	" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " +
        	descricaoQuery + " " + orderBy );

        q.setParameter( "codigo", codigo );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );

        if ( descricao != null )
        {
        	q.setParameter( "descricao", descricao );
        }

        return q.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
    }
	
	public static Map< String, AreaTitulacao > buildAreaTitulacaoCodigoToAreaTitulacaoMap(
		List< AreaTitulacao > areasTitulacoes )
	{
		Map< String, AreaTitulacao > areasTitulacoesMap
			= new HashMap< String, AreaTitulacao >();

		for ( AreaTitulacao areaTitulacao : areasTitulacoes )
		{
			areasTitulacoesMap.put(
				areaTitulacao.getCodigo(), areaTitulacao );
		}

		return areasTitulacoesMap;
	}
    
	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Descricao: " ).append( getDescricao() ).append( ", " );
        sb.append( "Cursos: " ).append( getCursos() == null ? "null" : getCursos().size() );

        return sb.toString();
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

	public Set< Curso > getCursos()
	{
        return this.cursos;
    }

	public void setCursos( Set< Curso > cursos )
	{
        this.cursos = cursos;
    }
}
