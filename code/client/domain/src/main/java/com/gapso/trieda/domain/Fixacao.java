package com.gapso.trieda.domain;

import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.HashSet;
import java.util.List;
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
@RooEntity( identifierColumn = "FIX_ID" )
@Table( name = "FIXACOES" )
public class Fixacao
	implements Serializable
{
	private static final long serialVersionUID = -7545908494415718467L;


	@NotNull
    @Column( name = "FIX_CODIGO" )
    @Size( min = 1, max = 50 )
    private String codigo;
    
    @NotNull
    @Column( name = "FIX_DESCRICAO" )
    @Size( min = 1, max = 50 )
    private String descricao;
    
    @ManyToOne( targetEntity = Professor.class, fetch=FetchType.LAZY )
    @JoinColumn( name = "PRF_ID" )
    private Professor professor;
	
    @ManyToOne( targetEntity = Disciplina.class, fetch=FetchType.LAZY )
    @JoinColumn( name = "DIS_ID" )
    private Disciplina disciplina;
    
    @NotNull
    @Column( name = "TURMA" )
    @Size( min = 1, max = 255 )
    private String turma;
    
    @ManyToOne( targetEntity = Campus.class )
    @JoinColumn( name = "CAM_ID" )
    private Campus campus;
    
    @ManyToOne( targetEntity = Unidade.class )
    @JoinColumn( name = "UNI_ID" )
    private Unidade unidade;
    
    @ManyToOne( targetEntity = Sala.class )
    @JoinColumn( name = "SAL_ID" )
    private Sala sala;
    
    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "fixacoes" )
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "FIX_ID" )
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
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
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
        	this.removeHorariosDisponivelCenario();
            this.entityManager.remove( this );
        }
        else
        {
            Fixacao attached = this.entityManager.find(
            	this.getClass(), this.id );

            attached.removeHorariosDisponivelCenario();
            this.entityManager.remove( attached );
        }
    }

    @Transactional
    public void removeHorariosDisponivelCenario()
    {
    	Set< HorarioDisponivelCenario > horarios = this.getHorarios();

    	for ( HorarioDisponivelCenario horario : horarios )
    	{
    		horario.getFixacoes().remove( this );
    		horario.merge();
    	}
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
    public void flush()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        this.entityManager.flush();
    }

    @Transactional
    public Fixacao merge()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }
	
        Fixacao merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager()
    {
        EntityManager em = new Fixacao().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

    public static int count(
    	InstituicaoEnsino instituicaoEnsino )
    {
    	List< Fixacao > list = Fixacao.findAll( instituicaoEnsino );
        return ( list == null ? 0 : list.size() );
    }

    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
        Query q = entityManager().createQuery(
        	"SELECT o " +
        	"FROM Fixacao o " +
        	"WHERE o.instituicaoEnsino = :instituicaoEnsino ");

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        List< Fixacao > list = q.getResultList();
        return list;
    }

    public static Fixacao find(
    	Long id, InstituicaoEnsino instituicaoEnsino )
    {
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Fixacao f = entityManager().find( Fixacao.class, id );

        if ( f != null
        	&& f.getInstituicaoEnsino() != null
        	&& f.getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return f;
        }

        return null;
    }

    public static List< Fixacao > find(
    	InstituicaoEnsino instituicaoEnsino,
        int firstResult, int maxResults )
    {
        return Fixacao.find( instituicaoEnsino,
        	firstResult, maxResults, null );
    }

    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults, String orderBy )
    {
        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
        	"SELECT o FROM Fixacao o " +
        	"WHERE o.instituicaoEnsino = :instituicaoEnsino " + orderBy );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );
        
        List< Fixacao > list = q.getResultList();
        return list;
    }

    @SuppressWarnings( "unchecked" )
	public static List<Fixacao> findByCenario(InstituicaoEnsino instituicaoEnsino, Cenario cenario)
	{
        Query q = entityManager().createQuery(
        	        	" SELECT o FROM Fixacao o " +
        	        	" WHERE o.campus.cenario = :cenario " +
        	        	" AND o.instituicaoEnsino = :instituicaoEnsino " );

        q.setParameter( "cenario", cenario );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        List< Fixacao > list = q.getResultList();
        return list;
	}

    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > findAllBy(
    	InstituicaoEnsino instituicaoEnsino, Disciplina disciplina )
    {
        Query q = entityManager().createQuery(
        	" SELECT o FROM Fixacao o " +
        	" WHERE o.disciplina = :disciplina " +
        	" AND o.instituicaoEnsino = :instituicaoEnsino " );

        q.setParameter( "disciplina", disciplina );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        List< Fixacao > list = q.getResultList();
        return list;
    }
    
    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > findBy(
    	InstituicaoEnsino instituicaoEnsino, String codigo,
    	int firstResult, int maxResults, String orderBy )
    {
    	codigo = ( ( codigo == null || codigo.length() == 0 ) ? "" : codigo );
        codigo = codigo.replace( '*', '%' );

        if ( codigo == "" || codigo.charAt( 0 ) != '%' )
        {
            codigo = ( "%" + codigo );
        }

        if ( codigo.charAt( codigo.length() - 1 ) != '%' )
        {
            codigo = ( codigo + "%" );
        }

        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
        	" SELECT o FROM Fixacao o WHERE " +
        	" LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " +
        	" AND o.instituicaoEnsino = :instituicaoEnsino " + orderBy );

        q.setParameter( "codigo", codigo );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        List< Fixacao > list = q.getResultList();
        return list;
    }

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.fixacoes ) c " +
			" WHERE c = :fixacao " +
			" AND c.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.horarioAula.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "fixacao", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
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

    public Professor getProfessor()
    {
    	return this.professor;
    }

    public void setProfessor( Professor professor )
    {
    	this.professor = professor;
    }

	public Disciplina getDisciplina()
	{
        return this.disciplina;
    }

	public void setDisciplina( Disciplina disciplina )
	{
        this.disciplina = disciplina;
    }
	
	public String getTurma()
	{
        return this.turma;
    }

	public void setTurma( String turma )
	{
        this.turma = turma;
    }
	
	public Campus getCampus()
	{
		return this.campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}

	public Unidade getUnidade()
	{
		return this.unidade;
	}

	public void setUnidade( Unidade unidade )
	{
		this.unidade = unidade;
	}

	public Sala getSala()
	{
		return this.sala;
	}

	public void setSala( Sala sala )
	{
		this.sala = sala;
	}

	private Set< HorarioDisponivelCenario > getHorarios()
	{
        return this.horarios;
    }

	public void setHorarios(
		Set< HorarioDisponivelCenario > horarios )
	{
        this.horarios = horarios;
    }

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Professor: " ).append( getProfessor() ).append( ", " );
        sb.append( "Descricao: " ).append( getDescricao() ).append( ", " );
        sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
        sb.append( "Turma: " ).append( getTurma() ).append( ", " );
        sb.append( "Campus: " ).append( getCampus() ).append( ", " );
        sb.append( "Unidade: " ).append( getUnidade() ).append( ", " );
        sb.append( "Sala: " ).append( getSala() ).append( ", " );
        sb.append( "Horarios: " ).append( getHorarios() == null ?
        	"null" : getHorarios().size() );

        return sb.toString();
    }

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( id == null) ? 0 : id.hashCode() ) );
		result = ( prime * result + ( ( version == null ) ? 0 : version.hashCode() ) );

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

		Fixacao other = (Fixacao) obj;

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

		if ( version == null )
		{
			if ( other.version != null )
			{
				return false;
			}
		}
		else if ( !version.equals( other.version ) )
		{
			return false;
		}

		return true;
	}

	public String getHorariosStr()
	{
		SimpleDateFormat dateTimeFormat = new SimpleDateFormat("HH:mm");
		StringBuilder horariosDisponiveisCenario = new StringBuilder();

		for (HorarioDisponivelCenario horarioDisponivelCenario : this.getHorarios())
		{
			if (horariosDisponiveisCenario.length() > 0) {
				horariosDisponiveisCenario.append(";");
			}
			
			horariosDisponiveisCenario
				.append(horarioDisponivelCenario.getDiaSemana().name())
				.append(" ")
				.append(horarioDisponivelCenario.getHorarioAula().getSemanaLetiva().getCodigo())
				.append(" ")
				.append(horarioDisponivelCenario.getHorarioAula().getTurno().getId())
				.append(" ")
				.append(dateTimeFormat.format(horarioDisponivelCenario.getHorarioAula().getHorario()));
		}

		return horariosDisponiveisCenario.toString();
	}
}
