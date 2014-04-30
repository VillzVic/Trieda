package com.gapso.trieda.domain;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
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
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.Version;
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.Size;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.core.io.ClassPathResource;
import org.springframework.core.io.Resource;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;


@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "CEN_ID" )
@Table( name = "CENARIOS" )
public class Cenario
	implements Serializable, Comparable< Cenario >
{
	private static final long serialVersionUID = -8610380359760552949L;

	@OneToMany( mappedBy = "cenario" )
	private Set< Parametro > parametros;

	@ManyToOne( targetEntity = Usuario.class )
	@JoinColumn( name = "USU_CRIACAO_ID" )
	private Usuario criadoPor;

	@ManyToOne( targetEntity = Usuario.class )
	@JoinColumn( name = "USU_ATUALIZACAO_ID" )
	private Usuario atualizadoPor;

	@NotNull
	@Column( name = "CEN_MASTERDATA" )
	private Boolean masterData;

	@NotNull
	@Column( name = "CEN_NOME" )
	@Size( min = 1, max = 50 )
	private String nome;

	@Column( name = "CEN_ANO" )
	@Min( 1L )
	@Max( 9999L )
	private Integer ano;

	@Column( name = "CEN_SEMESTRE" )
	@Min( 1L )
	@Max( 12L )
	private Integer semestre;

	@Column( name = "CEN_DT_CRIACAO" )
	@Temporal( TemporalType.TIMESTAMP )
	@DateTimeFormat( style = "S-" )
	private Date dataCriacao;

	@Column( name = "CEN_DT_ATUALIZACAO" )
	@Temporal( TemporalType.TIMESTAMP )
	@DateTimeFormat( style = "S-" )
	private Date dataAtualizacao;

	@Column( name = "CEN_COMENTARIO" )
	private String comentario;

	@Column( name = "CEN_OFICIAL" )
	private Boolean oficial;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST,
		CascadeType.MERGE, CascadeType.REFRESH },
		targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;
	
	public boolean hasAlunos(){
		return !this.alunos.isEmpty();
	}
	
	public boolean hasProfessores(){
		return !this.professores.isEmpty();
	}
	
	public boolean hasSalas(){
		boolean output = false;
		
		for(Campus campus : this.campi){
			for(Unidade unidade : campus.getUnidades() ){
				if(!unidade.getSalas().isEmpty()){
					output = true;
				}
			}
		}
		
		return output;
	}
	
	public boolean isOptimized(){
		boolean output = false;
		for(Campus campus : this.campi){
			if(campus.isOtimizado(this.instituicaoEnsino)){
				output = true;
			}
		}
		
		return output;
		
	}

	public InstituicaoEnsino getInstituicaoEnsino()
	{
		return this.instituicaoEnsino;
	}

	public void setInstituicaoEnsino(
		InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
	}

	@ManyToMany( cascade = CascadeType.ALL, mappedBy = "cenario" )
	private Set< DivisaoCredito > divisoesCredito = new HashSet< DivisaoCredito >();

	@OneToMany( mappedBy = "cenario" )
	private Set< Turno > turnos = new HashSet< Turno >();

	@OneToMany( mappedBy = "cenario" )
	private Set< Curso > cursos = new HashSet< Curso >();

	@OneToMany( mappedBy = "cenario" )
	private Set< Campus > campi = new HashSet< Campus >();

	@OneToMany( mappedBy = "cenario" )
	private Set< Disciplina > disciplinas = new HashSet< Disciplina >();

	@OneToMany( mappedBy = "cenario" )
	private Set< Professor > professores = new HashSet< Professor >();

	@OneToMany( mappedBy = "cenario" )
	private Set< Curriculo > curriculos = new HashSet< Curriculo >();
	
	@OneToMany( mappedBy = "cenario" )
	private Set< Turma > turmas = new HashSet< Turma >();
	
	@OneToMany( mappedBy = "cenario" )
	private Set< Aula > aulas = new HashSet< Aula >();
	
	@OneToMany( mappedBy = "cenario" )
	private Set< SemanaLetiva > semanasLetivas = new HashSet< SemanaLetiva >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "cenario" )
	private Set< AtendimentoOperacional > atendimentosOperacionais = new HashSet< AtendimentoOperacional >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "cenario" )
	private Set< AtendimentoTatico > atendimentosTaticos = new HashSet< AtendimentoTatico >();
	
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "cenario" )
	private Set< Aluno > alunos = new HashSet< Aluno >();
	
	@OneToMany(mappedBy = "cenario")
	private Set<RequisicaoOtimizacao> requisicoesDeOtimizacao = new HashSet<RequisicaoOtimizacao>();

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "CriadoPor: " ).append( getCriadoPor() ).append( ", " );
		sb.append( "AtualizadoPor: " ).append( getAtualizadoPor() ).append( ", " );
		sb.append( "MasterData: " ).append( getMasterData() ).append( ", " );
		sb.append( "Nome: " ).append( getNome() ).append( ", " );
		sb.append( "Ano: " ).append( getAno() ).append( ", " );
		sb.append( "Semestre: " ).append( getSemestre() ).append( ", " );
		sb.append( "DataCriacao: " ).append( getDataCriacao() ).append( ", " );
		sb.append( "DataAtualizacao: " ).append( getDataAtualizacao() ).append( ", " );
		sb.append( "Comentario: " ).append( getComentario() ).append( ", " );
		sb.append( "Oficial: " ).append( getOficial() ).append( ", " );
		sb.append( "DivisoesCredito: " ).append( getDivisoesCredito() == null ?
			"null" : getDivisoesCredito().size() ).append( ", " );
		sb.append( "Turnos: " ).append( getTurnos() == null ? "null" : getTurnos().size() ).append( ", " );
		sb.append( "Cursos: " ).append( getCursos() == null ? "null" : getCursos().size() ).append( ", " );
		sb.append( "Campi: " ).append( getCampi() == null ? "null" : getCampi().size() ).append( ", " );
		sb.append( "Disciplinas: " ).append( getDisciplinas() == null ? "null" : getDisciplinas().size() ).append( ", " );
		sb.append( "Professores: " ).append( getProfessores() == null ? "null" : getProfessores().size() ).append( ", " );
		sb.append( "Curriculos: " ).append( getCurriculos() == null ? "null" : getCurriculos().size() ).append( ", " );
		sb.append( "Atendimentos Operacionais: " ).append( getAtendimentosOperacionais() == null ?
			"null" : getAtendimentosOperacionais().size() ).append( ", " );
		sb.append( "Atendimentos Taticos: " ).append( getAtendimentosTaticos() == null ?
			"null" : getAtendimentosTaticos().size() ).append( ", " );
		sb.append( "Parametros: " ).append( getParametros() == null ?
			"null" : getParametros().size() );
		sb.append("Requisições de Otimização: ").append(getRequisicoesDeOtimizacao() == null ? "null" : getRequisicoesDeOtimizacao().size() ).append( ", " );

		return sb.toString();
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "CEN_ID" )
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
			removeDivisoesCredito();
			removeEntities(this);
			this.entityManager.remove( this );
		}
		else
		{
			Cenario attached = this.entityManager.find(
				this.getClass(), this.id );

			attached.removeDivisoesCredito();
			removeEntities(this);
			this.entityManager.remove( attached );
		}
	}

	private void removeEntities(Cenario cenario) {
		List<Query> nativeQueries = new ArrayList<Query>();
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adao FROM alunos_demanda_atendimentos_operacional adao JOIN atendimento_operacional ao ON adao.atendimentos_operacional = ao.atp_id WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE ao FROM atendimento_operacional ao WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adat FROM alunos_demanda_atendimentos_tatico adat JOIN atendimento_tatico at ON adat.atendimentos_tatico = at.att_id WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE at FROM atendimento_tatico at WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE ad FROM alunos_demanda ad JOIN demandas d ON ad.dem_id = d.dem_id JOIN ofertas o ON d.ofe_id = o.ofe_id JOIN campi c ON o.cam_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE d FROM demandas d JOIN ofertas o ON d.ofe_id = o.ofe_id JOIN campi c ON o.cam_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE o FROM ofertas o JOIN campi c ON o.cam_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE e FROM equivalencias e JOIN disciplinas d ON e.dis_cursou_id = d.dis_id WHERE d.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE d FROM deslocamentos_unidades d JOIN unidades u ON d.uni_dest_id = u.uni_id JOIN campi c ON u.cam_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hs FROM horario_disponivel_cenario_salas hs JOIN salas s ON hs.salas = s.sal_id JOIN unidades u ON s.uni_id = u.uni_id join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hs FROM horario_disponivel_cenario_campi hs JOIN campi c ON hs.campi = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hs FROM horario_disponivel_cenario_disciplinas hs JOIN disciplinas d ON hs.disciplinas = d.dis_id WHERE d.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hs FROM horario_disponivel_cenario_fixacoes hs JOIN fixacoes f ON hs.fixacoes = f.fix_id JOIN campi c ON f.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hs FROM horario_disponivel_cenario_professores hs JOIN professores p ON hs.professores = p.prf_id WHERE p.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hs FROM horario_disponivel_cenario_unidades hs JOIN unidades u ON hs.unidades = u.uni_id join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE ds FROM disciplinas_salas ds JOIN salas s ON s.sal_id = ds.sal_id JOIN unidades u ON s.uni_id = u.uni_id join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE cd FROM curriculos_disciplinas cd JOIN disciplinas d ON cd.dis_id = d.dis_id WHERE d.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pd FROM professores_disciplinas pd JOIN disciplinas d ON pd.dis_id = d.dis_id WHERE d.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE gss FROM grupos_sala_salas gss JOIN grupos_sala gs ON gss.grupos_sala = gs.grs_id JOIN unidades u ON gs.uni_id = u.uni_id join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE gs FROM grupos_sala gs JOIN unidades u ON gs.uni_id = u.uni_id join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE s FROM salas s JOIN unidades u ON s.uni_id = u.uni_id join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE u FROM unidades u join campi c ON u.cam_id = c.cam_id WHERE c.cen_id =:cenario "));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE d FROM deslocamentos_campi d JOIN campi c ON d.cam_dest_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE a FROM atendimentos_faixa_demanda a JOIN campi c ON a.cam_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE hdc FROM horario_disponivel_cenario hdc JOIN horarios_aula ha ON hdc.hor_id = ha.hor_id JOIN turnos t ON ha.tur_id = t.tur_id WHERE t.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE ha FROM horarios_aula ha JOIN turnos t ON ha.tur_id = t.tur_id WHERE t.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pt FROM parametros_turnos pt JOIN turnos t ON pt.tur_id = t.tur_id WHERE t.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pc FROM professores_campi pc JOIN campi c ON pc.campi = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pc FROM parametros_campi pc JOIN campi c ON pc.cam_id = c.cam_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE atc FROM areas_titulacao_cursos atc JOIN cursos c ON atc.cursos = c.cur_id WHERE c.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE pvd FROM professores_virtuais_disciplinas pvd JOIN professores_virtuais pv ON pvd.professores_virtuais = pv.prf_id WHERE pv.cen_id = :cenario"));
		
		List<Query> queries = new ArrayList<Query>();
		queries.add(entityManager().createQuery( "DELETE FROM AtendimentoTatico WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM AtendimentoOperacional WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Turno WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Aluno WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Curriculo WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Professor WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM ProfessorVirtual WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Disciplina WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Curso WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Parametro WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Campus WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM TipoContrato WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM TipoDisciplina WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM TipoSala WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM TipoCurso WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM Titulacao WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM AreaTitulacao WHERE cenario = :cenario"));
		queries.add(entityManager().createQuery( "DELETE FROM SemanaLetiva WHERE cenario = :cenario"));
		for (Query q : nativeQueries)
		{
			q.setParameter( "cenario", cenario.getId() );
			q.executeUpdate();
		}
		
		for (Query q : queries)
		{
			q.setParameter( "cenario", cenario );
			q.executeUpdate();
		}
	}
	
	public static void limpaSolucoesCenario(Cenario cenario) {
		List<Query> nativeQueries = new ArrayList<Query>();
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adao FROM alunos_demanda_atendimentos_operacional adao JOIN atendimento_operacional ao ON adao.atendimentos_operacional = ao.atp_id WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adat FROM alunos_demanda_atendimentos_tatico adat JOIN atendimento_tatico at ON adat.atendimentos_tatico = at.att_id WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE adat FROM alunos_demanda_atendimentos_tatico adat JOIN atendimento_tatico at ON adat.atendimentos_tatico = at.att_id WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE aomu FROM atendimento_operacional_motivos_uso aomu JOIN atendimento_operacional ao ON aomu.atp_id = ao.atp_id  WHERE ao.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE at FROM atendimento_tatico at WHERE at.cen_id = :cenario"));
		nativeQueries.add(entityManager().createNativeQuery( "DELETE ao FROM atendimento_operacional ao WHERE ao.cen_id = :cenario"));
		
		for (Query q : nativeQueries)
		{
			q.setParameter( "cenario", cenario.getId() );
			q.executeUpdate();
		}
		
	}

	@Transactional
	public void removeDivisoesCredito()
	{
		Set< DivisaoCredito > divisoesCredito
			= this.getDivisoesCredito();

		for ( DivisaoCredito divisaoCredito : divisoesCredito )
		{
			divisaoCredito.getCenario().remove( this );
			divisaoCredito.merge();
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
	public Cenario merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Cenario merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Cenario().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino )
	{
		return ( (Number) entityManager().createQuery(
			" SELECT count( o ) FROM Cenario o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino )
			.getSingleResult() ).intValue();
	}

	public Parametro getUltimoParametro(InstituicaoEnsino instituicaoEnsino) {
		long maiorId = -1;
		Parametro parametro = null;
		
		for (Parametro param : parametros) {
			if (param.getId() > maiorId) {
				maiorId = param.getId();
				parametro = param;
			}
		}
		
		return parametro;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Cenario > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Cenario o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.masterData = :masterData " );

		q.setParameter( "masterData", false );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static Cenario find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Cenario cenario = entityManager().find( Cenario.class, id );
		
		if (cenario != null && cenario.getInstituicaoEnsino().equals(instituicaoEnsino))
		{
			return cenario;
		}

		return null;
	}

	@SuppressWarnings( "unchecked" )
	public static Cenario findMasterData(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Cenario o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.masterData = :masterData " );

		q.setParameter( "masterData", true );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Cenario cenario = null;

		try
		{
			List< Cenario > listCenarios = q.getResultList(); 

			if ( listCenarios != null && listCenarios.size() > 0 )
			{
				cenario = listCenarios.get( 0 );
			}
		}
		catch ( EmptyResultDataAccessException e ) { }

		return cenario;
	}

	public static List< Cenario > find(
		InstituicaoEnsino instituicaoEnsino, int firstResult, int maxResults )
	{
		return find( instituicaoEnsino, firstResult, maxResults, null );
	}

	@SuppressWarnings( "unchecked" )
	public static List< Cenario > find( InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		Query q = entityManager().createQuery(
			" SELECT o FROM Cenario o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.masterData = :masterData " + orderBy );

		q.setParameter( "masterData", false );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Cenario > findByAnoAndSemestre(
		InstituicaoEnsino instituicaoEnsino, Integer ano,
		Integer semestre, int firstResult, int maxResults, String orderBy )
	{
		orderBy = ( ( orderBy != null ) ? " ORDER BY o." + orderBy : "" );

		String queryAno = "";
		String querySemestre = "";

		if ( ano != null )
		{
			queryAno = " o.ano = :ano AND ";
		}

		if ( ano != null )
		{
			querySemestre = " o.semestre = :semestre AND ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM Cenario o " +
			" WHERE " + queryAno + querySemestre +
			" o.masterData = :masterData " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "masterData", false );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		if ( ano != null )
		{
			q.setParameter( "ano", semestre );
		}

		if ( semestre != null )
		{
			q.setParameter( "ano", semestre );
		}

		return q.getResultList();
	}
	
	public static Integer getDBVersion(){
		Query q = entityManager().createNativeQuery(
				" SELECT MAX(db_version) FROM db_version d " );
		
		return (Integer) q.getSingleResult();
	}

	public Usuario getCriadoPor()
	{
		return this.criadoPor;
	}

	public void setCriadoPor( Usuario criadoPor )
	{
		this.criadoPor = criadoPor;
	}

	public Usuario getAtualizadoPor()
	{
		return this.atualizadoPor;
	}

	public void setAtualizadoPor( Usuario atualizadoPor )
	{ 
		this.atualizadoPor = atualizadoPor;
	}

	public Boolean getMasterData()
	{
		return masterData;
	}

	public void setMasterData( Boolean masterData )
	{
		this.masterData = masterData;
	}

	public String getNome()
	{
		return this.nome;
	}

	public void setNome( String nome )
	{
		this.nome = nome;
	}

	public Integer getAno()
	{
		return this.ano;
	}

	public void setAno( Integer ano )
	{
		this.ano = ano;
	}

	public Integer getSemestre()
	{
		return this.semestre;
	}

	public void setSemestre( Integer semestre )
	{
		this.semestre = semestre;
	}

	public Date getDataCriacao()
	{
		return this.dataCriacao;
	}

	public void setDataCriacao( Date dataCriacao )
	{
		this.dataCriacao = dataCriacao;
	}

	public Date getDataAtualizacao()
	{
		return this.dataAtualizacao;
	}

	public void setDataAtualizacao( Date dataAtualizacao )
	{
		this.dataAtualizacao = dataAtualizacao;
	}

	public String getComentario()
	{
		return this.comentario;
	}

	public void setComentario( String comentario )
	{
		this.comentario = comentario;
	}

	public Boolean getOficial()
	{
		return this.oficial;
	}

	public void setOficial( Boolean oficial )
	{
		this.oficial = oficial;
	}

	public Set< DivisaoCredito > getDivisoesCredito()
	{
		return this.divisoesCredito;
	}

	public void setDivisoesCredito(
		Set< DivisaoCredito > divisoesCredito )
	{
		this.divisoesCredito = divisoesCredito;
	}

	public Set< Turno > getTurnos()
	{
		return this.turnos;
	}

	public void setTurnos( Set< Turno > turnos )
	{
		this.turnos = turnos;
	}

	public Set< Curso > getCursos()
	{
		return this.cursos;
	}

	public void setCursos( Set< Curso > cursos )
	{
		this.cursos = cursos;
	}

	public Set< Campus > getCampi()
	{
		return this.campi;
	}

	public void setCampi( Set< Campus > campi )
	{
		this.campi = campi;
	}
	
	public Set<RequisicaoOtimizacao> getRequisicoesDeOtimizacao() {
		return this.requisicoesDeOtimizacao;
	}

	public void setRequisicoesDeOtimizacao(Set<RequisicaoOtimizacao> requisicoesDeOtimizacao) {
		this.requisicoesDeOtimizacao = requisicoesDeOtimizacao;
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

	public Set< Professor > getProfessores()
	{
		return this.professores;
	}

	public void setProfessores( Set< Professor > professores )
	{
		this.professores = professores;
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

	public Set< Parametro > getParametros()
	{
		return parametros;
	}

	public void setParametro(
		Set< Parametro > parametros )
	{
		this.parametros = parametros;
	}
	
	public Set<Aluno> getAlunos() {
		return alunos;
	}

	public void setAlunos(Set<Aluno> alunos) {
		this.alunos = alunos;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Cenario ) )
		{
			return false;
		}

		Cenario other = (Cenario) obj;

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

		return true;
	}

	//@Override
	public int compareTo( Cenario o )
	{
		int result = getAno().compareTo(o.getAno());
		if (result == 0) {
			result = getSemestre().compareTo(o.getSemestre());
			if (result == 0) {
				result = getNome().compareTo(o.getNome());
			}
		}

		return result;
	}
	
	public static void executeSqlUpdate(Integer currentVer, Integer finalVer){
		Query q = entityManager().createNativeQuery("BEGIN " + getSqlFileContents("convertDBFrom_Version013_To_Version014.sql") + " END;");
		
		q.executeUpdate();
	}
	
	public static String getSqlFileContents(String fileName) {
	    StringBuffer sb = new StringBuffer();
	    try {
	        Resource resource = new ClassPathResource(fileName);
	        DataInputStream in = new DataInputStream(resource.getInputStream());
	        BufferedReader br = new BufferedReader(new InputStreamReader(in));
	        String strLine;
	        while ((strLine = br.readLine()) != null) {
	            sb.append(" " + strLine);
	        }
	    } catch (FileNotFoundException e) {
	        e.printStackTrace();
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
	    return sb.toString();
	}

	public Set< Turma > getTurmas() {
		return turmas;
	}

	public void setTurmas(Set< Turma > turmas) {
		this.turmas = turmas;
	}

	public Set< Aula > getAulas() {
		return aulas;
	}

	public void setAulas(Set< Aula > aulas) {
		this.aulas = aulas;
	}

	public Set<SemanaLetiva> getSemanasLetivas() {
		return semanasLetivas;
	}

	public void setSemanasLetivas(Set<SemanaLetiva> semanasLetivas) {
		this.semanasLetivas = semanasLetivas;
	}
	
	
}
