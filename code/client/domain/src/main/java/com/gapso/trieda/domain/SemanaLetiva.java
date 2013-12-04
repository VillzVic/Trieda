package com.gapso.trieda.domain;

import java.io.Serializable;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
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

import com.gapso.trieda.misc.Semanas;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "SLE_ID" )
@Table( name = "SEMANA_LETIVA", uniqueConstraints =
@UniqueConstraint( columnNames = { "SLE_CODIGO" } ) )
public class SemanaLetiva
	implements Serializable, Comparable< SemanaLetiva >
{
	private static final long serialVersionUID = 6807360646327130208L;

	@NotNull
	@Column( name = "SLE_CODIGO" )
	@Size( min = 1, max = 500 )
	private String codigo;

	@NotNull
	@Column( name = "SLE_DESCRICAO" )
	@Size( max = 500 )
	private String descricao;

    @NotNull
    @Column( name = "SLE_TEMPO" )
    @Min( 1L )
    @Max( 1000L )
    private Integer tempo;

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "semanaLetiva" )
	private Set< HorarioAula > horariosAula = new HashSet< HorarioAula >();

	@OneToMany( mappedBy = "semanaLetiva" )
	private Set< Curriculo > curriculos = new HashSet< Curriculo >();

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

	public Set< Curriculo > getCurriculos()
	{
		return this.curriculos;
	}

	public void setCurriculos(
		Set< Curriculo > curriculos )
	{
		this.curriculos = curriculos;
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

	public Set< HorarioAula > getHorariosAula()
	{
		return this.horariosAula;
	}

	public void setHorariosAula(
		Set< HorarioAula > horariosAula )
	{
		this.horariosAula = horariosAula;
	}

	public Integer getTempo()
	{
		return this.tempo;
	}

	public void setTempo( Integer tempo )
	{
		this.tempo = tempo;
	}
	
	public int calcTotalCreditosSemanais(Turno turno) {
		int totalCreditos = 0;
		for (HorarioAula horarioAula : this.getHorariosAula()) {
			if (horarioAula.getTurno().equals(turno)) {
				totalCreditos += horarioAula.getHorariosDisponiveisCenario().size();
			}
		}
		return totalCreditos;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "SLE_ID" )
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
			SemanaLetiva attached = this.entityManager.find(
				this.getClass(), this.id );

			if ( attached != null )
			{
				this.entityManager.remove( attached );
			}
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
	public SemanaLetiva merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		SemanaLetiva merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new SemanaLetiva().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}
	
	public static SemanaLetiva getSemanaLetivaComMaiorCargaHoraria(Collection<SemanaLetiva> semanasLetivas) {
		SemanaLetiva semanaLetivaComMaiorCargaHoraria = null;
		int maxCargaHoraria = 0;
		for (SemanaLetiva semLet : semanasLetivas) {
			int calcMaxCreditos = semLet.calculaMaxCreditos();
			int calcMaxCargaHoraria = calcMaxCreditos * semLet.getTempo();
			if (calcMaxCargaHoraria > maxCargaHoraria) {
				maxCargaHoraria = calcMaxCargaHoraria;
				semanaLetivaComMaiorCargaHoraria = semLet;
			}
		}
		return semanaLetivaComMaiorCargaHoraria;
	}
	
	public static TriedaPar<Integer, Boolean> caculaMaximoDivisorComumParaTemposDeAulaDasSemanasLetivas(Collection<SemanaLetiva> semanasLetivas) {
		int mdc = 1;
		boolean temIntersecao = false;
		Iterator<SemanaLetiva> it = semanasLetivas.iterator();
		if (semanasLetivas.size() == 1) {
			mdc = it.next().getTempo();
		} else {
			//Obtem todosos horariosde aulas das semanas letivas e os ordena
			Set<HorarioAula> horariosAulas = new HashSet<HorarioAula>();
			for (SemanaLetiva semanaLetiva : semanasLetivas ){
				horariosAulas.addAll(semanaLetiva.getHorariosAula());
			}
			List<HorarioAula> horariosAulasOrdenadosAulas = new ArrayList<HorarioAula>(horariosAulas);
			Collections.sort(horariosAulasOrdenadosAulas);
			
			//Calcula o tempo entre os horarios ordenados em minutos
			Set<Long> intervaloHorarios = new HashSet<Long>();
			HorarioAula horarioAulaAnterior = horariosAulasOrdenadosAulas.get(0);
			for (HorarioAula horarioAula : horariosAulasOrdenadosAulas ){
		    	Calendar horarioAnterior = Calendar.getInstance();
		    	horarioAnterior.setTime(horarioAulaAnterior.getHorario());
		    	horarioAnterior.set(1979,Calendar.NOVEMBER,6);
		    	
		    	Calendar horarioAtual = Calendar.getInstance();
		    	horarioAtual.setTime(horarioAula.getHorario());
		    	horarioAtual.set(1979,Calendar.NOVEMBER,6);
		    	
				Long intervaloHorario = ((horarioAtual.getTimeInMillis() - horarioAnterior.getTimeInMillis())/60000);
				if(intervaloHorario != 0 && intervaloHorario < horarioAulaAnterior.getSemanaLetiva().getTempo()){
					temIntersecao = true;
				}
				intervaloHorarios.add(intervaloHorario);
				horarioAulaAnterior = horarioAula;
			}
			if(!temIntersecao){
				if (semanasLetivas.size() == 2) {
					BigInteger tempo1 = BigInteger.valueOf(it.next().getTempo().longValue());
					BigInteger tempo2 = BigInteger.valueOf(it.next().getTempo().longValue());
					mdc = tempo1.gcd(tempo2).intValue();
				} else {
					BigInteger tempo1 = BigInteger.valueOf(it.next().getTempo().longValue());
					BigInteger tempo2 = BigInteger.valueOf(it.next().getTempo().longValue());
					mdc = tempo1.gcd(tempo2).intValue();
					while (it.hasNext()) {
						tempo1 = BigInteger.valueOf(it.next().getTempo().longValue());
						tempo2 = BigInteger.valueOf(mdc);
						mdc = tempo1.gcd(tempo2).intValue();
					}
				}
			} else{			
				//Calcula o mdc dos tempos entre os horarios
				Iterator<Long> it2 = intervaloHorarios.iterator();
				BigInteger tempo1 = BigInteger.valueOf(it2.next());
				BigInteger tempo2 = BigInteger.valueOf(it2.next());
				mdc = tempo1.gcd(tempo2).intValue();
				while (it2.hasNext()) {
					tempo1 = BigInteger.valueOf(it2.next());
					tempo2 = BigInteger.valueOf(mdc);
					mdc = tempo1.gcd(tempo2).intValue();
				}
			}
		}
		return TriedaPar.create(mdc, temIntersecao) ;
	}

	public static Map< String, SemanaLetiva > buildSemanaLetivaCodigoToSemanaLetivaMap(
		List< SemanaLetiva > semanasLetivas )
	{
		Map< String, SemanaLetiva> semanasLetivasMap
			= new HashMap< String, SemanaLetiva >();

		for ( SemanaLetiva semanaLetiva : semanasLetivas )
		{
			semanasLetivasMap.put( semanaLetiva.getCodigo(), semanaLetiva );
		}

		return semanasLetivasMap;
	}
	
	public static Map<Long, SemanaLetiva> buildSemanaLetivaIDToSemanaLetivaMap(List<SemanaLetiva> semanasLetivas) {
		Map<Long,SemanaLetiva> semanasLetivasMap = new HashMap<Long,SemanaLetiva>();

		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			semanasLetivasMap.put(semanaLetiva.getId(),semanaLetiva);
		}

		return semanasLetivasMap;
	}

	@SuppressWarnings( "unchecked" )
	public static List< SemanaLetiva > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		List< SemanaLetiva > result = null;

		try
		{
			result = q.getResultList();
		}
		catch( Exception e )
		{
			result = null;
		}

		return result;
	}

	public static SemanaLetiva find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		SemanaLetiva semanaLetiva
			= entityManager().find( SemanaLetiva.class, id );

		if ( semanaLetiva != null
			&& semanaLetiva.getInstituicaoEnsino() != null
			&& semanaLetiva.getInstituicaoEnsino() == instituicaoEnsino )
		{
			return semanaLetiva;
		}

		return null;
	}

	public static List< SemanaLetiva > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< SemanaLetiva > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " + orderBy );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino,
		String codigo, String descricao )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		descricao = ( ( descricao == null ) ? "" : descricao );
		descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );

		EntityManager em = Turno.entityManager();
		Query q = em.createQuery(
			" SELECT COUNT ( o ) FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) " +
			" AND LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " );

		q.setParameter( "codigo", codigo );
		q.setParameter( "descricao", descricao );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< SemanaLetiva > findBy(
		InstituicaoEnsino instituicaoEnsino, String codigo,
		String descricao, int firstResult, int maxResults, String orderBy )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		descricao = ( ( descricao == null ) ? "" : descricao );
		descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );

		EntityManager em = Turno.entityManager();
		orderBy = ( ( orderBy != null ) ? "ORDER BY o." + orderBy : "" );

		Query q = em.createQuery(
			" SELECT o FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND LOWER ( o.codigo ) LIKE  LOWER ( :codigo ) " +
			" AND LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " + orderBy );

		q.setParameter( "codigo", codigo );
		q.setParameter( "descricao", descricao );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM SemanaLetiva o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.codigo = :codigo " );

		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = ( (Number) ( q.setMaxResults( 1 ).getSingleResult() ) );
		return ( size.intValue() > 0 );
	}

    public int calculaMaxCreditos ()
    {
    	Map< Integer, Integer > countHorariosAula
    		= new HashMap< Integer, Integer >();

		for ( HorarioAula ha : this.getHorariosAula() )
		{
			for ( HorarioDisponivelCenario hdc
				: ha.getHorariosDisponiveisCenario() )
			{
				int semanaInt = Semanas.toInt( hdc.getDiaSemana() );
				Integer value = countHorariosAula.get( semanaInt );
				value = ( ( value == null ) ? 0 : value );
				countHorariosAula.put( semanaInt, value + 1 );
			}
		}

		return Collections.max( countHorariosAula.values() );
    }
    
    public long calculaMaiorIntervalo()
    {
		long maiorIntervalod = 0;
		
		Map<Semanas, List<HorarioAula>> semanaMapHorarios = new HashMap<Semanas, List<HorarioAula>>();
		for (HorarioAula horario : this.getHorariosAula())
		{
			for (HorarioDisponivelCenario hdc : horario.getHorariosDisponiveisCenario())
			{
				if (semanaMapHorarios.get(hdc.getDiaSemana()) == null)
				{
					List<HorarioAula> newHorarios = new ArrayList<HorarioAula>();
					newHorarios.add(horario);
					semanaMapHorarios.put(hdc.getDiaSemana(), newHorarios);
				}
				else
				{
					semanaMapHorarios.get(hdc.getDiaSemana()).add(horario);
				}
			}
		}
		for (Entry<Semanas, List<HorarioAula>> horarioSemana : semanaMapHorarios.entrySet())
		{
		   	List<HorarioAula> horarioOrdenado = new ArrayList<HorarioAula>();
	    	horarioOrdenado.addAll(horarioSemana.getValue());  	
			Collections.sort(horarioOrdenado);	
			if (!horarioOrdenado.isEmpty())
			{
				for (int i = 1; i < horarioOrdenado.size(); i++)
				{
					Calendar h1 = Calendar.getInstance();
					h1.setTime(horarioOrdenado.get(i-1).getHorario());
					h1.add(Calendar.MINUTE,horarioOrdenado.get(i-1).getSemanaLetiva().getTempo());
					Calendar h2 = Calendar.getInstance();
					h2.setTime(horarioOrdenado.get(i).getHorario());
					
					if (h2.getTimeInMillis() - h1.getTimeInMillis() > maiorIntervalod)
					{
						maiorIntervalod = h2.getTimeInMillis() - h1.getTimeInMillis();
					}
				}
			}
		}
		
		return maiorIntervalod;
    }

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicoes de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
		sb.append( "Tempo: " ).append( getTempo() ).append( ", " );
		sb.append( "HorariosAula: " ).append(
			getHorariosAula() == null ? "null" : getHorariosAula().size() ).append( ", " );
		sb.append( "Descricao: " ).append( getDescricao() ).append( ", " );

		return sb.toString();
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof SemanaLetiva ) )
		{
			return false;
		}

		SemanaLetiva other = (SemanaLetiva) obj;

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
	public int compareTo( SemanaLetiva o )
	{
		int result = getInstituicaoEnsino().compareTo( o.getInstituicaoEnsino() );
		
		if ( result == 0 )
		{
			result = getCodigo().compareTo( o.getCodigo() );
		}

		return result;
	}
}
