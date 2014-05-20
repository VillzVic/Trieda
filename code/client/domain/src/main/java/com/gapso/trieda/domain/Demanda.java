package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
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
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
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
@RooEntity( identifierColumn = "DEM_ID" )
@Table( name = "DEMANDAS" )
public class Demanda
	implements Serializable, Clonable< Demanda >
{
	private static final long serialVersionUID = -3935898184270072639L;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Oferta.class, fetch = FetchType.LAZY )
    @JoinColumn( name = "OFE_ID" )
    private Oferta oferta;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Disciplina.class )
    @JoinColumn( name = "DIS_ID" )
    private Disciplina disciplina;
    
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "demanda" )
	private Set< AlunoDemanda > alunosDemanda = new HashSet< AlunoDemanda >();

	public Oferta getOferta()
	{
        return this.oferta;
    }

	public void setOferta( Oferta oferta )
	{
        this.oferta = oferta;
    }

	public Disciplina getDisciplina()
	{
        return this.disciplina;
    }

	public void setDisciplina( Disciplina disciplina )
	{
        this.disciplina = disciplina;
    }

	public Integer getQuantidade()
	{
        return this.getAlunosDemanda().size();
    }
	
	/**
	 * Informa se a demanda ocupa grade de horários ou não. Uma demanda ocupa grade de horários nos seguintes casos:
	 *    - a demanda está associada com uma disciplina do tipo "Presencial" ou "Telepresencial";
	 *    - a demanda está associada com uma disciplina que possui créditos teóricos ou práticos;
	 * @return true caso a demanda ocupa grade de horários e false caso contrário
	 */
	public boolean ocupaGrade() {
		return this.disciplina.ocupaGrade();
	}

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "DEM_ID" )
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
		return getOferta().getNaturalKeyString() + "-" + 
		       getDisciplina().getCodigo();
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
        	this.removeAlunosDemanda();
            this.entityManager.remove( this );
        }
        else
        {
            Demanda attached = this.entityManager.find(
            	this.getClass(), this.id );

            attached.removeAlunosDemanda();
            this.entityManager.remove( attached );
        }
    }

	@Transactional
    public void removeAlunosDemanda()
	{
		List< AlunoDemanda > alunosDemanda = AlunoDemanda.findByDemanda(
			this.getOferta().getCampus().getInstituicaoEnsino(), this );

		for ( AlunoDemanda alunoDemanda : alunosDemanda )
		{
			alunoDemanda.remove();
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
    public Demanda merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Demanda merged = this.entityManager.merge( this );
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Demanda().entityManager;

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
			" SELECT COUNT ( o ) FROM Demanda o " +
			" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Demanda o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int sumDemanda(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( q.getSingleResult() == null ? 0 : (Number) q.getSingleResult() ).intValue();
	}

	public static int sumDemanda(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus = :campus " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campus", campus );

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}
	
	@SuppressWarnings("unchecked")
    public static List< Demanda > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Demanda o " +
        	" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
    }
	
	@SuppressWarnings("unchecked")
    public static List< Demanda > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Demanda o " +
        	" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.oferta.campus.cenario = :cenario " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setParameter( "cenario", cenario ).getResultList();
    }

	@SuppressWarnings("unchecked")
    public static Demanda findbyOfertaAndDisciplina(
    	InstituicaoEnsino instituicaoEnsino, Oferta oferta, Disciplina disciplina )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM Demanda o " +
    		" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.oferta = :oferta " +
    		" AND o.disciplina = :disciplina " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "disciplina", disciplina );
		q.setParameter( "oferta", oferta );

		List<Demanda> result = q.getResultList();
		if (result.size() > 0)
		{
			return result.get(0);
		}
		else
		{
			return null;
		}
    }

	@SuppressWarnings("unchecked")
	public static List< Demanda > findAllByCampus(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Demanda o " +
			" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campus", campus );

		return q.getResultList();
	}
	
	public static int count( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Campus campus, Curso curso, Curriculo curriculo,
		Turno turno, Disciplina disciplina,
		Integer periodo, String demandaRealOperador, Long demandaReal, String demandaVirtualOperador, 
		Long demandaVirtual, String demandaTotalOperador,Long  demandaTotal)
	{
		String queryString = "";
		if ( campus != null )
		{
			queryString	+= " o.oferta.campus = :campus AND ";
		}

		if ( curso != null )
		{
			queryString += " o.oferta.curriculo.curso = :curso AND ";
		}

		if ( curriculo != null )
		{
			queryString += " o.oferta.curriculo = :curriculo AND ";
			if(periodo != null){
				queryString += " o.disciplina in (select d from Disciplina d where d in" +
						" (select distinct c.disciplina from CurriculoDisciplina c where c.periodo = :periodo and c.curriculo = :curriculo)) and ";
			}
		}

		if ( turno != null )
		{
			queryString += " o.oferta.turno = :turno AND ";
		}

		if ( disciplina != null )
		{
			queryString	+= " o.disciplina = :disciplina AND ";
		}
		
		
		if(demandaReal != null){
			if(demandaRealOperador != null)
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is FALSE) "+demandaRealOperador+" :demandaReal and " ;
			else
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is FALSE) = :demandaReal and " ;
		}
		
		if(demandaVirtual != null){
			if(demandaVirtualOperador != null)
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is TRUE) "+demandaVirtualOperador+" :demandaVirtual and " ;
			else
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is TRUE) = :demandaVirtual and " ;
		}

		
		if(demandaTotal != null){
			if(demandaTotalOperador != null)
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o ) " + demandaTotalOperador + " :demandaTotal and " ;
			else
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o ) = :demandaTotal and " ;
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Demanda o " +
			" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus.cenario = :cenario " +
			" AND" + queryString + " 1=1 " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

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

		if ( turno != null )
		{
			q.setParameter( "turno", turno );
		}

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}
		
        if( periodo != null ){
        	q.setParameter("periodo", periodo);
        }
        
        if(demandaReal != null){
        	q.setParameter("demandaReal", demandaReal);
        }
        
        if(demandaVirtual != null){
        	q.setParameter("demandaVirtual", demandaVirtual);
        }

        if(demandaTotal != null){
        	q.setParameter("demandaTotal", demandaTotal);
        }
        
		return ( (Number)q.getSingleResult() ).intValue();
	}

    @SuppressWarnings( "unchecked" )
	public static List< Demanda > findByCampusTurno(
		InstituicaoEnsino instituicaoEnsino,
		Campus campus, Turno turno )
	{
		String queryCampus = "";
		if ( campus != null )
		{
			queryCampus	= " o.oferta.campus = :campus AND ";
		}

		String queryTurno = "";
		if ( turno != null )
		{
			queryTurno = " o.oferta.turno = :turno AND ";
		}

        String queryString = queryCampus + queryTurno;

        Query q = entityManager().createQuery(
        	" SELECT o FROM Demanda o " +
        	" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND " + queryString + " 1=1 " );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        if ( campus != null )
        {
        	q.setParameter( "campus", campus );
        }

        if ( turno != null )
        {
        	q.setParameter( "turno", turno );
        }

        return q.getResultList();
	}
    
    @SuppressWarnings("unchecked")
	public static List<Demanda> findBy(InstituicaoEnsino instituicaoEnsino, Collection<Campus> campi, Collection<Turno> turnos) {
		String queryCampi = "";
		if (campi != null && !campi.isEmpty()) {
			queryCampi = " o.oferta.campus IN ( :campi ) AND ";
		}

		String queryTurnos = "";
		if (turnos != null && !turnos.isEmpty()) {
			queryTurnos = " o.oferta.turno IN ( :turnos ) AND ";
		}

		String queryString = queryCampi + queryTurnos;

		Query q = entityManager().createQuery(
			" SELECT o FROM Demanda o "
			+ " WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino "
			+ " AND " + queryString + " 1=1 ");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);

		if (!queryCampi.isEmpty()) {
			q.setParameter("campi",campi);
		}

		if (!queryTurnos.isEmpty()) {
			q.setParameter("turnos", turnos);
		}

		return q.getResultList();
	}
	
    @SuppressWarnings( "unchecked" )
	public static List< Demanda > findBy( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, Campus campus, Curso curso, Curriculo curriculo,
		Turno turno, Disciplina disciplina,
		Integer periodo, String demandaRealOperador, Long demandaReal, String demandaVirtualOperador, 
		Long demandaVirtual, String demandaTotalOperador,Long  demandaTotal,
		int firstResult, int maxResults, String orderBy )
	{
    	
        if (orderBy != null)
        {
        	if (orderBy.contains("disciplina"))
        		orderBy = "ORDER BY o." + orderBy.replace("String", "");
        	else
        		orderBy = "ORDER BY o.oferta." + orderBy.replace("String", "");
        }
        else
        {
        	orderBy = "";
        }
        
        String queryString = "";

		if ( campus != null )
		{
			queryString	= " o.oferta.campus = :campus AND ";
		}

		if ( curso != null )
		{
			queryString += " o.oferta.curriculo.curso = :curso AND ";
		}

		if ( curriculo != null )
		{
			queryString += " o.oferta.curriculo = :curriculo AND ";
			if(periodo != null){
				queryString += " o.disciplina in (select d from Disciplina d where d in" +
						" (select distinct c.disciplina from CurriculoDisciplina c where c.periodo = :periodo and c.curriculo = :curriculo)) and ";
			}
		}

		if ( turno != null )
		{
			queryString += " o.oferta.turno = :turno AND ";
		}

		if ( disciplina != null )
		{
			queryString += " o.disciplina = :disciplina AND ";
		}
		
		
		if(demandaReal != null){
			if(demandaRealOperador != null)
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is FALSE) "+demandaRealOperador+" :demandaReal and " ;
			else
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is FALSE) = :demandaReal and " ;
		}
		
		if(demandaVirtual != null){
			if(demandaVirtualOperador != null)
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is TRUE) "+demandaVirtualOperador+" :demandaVirtual and " ;
			else
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o " +
					" AND a.aluno.criadoTrieda is TRUE) = :demandaVirtual and " ;
		}
		
		if(demandaTotal != null){
			if(demandaTotalOperador != null)
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o ) " + demandaTotalOperador + " :demandaTotal and " ;
			else
				queryString += " ( SELECT count(a) FROM AlunoDemanda a " +
					" WHERE a.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
					" AND a.demanda = o ) = :demandaTotal and " ;
		}

        

        Query q = entityManager().createQuery(
        	" SELECT o FROM Demanda o " +
        	" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.oferta.campus.cenario = :cenario " +
        	" AND " + queryString + " 1=1 " + orderBy );

        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );

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

        if ( turno != null )
        {
        	q.setParameter( "turno", turno );
        }

        if ( disciplina != null )
        {
        	q.setParameter( "disciplina", disciplina );
        }
        
        if(periodo != null){
        	q.setParameter("periodo", periodo);
        }
        
        if(demandaReal != null){
        	q.setParameter("demandaReal", demandaReal);
        }
        
        if(demandaVirtual != null){
        	q.setParameter("demandaVirtual", demandaVirtual);
        }
        
        
        if(demandaTotal != null){
        	q.setParameter("demandaTotal", demandaTotal);
        }

        return q.getResultList();
    }

	public static Demanda find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Demanda demanda = entityManager().find( Demanda.class, id );

        if ( demanda != null && demanda.getOferta() != null
        	&& demanda.getOferta().getCampus() != null
        	&& demanda.getOferta().getCampus().getInstituicaoEnsino() != null
        	&& demanda.getOferta().getCampus().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return demanda;
        }

        return null;
    }

	@SuppressWarnings("unchecked")
    public static List< Demanda > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
        return entityManager().createQuery(
        	" SELECT o FROM Demanda o " +
        	" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " )
        	.setParameter( "instituicaoEnsino", instituicaoEnsino )
        	.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
    }

	public static Map< String, Demanda > buildCampusTurnoCurriculoDisciplinaToDemandaMap(
		List< Demanda > demandas )
	{
		Map< String, Demanda > demandasMap
			= new HashMap< String, Demanda >();

		for ( Demanda demanda : demandas )
		{
			String codigo = Demanda.getCodeDemanda( demanda );
			demandasMap.put( codigo, demanda );
		}

		return demandasMap;
	}

	// [ Campus + Turno + Matriz Curricular + Disciplina ]
	private static String getCodeDemanda( Demanda domain )
	{
		String codigo = domain.getOferta().getCampus().getCodigo()
			+ "-" + domain.getOferta().getTurno().getNome()
			+ "-" + domain.getOferta().getCurriculo().getCodigo()
			+ "-" + domain.getDisciplina().getCodigo();

		return codigo;
	}
	
	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Oferta: " ).append( getOferta() ).append( ", " );
        sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
        sb.append( "Quantidade: " ).append( getQuantidade() );

        return sb.toString();
    }

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Demanda ) )
		{
			return false;
		}

		Demanda other = (Demanda) obj;

		// Comparando os id's
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

	public static boolean existeDemanda(
		InstituicaoEnsino instituicaoEnsino,
		Curriculo curriculo, Disciplina disciplina )
	{
		String queryCurriculo = "";

		if ( curriculo != null )
		{
			queryCurriculo = " AND o.oferta.curriculo = :curriculo ";
		}

        Query q = entityManager().createQuery(
           	" SELECT o FROM Demanda o " +
           	" WHERE o.disciplina = :disciplina " + queryCurriculo +
           	" AND o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "disciplina", disciplina );

        if ( curriculo != null )
        {
        	q.setParameter( "curriculo", curriculo );
        }

		return ( q.getResultList().size() > 0 );
	}

	public static boolean existeDemanda(
		InstituicaoEnsino instituicaoEnsino,
		Curriculo curriculo, Disciplina disciplina, Integer periodo )
	{
		boolean existeDemanda = Demanda.existeDemanda(
			instituicaoEnsino, curriculo, disciplina );

		if ( !existeDemanda )
		{
			return false;
		}

		Set< CurriculoDisciplina > disciplinas
			= curriculo.getDisciplinas();

		for ( CurriculoDisciplina cd : disciplinas )
		{
			if ( cd.getPeriodo() == periodo
				&& cd.getDisciplina() == disciplina )
			{
				existeDemanda = true;
				break;
			}
		}

		return existeDemanda;
	}

	@SuppressWarnings("unchecked")
	public static List<Demanda> findBy(InstituicaoEnsino instituicaoEnsino, Cenario cenario, Campus campus, Disciplina disciplina)
	{
		String queryCampus = "";
		if ( campus != null )
		{
			queryCampus	= " o.oferta.campus = :campus AND ";
		}

		String queryDisciplina = "";
		if ( disciplina != null )
		{
			queryDisciplina	= " o.disciplina = :disciplina AND ";
		}

        String queryString = queryCampus + queryDisciplina;

        Query q = entityManager().createQuery(
        	" SELECT o FROM Demanda o " +
        	" WHERE o.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.oferta.campus.cenario = :cenario " +
        	" AND " + queryString + " 1=1 " );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );

        if ( campus != null )
        {
        	q.setParameter( "campus", campus );
        }

        if ( disciplina != null )
        {
        	q.setParameter( "disciplina", disciplina );
        }

        return q.getResultList();
	}

	public Set< AlunoDemanda > getAlunosDemanda() {
		return alunosDemanda;
	}

	public void setAlunosDemanda(Set< AlunoDemanda > alunosDemanda) {
		this.alunosDemanda = alunosDemanda;
	}

	public Demanda clone(CenarioClone novoCenario) {
		Demanda clone = new Demanda();
		clone.setDisciplina(novoCenario.getEntidadeClonada(this.getDisciplina()));
		clone.setOferta(novoCenario.getEntidadeClonada(this.getOferta()));
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Demanda entidadeClone) {
		for (AlunoDemanda alunoDemanda : this.getAlunosDemanda())
		{
			entidadeClone.getAlunosDemanda().add(novoCenario.clone(alunoDemanda));
		}
	}
}
