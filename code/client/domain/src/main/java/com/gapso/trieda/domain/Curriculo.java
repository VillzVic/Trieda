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
@RooEntity( identifierColumn = "CRC_ID" )
@Table( name = "CURRICULOS", uniqueConstraints =
@UniqueConstraint( columnNames = { "CRC_COD", "CUR_ID" } ) )
public class Curriculo
	implements Serializable, Comparable< Curriculo >, Clonable< Curriculo >
{
	private static final long serialVersionUID = -9204016994046445376L;

    @NotNull
    @ManyToOne( targetEntity = Curso.class )
    @JoinColumn( name = "CUR_ID" )
    private Curso curso;

    @NotNull
    @ManyToOne( targetEntity = SemanaLetiva.class )
    @JoinColumn( name = "SLE_ID" )
    private SemanaLetiva semanaLetiva;

    @NotNull
    @ManyToOne( targetEntity = Cenario.class )
    @JoinColumn( name = "CEN_ID" )
    private Cenario cenario;

    @NotNull
    @Column( name = "CRC_COD" )
    @Size( min = 1, max = 20 )
    private String codigo;

    @Column( name = "CRC_DESCRICAO" )
    @Size( max = 255 )
    private String descricao;

    @OneToMany( cascade = CascadeType.ALL, mappedBy = "curriculo" )
    private Set< CurriculoDisciplina > disciplinas = new HashSet< CurriculoDisciplina >();

    @OneToMany( cascade = CascadeType.ALL, mappedBy = "curriculo" )
    private Set< Oferta > ofertas = new HashSet< Oferta >();

	public SemanaLetiva getSemanaLetiva()
	{
		return this.semanaLetiva;
	}

	public void setSemanaLetiva(
		SemanaLetiva semanaLetiva )
	{
		this.semanaLetiva = semanaLetiva;
	}

	public Curso getCurso()
	{
        return this.curso;
    }

	public void setCurso( Curso curso )
	{
        this.curso = curso;
    }

	public Cenario getCenario()
	{
        return this.cenario;
    }

	public void setCenario( Cenario cenario )
	{
        this.cenario = cenario;
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

	public Set< CurriculoDisciplina > getDisciplinas()
	{
        return this.disciplinas;
    }

	public void setDisciplinas(
		Set< CurriculoDisciplina > disciplinas )
	{
        this.disciplinas = disciplinas;
    }

	public Set< Oferta > getOfertas()
	{
        return this.ofertas;
    }

	public void setOfertas(
		Set< Oferta > ofertas )
	{
        this.ofertas = ofertas;
    }

	@PersistenceContext
    transient EntityManager entityManager;

	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "CRC_ID" )
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
            Curriculo attached = this.entityManager.find(
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
    public Curriculo merge()
	{
        if ( this.entityManager == null )
        {
        	this.entityManager = entityManager();
        }

        Curriculo merged = this.entityManager.merge( this );
        this.entityManager.flush();
        return merged;
    }

	public static final EntityManager entityManager()
	{
        EntityManager em = new Curriculo().entityManager;

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
			" SELECT COUNT ( o ) FROM Curriculo o " +
			" WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
    public static List< Curriculo > findAll(
    	InstituicaoEnsino instituicaoEnsino )
    {
		Query q = entityManager().createQuery(
	        " SELECT o FROM Curriculo o " +
    		" WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

        return q.getResultList();
    }

	public static Map< String, Curriculo > buildCurriculoCodigoToCurriculoMap(
		List< Curriculo > curriculos )
	{
		Map< String, Curriculo > curriculosMap
			= new HashMap< String, Curriculo >();

		for ( Curriculo curriculo : curriculos )
		{
			curriculosMap.put( curriculo.getCodigo(), curriculo );
		}

		return curriculosMap;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Curriculo > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Curriculo o " +
			" WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND cenario = :cenario " );

		q.setParameter( "cenario", cenario );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return q.getResultList();
    }

	public static Curriculo find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
        if ( id == null || instituicaoEnsino == null )
        {
        	return null;
        }

        Curriculo curriculo = entityManager().find( Curriculo.class, id );

        if ( curriculo != null && curriculo.getCurso() != null
        	&& curriculo.getCurso().getTipoCurso() != null
        	&& curriculo.getCurso().getTipoCurso().getInstituicaoEnsino() != null
        	&& curriculo.getCurso().getTipoCurso().getInstituicaoEnsino() == instituicaoEnsino )
        {
        	return curriculo;
        }

        return null;
    }

	public static List< Curriculo > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return Curriculo.find( instituicaoEnsino,
			firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
    public static List< Curriculo > find(
    	InstituicaoEnsino instituicaoEnsino,
    	int firstResult, int maxResults, String orderBy )
    {
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
	        " SELECT o FROM Curriculo o " +
	        " WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " + orderBy );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

        return q.getResultList();
    }

	@SuppressWarnings( "unchecked" )
	public static List< Curriculo > findByCampusAndTurno(
		InstituicaoEnsino instituicaoEnsino, Campus campus, Turno turno )
	{
		Query q = entityManager().createQuery(
			" SELECT o.curriculo FROM Oferta o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus = :campus " +
			" AND o.turno = :turno " );

		q.setParameter( "campus", campus );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	public List<CurriculoDisciplina> getCurriculoDisciplinasByPeriodo(Integer periodo) {
		List<CurriculoDisciplina> curriculoDisciplinaList = new ArrayList<CurriculoDisciplina>();
		for (CurriculoDisciplina cd : disciplinas) {
			if (cd.getPeriodo().equals(periodo)) {
				curriculoDisciplinaList.add(cd);
			}
		}
		return curriculoDisciplinaList;
	}

	@SuppressWarnings( "unchecked" )
	public List< CurriculoDisciplina > getCurriculoDisciplinasByPeriodo(
		InstituicaoEnsino instituicaoEnsino, Integer periodo )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o " +
			" WHERE o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.curriculo = :curriculo " +
			" AND o.periodo = :periodo" );

		q.setParameter( "curriculo", this );
		q.setParameter( "periodo", periodo );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public List< CurriculoDisciplina > getCurriculoDisciplinas() {
		Query q = entityManager().createQuery(
			" SELECT o FROM CurriculoDisciplina o " +
			" WHERE o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.curriculo = :curriculo "
		);

		q.setParameter( "curriculo", this );
		q.setParameter( "instituicaoEnsino", this.cenario.getInstituicaoEnsino() );

		return q.getResultList();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Curso curso, SemanaLetiva semanaLetiva, String codigo, String descricao, String periodo )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
		descricao = ( ( descricao == null ) ? "" : descricao );
		descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );

		String queryCurso = "";
		if ( curso != null )
		{
			queryCurso = ( " o.curso = :curso AND " );
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Curriculo o " +
			" WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND " + queryCurso + " LOWER ( o.descricao ) " +
			" LIKE LOWER ( :descricao ) " +
			" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo ) ");

		if ( curso != null )
		{
			q.setParameter( "curso", curso );
		}

		q.setParameter( "codigo", codigo );
		q.setParameter( "descricao", descricao );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return ( (Number) q.getSingleResult() ).intValue();
	}

    @SuppressWarnings( "unchecked" )
	public static List< Curriculo > findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Curso curso, SemanaLetiva semanaLetiva, String codigo, String descricao, String periodo,
		int firstResult, int maxResults, String orderBy )
	{
        codigo = ( ( codigo == null ) ? "" : codigo );
        codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );
        descricao = ( ( descricao == null ) ? "" : descricao );
        descricao = ( "%" + descricao.replace( '*', '%' ) + "%" );

        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy.replace("String", "") : "" );
        String whereString = "";

        if ( curso != null )
        {
        	whereString += ( " o.curso = :curso AND " );
        }
        
        if(semanaLetiva != null){
        	whereString += " o.semanaLetiva = :semanaLetiva and ";
        }
        
        if(periodo != null){
        	whereString += " o in (select a.curriculo from CurriculoDisciplina a where a.periodo = :periodo ) and ";
        }

        Query q = entityManager().createQuery(
        	" SELECT o FROM Curriculo o " +
        	" WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.cenario = :cenario " +
        	" AND " + whereString +
        	" LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " +
        	" AND LOWER ( o.codigo ) LIKE LOWER ( :codigo )" + orderBy );

        if ( curso != null )
        {
        	q.setParameter( "curso", curso );
        }
        if(semanaLetiva != null)
        	q.setParameter("semanaLetiva", semanaLetiva);
        
        if(periodo != null){
        	q.setParameter("periodo", periodo);
        }

        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );
        q.setParameter( "codigo", codigo );
        q.setParameter( "descricao", descricao );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );

        return q.getResultList();
    }
    
    public Integer getPeriodo(Disciplina disciplina) {
    	for (CurriculoDisciplina cd : disciplinas) {
    		if (cd.getDisciplina().getId().equals(disciplina.getId())) {
    			return cd.getPeriodo();
    		}
    	}
    	return null;
    }

    @SuppressWarnings( "unchecked" )
	public Integer getPeriodo(
    	InstituicaoEnsino instituicaoEnsino,
    	Disciplina disciplina, Oferta oferta )
    {
    	Query q = entityManager().createQuery(
    		" SELECT o FROM CurriculoDisciplina o " +
    		" WHERE o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.curriculo = :curriculo " +
    		" AND o.disciplina = :disciplina" );

    	q.setParameter( "curriculo", this );
    	q.setParameter( "disciplina", disciplina );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	List< CurriculoDisciplina > curriculosDisciplina = q.getResultList();

    	if ( curriculosDisciplina.size() == 1 )
    	{
    		return curriculosDisciplina.get( 0 ).getPeriodo();
    	}

    	Integer periodo = null;

    	for ( CurriculoDisciplina cd : curriculosDisciplina )
    	{
    		for ( Oferta ofertaCurriculo : cd.getCurriculo().getOfertas() )
    		{
    			if( ofertaCurriculo.getId() == oferta.getId() )
    			{
    				periodo = cd.getPeriodo();
    				break;
    			}
    		}

    		if ( periodo != null )
    		{
    			break;
    		}
    	}

    	return periodo;
    }
    
    public List<Integer> getPeriodos() {
    	Set<Integer> periodos = new HashSet<Integer>();
    	for (CurriculoDisciplina cd : disciplinas) {
    		periodos.add(cd.getPeriodo());
    	}
    	return new ArrayList<Integer>(periodos);
    }

    @SuppressWarnings( "unchecked" )
    public List< Integer > getPeriodos(
    	InstituicaoEnsino instituicaoEnsino )
    {
    	Query q = entityManager().createQuery(
    		" SELECT DISTINCT ( o.periodo ) " +
    		" FROM CurriculoDisciplina o " +
    		" WHERE o.curriculo.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.curriculo = :curriculo " );

    	q.setParameter( "curriculo", this );
    	q.setParameter( "instituicaoEnsino", instituicaoEnsino );

    	return ( List< Integer > ) q.getResultList();
    }

	public static boolean checkCodigoUnique(
		InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String codigo )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Curriculo o " +
			" WHERE o.curso.tipoCurso.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.curso.cenario = :cenario " +
			" AND o.codigo = :codigo " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Curso: " ).append( getCurso() ).append( ", " );
        sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
        sb.append( "Codigo: " ).append( getCodigo() ).append( ", " );
        sb.append( "Descricao: " ).append( getDescricao() ).append( ", " );
        sb.append( "Disciplinas: " ).append( getDisciplinas() == null ?
        	"null" : getDisciplinas().size() ).append( ", " );
        sb.append( "Ofertas: " ).append( getOfertas() == null ?
        	"null" : getOfertas().size() );

        return sb.toString();
    }

	//@Override
	public int compareTo( Curriculo o )
	{
		int result = getCurso().compareTo( o.getCurso() );
		
		if ( result == 0 )
		{
			result = getSemanaLetiva().compareTo( o.getSemanaLetiva() );
	
			if ( result == 0 )
			{
				result = getCenario().compareTo( o.getCenario() );

				if ( result == 0 )
				{
					result = getCodigo().compareTo( o.getCodigo() );
				}
			}
		}

		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Curriculo ) )
		{
			return false;
		}

		Curriculo other = (Curriculo) obj;

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

	public Curriculo clone(CenarioClone novoCenario) {
		Curriculo clone = new Curriculo();
		clone.setCenario(novoCenario.getCenario());
		clone.setCodigo(this.getCodigo());
		clone.setCurso(novoCenario.getEntidadeClonada(this.getCurso()));
		clone.setDescricao(this.getDescricao());
		clone.setSemanaLetiva(novoCenario.getEntidadeClonada(this.getSemanaLetiva()));
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Curriculo entidadeClone) {
		for (CurriculoDisciplina curriculoDisciplina : this.getDisciplinas())
		{
			entidadeClone.getDisciplinas().add(novoCenario.clone(curriculoDisciplina));
		}
	}
}
