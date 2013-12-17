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
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

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
@RooEntity( identifierColumn = "DCR_ID" )
@Table( name = "DIVISOES_CREDITO" )
public class DivisaoCredito
	implements Serializable
{
	private static final long serialVersionUID = 4185000264330934580L;

    @NotNull
    @Column( name = "DRC_CREDITOS" )
    @Min( 1L )
    @Max( 99L )
    private Integer creditos;

    @NotNull
    @Column( name = "DRC_DIA1" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia1;

    @NotNull
    @Column( name = "DRC_DIA2" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia2;

    @NotNull
    @Column( name = "DRC_DIA3" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia3;

    @NotNull
    @Column( name = "DRC_DIA4" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia4;

    @NotNull
    @Column( name = "DRC_DIA5" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia5;

    @NotNull
    @Column( name = "DRC_DIA6" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia6;

    @NotNull
    @Column( name = "DRC_DIA7" )
    @Min( 0L )
    @Max( 99L )
    private Integer dia7;

    @ManyToMany( cascade = { CascadeType.PERSIST,
    	CascadeType.MERGE, CascadeType.REFRESH } )
    private Set< Cenario > cenario = new HashSet< Cenario >();

    @OneToOne( cascade = CascadeType.ALL,
    	targetEntity = Disciplina.class, mappedBy = "divisaoCreditos" )
    private Disciplina disciplina;
    
	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "DCR_ID" )
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
        	this.removeCenario( this );

            this.entityManager.remove( this );
        }
        else
        {
        	DivisaoCredito attached
        		= this.entityManager.find( this.getClass(), this.id );

        	this.removeCenario( attached );

            this.entityManager.remove( attached );
        }
    }

	private void removeCenario( DivisaoCredito cd )
	{
		if ( cd.getCenario() == null )
		{
			return;
		}

		for ( Cenario cenario : cd.getCenario() )
		{
			cenario.getDivisoesCredito().remove( cd );
			cenario.merge();
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
    public DivisaoCredito merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        DivisaoCredito merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new DivisaoCredito().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static int count(
		Cenario cenario, InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM DivisaoCredito o, IN ( o.cenario ) c " +
			" WHERE c = :cenario " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return ( (Number) q.getSingleResult() ).intValue();
    }

	public static DivisaoCredito find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        DivisaoCredito dc = entityManager().find(
        	DivisaoCredito.class, id );

        if ( dc != null
        	&& dc.getInstituicaoEnsino() != null 
        	&& dc.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return dc;
        }

        return null;
    }
	
	@SuppressWarnings( "unchecked" )
	public static List<DivisaoCredito> findByInstituicaoEnsino(InstituicaoEnsino instituicaoEnsino) { 
		Query q = entityManager().createQuery(
			" SELECT o FROM DivisaoCredito o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino" +
			" ORDER BY o.creditos"
		);
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		try {
			return q.getResultList();
		}
		catch ( EmptyResultDataAccessException e ) { }

		return null;
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<DivisaoCredito> findByCenario(InstituicaoEnsino instituicaoEnsino, Cenario cenario) { 
		Query q = entityManager().createQuery(
			" SELECT o FROM DivisaoCredito o, IN ( o.cenario ) c  " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino" +
			" c = :cenario " +
			" ORDER BY o.creditos"
		);

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		try {
			return q.getResultList();
		}
		catch ( EmptyResultDataAccessException e ) { }

		return null;
	}
	
	public static DivisaoCredito findByCredito(
		Integer credito, InstituicaoEnsino instituicaoEnsino )
	{ 
		Query q = entityManager().createQuery(
			" SELECT o FROM DivisaoCredito o " +
			" WHERE o.creditos = :credito " +
			" AND o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "credito", credito );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		try
		{
			return (DivisaoCredito) q.setMaxResults( 1 ).getSingleResult();
		}
		catch ( EmptyResultDataAccessException e ) { }

		return null;
	}

	@SuppressWarnings( "unchecked" )
    public static List< DivisaoCredito > findWithoutDisciplina(
    	Cenario cenario, int firstResult,
    	int maxResults, InstituicaoEnsino instituicaoEnsino )
    {
        Query q = entityManager().createQuery(
        	" SELECT o FROM DivisaoCredito o, IN ( o.cenario ) c " +
        	" WHERE c = :cenario " +
        	" AND o.instituicaoEnsino = :instituicaoEnsino " +
        	" ORDER BY o.creditos ASC " );

        q.setParameter( "cenario", cenario );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
    public static List< DivisaoCredito > findByDias(
    	InstituicaoEnsino instituicaoEnsino, int dia1, int dia2, int dia3, int dia4,
    	int dia5, int dia6, int dia7)
    {
        Query q = entityManager().createQuery(
        	" SELECT o FROM DivisaoCredito o " +
        	" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.dia1 = :dia1 AND o.dia2 = :dia2 AND o.dia3 = :dia3" +
        	" AND o.dia4 = :dia4 AND o.dia5 = :dia5 AND o.dia6 = :dia6" +
        	" AND o.dia7 = :dia7"
        	 );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "dia1", dia1 );
        q.setParameter( "dia2", dia2 );
        q.setParameter( "dia3", dia3 );
        q.setParameter( "dia4", dia4 );
        q.setParameter( "dia5", dia5 );
        q.setParameter( "dia6", dia6 );
        q.setParameter( "dia7", dia7 );

        return q.getResultList();
    }
	
	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append(", " );
        sb.append( "Inetituicao de Ensino: " ).append( getInstituicaoEnsino() ).append(", " );
        sb.append( "Creditos: " ).append( getCreditos() ).append(", " );
        sb.append( "Dia1: " ).append( getDia1() ).append( ", " );
        sb.append( "Dia2: " ).append( getDia2() ).append( ", " );
        sb.append( "Dia3: " ).append( getDia3() ).append( ", " );
        sb.append( "Dia4: " ).append( getDia4() ).append( ", " );
        sb.append( "Dia5: " ).append( getDia5() ).append( ", " );
        sb.append( "Dia6: " ).append( getDia6() ).append( ", " );
        sb.append( "Dia7: " ).append( getDia7() ).append( ", " );
        sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
        sb.append( "Cenario: " ).append( getCenario() == null ? "null" : getCenario().size() );

        return sb.toString();
    }

	public Integer getCreditos()
	{
        return this.creditos;
    }

	public void setCreditos( Integer creditos )
	{
        this.creditos = creditos;
    }

	public Integer getDia1()
	{
        return this.dia1;
    }

	public void setDia1( Integer dia1 )
	{
        this.dia1 = dia1;
    }

	public Integer getDia2()
	{
        return this.dia2;
    }

	public void setDia2( Integer dia2 )
	{
        this.dia2 = dia2;
    }

	public Integer getDia3()
	{
        return this.dia3;
    }

	public void setDia3( Integer dia3 )
	{
        this.dia3 = dia3;
    }

	public Integer getDia4()
	{
        return this.dia4;
    }

	public void setDia4( Integer dia4 )
	{
        this.dia4 = dia4;
    }

	public Integer getDia5()
	{
        return this.dia5;
    }

	public void setDia5( Integer dia5 )
	{
        this.dia5 = dia5;
    }

	public Integer getDia6()
	{
        return this.dia6;
    }

	public void setDia6( Integer dia6 )
	{
        this.dia6 = dia6;
    }

	public Integer getDia7()
	{
        return this.dia7;
    }

	public void setDia7( Integer dia7 )
	{
        this.dia7 = dia7;
    }

	public Set< Cenario > getCenario()
	{
        return this.cenario;
    }

	public void setCenario(
		Set< Cenario > cenario )
	{
        this.cenario = cenario;
    }

	public Disciplina getDisciplina()
	{
		return this.disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}
	
	public static Map<String, DivisaoCredito> buildDivisaoCreditoKeyToDivisaoCreditoMap(
		List<DivisaoCredito> divisoesCredito )
	{
		Map<String, DivisaoCredito> divisoesMap = new HashMap<String, DivisaoCredito>();
				
		for (DivisaoCredito divisaoCredito : divisoesCredito)
		{
			String key = divisaoCredito.getDia1() + "-" + divisaoCredito.getDia2() + "-" + divisaoCredito.getDia3()
				+ "-" + divisaoCredito.getDia4() + "-" + divisaoCredito.getDia5() + "-" + divisaoCredito.getDia6()
				+ "-" + divisaoCredito.getDia7();
			
			divisoesMap.put(key, divisaoCredito);
		}
		
		return divisoesMap;
	}
}
