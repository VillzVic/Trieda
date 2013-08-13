package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
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
@RooEntity( identifierColumn = "CDI_ID" )
@Table( name = "CURRICULOS_DISCIPLINAS" )
public class CurriculoDisciplina
	implements Serializable
{
	private static final long serialVersionUID = -5429743673577487971L;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST,
    	CascadeType.MERGE, CascadeType.REFRESH },
    	targetEntity = Curriculo.class )
    @JoinColumn( name = "CRC_ID" )
    private Curriculo curriculo;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH },
    	targetEntity = Disciplina.class )
    @JoinColumn( name = "DIS_ID" )
    private Disciplina disciplina;

    @NotNull
    @Column( name = "CDI_PERIODO" )
    @Min( 0L )
    @Max( 100L )
    private Integer periodo;

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Curriculo: " ).append( getCurriculo() ).append( ", " );
        sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
        sb.append( "Periodo: " ).append( getPeriodo() ).append( ", " );

        return sb.toString();
    }

	public Curriculo getCurriculo()
	{
        return this.curriculo;
    }

	public void setCurriculo( Curriculo curriculo )
	{
        this.curriculo = curriculo;
    }

	public Disciplina getDisciplina()
	{
        return this.disciplina;
    }

	public void setDisciplina( Disciplina disciplina )
	{
        this.disciplina = disciplina;
    }

	public Integer getPeriodo()
	{
        return this.periodo;
    }

	public void setPeriodo( Integer periodo )
	{
        this.periodo = periodo;
    }
	
	public String getNaturalKeyString()
	{
		String key = ( getCurriculo().getCurso().getCodigo()
			+ "-" + getCurriculo().getCodigo()
			+ "-" + getPeriodo().toString()
			+ "-" + getDisciplina().getCodigo() );

		return key;
	}

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "CDI_ID" )
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
            CurriculoDisciplina attached
            	= this.entityManager.find( this.getClass(), this.id );

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
    public CurriculoDisciplina merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        CurriculoDisciplina merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }
	
	@Transactional
    public CurriculoDisciplina mergeWithoutFlush() {
        if (this.entityManager == null) {
        	this.entityManager = entityManager();
        }

        CurriculoDisciplina merged = this.entityManager.merge(this);
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new CurriculoDisciplina().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		" Entity manager has not been injected (is the Spring " +
        		" Aspects JAR configured as an AJC/AJDT aspects library?) " );
        }

        return em;
    }

	public static int count(
		InstituicaoEnsino instituicaoEnsino )
	{
        return CurriculoDisciplina.findAll( instituicaoEnsino ).size();
    }

	public static Map< String, CurriculoDisciplina > buildNaturalKeyToCurriculoDisciplinaMap(
		List< CurriculoDisciplina > curriculosDisciplina )
	{
		Map< String, CurriculoDisciplina > curriculosDisciplinaMap
			= new HashMap< String, CurriculoDisciplina >();

		for ( CurriculoDisciplina curriculoDisciplina : curriculosDisciplina )
		{
			curriculosDisciplinaMap.put(
				curriculoDisciplina.getNaturalKeyString(), curriculoDisciplina );
		}

		return curriculosDisciplinaMap;
	}
	
	public static Map<String,List<CurriculoDisciplina>> buildDisciplinaToCurriculoDisciplinaMap(Collection<CurriculoDisciplina> curriculosDisciplina) {
		Map<String,List<CurriculoDisciplina>> map = new HashMap<String,List<CurriculoDisciplina>>();

		for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplina) {
			List<CurriculoDisciplina> list = map.get(curriculoDisciplina.getDisciplina().getCodigo());
			if (list == null) {
				list = new ArrayList<CurriculoDisciplina>();
				map.put(curriculoDisciplina.getDisciplina().getCodigo(),list);
			}
			list.add(curriculoDisciplina);
		}

		return map;
	}

	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o " +
			" WHERE o.curriculo.cenario = :cenario " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "cenario", cenario );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findAllPeriodosBy(
		InstituicaoEnsino instituicaoEnsino, Sala sala, Oferta oferta )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o, " +
			" IN ( o.salas ) sala, IN ( o.curriculo.ofertas ) oferta " +
			" WHERE sala = :sala " +
			" AND oferta = :oferta " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino" +
        	" AND sala.unidade.campus.instituicaoEnsino = :instituicaoEnsino" +
        	" AND oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" GROUP BY o.periodo " );

		q.setParameter( "sala", sala );
		q.setParameter( "oferta", oferta );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();		
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findAllPeriodosBy(
		InstituicaoEnsino instituicaoEnsino, GrupoSala grupoSala, Oferta oferta )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o, " +
			" IN ( o.gruposSala ) grupoSala, IN ( o.curriculo.ofertas ) oferta " +
			" WHERE grupoSala = :grupoSala " +
			" AND grupoSala.unidade.campus.instituicaoEnsino = :instituicaoEnsino" +
			" AND oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND oferta = :oferta " +
			" GROUP BY o.periodo " );

		q.setParameter( "grupoSala", grupoSala );
		q.setParameter( "oferta", oferta );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();		
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findAllByCurriculoAndPeriodo(
		InstituicaoEnsino instituicaoEnsino, Curriculo curriculo, Integer periodo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o " +
			" WHERE o.curriculo = :curriculo " +
			" AND o.periodo = :periodo " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "curriculo", curriculo );
		q.setParameter( "periodo", periodo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findBy(
		InstituicaoEnsino instituicaoEnsino,
		GrupoSala grupoSala, Oferta oferta, Integer periodo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o, " +
			" IN ( o.gruposSala ) grupoSala " +
			" WHERE o.periodo = :periodo " +
			" AND o.curriculo = :curriculo " +
			" AND grupoSala = :grupoSala " +
			" AND grupoSala.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "periodo", periodo );
		q.setParameter( "curriculo", oferta.getCurriculo() );
		q.setParameter( "grupoSala", grupoSala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findBy(
		InstituicaoEnsino instituicaoEnsino,
		Sala sala, Oferta oferta, Integer periodo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o, " +
			" IN ( o.salas ) sala " +
			" WHERE o.periodo = :periodo " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND sala.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.curriculo = :curriculo " +
			" AND sala = :sala " );

		q.setParameter( "periodo", periodo );
		q.setParameter( "curriculo", oferta.getCurriculo() );
		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< CurriculoDisciplina > findBySala(
		InstituicaoEnsino instituicaoEnsino, Sala sala )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o, " +
			" IN ( o.disciplina.salas ) sala " +
			" WHERE sala = :sala " +
			" AND o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND sala.unidade.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
    public static List< CurriculoDisciplina > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
        return entityManager().createQuery(
        	" SELECT o FROM CurriculoDisciplina o " +
        	" WHERE o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
    }

	public static CurriculoDisciplina find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        CurriculoDisciplina cd = entityManager().find( CurriculoDisciplina.class, id );

        if ( cd != null && cd.getCurriculo() != null
        	&& cd.getCurriculo().getCurso() != null
        	&& cd.getCurriculo().getCurso().getTipoCurso() != null
        	&& cd.getCurriculo().getCurso().getTipoCurso().getInstituicaoEnsino() == instituicaoEnsino
        	&& cd.getDisciplina() != null
        	&& cd.getDisciplina().getTipoDisciplina() != null
        	&& cd.getDisciplina().getTipoDisciplina().getInstituicaoEnsino() != null
        	&& cd.getDisciplina().getTipoDisciplina().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return cd;
        }

        return null;
    }

	@SuppressWarnings( "unchecked" )
    public static List< CurriculoDisciplina > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o " +
			" WHERE o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	public static Map< String, CurriculoDisciplina > buildCurriculoDisciplinaPeriodoMap(
		List< CurriculoDisciplina >  curriculosDisciplina )
	{
		Map< String, CurriculoDisciplina > result
			= new HashMap< String, CurriculoDisciplina >();

		for ( CurriculoDisciplina cd : curriculosDisciplina )
		{
			String key = CurriculoDisciplina.getCurriculoDisciplinaPeriodoKey( cd );
			result.put( key, cd );
		}

		return result;
	}

	public static String getCurriculoDisciplinaPeriodoKey(
		CurriculoDisciplina cd )
	{
		String key = "";
		key += cd.getCurriculo().getId();
		key += "-";
		key += cd.getDisciplina().getId();
		key += "-";
		key += cd.getPeriodo();

		return key;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null
			|| !( obj instanceof CurriculoDisciplina ) )
		{
			return false;
		}

		CurriculoDisciplina other = (CurriculoDisciplina) obj;
		return this.getNaturalKeyString().equals( other.getNaturalKeyString() );
	}
}
