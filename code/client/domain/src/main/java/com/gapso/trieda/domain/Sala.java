package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
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
import javax.persistence.JoinTable;
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
@RooEntity( identifierColumn = "SAL_ID" )
@Table( name = "SALAS", uniqueConstraints =
@UniqueConstraint( columnNames = { "UNI_ID", "SAL_CODIGO" } ) )
public class Sala
	implements Serializable, Comparable< Sala >
{
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((codigo == null) ? 0 : codigo.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Sala other = (Sala) obj;
		if (codigo == null) {
			if (other.codigo != null)
				return false;
		} else if (!codigo.equals(other.codigo))
			return false;
		return true;
	}

	private static final long serialVersionUID = -2533999449644229682L;

    @NotNull
    @ManyToOne(targetEntity = TipoSala.class)
    @JoinColumn(name = "TSA_ID")
    private TipoSala tipoSala;

    @NotNull
    @ManyToOne(targetEntity = Unidade.class)
    @JoinColumn(name = "UNI_ID")
    private Unidade unidade;

    @NotNull
    @Column(name = "SAL_CODIGO")
    @Size(min = 1, max = 200)
    private String codigo;

    @NotNull
    @Column(name = "SAL_NUMERO")
    @Size(min = 1, max = 200)
    private String numero;

    @NotNull
    @Column(name = "SAL_ANDAR")
    @Size(min = 1, max = 20)
    private String andar;

    @NotNull
    @Column(name = "SAL_CAPACIDADE")
    @Min(1L)
    @Max(9999L)
    private Integer capacidade;

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE})
	@JoinTable(name="disciplinas_salas",
	joinColumns={ @JoinColumn(name="sal_id") },
	inverseJoinColumns={ @JoinColumn(name="dis_id") })
    private Set< Disciplina > disciplinas = new HashSet< Disciplina >();

    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE}, mappedBy = "salas")
    private Set< GrupoSala > gruposSala = new HashSet< GrupoSala >();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="sala")
    private Set< AtendimentoOperacional > atendimentosOperacionais =  new HashSet< AtendimentoOperacional >();

    @OneToMany(cascade = CascadeType.ALL, mappedBy="sala")
    private Set< AtendimentoTatico > atendimentosTaticos =  new HashSet< AtendimentoTatico >();

    @OneToMany(cascade = CascadeType.REMOVE, mappedBy = "sala")
    private Set< Fixacao > fixacoes = new HashSet< Fixacao >();
    
    public boolean isLaboratorio() {
    	return getTipoSala().getNome().equals(TipoSala.TIPO_LABORATORIO);
    }

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "TipoSala: " ).append( getTipoSala() ).append( ", " );
        sb.append( "Unidade: " ).append( getUnidade() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Numero: " ).append( getNumero() ).append( ", " );
        sb.append( "Andar: " ).append( getAndar() ).append( ", " );
        sb.append( "Capacidade: " ).append( getCapacidade() ).append(", ");
        sb.append( "Horarios: " ).append( getHorarios() == null ? "null" : getHorarios().size() ).append(", ");
        sb.append( "Disciplinas: " ).append(
        	getDisciplinas() == null ? "null" : getDisciplinas().size() ).append(", ");
        sb.append( "GruposSala: " ).append(
        	getGruposSala() == null ? "null" : getGruposSala().size() );
        sb.append( "Atendimentos Operacionais: " ).append(
        	getAtendimentosOperacionais() == null ? "null" : getAtendimentosOperacionais().size() );
        sb.append( "Atendimentos Taticos: " ).append(
        	getAtendimentosTaticos() == null ? "null" : getAtendimentosTaticos().size() );
        sb.append( "Fixacoes: " ).append( getFixacoes() == null ? "null" : getFixacoes().size() );

        return sb.toString();
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "SAL_ID")
    private Long id;

	@Version
    @Column(name = "version")
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

		InstituicaoEnsino instituicaoEnsino
			= this.getUnidade().getCampus().getInstituicaoEnsino();

        Sala find = Sala.find( this.getId(), instituicaoEnsino );
        if ( find == null )
        {
        	this.entityManager.persist( this );
        }
    }
	
	@Transactional
    public void persistAndPreencheHorarios()
	{
        persist();
        preencheHorarios();
    }
	
	@Transactional
	static public void preencheHorariosDasSalas(List<Sala> salas, List<SemanaLetiva> semanasLetivas) {
		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					hdc.getSalas().addAll(salas);
					hdc.merge();
				}
			}
		}
	}

	public void preencheHorarios()
	{
		InstituicaoEnsino instituicaoEnsino
			= this.getUnidade().getCampus().getInstituicaoEnsino();

		List< HorarioDisponivelCenario > listHdcs
			= this.getUnidade().getHorarios( instituicaoEnsino );

		for ( HorarioDisponivelCenario hdc : listHdcs )
		{
			hdc.getSalas().add( this );
			hdc.merge();
		}

		/*
		List< SemanaLetiva > listSemanasLetivas
			= SemanaLetiva.findAll( instituicaoEnsino );

		for ( SemanaLetiva semanaLetiva : listSemanasLetivas )
		{
			List< HorarioDisponivelCenario > listHdcs
				= this.getUnidade().getHorarios( instituicaoEnsino, semanaLetiva );

			for ( HorarioDisponivelCenario hdc : listHdcs )
			{
				hdc.getSalas().add( this );
				hdc.merge();
			}
		}
		*/
	}

	@Transactional
	public void remove()
	{
		this.remove( true, true );
	}

	@Transactional
    public void remove( boolean removeHorariosDisponiveisCenario,
    	boolean removeDisciplinas )
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        if ( this.entityManager.contains( this ) )
        {
			if ( removeHorariosDisponiveisCenario )
        	{
				this.removeHorariosDisponivelCenario();
        	}

			if ( removeDisciplinas )
			{
				this.removeDisciplinas();
			}

        	this.removeGruposSala();

            this.entityManager.remove( this );
        }
        else
        {
            Sala attached = this.entityManager.find(
            	this.getClass(), this.id );

			if ( attached != null )
			{
				if ( removeHorariosDisponiveisCenario )
	        	{
					attached.removeHorariosDisponivelCenario();
	        	}

				if ( removeDisciplinas )
				{
					attached.removeDisciplinas();
				}

            	attached.removeGruposSala();

            	this.entityManager.remove( attached );
			}
        }
    }
	
    @Transactional
    public void removeHorariosDisponivelCenario()
    {
    	Set< HorarioDisponivelCenario > horarios = this.getHorarios();

    	for ( HorarioDisponivelCenario horario : horarios )
    	{
    		horario.getSalas().remove( this );
    		horario.merge();
    	}
    }

    @Transactional
    public void removeDisciplinas()
    {
    	Set< Disciplina > disciplinas
    		= this.getDisciplinas();

    	for ( Disciplina disciplina : disciplinas )
    	{
    		disciplina.getSalas().remove( this );
    		disciplina.merge();
    	}
    }

    @Transactional
    public void removeGruposSala()
    {
    	Set< GrupoSala > gruposSala = this.getGruposSala();

    	for ( GrupoSala grupoSala : gruposSala )
    	{
    		grupoSala.getSalas().remove( this );
    		grupoSala.merge();
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
    public Sala merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Sala merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Sala().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int countLaboratorio(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.tipoSala.id = 2 " +
			" AND o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int countLaboratorio(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.tipoSala.id = 2 " +
			" AND o.unidade.campus = :campus " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int countSalaDeAula(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.tipoSala.id = 1 " +
			" AND o.unidade.campus.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static int countSalaDeAula(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.tipoSala.id = 1 " +
			" AND o.unidade.campus = :campus " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	public static Map< String, Sala > buildSalaCodigoToSalaMap( List< Sala > salas )
	{
		Map< String, Sala > salasMap
			= new HashMap< String, Sala >();

		for ( Sala sala : salas )
		{
			salasMap.put( sala.getCodigo(), sala );
		}

		return salasMap;
	}
	
	public static Map< Long, Sala > buildSalaIdToSalaMap( List< Sala > salas )
	{
		Map< Long, Sala > salasMap
			= new HashMap< Long, Sala >();

		for ( Sala sala : salas )
		{
			salasMap.put( sala.getId(), sala );
		}

		return salasMap;
	}

	public static List< Sala > findAndaresAll( InstituicaoEnsino instituicaoEnsino )
	{
		return findAndaresAll( instituicaoEnsino, null );
	}

	@SuppressWarnings("unchecked")
	public static List< Sala > findAndaresAll(
		InstituicaoEnsino instituicaoEnsino, Unidade unidade )
	{
		List< Sala > list;
		Query q = null;

		if ( unidade == null )
		{
			q = entityManager().createQuery(
				" SELECT o FROM Sala o " +
				" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
				" GROUP BY o.andar " );

			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			list = q.getResultList();
		}
		else
		{
			q = entityManager().createQuery(
				" SELECT o FROM Sala o " +
				" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
				" AND o.unidade = :unidade GROUP BY o.andar " );

			q.setParameter( "unidade", unidade );
			q.setParameter( "instituicaoEnsino", instituicaoEnsino );

			list = q.getResultList();
		}

		return list;
	}

	@SuppressWarnings("unchecked")
	public static List< Sala > findSalasDoAndarAll(
		InstituicaoEnsino instituicaoEnsino,
		Unidade unidade, List< String > andares )
	{
		if ( andares.size() == 0 )
		{
			return new ArrayList< Sala >();
		}

		String whereQuery = " SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino AND ( ";

		for ( int i = 1; i < andares.size(); i++ )
		{
			whereQuery += ( " o.andar = :andares" + i + " OR " );
		}

		whereQuery += ( " o.andar = :andares0 ) AND o.unidade = :unidade " );
		Query query = entityManager().createQuery( whereQuery );
		for ( int i = 0; i < andares.size(); i++ )
		{
			query.setParameter( "andares" + i, andares.get( i ) );
		}

		query.setParameter( "unidade", unidade );
		query.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return query.getResultList();
	}

	@SuppressWarnings("unchecked")
    public static List< Sala > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Sala o " +
    		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	public static Sala find( Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Sala sala = entityManager().find( Sala.class, id );

        if ( sala != null && sala.getUnidade() != null
        	&& sala.getUnidade().getCampus() != null
        	&& sala.getUnidade().getCampus().getInstituicaoEnsino() != null
        	&& sala.getUnidade().getCampus().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return sala;
        }

        return null;
    }

	@SuppressWarnings("unchecked")
	public static List< Sala > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Sala o " +
    		" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.unidade.campus.cenario = :cenario " );

    	q.setParameter( "cenario", cenario );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

	public static Sala findByCodigo(
		InstituicaoEnsino instituicaoEnsino, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND codigo = :codigo " );

		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return (Sala) q.getSingleResult();
	}

	@SuppressWarnings("unchecked")
	public static List< Sala > findByUnidade(
		InstituicaoEnsino instituicaoEnsino, Unidade unidade )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade = :unidade " );

		q.setParameter( "unidade", unidade );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static Integer count( InstituicaoEnsino instituicaoEnsino,
		Campus campus, Unidade unidade )
	{
		String whereString = " WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino ";

		if ( campus != null || unidade != null )
		{
			whereString += " AND ";
		}

		if ( campus != null )
		{
			whereString += " o.unidade.campus = :campus ";
		}

		if ( campus != null && unidade != null )
		{
			whereString += " AND ";
		}

		if ( unidade != null )
		{
			whereString += " o.unidade = :unidade ";
		}

		Query q = entityManager().createQuery(
			"SELECT COUNT ( o ) FROM Sala o " + whereString );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( campus != null )
		{
			q.setParameter( "campus", campus );
		}

		if ( unidade != null )
		{
			q.setParameter( "unidade", unidade );
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings("unchecked")
    public static List< Sala > find( InstituicaoEnsino instituicaoEnsino,
    	Campus campus, Unidade unidade, int firstResult, int maxResults, String orderBy )
    {
		String whereString = " WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino ";
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy.replace("String", "") : "" );

		if ( campus != null || unidade != null )
		{
			whereString += " AND ";
		}

		if ( campus != null )
		{
			whereString += " o.unidade.campus = :campus ";
		}

		if ( campus != null && unidade != null )
		{
			whereString += " AND ";
		}

		if ( unidade != null )
		{
			whereString += " o.unidade = :unidade ";
		}

		Query q = entityManager().createQuery(
			"SELECT o FROM Sala o " + whereString + orderBy);

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		if ( campus != null )
		{
			q.setParameter( "campus", campus );
		}

		if ( unidade != null )
		{
			q.setParameter( "unidade", unidade );
		}

        return q.getResultList();
    }
	
	@SuppressWarnings("unchecked")
    public static List< Sala > find( InstituicaoEnsino instituicaoEnsino,
    	String codigo, int firstResult, int maxResults )
    {
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND codigo LIKE LOWER (:codigo) " );

		q.setParameter( "codigo", "%"+codigo+"%" );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.salas ) c " +
			" WHERE c.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND c = :sala " );

		q.setParameter( "sala", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Sala o " +
			" WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.unidade.campus.cenario = :cenario " +
			" AND o.codigo = :codigo " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}

	public TipoSala getTipoSala()
	{
        return this.tipoSala;
    }

	public void setTipoSala( TipoSala tipoSala )
	{
        this.tipoSala = tipoSala;
    }

	public Unidade getUnidade()
	{
        return this.unidade;
    }

	public void setUnidade( Unidade unidade )
	{
        this.unidade = unidade;
    }

	public String getCodigo()
	{
        return this.codigo;
    }

	public void setCodigo( String codigo )
	{
        this.codigo = codigo;
    }

	public String getNumero()
	{
        return this.numero;
    }

	public void setNumero( String numero )
	{
        this.numero = numero;
    }

	public String getAndar()
	{
        return this.andar;
    }

	public void setAndar( String andar )
	{
        this.andar = andar;
    }

	public Integer getCapacidade()
	{
        return this.capacidade;
    }

	public void setCapacidade( Integer capacidade )
	{
        this.capacidade = capacidade;
    }

	private Set< HorarioDisponivelCenario > getHorarios()
	{
        return this.horarios;
    }

	public void setHorarios( Set< HorarioDisponivelCenario > horarios )
	{
        this.horarios = horarios;
    }

	public Set< Disciplina > getDisciplinas()
	{
        return this.disciplinas;
    }

	public void setDisciplinas(
		Set< Disciplina > disciplinas )
	{
        this.disciplinas = disciplinas;
    }

	public Set< GrupoSala > getGruposSala()
	{
        return this.gruposSala;
    }

	public void setGruposSala( Set< GrupoSala > gruposSala )
	{
        this.gruposSala = gruposSala;
    }

	public Set< AtendimentoOperacional > getAtendimentosOperacionais()
	{
		return this.atendimentosOperacionais;
	}

	public void setAtendimentosOperacionais(
		Set< AtendimentoOperacional > atendimentosOperacionais )
	{
		this.atendimentosOperacionais = atendimentosOperacionais;
	}

	public Set< AtendimentoTatico > getAtendimentosTaticos()
	{
		return this.atendimentosTaticos;
	}

	public void setAtendimentosTaticos(
		Set< AtendimentoTatico > atendimentosTaticos )
	{
		this.atendimentosTaticos = atendimentosTaticos;
	}

	public Set< Fixacao > getFixacoes()
	{
		return this.fixacoes;
	}

	public void setFixacoes( Set< Fixacao > fixacoes )
	{
		this.fixacoes = fixacoes;
	}

	//@Override
	public int compareTo( Sala o )
	{
		return getCodigo().compareTo( o.getCodigo() );
	}
}
