package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Date;
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
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "HOR_ID" )
@Table( name = "HORARIOS_AULA" )
public class HorarioAula
	implements Serializable, Comparable< HorarioAula >, Clonable< HorarioAula >
{
	private static final long serialVersionUID = 6415195416443296422L;

    @NotNull
    @ManyToOne( targetEntity = SemanaLetiva.class )
    @JoinColumn( name = "SLE_ID" )
    private SemanaLetiva semanaLetiva;

    @NotNull
    @ManyToOne( targetEntity = Turno.class )
    @JoinColumn( name = "TUR_ID" )
    private Turno turno;

    @NotNull
    @Column( name = "HOR_HORARIO" )
    @Temporal( TemporalType.TIMESTAMP )
    @DateTimeFormat( style = "--" )
    private Date horario;

    @OneToMany( cascade = CascadeType.ALL, mappedBy="horarioAula", orphanRemoval=true )
    private Set< HorarioDisponivelCenario > horariosDisponiveisCenario =  new HashSet< HorarioDisponivelCenario >();

    public String toString()
    {
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Semana Letiva: " ).append( getSemanaLetiva() ).append( ", " );
        sb.append( "Turno: " ).append( getTurno() ).append( ", " );
        sb.append( "HorariosDisponiveisCenario: " ).append( getHorariosDisponiveisCenario() == null ?
        	"null" : getHorariosDisponiveisCenario().size() ).append( ", " );
        sb.append( "Horario: ").append( getHorario() );

        return sb.toString();
    }
    
    public boolean estahContidoEm(Calendar horarioInicio, Calendar horarioFim) {
    	// dado que iremos comparar apenas horas é necessário que todas as instâncias de calendário estejam no mesmo dia
    	
    	horarioInicio.set(1979,Calendar.NOVEMBER,6,horarioInicio.get(Calendar.HOUR_OF_DAY),horarioInicio.get(Calendar.MINUTE),0);
    	horarioFim.set(1979,Calendar.NOVEMBER,6,horarioFim.get(Calendar.HOUR_OF_DAY),horarioFim.get(Calendar.MINUTE),0);
    	
    	Calendar thisHorarioInicio = Calendar.getInstance();
    	thisHorarioInicio.setTime(horario);
    	thisHorarioInicio.set(1979,Calendar.NOVEMBER,6,thisHorarioInicio.get(Calendar.HOUR_OF_DAY),thisHorarioInicio.get(Calendar.MINUTE),0);
    	Calendar thisHorarioFim = Calendar.getInstance();
    	thisHorarioFim.setTime(thisHorarioInicio.getTime());
    	thisHorarioFim.add(Calendar.MINUTE,semanaLetiva.getTempo());
    	
    	return (horarioInicio.compareTo(thisHorarioInicio) <= 0) && 
    		   (thisHorarioFim.compareTo(horarioFim) <= 0);
    }

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "HOR_ID" )
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
            HorarioAula attached = this.entityManager.find(
            	this.getClass(), this.id );

            this.entityManager.remove( attached );
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
    public HorarioAula merge()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        HorarioAula merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager()
    {
        EntityManager em = new HorarioAula().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

    @SuppressWarnings( "unchecked" )
    public static List< HorarioAula > findBySemanaLetiva(
    	InstituicaoEnsino instituicaoEnsino, SemanaLetiva semanaLetiva )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM HorarioAula o " +
       		" WHERE o.semanaLetiva = :semanaLetiva " +
       		" AND o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " );

       	q.setParameter( "instituicaoEnsino", instituicaoEnsino );
       	q.setParameter( "semanaLetiva", semanaLetiva );

       	return q.getResultList();
    }

    @SuppressWarnings( "unchecked" )
    public static List< HorarioAula > findByTurno(
    	InstituicaoEnsino instituicaoEnsino, Turno turno )
    {
    	Query q = entityManager().createQuery(
            " SELECT o FROM HorarioAula o " +
    		" WHERE o.turno = :turno " +
    		" AND o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );
    	q.setParameter( "turno", turno );

        return q.getResultList();
    }

    @SuppressWarnings( "unchecked" )
    public static List< HorarioAula > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
    	Query q = entityManager().createQuery(
           	" SELECT o FROM HorarioAula o " +
    		" WHERE o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

    public static HorarioAula find(
    	Long id, InstituicaoEnsino instituicaoEnsino )
    {
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        HorarioAula ha = entityManager().find( HorarioAula.class, id );

        if ( ha != null && ha.getSemanaLetiva() != null
        	&& ha.getSemanaLetiva().getInstituicaoEnsino() != null
        	&& ha.getSemanaLetiva().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return ha;
        }

        return null;
    }

    public static List< HorarioAula > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
        return find( instituicaoEnsino, firstResult, maxResults, null );
    }

    @SuppressWarnings( "unchecked" )
    public static List< HorarioAula > find(
   		InstituicaoEnsino instituicaoEnsino,
   		int firstResult, int maxResults, String orderBy )
    {
        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
           	" SELECT o FROM HorarioAula o " +
           	" WHERE o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino" + orderBy );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        return q.getResultList();
    }

    public static int count( InstituicaoEnsino instituicaoEnsino,
    	SemanaLetiva semanaLetiva, Turno turno, Date horario )
    {
    	String horarioQuery = ( ( horario == null )? "" : " o.horario = :horario AND " );
    	String semanaLetivaQuery = ( ( semanaLetiva == null ) ? "" : " o.semanaLetiva = :semanaLetiva AND " );
    	String turnoQuery = ( ( turno == null ) ? "" : " o.turno = :turno AND " );

    	Query q = entityManager().createQuery(
    		" SELECT COUNT( o ) FROM HorarioAula o " +
    		" WHERE o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " +
    		" AND " + horarioQuery + " " + semanaLetivaQuery + " " + turnoQuery + " 1=1 " );

    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	if ( horario != null )
    	{
    		q.setParameter( "horario", horario );
    	}

    	if ( semanaLetiva != null )
    	{
    		q.setParameter( "semanaLetiva", semanaLetiva );
    	}

    	if ( turno != null )
    	{
    		q.setParameter( "turno", turno );
    	}

    	return ( ( (Number) q.getSingleResult() ).intValue() );
    }

    @SuppressWarnings( "unchecked" )
    public static List< HorarioAula > findBy( InstituicaoEnsino instituicaoEnsino,
    	SemanaLetiva semanaLetiva, Turno turno, Date horario,
    	int firstResult, int maxResults, String orderBy )
    {
    	orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        String horarioQuery = ( ( horario == null )? "" : " o.horario = :horario AND " );
        String semanaLetivaQuery = ( ( semanaLetiva == null ) ? "" : " o.semanaLetiva = :semanaLetiva AND " );
        String turnoQuery = ( ( turno == null )? "" : " o.turno = :turno AND " );

        Query q = entityManager().createQuery(
        	" SELECT o FROM HorarioAula o " +
        	" WHERE o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " +
        	" AND " + horarioQuery
        	+ " " + semanaLetivaQuery + " " + turnoQuery + " 1=1 " + orderBy );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        if ( horario != null )
        {
        	q.setParameter( "horario", horario );
        }

        if ( semanaLetiva != null )
        {
        	q.setParameter( "semanaLetiva", semanaLetiva );
        }

        if ( turno != null )
        {
        	q.setParameter( "turno", turno );
        }

        return q.getResultList();
    }

    @SuppressWarnings( "unchecked" )
    public static List< HorarioAula > findHorarioAulasBySemanaLetivaAndTurno(
    	InstituicaoEnsino instituicaoEnsino,
    	SemanaLetiva semanaLetiva, Turno turno )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM HorarioAula o " +
    		" WHERE o.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.semanaLetiva = :semanaLetiva " +
    		" AND o.turno.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.turno = :turno " );

    	q.setParameter( "semanaLetiva", semanaLetiva );
    	q.setParameter( "turno", turno );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

	public static Map< Long, HorarioAula > buildHorarioAulaIdToHorarioAulaMap(
		List< HorarioAula > horariosAulas )
	{
		Map< Long, HorarioAula > horariosMap
			= new HashMap< Long, HorarioAula >();

		for ( HorarioAula horario : horariosAulas )
		{
			horariosMap.put( horario.getId(), horario );
		}

		return horariosMap;
	}

	public SemanaLetiva getSemanaLetiva()
    {
        return this.semanaLetiva;
    }

    public void setSemanaLetiva(
    	SemanaLetiva semanaLetiva )
    {
        this.semanaLetiva = semanaLetiva;
    }

    public Turno getTurno()
    {
        return this.turno;
    }

    public void setTurno( Turno turno )
    {
        this.turno = turno;
    }

    public Date getHorario()
    {
        return this.horario;
    }

    public void setHorario( Date horario )
    {
        this.horario = horario;
    }

    public Set< HorarioDisponivelCenario > getHorariosDisponiveisCenario()
	{
        return this.horariosDisponiveisCenario;
    }

    public void setHorariosDisponiveisCenario(
		Set< HorarioDisponivelCenario > horariosDisponiveisCenario )
	{
        this.horariosDisponiveisCenario = horariosDisponiveisCenario;
    }

    //@Override
	public int compareTo( HorarioAula o )
	{
		if ( this.getHorario() == null )
		{
			if ( o.getHorario() == null )
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		
    	Calendar thisHorario = Calendar.getInstance();
    	thisHorario.setTime(this.getHorario());
    	thisHorario.set(1979,Calendar.NOVEMBER,6);
    	
    	Calendar oHorario = Calendar.getInstance();
    	oHorario.setTime(o.getHorario());
    	oHorario.set(1979,Calendar.NOVEMBER,6);
    	
		return thisHorario.compareTo( oHorario );
	}
	
	public HorarioAula clone(CenarioClone novoCenario)
	{
		HorarioAula clone = new HorarioAula();
		clone.setHorario(this.getHorario());
		clone.setSemanaLetiva(novoCenario.getEntidadeClonada(this.getSemanaLetiva()));
		clone.setTurno(novoCenario.getEntidadeClonada(this.getTurno()));

		return clone;
	}
	
	public void cloneChilds(CenarioClone novoCenario, HorarioAula clone) {
		for (HorarioDisponivelCenario horarioDisponivelCenario : this.getHorariosDisponiveisCenario())
		{
			clone.getHorariosDisponiveisCenario().add(novoCenario.clone(horarioDisponivelCenario));
		}
	}
}
