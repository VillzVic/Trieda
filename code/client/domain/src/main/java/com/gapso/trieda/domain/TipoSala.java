package com.gapso.trieda.domain;

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
@RooEntity(identifierColumn = "TSA_ID")
@Table(name = "TIPOS_SALA")
public class TipoSala	
	implements java.io.Serializable, Clonable< TipoSala >
{
	private static final long serialVersionUID = -1633461518380764117L;
	
	public static final String TIPO_SALA_DE_AULA = "Sala de Aula";
	public static final String TIPO_LABORATORIO = "Laboratório";
	public static final String TIPO_AUDITORIO = "Auditório";

    @NotNull
    @Column(name = "TSA_NOME")
    @Size(min = 1, max = 255)
    private String nome;

    @Column(name = "TSA_DESCRICAO")
    @Size(max = 255)
    private String descricao;
    
	@Column( name = "TSA_ACEITA_AULA_PRATICA" )
	private Boolean aceitaAulaPratica;
    
	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

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
	
	public Cenario getCenario() {
		return this.cenario;
	}
	
	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append(getId()).append(", ");
        sb.append( "Version: " ).append(getVersion()).append(", ");
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
        sb.append( "Nome: " ).append(getNome()).append(", ");
        sb.append( "Descricao: " ).append(getDescricao());

        return sb.toString();
    }

	public String getNome() {
        return this.nome;
    }

	public void setNome(String nome) {
        this.nome = nome;
    }

	public String getDescricao() {
        return this.descricao;
    }

	public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "TSA_ID")
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

	public Integer getVersion() {
        return this.version;
    }

	public void setVersion(Integer version) {
        this.version = version;
    }
	
	public Boolean getAceitaAulaPratica() {
		return this.aceitaAulaPratica;
	}
	
	public void setAceitaAulaPratica(Boolean aceitaAulaPratica) {
		this.aceitaAulaPratica = aceitaAulaPratica;
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
    public void remove() {
        if (this.entityManager == null) this.entityManager = entityManager();
        if (this.entityManager.contains(this)) {
            this.entityManager.remove(this);
        } else {
            TipoSala attached = this.entityManager.find(this.getClass(), this.id);
            this.entityManager.remove(attached);
        }
    }

	@Transactional
    public void flush() {
        if (this.entityManager == null) this.entityManager = entityManager();
        this.entityManager.flush();
    }

	@Transactional
    public TipoSala merge() {
        if (this.entityManager == null) this.entityManager = entityManager();
        TipoSala merged = this.entityManager.merge(this);
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new TipoSala().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static long count( InstituicaoEnsino instituicaoEnsino )
	{
        return ( (Number) TipoSala.findAll( instituicaoEnsino ).size() ).longValue();
    }

	@SuppressWarnings("unchecked")
    public static List< TipoSala > findAll( InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
    		" SELECT o FROM TipoSala o " +
    		" WHERE o.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }
	
	@SuppressWarnings("unchecked")
    public static List< TipoSala > findByCenario( InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
		Query q = entityManager().createQuery(
    		" SELECT o FROM TipoSala o " +
    		" WHERE o.instituicaoEnsino = :instituicaoEnsino" +
    		" AND o.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

        return q.getResultList();
    }

	public static Map< String,TipoSala > buildTipoSalaNomeToTipoSalaMap(
		List< TipoSala > tiposSala )
	{
		Map< String, TipoSala > tiposSalaMap
			= new HashMap< String, TipoSala >();

		for ( TipoSala tipoSala : tiposSala )
		{
			tiposSalaMap.put( tipoSala.getNome(), tipoSala );
		}

		return tiposSalaMap;
	}

	public static TipoSala find( Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        TipoSala ts = entityManager().find( TipoSala.class, id );

        if ( ts != null
        	&& ts.getInstituicaoEnsino()!= null
        	&& ts.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return ts;
        }

        return null;
    }

	public static List< TipoSala > find( InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )	
	{
		return find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings("unchecked")
    public static List< TipoSala > find( InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults, String orderBy )
    {
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
	        "SELECT o FROM TipoSala o " +
	        "WHERE o.instituicaoEnsino = :instituicaoEnsino " + orderBy );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		List< TipoSala > list = q.getResultList();
        return list;
    }
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		int code = this.nome == null ? 0 : this.nome.hashCode();
		code += this.cenario == null ? 0 : this.cenario.getId().hashCode();
		result = prime * result + code;
		return result;
	}
	
	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof TipoSala ) )
		{
			return false;
		}

		TipoSala other = (TipoSala) obj;

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
		if ( this.nome == null )
		{
			if ( other.nome != null )
			{
				return false;
			}
		}
		else if ( !this.nome.equals( other.nome ) )
		{
			return false;
		}
		if ( this.cenario == null )
		{
			if ( other.cenario != null )
			{
				return false;
			}
		}
		else if ( !this.cenario.equals( other.cenario ) )
		{
			return false;
		}
		

		return true;
	}

	public TipoSala clone(CenarioClone novoCenario) {
		TipoSala clone = new TipoSala();
		clone.setAceitaAulaPratica(this.getAceitaAulaPratica());
		clone.setCenario(novoCenario.getCenario());
		clone.setDescricao(this.getDescricao());
		clone.setInstituicaoEnsino(this.getInstituicaoEnsino());
		clone.setNome(this.getNome());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, TipoSala entidadeClone) {
		
	}
}
