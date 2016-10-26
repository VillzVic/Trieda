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
    @Column( name = "FIX_DESCRICAO" )
    @Size( min = 1, max = 50 )
    private String descricao;
    
    /*@ManyToOne( targetEntity = Professor.class, fetch=FetchType.LAZY )
    @JoinColumn( name = "PRF_ID" )
    private Professor professor;*/
	
    @ManyToOne( targetEntity = Disciplina.class, fetch=FetchType.LAZY )
    @JoinColumn( name = "DIS_ID" )
    private Disciplina disciplina;
    
    @NotNull
    @Column( name = "TURMA" )
    @Size( min = 1, max = 255 )
    private String turma;
    
    @NotNull
    @Column( name = "FIX_AMBIENTE" )
    private Boolean fix_ambiente;
    
    @NotNull
    @Column( name = "FIX_DIASEHORARIOS" )
    private Boolean fix_diasHorarios;
    
    @NotNull
    @Column( name = "FIX_PROFESSOR" )
    private Boolean fix_professor;
    
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class)
    @JoinColumn( name = "CEN_ID" )
    private Cenario cenario;
    
    @ManyToOne( targetEntity = Campus.class )
    @JoinColumn( name = "CAM_ID" )
    private Campus campus;
    
   /* @ManyToOne( targetEntity = Unidade.class )
    @JoinColumn( name = "UNI_ID" )
    private Unidade unidade;
    
    @ManyToOne( targetEntity = Sala.class )
    @JoinColumn( name = "SAL_ID" )
    private Sala sala;*/
    
    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "fixacoes" )
    private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();
    
    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "fixacoes" )
    private Set< Sala > salas = new HashSet< Sala >();
    
    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "fixacoes" )
    private Set< Professor > professores = new HashSet< Professor >();
    
    @ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "fixacoes" )
    private Set< Unidade > unidades = new HashSet< Unidade >();

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
        	this.removeAmbientesDisponivelCenario();
        	this.removeProfessoresDisponivelCenario();
        	this.removeUnidadesDisponivelCenario();
            this.entityManager.remove( this );
        }
        else
        {
            Fixacao attached = this.entityManager.find(
            	this.getClass(), this.id );

            attached.removeHorariosDisponivelCenario();
            attached.removeAmbientesDisponivelCenario();
            attached.removeProfessoresDisponivelCenario();
            attached.removeUnidadesDisponivelCenario();
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
    public void removeProfessoresDisponivelCenario()
    {
    	Set< Professor > professores = this.getProfessores();

    	for ( Professor professor : professores )
    	{
    		professor.getFixacoes().remove( this );
    		professor.merge();
    	}
    }
    
    @Transactional
    public void removeAmbientesDisponivelCenario()
    {
    	Set< Sala > salas = this.getSalas();

    	for ( Sala sala : salas )
    	{
    		sala.getFixacoes().remove( this );
    		sala.merge();
    	}
    }
    
    @Transactional
    public void removeUnidadesDisponivelCenario()
    {
    	Set< Unidade > unidades = this.getUnidades();

    	for ( Unidade unidade : unidades )
    	{
    		unidade.getFixacoes().remove( this );
    		unidade.merge();
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
    	InstituicaoEnsino instituicaoEnsino,
    	Cenario cenario)
    {
    	List< Fixacao > list = Fixacao.findAll( instituicaoEnsino,cenario );
        return ( list == null ? 0 : list.size() );
    }
    
    @SuppressWarnings("unchecked")
	public static List<Professor> findProfessorFixado(String turmaCB, Disciplina disciplina,
			InstituicaoEnsino instituicaoEnsino, Fixacao fixacao) {
		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT o "
								+ " FROM Professor o JOIN o.fixacoes f "
								+ " WHERE f.instituicaoEnsino = :instituicaoEnsino "
								+ " AND f.disciplina = :disciplina "
								+ " AND f.turma = :turma " 
								+ " AND o.fixacoes.id = :fixacao ");

		q.setParameter("turma", turmaCB);
		q.setParameter("disciplina", disciplina);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("fixacao", fixacao.getId());

		List<Professor> list = q.getResultList();
		return list;
	}
    
    @SuppressWarnings("unchecked")
    public static List< Unidade > findByTurmaFixada(
    	InstituicaoEnsino instituicaoEnsino, String turma , Campus campus, Fixacao fixacao)
    {
    	Query q = entityManager().createQuery(
    		" SELECT DISTINCT o FROM Unidade o " +
    		" JOIN o.salas s JOIN s.fixacoes f " +
    		" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.campus = :campus " +
    		" AND f.turma = :turma " +
    		" AND s.fixacoes.id = :fixacao ORDER BY o.nome ASC " );

		q.setParameter( "turma", turma );
		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "fixacao", fixacao.getId() );

    	return q.getResultList();
    }
    
    @SuppressWarnings("unchecked")
	public static List<Sala> findBySalasFixadas(
			InstituicaoEnsino instituicaoEnsino, String turma,
			Cenario cenario, Campus campus, Disciplina disciplina, Fixacao fixacao) {
		String queryTurma = "", queryCampus ="", queryDisciplina = "";
		
		if(turma != null)
		{
			queryTurma = " AND f.turma = :turma ";
		}
		if(disciplina != null)
		{
			queryDisciplina = " AND f.disciplina = :disciplina ";
		}
		if(campus != null)
		{
			queryCampus = " AND o.unidade.campus = :campus ";
		}
		
		Query q = entityManager()
				.createQuery(
						" SELECT DISTINCT o FROM Sala o "
								+ " JOIN o.fixacoes f "
								+ " WHERE o.unidade.campus.instituicaoEnsino = :instituicaoEnsino "
								+ " AND o.unidade.campus.cenario = :cenario " 
								+ " AND o.fixacoes.id = :fixacao "
								+ queryTurma
								+ queryDisciplina
								+ queryCampus );

		q.setParameter("cenario", cenario);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("fixacao", fixacao.getId());
		
		if(turma != null)
		{
			q.setParameter("turma", turma);
		}
		if(disciplina != null)
		{
			q.setParameter("disciplina", disciplina);
		}
		if(campus != null)
		{
			q.setParameter("campus", campus);
		}

		return q.getResultList();
	}
    
    @SuppressWarnings("unchecked")
	public static List< HorarioDisponivelCenario > getHorariosFixados( InstituicaoEnsino instituicaoEnsino, String turma,
			Curriculo curriculo, Turno turno, Fixacao fixacao){
		Query q = entityManager().createQuery(
				" SELECT o FROM HorarioDisponivelCenario o " +
				" JOIN o.fixacoes f " +
			    " WHERE f.instituicaoEnsino = :instituicaoEnsino " +
			    " AND f.turma = :turma " +
			    " AND f.campus.ofertas.turno = :turno " +
			    " AND f.campus.ofertas.curriculo = :curriculo " +
			    " AND o.fixacoes.id = :fixacao " );
		
		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("turma", turma );
		q.setParameter("turno", turno );
		q.setParameter("curriculo", curriculo );
		q.setParameter("fixacao", fixacao.getId() );
		
		List< HorarioDisponivelCenario > list = q.getResultList();
		return list;
	}

    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > findAll(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario )
    {
        Query q = entityManager().createQuery(
        	"SELECT o " +
        	"FROM Fixacao o " +
        	"WHERE o.instituicaoEnsino = :instituicaoEnsino " +
    		"AND o.cenario = :cenario ");

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );

        List< Fixacao > list = q.getResultList();
        return list;
    }
    
    public Boolean getTeoricoSala( Integer sala )
    {
        Query q = entityManager().createNativeQuery(
        	" select distinct a.atp_creditoteotico " +
        	" from atendimento_operacional a " + 
        	" inner join fixacoes f on f.dis_id = a.dis_id " +
        	" inner join salas_fixacoes sf on sf.fixacoes = f.fix_id " +
        	" inner join salas s on s.sal_id = sf.salas and s.sal_id = a.sal_id " +
        	" where f.fix_id = :fixacao and s.sal_id = :sala ");
      
        q.setParameter( "sala", sala );
        q.setParameter( "fixacao", this.getId() );
               
        return (Boolean) q.getSingleResult();
        
    }
    
    public Boolean getTeoricoHorario( Integer hdc )
    {
        Query q = entityManager().createNativeQuery(
        	" select distinct a.atp_creditoteotico " +
        	" from atendimento_operacional a " + 
        	" inner join horario_disponivel_cenario h on a.hdc_id = h.hdc_id " +
        	" inner join horario_disponivel_cenario_fixacoes hf on hf.horarios = h.hdc_id " +
        	" inner join fixacoes f on f.fix_id = hf.fixacoes and f.dis_id = a.dis_id " +
        	" where f.fix_id = :fixacao and h.hdc_id = :hdc ");
      
        	q.setParameter( "hdc", hdc );
        	q.setParameter( "fixacao", this.getId() );
               
        return (Boolean) q.getSingleResult();
        
    }
    
    @Transactional
    public void setTeoricoSala(Integer sala){
    	Boolean teorico = getTeoricoSala(sala);
    	
    	Query q = entityManager().createNativeQuery(
    			" update salas_fixacoes " +
    			" set aula_teorica = :teorico " +
    			" where fixacoes = :fixacoes " +
    			" and salas = :sala ");
    	
    	q.setParameter( "teorico", teorico );
    	q.setParameter( "fixacoes", this.getId() );
        q.setParameter( "sala", sala );
        
        q.executeUpdate();
    	
    }
    
    @Transactional
    public void setTeoricoHorario(Integer hdc){
    	Boolean teorico = getTeoricoHorario(hdc);
    	Query q = entityManager().createNativeQuery(
    			" update horario_disponivel_cenario_fixacoes " +
    			" set aula_teorica = :teorico " +
    			" where fixacoes = :fixacoes " +
    			" and horarios = :hdc ");
    	
    	q.setParameter( "teorico", teorico );
    	q.setParameter( "fixacoes", this.getId() );
        q.setParameter( "hdc", hdc );
        
        q.executeUpdate();
    	
    }
    
    public Boolean isTeoricoHorario(Integer hdc){
    	Query q = entityManager().createNativeQuery(
    			" select aula_teorica " +
    			" from horario_disponivel_cenario_fixacoes " +
    			" where fixacoes = :fixacoes and horarios = :hdc ");
    	
    	q.setParameter( "hdc", hdc );
    	q.setParameter( "fixacoes", this.getId() );
    	
    	return (Boolean) q.getSingleResult();
    }
    
    public Boolean isTeoricoSala(Integer sala){
    	Query q = entityManager().createNativeQuery(
    			" select aula_teorica " +
    			" from salas_fixacoes " +
    			" where fixacoes = :fixacoes and salas = :sala ");
    	
    	q.setParameter( "sala", sala );
    	q.setParameter( "fixacoes", this.getId() );
    	
    	return (Boolean) q.getSingleResult();
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
    	Cenario cenario,
        int firstResult, int maxResults )
    {
        return Fixacao.find( instituicaoEnsino, cenario,
        	firstResult, maxResults, null );
    }

    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > find(
    	InstituicaoEnsino instituicaoEnsino,
    	Cenario cenario,
    	int firstResult, int maxResults, String orderBy )
    {
        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
        	"SELECT o FROM Fixacao o " +
        	"WHERE o.instituicaoEnsino = :instituicaoEnsino " +
        	"AND o.cenario = :cenario " + orderBy );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
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
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario, Disciplina disciplina )
    {
        Query q = entityManager().createQuery(
        	" SELECT o FROM Fixacao o " +
        	" WHERE o.disciplina = :disciplina " +
        	" AND o.instituicaoEnsino = :instituicaoEnsino " +
    		" AND o.cenario = :cenario " );

        q.setParameter( "disciplina", disciplina );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );

        List< Fixacao > list = q.getResultList();
        return list;
    }
    
    @SuppressWarnings( "unchecked" )
    public static List< Fixacao > findBy(
    	InstituicaoEnsino instituicaoEnsino, Cenario cenario, String descricao,
    	int firstResult, int maxResults, String orderBy )
    {
    	descricao = ( ( descricao == null || descricao.length() == 0 ) ? "" : descricao );
    	descricao = descricao.replace( '*', '%' );

        if ( descricao == "" || descricao.charAt( 0 ) != '%' )
        {
        	descricao = ( "%" + descricao );
        }

        if ( descricao.charAt( descricao.length() - 1 ) != '%' )
        {
        	descricao = ( descricao + "%" );
        }

        orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

        Query q = entityManager().createQuery(
        	" SELECT o FROM Fixacao o WHERE " +
        	" LOWER ( o.descricao ) LIKE LOWER ( :descricao ) " +
        	" AND o.instituicaoEnsino = :instituicaoEnsino " + 
        	" AND o.cenario = :cenario " + orderBy );

        q.setParameter( "descricao", descricao );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );

        List< Fixacao > list = q.getResultList();
        return list;
    }

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.fixacoes ) c " +
			" WHERE c = :fixacao " +
			" AND c.instituicaoEnsino = :instituicaoEnsino " +
			" AND c.cenario = :cenario " +
			" AND o.horarioAula.semanaLetiva.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "fixacao", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public List< Professor > getProfessores(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o, IN ( o.fixacoes ) c " +
			" WHERE c = :fixacao " +
			" AND c.instituicaoEnsino = :instituicaoEnsino " +
			" AND c.cenario = :cenario " );

		q.setParameter( "fixacao", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public List< Unidade > getUnidades(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Unidade o, IN ( o.fixacoes ) c " +
			" WHERE c = :fixacao " +
			" AND c.instituicaoEnsino = :instituicaoEnsino " +
			" AND c.cenario = :cenario " );

		q.setParameter( "fixacao", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public List< Sala > getSalas(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Sala o, IN ( o.fixacoes ) c " +
			" WHERE c = :fixacao " +
			" AND c.instituicaoEnsino = :instituicaoEnsino " +
			" AND c.cenario = :cenario " );

		q.setParameter( "fixacao", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}

    public String getDescricao()
    {
    	return this.descricao;
    }

    public void setDescricao( String descricao )
    {
    	this.descricao = descricao;
    }

   /* public Professor getProfessor()
    {
    	return this.professor;
    }

    public void setProfessor( Professor professor )
    {
    	this.professor = professor;
    }*/

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
	
	
	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
	}

	
	public Campus getCampus()
	{
		return this.campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}
	
	

	/*public Unidade getUnidade()
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
	}*/

	public Set<Sala> getSalas() {
		return salas;
	}

	public void setSalas(Set<Sala> salas) {
		this.salas = salas;
	}

	public Set<Professor> getProfessores() {
		return professores;
	}

	public void setProfessores(Set<Professor> professores) {
		this.professores = professores;
	}

	public Set<Unidade> getUnidades() {
		return unidades;
	}

	public void setUnidades(Set<Unidade> unidades) {
		this.unidades = unidades;
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

	public Boolean getFix_ambiente() {
		return fix_ambiente;
	}

	public void setFix_ambiente(Boolean fix_ambiente) {
		this.fix_ambiente = fix_ambiente;
	}

	public Boolean getFix_diasHorarios() {
		return fix_diasHorarios;
	}

	public void setFix_diasHorarios(Boolean fix_diasHorarios) {
		this.fix_diasHorarios = fix_diasHorarios;
	}

	public Boolean getFix_professor() {
		return fix_professor;
	}

	public void setFix_professor(Boolean fix_professor) {
		this.fix_professor = fix_professor;
	}

	public String toString()
	{
        StringBuilder sb = new StringBuilder();

        sb.append( "Id: " ).append( getId() ).append( ", " );
        sb.append( "Version: " ).append( getVersion() ).append( ", " );
        sb.append( "Instituicao de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
        sb.append( "Professor: " ).append( getProfessores() ).append( ", " );
        sb.append( "Descricao: " ).append( getDescricao() ).append( ", " );
        sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
        sb.append( "Turma: " ).append( getTurma() ).append( ", " );
        sb.append( "Campus: " ).append( getCampus() ).append( ", " );
        sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
        sb.append( "Unidade: " ).append( getUnidades() ).append( ", " );
        sb.append( "Sala: " ).append( getSalas() ).append( ", " );
        sb.append( "Fixar Ambiente: " ).append( getFix_ambiente() ).append( ", " );
        sb.append( "Fixar Dias e Horarios: " ).append( getFix_diasHorarios() ).append( ", " );
        sb.append( "Fixar Professores: " ).append( getFix_professor() ).append( ", " );
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
	
	public Long getTurnoId(){
		Long turno = 0L;
		for (HorarioDisponivelCenario horarioDisponivelCenario : this.getHorarios())
		{	if(this != null){
			turno = horarioDisponivelCenario.getHorarioAula().getTurno().getId();
			break;
			}
			
		}
		return turno;
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
