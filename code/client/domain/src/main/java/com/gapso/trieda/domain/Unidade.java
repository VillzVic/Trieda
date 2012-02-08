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
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
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
@RooEntity( identifierColumn = "UNI_ID" )
@Table( name = "UNIDADES", uniqueConstraints =
@UniqueConstraint( columnNames = { "CAM_ID", "UNI_CODIGO" } ) )
public class Unidade implements Serializable
{
    private static final long serialVersionUID = -5763084706316974453L;

    @NotNull
    @ManyToOne( targetEntity = Campus.class )
    @JoinColumn( name = "CAM_ID" )
    private Campus campus;

    @NotNull
    @Column( name = "UNI_CODIGO" )
    @Size( min = 1, max = 20 )
    private String codigo;

    @NotNull
    @Column( name = "UNI_NOME" )
    @Size( min = 1, max = 500 )
    private String nome;

    @OneToMany( cascade = CascadeType.ALL, mappedBy = "origem" )
    private Set< DeslocamentoUnidade > deslocamentos = new HashSet< DeslocamentoUnidade >();

    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "unidades" )
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();

    @OneToMany( cascade = CascadeType.ALL, mappedBy="unidade" )
    private Set< GrupoSala > gruposSalas = new HashSet< GrupoSala >();

    @OneToMany( cascade = CascadeType.ALL, mappedBy="unidade" )
    private Set< Sala > salas = new HashSet< Sala >();

    @PersistenceContext
    transient EntityManager entityManager;

    @Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "UNI_ID" )
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
    
    public Set<Sala> getLaboratorios() {
		Set<Sala> laboratorios = new HashSet<Sala>();
		for (Sala sala : this.salas) {
			if (sala.isLaboratorio()) {
				laboratorios.add(sala);
			}
		}
		return laboratorios;
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
    public void persistAndPreencheHorarios()
    {
    	persist();
        preencheHorarios();
    }
    
    @Transactional
	static public void preencheHorariosDasUnidades(List<Unidade> unidades, List<SemanaLetiva> semanasLetivas) {
		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					hdc.getUnidades().addAll(unidades);
					hdc.merge();
				}
			}
		}
	}

    @Transactional
    public void remove()
    {
    	this.remove( true, true );
    }

    @Transactional
    public void remove( boolean removeHorariosDisponiveisCenario,
    	boolean removeCurriculosDisciplinas )
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

        	if ( removeCurriculosDisciplinas )
        	{
        		removeCurriculoDisciplinas();
        	}

        	this.removeSalas( false );
        	this.removeDeslocamentos();

            this.entityManager.remove( this );
        }
        else
        {
            Unidade attached = this.entityManager.find(
            	this.getClass(), this.id );

			if ( attached != null )
			{
				if ( removeHorariosDisponiveisCenario )
				{
					attached.removeHorariosDisponivelCenario();
				}

	        	if ( removeCurriculosDisciplinas )
	        	{
	        		removeCurriculoDisciplinas();
	        	}

            	attached.removeSalas( false );
            	attached.removeDeslocamentos();

            	this.entityManager.remove( attached );
			}
        }
    }

    @Transactional
    private void removeCurriculoDisciplinas()
    {
    	Set< CurriculoDisciplina > curriculoDisciplinas
    		= new HashSet< CurriculoDisciplina >();

    	List< Sala > listSalas = Sala.findByUnidade(
    		this.getCampus().getInstituicaoEnsino(), this );

    	for ( Sala sala : listSalas )
    	{
    		List< CurriculoDisciplina > listCurriculosDisciplinas
    			= CurriculoDisciplina.findBySala(
    				this.getCampus().getInstituicaoEnsino(), sala );

			for ( CurriculoDisciplina cd : listCurriculosDisciplinas )
			{
				curriculoDisciplinas.add( cd );
			}
    	}

    	for ( CurriculoDisciplina cd : curriculoDisciplinas )
    	{
    		for ( Sala s : this.getSalas() )
    		{
    			cd.getSalas().remove( s );
    			cd.merge();
    		}
    	}
    }

	private void preencheHorarios()
	{
		List< HorarioDisponivelCenario > listHdcs = this.getCampus().getHorarios(
				this.getCampus().getInstituicaoEnsino() );

		for ( HorarioDisponivelCenario hdc : listHdcs )
		{
			hdc.getUnidades().add( this );
			hdc.merge();
		}

		/*
		List< SemanaLetiva > semanasLetivas = SemanaLetiva.findAll(
			this.getCampus().getInstituicaoEnsino() );

		for ( SemanaLetiva semanaLetiva : semanasLetivas )
		{
			List< HorarioDisponivelCenario > listHdcs
				= this.getCampus().getHorarios(
					this.getCampus().getInstituicaoEnsino(), semanaLetiva );

			for ( HorarioDisponivelCenario hdc : listHdcs )
			{
				hdc.getUnidades().add( this );
				hdc.merge();
			}
		}
		*/
	}

    @Transactional
    private void removeHorariosDisponivelCenario()
    {
    	List< HorarioDisponivelCenario > horarios
    		= HorarioDisponivelCenario.findAll( this.getCampus().getInstituicaoEnsino() );

		List< Sala > salasUnidade = new ArrayList< Sala >();
		for ( Sala s : this.getSalas() )
		{
			salasUnidade.add( s );
		}

    	for ( HorarioDisponivelCenario horario : horarios )
    	{
    		for ( Sala s : salasUnidade )
    		{
        		horario.getSalas().remove( s );
        		horario.merge();
    		}

    		horario.getUnidades().remove( this );
    		horario.merge();
    	}
    }

	@Transactional
	private void removeDeslocamentos()
	{
		List< DeslocamentoUnidade > deslocamentos
			= DeslocamentoUnidade.findAllByUnidade(
				this.getCampus().getInstituicaoEnsino(), this );

		for ( DeslocamentoUnidade deslocamento : deslocamentos )
		{
			deslocamento.remove();
		}
	}

	@Transactional
	private void removeSalas( boolean removeCurriculosDisciplinas )
	{
		Set< Sala > salas = this.getSalas();

		for ( Sala sala : salas )
		{
			sala.remove( false, removeCurriculosDisciplinas );
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
    public Unidade merge()
    {
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Unidade merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

    public static final EntityManager entityManager()
    {
        EntityManager em = new Unidade().entityManager;

        if ( em == null )
        {
        	throw new IllegalStateException(
        		"Entity manager has not been injected (is the Spring " +
        		"Aspects JAR configured as an AJC/AJDT aspects library?)" );
        }

        return em;
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Unidade o " +
    		" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findByCenario(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM Unidade o " +
    		" WHERE o.campus.cenario = :cenario " +
    		" AND o.campus.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "cenario", cenario );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino ); 

    	return q.getResultList();
    }

    public static Map< String, Unidade > buildUnidadeCodigoToUnidadeMap(
    	List< Unidade > unidades )
    {
		Map< String, Unidade > unidadesMap
			= new HashMap< String, Unidade >();

		if ( unidades != null && unidades.size() > 0 )
		{
			for ( Unidade unidade : unidades )
			{
				unidadesMap.put( unidade.getCodigo(), unidade );
			}
		}

		return unidadesMap;
	}

    public static Unidade find( Long id, InstituicaoEnsino instituicaoEnsino )
    {
        if ( id == null && instituicaoEnsino == null )
        {
        	return null;
        }

        Unidade u = entityManager().find( Unidade.class, id );
        if ( u != null
        	&& u.getCampus().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return u;
        }

        return null;
    }

    public static List< Unidade > find( InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults )
    {
        return find( instituicaoEnsino, firstResult, maxResults, null );
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > find( InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults, String orderBy )
    {
        orderBy = ( ( orderBy != null ) ? "ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
            " SELECT o FROM Unidade o " +
            " WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " + orderBy );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        return q.getResultList();
    }

    public static Integer getCapacidadeMedia(
    	InstituicaoEnsino instituicaoEnsino, Unidade unidade )
    {
    	Query q =  entityManager().createQuery(
    		" SELECT AVG ( o.capacidade ) FROM Sala o " +
    		" WHERE o.unidade = :unidade " +
    		" AND o.unidade.campus.instituicaoEnsino = :instituicaoEnsino " );

    	q.setParameter( "unidade", unidade );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	Object obj = q.getSingleResult();

    	if ( obj == null )
    	{
    		return 0;
    	}

    	return ( (Number) obj ).intValue();
    }

    @SuppressWarnings("unchecked")
    public static List< Unidade > findByCampus(
    	InstituicaoEnsino instituicaoEnsino, Campus campus )
    {
    	Query q = entityManager().createQuery(
    		" SELECT Unidade FROM Unidade AS unidade " +
    		" WHERE unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND unidade.campus = :campus ORDER BY unidade.nome ASC " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

    public static int count( InstituicaoEnsino instituicaoEnsino,
    	Campus campus, String nome, String codigo )
    {
        nome = ( ( nome == null ) ? "" : nome );
        nome = ( "%" + nome.replace( '*', '%' ) + "%" );
        codigo = ( ( codigo == null ) ? "" : codigo );
        codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

        String queryCampus = "";
        if ( campus != null )
        {
        	queryCampus = " unidade.campus = :campus AND ";
        }

        Query q = entityManager().createQuery(
        	" SELECT COUNT ( Unidade ) FROM Unidade AS unidade " +
        	" WHERE " + queryCampus +
        	" LOWER ( unidade.nome ) LIKE LOWER ( :nome ) " +
        	" AND unidade.campus.instituicaoEnsino = :instituicaoEnsino " + 
        	" AND LOWER ( unidade.codigo ) LIKE LOWER ( :codigo ) " );

        if ( campus != null )
        {
        	q.setParameter( "campus", campus );
        }

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "nome", nome );
        q.setParameter( "codigo", codigo );

        return ( (Number) q.getSingleResult() ).intValue();
    }

    @SuppressWarnings("unchecked")
	public static List< Unidade > findBy(
		InstituicaoEnsino instituicaoEnsino, Campus campus, String nome,
		String codigo, int firstResult, int maxResults, String orderBy )
	{
        nome = ( ( nome == null )? "" : nome );
        nome = ( "%" + nome.replace( '*', '%' ) + "%" );
        codigo = ( ( codigo == null )? "" : codigo );
        codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );
        String queryCampus = "";
        if ( campus != null )
        {
        	queryCampus = " unidade.campus = :campus AND ";
        }

        Query q = entityManager().createQuery(
        	"SELECT Unidade FROM Unidade AS unidade " +
        	" WHERE " + queryCampus +
        	" LOWER ( unidade.nome ) LIKE LOWER ( :nome ) " +
        	" AND unidade.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND LOWER ( unidade.codigo ) LIKE LOWER ( :codigo ) " );

        if ( campus != null )
        {
        	q.setParameter( "campus", campus );
        }

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "nome", nome );
        q.setParameter( "codigo", codigo );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.unidades ) u " +
			" WHERE u = :unidade " +
			" AND u.campus.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "unidade", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList(); 
	}

	public static boolean checkCodigoUnique( 
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Unidade o " +
			" WHERE o.campus.cenario = :cenario " +
			" AND o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.codigo = :codigo " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setMaxResults( 1 );

		Number size = (Number) q.getSingleResult();
		return ( size.intValue() > 0 );
	}

    public Campus getCampus()
    {
        return this.campus;
    }

    public void setCampus( Campus campus )
    {
        this.campus = campus;
    }

    public String getCodigo()
    {
        return this.codigo;
    }

    public void setCodigo( String codigo )
    {
        this.codigo = codigo;
    }

    public String getNome()
    {
        return this.nome;
    }

    public void setNome( String nome )
    {
        this.nome = nome;
    }

    public Set< DeslocamentoUnidade > getDeslocamentos()
    {
        return this.deslocamentos;
    }

    public void setDeslocamentos( Set< DeslocamentoUnidade > deslocamentos )
    {
        this.deslocamentos = deslocamentos;
    }

    private Set< HorarioDisponivelCenario > getHorarios()
    {
        return this.horarios;
    }

    public void setHorarios( Set< HorarioDisponivelCenario > horarios )
    {
        this.horarios = horarios;
    }

    public Set< GrupoSala > getGruposSalas()
    {
    	return this.gruposSalas;
    }

    public void setGruposSalas( Set< GrupoSala > gruposSalas )
    {
    	this.gruposSalas = gruposSalas;
    }

    public Set< Sala > getSalas()
    {
    	return this.salas;
    }

    public void setSalas( Set< Sala > salas )
    {
    	this.salas = salas;
    }

    public String toString()
    {
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: ").append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Campus: " ).append( getCampus() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Nome: " ).append( getNome() ).append( ", " );
        sb.append( "Deslocamentos: " ).append(
        	getDeslocamentos() == null ? "null" : getDeslocamentos().size() ).append( ", " );
        sb.append( "Horarios: ").append( getHorarios() == null ? "null" : getHorarios().size() );
        sb.append( "GruposSalas: " ).append( getGruposSalas() == null ? "null" : getGruposSalas().size() );
        sb.append( "Salas: ").append( getSalas() == null ? "null" : getSalas().size() );

        return sb.toString();
    }
}
