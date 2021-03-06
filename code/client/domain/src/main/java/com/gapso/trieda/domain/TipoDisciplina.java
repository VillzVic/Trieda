package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
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

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "TDI_ID" )
@Table( name = "TIPOS_DISCIPLINA" )
public class TipoDisciplina
	implements Serializable, Comparable< TipoDisciplina >, Clonable< TipoDisciplina >
{
	private static final long serialVersionUID = 3145587865770084666L;

    @NotNull
    @Column( name = "TDI_NOME" )
    @Size( min = 1, max = 50 )
    private String nome;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST,
		CascadeType.MERGE, CascadeType.REFRESH }, targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;
	
	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}
	
	public Cenario getCenario() {
		return this.cenario;
	}
	
	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append(", ");
        sb.append( "Version: " ).append( getVersion() ).append(", ");
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
        sb.append( "Nome: " ).append( getNome() );

        return sb.toString();
    }
	
	/**
	 * Informa se uma disciplina do tipo em quest�o ocupa grade de horários ou não. Uma disciplina ocupa grade de horários
	 * caso a mesma seja dos tipos "Presencial" ou "Telepresencial" e n�o ocupa grade caso seja do tipo "Online".
	 * @return true caso uma disciplina do tipo em quest�o ocupa grade de horários e false caso contrário
	 */
	public boolean ocupaGrade() {
		return !this.nome.equals("Online");
	}

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "TDI_ID" )
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
            TipoDisciplina attached = this.entityManager.find(
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
    public TipoDisciplina merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        TipoDisciplina merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new TipoDisciplina().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static int count(
		InstituicaoEnsino instituicaoEnsino )
	{
        return TipoDisciplina.findAll( instituicaoEnsino ).size();
    }

	@SuppressWarnings( "unchecked" )
    public static List< TipoDisciplina > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	    	" SELECT o FROM TipoDisciplina o " +
	    	" WHERE o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		List< TipoDisciplina > list = q.getResultList(); 
        return list;
    }
	
	@SuppressWarnings( "unchecked" )
    public static List< TipoDisciplina > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
		Query q = entityManager().createQuery(
	    	" SELECT o FROM TipoDisciplina o " +
	    	" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
	    	" AND o.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		List< TipoDisciplina > list = q.getResultList(); 
        return list;
    }

	public static Map< String, TipoDisciplina > buildTipoDisciplinaNomeToTipoDisciplinaMap(
		List< TipoDisciplina > tiposDisciplina )
	{
		Map< String, TipoDisciplina > tiposDisciplinaMap
			= new HashMap< String, TipoDisciplina >();

		for ( TipoDisciplina tipoDisciplina : tiposDisciplina )
		{
			tiposDisciplinaMap.put(
				tipoDisciplina.getNome(), tipoDisciplina );
		}

		return tiposDisciplinaMap;
	}

	public static TipoDisciplina find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        TipoDisciplina td = entityManager().find( TipoDisciplina.class, id );

        if ( td != null
        	&& td.getInstituicaoEnsino() != null
        	&& td.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return td;
        }

        return null;
    }

	@SuppressWarnings( "unchecked" )
    public static List< TipoDisciplina > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
		Query q = entityManager().createQuery(
    		" SELECT o FROM TipoDisciplina o " +
    		" WHERE o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		List< TipoDisciplina > list = q.getResultList();
        return list;
    }

	public String getNome()
	{
        return this.nome;
    }

	public void setNome( String nome )
	{
        this.nome = nome;
    }


	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.getId() == null ) ? 0 : this.getId().hashCode() ) );
		result = ( prime * result + ( ( this.getVersion() == null ) ? 0 : this.getVersion().hashCode() ) );

		return result;
	}


	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof TipoDisciplina ) )
		{
			return false;
		}

		TipoDisciplina other = (TipoDisciplina) obj;

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

	//@Override
	public int compareTo( TipoDisciplina o )
	{
		int result = this.getInstituicaoEnsino().compareTo( o.getInstituicaoEnsino() );

		if ( result == 0 )
		{
			result = this.getId().compareTo( o.getId() );
		}

		return result;
	}

	public TipoDisciplina clone(CenarioClone novoCenario) {
		TipoDisciplina clone = new TipoDisciplina();
		clone.setCenario(novoCenario.getCenario());
		clone.setInstituicaoEnsino(this.getInstituicaoEnsino());
		clone.setNome(this.getNome());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario,
			TipoDisciplina entidadeClone) {
		
	}
}
