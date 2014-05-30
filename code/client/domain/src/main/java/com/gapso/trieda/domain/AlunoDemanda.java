package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
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
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

import org.hibernate.annotations.Type;
import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "ALD_ID" )
@Table( name = "ALUNOS_DEMANDA", uniqueConstraints =
@UniqueConstraint( columnNames = { "ALN_ID", "DEM_ID" } ) )
public class AlunoDemanda
	implements Serializable, Comparable< AlunoDemanda >, Clonable< AlunoDemanda >
{
	private static final long serialVersionUID = 5574796519360717359L;

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "ALD_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Aluno.class, fetch = FetchType.LAZY )
    @JoinColumn( name = "ALN_ID" )
    private Aluno aluno;

    @NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Demanda.class , fetch = FetchType.LAZY )
    @JoinColumn( name = "DEM_ID" )
    private Demanda demanda;
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
    private Set<AtendimentoTatico> atendimentosTatico = new HashSet<AtendimentoTatico>();
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
    private Set<AtendimentoOperacional> atendimentosOperacional = new HashSet<AtendimentoOperacional>();
    
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE})
	@JoinTable(name="TURMAS_ALUNOS_DEMANDA",
	joinColumns=@JoinColumn(name="ALD_ID"),
	inverseJoinColumns=@JoinColumn(name="TUR_ID"))
	private Set<Turma> turmas = new HashSet<Turma>();

	@Column( name = "ALD_ATENDIDO" )
	private Boolean atendido;
	
	@Column( name = "ALD_EXIGE_EQ_FORCADA" )
	private Boolean exigeEquivalenciaForcada;

	@NotNull
	@Column( name = "ALD_PRIORIDADE" )
	@Min( 0L )
	@Max( 10L )
	private Integer prioridade;

	@NotNull
	@Column( name = "ALD_PERIODO" )
	@Min( 0L )
	@Max( 100L )
	private Integer periodo;
	
	@Column(name = "ALD_MOTIVO_NAO_ATENDIMENTO")
	@Type(type="text")
	private String motivoNaoAtendimento;

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

	public Aluno getAluno()
	{
		return this.aluno;
	}

	public void setAluno( Aluno aluno )
	{
		this.aluno = aluno;
	}

	public Demanda getDemanda()
	{
		return this.demanda;
	}

	public void setDemanda( Demanda demanda )
	{
		this.demanda = demanda;
	}

	public Boolean getAtendido()
	{
		return this.atendido;
	}

	public void setAtendido( Boolean atendido )
	{
		this.atendido = atendido;
	}
	
	public Boolean getExigeEquivalenciaForcada()
	{
		return this.exigeEquivalenciaForcada;
	}

	public void setExigeEquivalenciaForcada( Boolean exigeEquivalenciaForcada )
	{
		this.exigeEquivalenciaForcada = exigeEquivalenciaForcada;
	}

	public Integer getPrioridade()
	{
		return this.prioridade;
	}

	public void setPrioridade( Integer prioridade )
	{
		this.prioridade = prioridade;
	}

	public Integer getPeriodo()
	{
		return this.periodo;
	}

	public void setPeriodo( Integer periodo )
	{
		this.periodo = periodo;
	}
	
	public String getMotivoNaoAtendimento()
	{
		return this.motivoNaoAtendimento;
	}

	public void setMotivoNaoAtendimento( String motivoNaoAtendimento )
	{
		this.motivoNaoAtendimento = motivoNaoAtendimento;
	}
	
	public void setAtendimentosTatico(Set<AtendimentoTatico> atendimentosTatico){
		this.atendimentosTatico = atendimentosTatico;
	}
	
	public Set<AtendimentoTatico> getAtendimentosTatico(){
		return this.atendimentosTatico;
	}

	public void setAtendimentosOperacional(Set<AtendimentoOperacional> atendimentosOperacional){
		this.atendimentosOperacional = atendimentosOperacional;
	}
	
	public Set<AtendimentoOperacional> getAtendimentosOperacional(){
		return this.atendimentosOperacional;
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
			AlunoDemanda attached = this.entityManager.find(
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
	public AlunoDemanda merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		AlunoDemanda merged = this.entityManager.merge( this );
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new AlunoDemanda().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				"Entity manager has not been injected (is the Spring " +
				"Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findAll(InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.cenario = :cenario ");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByDemanda(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda = :demanda " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "demanda", demanda );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByDisciplinaCampusAndTurma(
		InstituicaoEnsino instituicaoEnsino, Disciplina disciplina, Campus campus, String turma, Long turmaId )
	{
		List<AlunoDemanda> result = new ArrayList<AlunoDemanda>();

		String turmaQuery = "";
		if(turmaId != null)
		{
			turmaQuery = "t.id = :turmaId OR ";
		}
		
		Query q = entityManager().createQuery(
			" SELECT DISTINCT (o) FROM AlunoDemanda o LEFT JOIN o.atendimentosOperacional ao" +
			" LEFT JOIN o.atendimentosTatico at LEFT JOIN o.turmas t" +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina = :disciplina " +
			" AND o.demanda.oferta.campus = :campus " +
			" AND (" + turmaQuery + "o.turmas IS EMPTY) " +
			" AND (ao.turma = :turma OR at.turma = :turma OR o.atendido is FALSE) " +
			" AND (ao.disciplinaSubstituta IS NULL) " +
			" AND (at.disciplinaSubstituta IS NULL) " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "disciplina", disciplina );
		q.setParameter( "campus", campus );
		q.setParameter( "turma", turma );
		if(turmaId != null)
		{
			q.setParameter( "turmaId", turmaId );
		}
		result.addAll(q.getResultList());

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByDemandaReal(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda = :demanda " +
			" AND o.aluno.criadoTrieda is FALSE" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "demanda", demanda );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByDemandaVirtual(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda = :demanda " +
			" AND o.aluno.criadoTrieda is TRUE" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "demanda", demanda );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByCampusAndTurno(
		InstituicaoEnsino instituicaoEnsino, Collection<Campus> campi, Collection<Turno> turnos )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus IN ( :campi ) " +
			" AND o.demanda.oferta.turno IN ( :turnos ) " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campi", campi );
		q.setParameter( "turnos", turnos );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByCampusAndTurnoFetchAtendimentoOperacional(
		InstituicaoEnsino instituicaoEnsino, Collection<Campus> campi, Collection<Turno> turnos )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o FROM AlunoDemanda o LEFT JOIN FETCH o.atendimentosOperacional " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus IN ( :campi ) " +
			" AND o.demanda.oferta.turno IN ( :turnos ) " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campi", campi );
		q.setParameter( "turnos", turnos );

		return q.getResultList();
	}
	
	public static Map<Long, AlunoDemanda> buildIdMapAlunoDemandaFetchAtendimentoOperacional(
			InstituicaoEnsino instituicaoEnsino, Collection<Long> ids)
	{
		Map<Long, AlunoDemanda> idMapAlunoDemanda = new HashMap<Long, AlunoDemanda>();
		for (AlunoDemanda alunoDemanda : 
			findByIdsFetchAtendimentoOperacional(instituicaoEnsino, ids))
		{
			idMapAlunoDemanda.put(alunoDemanda.getId(), alunoDemanda);
		}
		
		return idMapAlunoDemanda;
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByIdsFetchAtendimentoOperacional(
		InstituicaoEnsino instituicaoEnsino, Collection<Long> ids)
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o FROM AlunoDemanda o LEFT JOIN FETCH o.atendimentosOperacional " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.id IN ( :ids ) " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "ids", ids );

		return q.getResultList();
	}
	
	public static Map<Long, AlunoDemanda> buildIdMapAlunoDemandaFetchAtendimentoTatico(
			InstituicaoEnsino instituicaoEnsino, Collection<Long> ids)
	{
		Map<Long, AlunoDemanda> idMapAlunoDemanda = new HashMap<Long, AlunoDemanda>();
		for (AlunoDemanda alunoDemanda : 
			findByIdsFetchAtendimentoTatico(instituicaoEnsino, ids))
		{
			idMapAlunoDemanda.put(alunoDemanda.getId(), alunoDemanda);
		}
		
		return idMapAlunoDemanda;
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByIdsFetchAtendimentoTatico(
		InstituicaoEnsino instituicaoEnsino, Collection<Long> ids)
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o FROM AlunoDemanda o LEFT JOIN FETCH o.atendimentosTatico " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.id IN ( :ids ) " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "ids", ids );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByCampusAndTurnoFetchAtendimentoTatico(
		InstituicaoEnsino instituicaoEnsino, Collection<Campus> campi, Set<Turno> turnos )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o FROM AlunoDemanda o LEFT JOIN FETCH o.atendimentosTatico " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus IN ( :campi ) " +
			" AND o.demanda.oferta.turno IN ( :turnos ) " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campi", campi );
		q.setParameter( "turnos", turnos );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static AlunoDemanda findByDemandaAndAluno(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda, Aluno aluno )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.aluno.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda = :demanda " +
			" AND o.aluno = :aluno " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "demanda", demanda );
		q.setParameter( "aluno", aluno );

		List< AlunoDemanda > alunosDemanda = q.getResultList();
		return ( alunosDemanda.size() == 0 ? null : alunosDemanda.get( 0 ) );
	}

	public static AlunoDemanda find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		AlunoDemanda alunoDemanda = entityManager().find(
			AlunoDemanda.class, id );

		if ( alunoDemanda != null
			&& alunoDemanda.getAluno() != null
			&& alunoDemanda.getAluno().getInstituicaoEnsino() != null
			&& alunoDemanda.getAluno().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return alunoDemanda;
		}

		return null;
	}

	public String getNaturalKey()
	{
		return this.getDemanda() + "-" + this.getAluno();
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Aluno: " ).append( getAluno() ).append( ", " );
		sb.append( "Periodo: " ).append( getPeriodo() ).append( ", " );
		sb.append( "Demanda: " ).append( getDemanda() );

		return sb.toString();
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + ( ( this.getId() == null ) ? 0 : this.getId().hashCode() ) );
		result = ( prime * result + ( ( this.getVersion() == null ) ? 0 : this.getVersion().hashCode() ) );
		result = ( prime * result + ( ( this.getDemanda() == null ) ? 0 : this.getDemanda().hashCode() ) );
		result = ( prime * result + ( ( this.getAluno() == null ) ? 0 : this.getAluno().hashCode() ) );

		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null
			|| !( obj instanceof AlunoDemanda ) )
		{
			return false;
		}

		AlunoDemanda other = (AlunoDemanda) obj;

		// Comparando as demandas
		if ( getDemanda() == null )
		{
			if ( other.getDemanda() != null )
			{
				return false;
			}
		}
		else if ( !getDemanda().equals( other.getDemanda() ) )
		{
			return false;
		}

		// Comparando os alunos
		if ( getAluno() == null )
		{
			if ( other.getAluno() != null )
			{
				return false;
			}
		}
		else if ( !getAluno().equals( other.getAluno() ) )
		{
			return false;
		}

		return true;
	}

	//@Override
	public int compareTo( AlunoDemanda o )
	{
		int compare = this.getDemanda().getId().compareTo( o.getDemanda().getId() );

		if ( compare == 0 )
		{
			compare = this.getAluno().getId().compareTo( o.getAluno().getId() );
		}

		return compare;
	}

	public static Map< String, AlunoDemanda > buildCodAlunoCodDemandaToAlunosDemandasMap(
		List< AlunoDemanda > alunosDemanda )
	{
		Map< String, AlunoDemanda > alunosDemandaMap
			= new HashMap< String, AlunoDemanda >();

		for ( AlunoDemanda alunoDemanda : alunosDemanda )
		{
			String codigo = "";
			codigo += alunoDemanda.getAluno().getMatricula();
			codigo += "-";
			codigo += alunoDemanda.getDemanda().getId();
			codigo += "-";
			codigo += alunoDemanda.getPrioridade();

			alunosDemandaMap.put( codigo, alunoDemanda );
		}

		return alunosDemandaMap;
	}
	
	public static Map<String,Integer[]> buildDemandaKeyToQtdAlunosMap(InstituicaoEnsino instituicaoEnsino, Cenario cenario) {
		// [OfertaId-DisciplinaId -> {totalDemandaP1,totalDemandaP2,totalDemanda}]
		Map<String,Integer[]> demandaKeyToQtdAlunosMap = new HashMap<String, Integer[]>();
		
		List<AlunoDemanda> alunosDemandas = AlunoDemanda.findAll(instituicaoEnsino,cenario);
		
		for (AlunoDemanda ad : alunosDemandas) {
			// monta a chave da demanda
			Long ofertaId = ad.getDemanda().getOferta().getId();
			Long disciplinaId = ad.getDemanda().getDisciplina().getId();
			String demandaId = ofertaId+"-"+disciplinaId;
			
			// atualiza map
			Integer[] trio = demandaKeyToQtdAlunosMap.get(demandaId);
			if (trio == null) {
				trio = new Integer[]{0,0,0};
				demandaKeyToQtdAlunosMap.put(demandaId,trio);
			}
			
			if (ad.getPrioridade() == 1) {
				trio[0]++;
			} else {
				trio[1]++;
			}
			trio[2]++;
		}
		
		return demandaKeyToQtdAlunosMap;
	}
	
	public static Map<Integer,AlunoDemanda> buildAlunoDemandaIdToAlunoDemandaMap(InstituicaoEnsino instituicaoEnsino, Cenario cenario) {
		// [OfertaId-DisciplinaId -> {totalDemandaP1,totalDemandaP2,totalDemanda}]
		Map<Integer, AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = new HashMap<Integer, AlunoDemanda>();
		
		List<AlunoDemanda> alunosDemandas = AlunoDemanda.findAll(instituicaoEnsino,cenario);
		
		for (AlunoDemanda ad : alunosDemandas) {
			alunoDemandaIdToAlunoDemandaMap.put(ad.getId().intValue(), ad);
		}
		
		return alunoDemandaIdToAlunoDemandaMap;
	}
	
	public static int sumDemandaPorPrioridade(InstituicaoEnsino instituicaoEnsino, Campus campus, int prioridade) {
		Query q = entityManager().createQuery(
			" SELECT COUNT(o) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus = :campus " +
			" AND o.prioridade = :prioridade " 
		);

		q.setParameter("instituicaoEnsino",instituicaoEnsino);
		q.setParameter("campus",campus);
		q.setParameter("prioridade",prioridade);

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}
	
	public static int sumAlunosComDemanda(InstituicaoEnsino instituicaoEnsino, Campus campus) {
		Query q = entityManager()
				.createNativeQuery(
						" select count(*) from (select distinct alunodeman0_.aln_id from alunos_demanda alunodeman0_" +
						" join demandas demanda1_" +
						" join ofertas oferta2_" +
						" join campi campus3_" +
						" join disciplinas disciplina5_" +
						" join tipos_disciplina tipodiscip6_" +
						" where alunodeman0_.dem_id=demanda1_.dem_id" +
						" and demanda1_.ofe_id=oferta2_.ofe_id" +
						" and oferta2_.cam_id=campus3_.cam_id" +
						" and demanda1_.dis_id=disciplina5_.dis_id  " +
						" and disciplina5_.tdi_id=tipodiscip6_.tdi_id and campus3_.ins_id=:instituicaoEnsino " +
						" and tipodiscip6_.ins_id=:instituicaoEnsino and oferta2_.cam_id= :campus " +
						" and alunodeman0_.ald_prioridade=:prioridade) as a;");

		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		q.setParameter("campus",campus);
		q.setParameter("prioridade",1);

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}
	
	public static int sumDemandaPorPrioridade(InstituicaoEnsino instituicaoEnsino, Cenario cenario, int prioridade) {
		Query q = entityManager().createQuery(
			" SELECT COUNT(o) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus.cenario = :cenario " +
			" AND o.prioridade = :prioridade " 
		);

		q.setParameter("instituicaoEnsino",instituicaoEnsino);
		q.setParameter("cenario",cenario);
		q.setParameter("prioridade",prioridade);

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}
	
	public static int sumDemandaPresencialPorPrioridade(InstituicaoEnsino instituicaoEnsino, Cenario cenario, 
			Campus campus, int prioridade) {
		// obtem os tipos de disciplinas que ocupam grade
		List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findByCenario(instituicaoEnsino, cenario);
		List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
		for (TipoDisciplina td : tiposDisciplinas) {
			if (td.ocupaGrade()) {
				tiposDisciplinasPresenciais.add(td);
			}
		}
		
		Query q = entityManager().createQuery(
			" SELECT COUNT(o) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus = :campus " +
			" AND o.prioridade = :prioridade " +
			" AND o.demanda.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais) " +
			" AND (o.demanda.disciplina.creditosTeorico + o.demanda.disciplina.creditosPratico) > 0 "
		);

		q.setParameter("instituicaoEnsino",instituicaoEnsino);
		q.setParameter("campus",campus);
		q.setParameter("prioridade",prioridade);
		q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}
	
	public static int sumDemandaAtendidaPorPrioridade(InstituicaoEnsino instituicaoEnsino, Campus campus, int prioridade) {
		Query q = entityManager().createQuery(
			" SELECT COUNT(o) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus = :campus " +
			" AND o.prioridade = :prioridade " +
			" AND o.atendido = :atendido " 
		);

		q.setParameter("instituicaoEnsino",instituicaoEnsino);
		q.setParameter("campus",campus);
		q.setParameter("prioridade",prioridade);
		q.setParameter("atendido",true);

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByAluno(
		InstituicaoEnsino instituicaoEnsino, Aluno aluno )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.aluno = :aluno " );
		
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "aluno", aluno );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByAlunos(
		InstituicaoEnsino instituicaoEnsino, List<Aluno> alunos, Curso curso )
	{
		String cursoQuery = curso == null ? "" : "AND o.demanda.oferta.curso = :curso";
		
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			cursoQuery +
			" AND o.aluno IN ( :alunos ) GROUP BY o.aluno" );
		
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "alunos", alunos );
		if (curso != null)
		{
			q.setParameter("curso", curso);
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByAlunosAtendidos(
		InstituicaoEnsino instituicaoEnsino, List<Aluno> alunos, Curso curso, Boolean formando )
	{
		String cursoQuery = curso == null ? "" : " AND o.demanda.oferta.curso = :curso ";
		String formandoQuery = formando == null ? "" : " AND o.aluno.formando = :formando ";
 		
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			cursoQuery + formandoQuery +
			" AND o.aluno IN ( :alunos ) AND o.atendido = TRUE GROUP BY o.aluno" );
		
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "alunos", alunos );
		if (curso != null)
		{
			q.setParameter( "curso", curso );
		}
		if (formando != null)
		{
			q.setParameter( "formando", formando );
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findByDisciplinaAndCampus(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, Disciplina disciplina, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus.cenario = :cenario " +
			" AND o.demanda.disciplina = :disciplina " +
			" AND o.demanda.oferta.campus = :campus ");
		
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "disciplina", disciplina );
		q.setParameter( "campus", campus );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findMatriculasBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		String aluno, String matricula, Campus campus, Curso curso,
		int firstResult, int maxResults, String orderBy )
	{
		aluno = ( ( aluno == null ) ? "" : aluno );
		aluno = ( "%" + aluno.replace( '*', '%' ) + "%" );
		matricula = ( ( matricula == null ) ? "" : matricula );
		matricula = ( "%" + matricula.replace( '*', '%' ) + "%" );

		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		String queryCampus = "";
		if ( campus != null )
		{
			queryCampus = ( " o.demanda.oferta.campus = :campus AND " );
		}
		
		String queryCurso = "";
		if ( curso != null )
		{
			queryCurso = ( " o.demanda.oferta.curso = :curso AND " );
		}
		
		List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findByCenario(instituicaoEnsino, cenario);
		List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
		for (TipoDisciplina td : tiposDisciplinas) {
			if (td.ocupaGrade()) {
				tiposDisciplinasPresenciais.add(td);
			}
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE  o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND  o.demanda.oferta.campus.cenario = :cenario " +
			" AND " + queryCampus + queryCurso + " LOWER ( o.aluno.nome ) LIKE LOWER ( :aluno ) " +
			" AND LOWER ( o.aluno.matricula ) LIKE LOWER ( :matricula ) " +
			" AND ( o.prioridade = 1 OR o.atendido = TRUE )" + " AND o.demanda.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais)" +
			" AND ( o.demanda.disciplina.creditosTeorico > 0 OR o.demanda.disciplina.creditosPratico > 0 )" +
			" GROUP BY o.aluno ");

		if ( curso != null )
		{
			q.setParameter("curso", curso);
		}
		if ( campus != null )
		{
			q.setParameter("campus", campus);
		}

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "aluno", aluno );
		q.setParameter( "matricula", matricula );
		q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AlunoDemanda > findDisciplinasBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, 
		String codigo, Campus campus, Curso curso,
		int firstResult, int maxResults, String orderBy )
	{
		codigo = ( ( codigo == null ) ? "" : codigo );
		codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		String queryCampus = "";
		if ( campus != null )
		{
			queryCampus = ( " o.demanda.oferta.campus = :campus AND " );
		}
		
		String queryCurso = "";
		if ( curso != null )
		{
			queryCurso = ( " o.demanda.oferta.curso = :curso AND " );
		}

		List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findByCenario(instituicaoEnsino, cenario);
		List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
		for (TipoDisciplina td : tiposDisciplinas) {
			if (td.ocupaGrade()) {
				tiposDisciplinasPresenciais.add(td);
			}
		}
		
		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE  o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND  o.demanda.oferta.campus.cenario = :cenario " +
			" AND " + queryCampus + queryCurso + " LOWER ( o.demanda.disciplina.codigo ) LIKE LOWER ( :codigo ) " +
			" AND o.demanda.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais) " +
			" GROUP BY o.demanda.disciplina, o.demanda.oferta.campus ");

		if ( curso != null )
		{
			q.setParameter("curso", curso);
		}
		if ( campus != null )
		{
			q.setParameter("campus", campus);
		}

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "codigo", codigo );
		q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}
	
	public static int countMatriculas(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		String aluno, String matricula, Campus campus, Curso curso )
	{
		aluno = ( ( aluno == null ) ? "" : aluno );
		aluno = ( "%" + aluno.replace( '*', '%' ) + "%" );
		matricula = ( ( matricula == null ) ? "" : matricula );
		matricula = ( "%" + matricula.replace( '*', '%' ) + "%" );

		String queryCampus = "";
		if ( campus != null )
		{
			queryCampus = ( " o.demanda.oferta.campus = :campus AND " );
		}
		
		String queryCurso = "";
		if ( curso != null )
		{
			queryCurso = ( " o.demanda.oferta.curso = :curso AND " );
		}
		
		List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findByCenario(instituicaoEnsino, cenario);
		List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
		for (TipoDisciplina td : tiposDisciplinas) {
			if (td.ocupaGrade()) {
				tiposDisciplinasPresenciais.add(td);
			}
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT (DISTINCT o.aluno ) FROM AlunoDemanda o " +
			" WHERE  o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND  o.demanda.oferta.campus.cenario = :cenario " +
			" AND " + queryCampus + queryCurso + " LOWER ( o.aluno.nome ) LIKE LOWER ( :aluno ) " +
			" AND ( o.prioridade = 1 OR o.atendido = TRUE )" + " AND o.demanda.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais)" +
			" AND ( o.demanda.disciplina.creditosTeorico > 0 OR o.demanda.disciplina.creditosPratico > 0 )" +
			" AND LOWER ( o.aluno.matricula ) LIKE LOWER ( :matricula ) ");

		if ( curso != null )
		{
			q.setParameter("curso", curso);
		}
		if ( campus != null )
		{
			q.setParameter("campus", campus);
		}

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "aluno", aluno );
		q.setParameter( "matricula", matricula );
		q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);

		return ( (Number) q.getSingleResult() ).intValue();
	}
	
	public static int countDisciplinas(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			String codigo, Campus campus, Curso curso )
		{
			codigo = ( ( codigo == null ) ? "" : codigo );
			codigo = ( "%" + codigo.replace( '*', '%' ) + "%" );

			String queryCampus = "";
			if ( campus != null )
			{
				queryCampus = ( " o.demanda.oferta.campus = :campus AND " );
			}
			
			String queryCurso = "";
			if ( curso != null )
			{
				queryCurso = ( " o.demanda.oferta.curso = :curso AND " );
			}
			
			List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findByCenario(instituicaoEnsino, cenario);
			List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
			for (TipoDisciplina td : tiposDisciplinas) {
				if (td.ocupaGrade()) {
					tiposDisciplinasPresenciais.add(td);
				}
			}

			Query q = entityManager().createQuery(
				" SELECT DISTINCT o.demanda.disciplina, o.demanda.oferta.campus FROM AlunoDemanda o " +
				" WHERE  o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
				" AND  o.demanda.oferta.campus.cenario = :cenario " +
				" AND " + queryCampus + queryCurso + " LOWER ( o.demanda.disciplina.codigo ) LIKE LOWER ( :codigo ) " +
				" AND o.demanda.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais) ");

			if ( curso != null )
			{
				q.setParameter("curso", curso);
			}
			if ( campus != null )
			{
				q.setParameter("campus", campus);
			}

			q.setParameter( "instituicaoEnsino", instituicaoEnsino );
			q.setParameter( "cenario", cenario );
			q.setParameter( "codigo", codigo );;
			q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);
			
			q.getResultList().size();

			return q.getResultList().size();
		}

	public static Integer sumCredAlunosP1Atendidos(
			InstituicaoEnsino instituicaoEnsino, Campus campus) {

		List<TipoDisciplina> tiposDisciplinas = TipoDisciplina.findAll(instituicaoEnsino);
		List<TipoDisciplina> tiposDisciplinasPresenciais = new ArrayList<TipoDisciplina>(tiposDisciplinas.size());
		for (TipoDisciplina td : tiposDisciplinas) {
			if (td.ocupaGrade()) {
				tiposDisciplinasPresenciais.add(td);
			}
		}

		Query q = entityManager().createQuery(
			" SELECT SUM(o.demanda.disciplina.creditosTeorico + o.demanda.disciplina.creditosPratico) " +
			" FROM AlunoDemanda o WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.oferta.campus = :campus AND o.prioridade = 1 AND o.atendido = true " +
			" AND o.demanda.disciplina.tipoDisciplina IN (:tiposDisciplinasPresenciais) "
		);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);
		q.setParameter("campus",campus);
		q.setParameter("tiposDisciplinasPresenciais",tiposDisciplinasPresenciais);

		Object rs = q.getSingleResult();
		return ( rs == null ? 0 : ( (Number) rs ).intValue() );
	}

	@SuppressWarnings("unchecked")
	public static List<AlunoDemanda> findByCampus(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus) {
		
		String campusQuery = "";
		if (campus != null)
		{
			campusQuery = "AND o.demanda.oferta.campus = :campus";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.cenario = :cenario " +
			campusQuery );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		if (campus != null)
		{
			q.setParameter( "campus", campus );
		}

		return q.getResultList();
	}
	
	public static int countAlunosUteis(InstituicaoEnsino instituicaoEnsino, Cenario cenario)
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT (o.aluno) FROM AlunoDemanda o " +
			" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.demanda.disciplina.cenario = :cenario " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		
		return q.getResultList().size();
	}

	public static int count(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus, Curso curso, Curriculo curriculo, Turno turno,
			Disciplina disciplina, Integer periodo,String matricula,String nome,
			Integer prioridade, Boolean atendido ) {
	   	
		String queryString = "";
		if ( campus != null )
		{
			queryString	+= " o.demanda.oferta.campus = :campus AND ";
		}

		if ( curso != null )
		{
			queryString += " o.demanda.oferta.curriculo.curso = :curso AND ";
		}

		if ( curriculo != null )
		{
			queryString += " o.demanda.oferta.curriculo = :curriculo AND ";
		}

		if ( turno != null )
		{
			queryString += " o.demanda.oferta.turno = :turno AND ";
		}

		if ( disciplina != null )
		{
			queryString	+= " o.demanda.disciplina = :disciplina AND ";
		}
		
		if(periodo != null){
			queryString += " o.periodo = :periodo and ";
		}
		
		if(prioridade != null){
			queryString += " o.prioridade = :prioridade and ";
		}
		
		if(atendido != null){
			queryString += " o.atendido = :atendido and ";
		}
		
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		matricula = ( ( matricula == null ) ? "" : matricula );
		matricula = ( "%" + matricula.replace( '*', '%' ) + "%" );

        Query q = entityManager().createQuery(
        	" SELECT o FROM AlunoDemanda o " +
        	" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.demanda.oferta.campus.cenario = :cenario " +
        	" AND LOWER ( o.aluno.nome ) LIKE LOWER ( :nome ) " +
        	" AND LOWER ( o.aluno.matricula ) LIKE LOWER ( :matricula ) " +
        	" AND " + queryString + " 1=1 " );

        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
        q.setParameter( "nome", nome );
        q.setParameter( "matricula", matricula );

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

        if( periodo != null)
        {
        	q.setParameter( "periodo", periodo );
        }
        
        if(prioridade != null){
        	q.setParameter("prioridade", prioridade);
        }
        
        if(atendido != null){
        	q.setParameter("atendido", atendido);
        }

        return q.getResultList().size();
	}
	
	@SuppressWarnings("unchecked")
	public static List<AlunoDemanda> findBy(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus, Curso curso, Curriculo curriculo, Turno turno,Disciplina disciplina,
			 Integer periodo,String matricula,String nome,
				Integer prioridade, Boolean atendido,
			int firstResult, int maxResults, String orderBy ) {
	   	
        if (orderBy != null)
        {
        	if (orderBy.contains("turno") || orderBy.contains("campus") || orderBy.contains("curso") || orderBy.contains("curriculo"))
        		orderBy = "ORDER BY o.demanda.oferta." + orderBy.replace("String", "");
        	else if (orderBy.contains("disciplina") || orderBy.contains("periodo"))
        		orderBy = "ORDER BY o.demanda." + orderBy.replace("String", "");
        	else
        		orderBy = "ORDER BY o." + orderBy.replace("String", "");
        }
        else
        {
        	orderBy = "";
        }

		String queryString = "";
		if ( campus != null )
		{
			queryString	+= " o.demanda.oferta.campus = :campus AND ";
		}

		if ( curso != null )
		{
			queryString += " o.demanda.oferta.curriculo.curso = :curso AND ";
		}

		if ( curriculo != null )
		{
			queryString += " o.demanda.oferta.curriculo = :curriculo AND ";
		}

		if ( turno != null )
		{
			queryString += " o.demanda.oferta.turno = :turno AND ";
		}

		if ( disciplina != null )
		{
			queryString	+= " o.demanda.disciplina = :disciplina AND ";
		}
		
		if(periodo != null){
			queryString += " o.periodo = :periodo and ";
		}
		
		if(prioridade != null){
			queryString += " o.prioridade = :prioridade and ";
		}
		
		if(atendido != null){
			queryString += " o.atendido = :atendido and ";
		}
		
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		matricula = ( ( matricula == null ) ? "" : matricula );
		matricula = ( "%" + matricula.replace( '*', '%' ) + "%" );


        Query q = entityManager().createQuery(
        	" SELECT o FROM AlunoDemanda o " +
        	" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.demanda.oferta.campus.cenario = :cenario " +
        	" AND LOWER ( o.aluno.nome ) LIKE LOWER ( :nome ) " +
        	" AND LOWER ( o.aluno.matricula ) LIKE LOWER ( :matricula ) " +
        	" AND " + queryString + " 1=1 " + orderBy );

        q.setFirstResult( firstResult );
        q.setMaxResults( maxResults );
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
        q.setParameter( "matricula", matricula );
        q.setParameter( "nome", nome );

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
        
        if(prioridade != null){
        	q.setParameter("prioridade", prioridade);
        }
        
        if(atendido != null){
        	q.setParameter("atendido", atendido);
        }

        return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List<AlunoDemanda> findAlunosEquivalentes(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Disciplina disciplina, String turma, Long turmaId)
	{
		String turmaQuery = "";
		if (turmaId != null)
		{
			turmaQuery = "t.id = :turmaId OR ";
		}
        Query q = entityManager().createQuery(
        	" SELECT DISTINCT(o) FROM AlunoDemanda o, IN (o.demanda.disciplina.eliminadasPor) eliminadasPor " +
        	" LEFT JOIN o.atendimentosOperacional ao LEFT JOIN o.atendimentosTatico at LEFT JOIN o.turmas t" +
        	" WHERE o.demanda.oferta.campus.instituicaoEnsino = :instituicaoEnsino " +
        	" AND o.demanda.oferta.campus.cenario = :cenario " +
        	" AND eliminadasPor.cursou = :disciplina " +
			" AND (" +turmaQuery + "o.turmas IS EMPTY) " +
			" AND ((ao.turma = :turma AND ao.disciplinaSubstituta = :disciplina) " +
			"      OR (at.turma = :turma AND at.disciplinaSubstituta = :disciplina) OR o.atendido is FALSE) ");
        
        q.setParameter( "instituicaoEnsino", instituicaoEnsino );
        q.setParameter( "cenario", cenario );
        q.setParameter( "disciplina", disciplina );
        q.setParameter( "turma", turma );
        
        if (turmaId != null)
        {
            q.setParameter( "turmaId", turmaId );
        }
        
        return q.getResultList();
	}
	
	public Set<Turma> getTurmas() {
		return turmas;
	}

	public void setTurmas(Set<Turma> turmas) {
		this.turmas = turmas;
	}

	public AlunoDemanda clone(CenarioClone novoCenario) {
		AlunoDemanda clone = new AlunoDemanda();
		clone.setAluno(novoCenario.getEntidadeClonada(this.getAluno()));
		clone.setAtendido(this.getAtendido());
		clone.setDemanda(novoCenario.getEntidadeClonada(this.getDemanda()));
		clone.setPeriodo(this.getPeriodo());
		clone.setPrioridade(this.getPrioridade());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, AlunoDemanda entidadeClone) {

	}
}
