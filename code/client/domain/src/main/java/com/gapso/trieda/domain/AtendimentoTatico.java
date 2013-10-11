package com.gapso.trieda.domain;

import java.io.Serializable;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.Enumerated;
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
import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

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
@RooEntity( identifierColumn = "ATT_ID" )
@Table( name = "ATENDIMENTO_TATICO" )
public class AtendimentoTatico
	implements Serializable
{
	private static final long serialVersionUID = 6191028820294903254L;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	@NotNull
	private String turma;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Sala.class )
	@JoinColumn( name = "SAL_ID" )
	private Sala sala;

	@NotNull
	@Enumerated
	private Semanas semana;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Oferta.class )
	@JoinColumn( name = "OFE_ID" )
	private Oferta oferta;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Disciplina.class )
	@JoinColumn( name = "DIS_ID" )
	private Disciplina disciplina;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Disciplina.class )
	@JoinColumn( name = "DIS_SUBSTITUTA_ID" )
	private Disciplina disciplinaSubstituta;
	
	@ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = HorarioAula.class )
	@JoinColumn( name = "HOR_ID" )
	private HorarioAula horarioAula;
	
	@ManyToMany(cascade = { CascadeType.PERSIST, CascadeType.MERGE},
		mappedBy="atendimentosTatico")
	private Set<AlunoDemanda> alunosDemanda = new HashSet<AlunoDemanda>();

	@NotNull
	@Column( name = "ATT_QUANTIDADE" )
	@Min( 0L )
	@Max( 999L )
	private Integer quantidadeAlunos;

	@NotNull
	@Column( name = "ATT_CRED_TEORICO" )
	@Min( 0L )
	@Max( 99L )
	private Integer creditosTeorico;

	@NotNull
	@Column( name = "ATT_CRED_PRATICO" )
	@Min( 0L )
	@Max( 99L )
	private Integer creditosPratico;

	@NotNull
	@ManyToOne( cascade = { CascadeType.PERSIST,
		CascadeType.MERGE, CascadeType.REFRESH }, targetEntity = InstituicaoEnsino.class )
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

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Instituicoes de Ensino: " ).append( getInstituicaoEnsino() ).append( ", " );
		sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
		sb.append( "Turma: " ).append( getTurma() ).append( ", " );
		sb.append( "Sala: " ).append( getSala() ).append( ", " );
		sb.append( "Semana: " ).append( getSemana() ).append( ", " );
		sb.append( "HorarioAula: " ).append( getHorarioAula() ).append( ", " );
		sb.append( "Oferta: " ).append( getOferta() ).append( ", " );
		sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
		sb.append( "DisciplinaSubstituta: " ).append( getDisciplinaSubstituta() ).append( ", " );
		sb.append( "QuantidadeAlunos: " ).append( getQuantidadeAlunos() ).append( ", " );
		sb.append( "CreditosTeorico: " ).append( getCreditosTeorico() ).append( ", " );
		sb.append( "CreditosPratico: " ).append( getCreditosPratico() );

		return sb.toString();
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "ATT_ID" )
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
			AtendimentoTatico attached = this.entityManager.find(
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
	public AtendimentoTatico merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		AtendimentoTatico merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new AtendimentoTatico().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?) " );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findBySalaAndTurno(
		InstituicaoEnsino instituicaoEnsino, Sala sala,
		Turno turno, SemanaLetiva semanaLetiva )
	{
		String semanaLetivaQuery = "";

		if ( semanaLetiva != null )
		{
			semanaLetivaQuery = " AND o.oferta.curriculo.semanaLetiva = :semanaLetiva ";
		}
		
		String turnoQuery = "";
		
		if ( turno != null )
		{
			turnoQuery = " AND o.oferta.turno = :turno ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.sala = :sala "
			+ turnoQuery
			+ semanaLetivaQuery );

		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( semanaLetiva != null )
		{
			q.setParameter( "semanaLetiva", semanaLetiva );
		}
		if( turno != null)
		{
			q.setParameter( "turno", turno );
		}

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<AtendimentoTatico> findBy(
		InstituicaoEnsino instituicaoEnsino, Aluno aluno, Turno turno, Campus campus)
	{
		String turnoQuery = "";
		
		if ( turno != null )
		{
			turnoQuery = " AND o.oferta.turno = :turno";
		}
		
		String campusQuery = "";
		
		if ( campus != null )
		{
			campusQuery = " AND o.oferta.campus = :campus ";
		}
		
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o, " +
			" IN (o.alunosDemanda) ald" +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			turnoQuery +
			campusQuery +
			" AND ald.aluno = :aluno "
		);

		q.setParameter("aluno", aluno);
		q.setParameter("instituicaoEnsino", instituicaoEnsino);
		
		if ( turno != null )
		{
			q.setParameter("turno", turno);
		}
		
		if ( campus != null )
		{
			q.setParameter("campus", campus);
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findBy(
		InstituicaoEnsino instituicaoEnsino, Campus campus,
		Curriculo curriculo, Integer periodo, Turno turno, Curso curso )
	{
		String cursoQuery = "";

		if ( curso != null )
		{
			cursoQuery = "AND o.oferta.curso = :curso ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.oferta.curriculo = :curriculo " +
			" AND o.oferta.campus = :campus " + cursoQuery +
			" AND o.oferta.turno = :turno " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina IN (  SELECT d.disciplina FROM CurriculoDisciplina d "
								 + " WHERE d.curriculo = :curriculo AND d.periodo = :periodo ) " );

		q.setParameter( "campus", campus );
		q.setParameter( "curriculo", curriculo );
		q.setParameter( "periodo", periodo );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( curso != null )
		{
			q.setParameter( "curso", curso );
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Sala sala, SemanaLetiva semanaLetiva )
	{
		String semanaLetivaQuery = "";

		if ( semanaLetiva != null )
		{
			semanaLetivaQuery = " AND o.oferta.curriculo.semanaLetiva = :semanaLetiva ";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o "
			+ " WHERE cenario = :cenario"
			+ " AND o.instituicaoEnsino = :instituicaoEnsino "
			+ " AND o.sala = :sala"
			+ semanaLetivaQuery );

		q.setParameter( "cenario", cenario );
		q.setParameter( "sala", sala );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		if ( semanaLetiva != null )
		{
			q.setParameter( "semanaLetiva", semanaLetiva );
		}

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " ); 

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findAllBy(
		InstituicaoEnsino instituicaoEnsino,
		Campus campus, Turno turno )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.turno = :turno " );

		q.setParameter( "campus", campus );
		q.setParameter( "turno", turno );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<AtendimentoTatico> findAllBy(InstituicaoEnsino instituicaoEnsino, Collection<Campus> campi, Collection<Turno> turnos) {
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o "
		  + " WHERE o.oferta.campus IN ( :campi ) "
		  + " AND o.instituicaoEnsino = :instituicaoEnsino "
		  + " AND o.oferta.turno IN ( :turnos ) "
		);

		q.setParameter("campi",campi);
		q.setParameter("turnos",turnos);
		q.setParameter("instituicaoEnsino",instituicaoEnsino);

		return q.getResultList();
	}

	public static AtendimentoTatico find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		AtendimentoTatico at = entityManager().find( AtendimentoTatico.class, id );

		if ( at != null
			&& at.getInstituicaoEnsino() != null 
			&& at.getInstituicaoEnsino() == instituicaoEnsino )
		{
			return at;
		}

		return null;
	}

	public static int countTurma(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT count ( * ) FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario GROUP BY o.disciplina, o.turma " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList().size();
	}

	public static int countTurma(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createNativeQuery(
			" SELECT o.dis_id, o.turma FROM atendimento_tatico o " +
			" WHERE o.ins_id = :instituicaoEnsino AND o.dis_substituta_id IS NULL" +
			" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
			" GROUP BY o.dis_id, o.turma " +
			" UNION " +
			" SELECT o1.dis_substituta_id, o1.turma FROM atendimento_tatico o1 " +
			" WHERE o1.ins_id = :instituicaoEnsino  AND o1.dis_substituta_id IS NOT NULL " +
			" AND o1.ofe_id IN (select f1.ofe_id from ofertas f1 where f1.cam_id = :campus)" +
			" GROUP BY o1.dis_substituta_id, o1.turma ");
		
		q.setParameter( "campus", campus.getId() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		

		return q.getResultList().size();
	}
	
	public static double countTurmaByDisciplinas(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		double countTurmas = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(att_quantidade), o.turma FROM atendimento_tatico o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.att_cred_teorico, o.att_cred_pratico, o.semana, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> turmaIdToAlnCredAtendMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> turmaIdToDisAlnCredAtendMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			String turma = ((String)((Object[])registro)[3]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String turmaId = disIdASerUsada + "-" + turma;
			
			Map<Long,Integer> disToAlnCredAtendMap = turmaIdToDisAlnCredAtendMap.get(turmaId);
			if (disToAlnCredAtendMap == null) {
				disToAlnCredAtendMap = new HashMap<Long, Integer>();
				turmaIdToDisAlnCredAtendMap.put(turmaId,disToAlnCredAtendMap);
			}
			Integer totalAlnCredAtend = disToAlnCredAtendMap.get(disId);
			totalAlnCredAtend = (totalAlnCredAtend == null) ? 0 : totalAlnCredAtend;
			disToAlnCredAtendMap.put(disId, totalAlnCredAtend + totalAlunos);
			
			totalAlnCredAtend = turmaIdToAlnCredAtendMap.get(turmaId);
			totalAlnCredAtend = (totalAlnCredAtend == null) ? 0 : totalAlnCredAtend;
			turmaIdToAlnCredAtendMap.put(turmaId, totalAlnCredAtend + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalTurmas = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : turmaIdToDisAlnCredAtendMap.entrySet()) {
			String turmaId = entry.getKey();
			int totalAlnCredAtendDaTurma = turmaIdToAlnCredAtendMap.get(turmaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int alnCredAtendDaDiscNaTurma = entry2.getValue();
				double parcelaTotalTurmasDaDisc = (double)alnCredAtendDaDiscNaTurma / (double)totalAlnCredAtendDaTurma;
				
				Double totalTurmasDaDisc = disIdToTotalTurmas.get(disId);
				totalTurmasDaDisc = (totalTurmasDaDisc == null) ? 0.0 : totalTurmasDaDisc;
				disIdToTotalTurmas.put(disId, totalTurmasDaDisc + parcelaTotalTurmasDaDisc);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalTurmas.entrySet()) {
			Long disId = entry.getKey();
			if (disciplinasIDs.contains(disId)) {
				countTurmas += entry.getValue();
			}
		}

		return countTurmas;
	}

	public static int countCreditos(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query qT = entityManager().createQuery(
			" SELECT sum ( o.creditosTeorico ) " +
			" FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		Query qP = entityManager().createQuery(
			" SELECT sum ( o.creditosPratico ) " +
			" FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		qT.setParameter( "cenario", cenario );
		qT.setParameter( "instituicaoEnsino", instituicaoEnsino );

		qP.setParameter( "cenario", cenario );
		qP.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Object srT = qT.getSingleResult();
		Object srP = qP.getSingleResult();

		int iT = ( ( srT == null ) ? 0 : ( (Number) qT.getSingleResult() ).intValue() );
		int iP = ( ( srP == null ) ? 0 : ( (Number) qP.getSingleResult() ).intValue() );

		return ( iT + iP );
	}
	
	public static double countCreditosByTurmas(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		double countCreditos = 0;
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, o.dis_substituta_id, SUM(att_quantidade), o.att_cred_teorico, o.att_cred_pratico, o.semana, o.turma FROM atendimento_tatico o " +
				" WHERE o.ins_id = :instituicaoEnsino" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.dis_substituta_id, o.att_cred_teorico, o.att_cred_pratico, o.semana, o.turma");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		Map<String,Integer> tempoAulaIdToTotalAlnMap = new HashMap<String,Integer>();
		Map<String,Map<Long,Integer>> tempoAulaIdToDisAlnMap = new HashMap<String,Map<Long,Integer>>();
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			Long disSubsId = (((Object[])registro)[1]) == null ? null : ((BigInteger)((Object[])registro)[1]).longValue();
			int totalAlunos = ((BigDecimal)((Object[])registro)[2]).intValue();
			int credPratico = ((Integer)((Object[])registro)[3]).intValue();
			int credTeorico = ((Integer)((Object[])registro)[4]).intValue();
			int semana = ((Integer)((Object[])registro)[5]).intValue();
			String turma = ((String)((Object[])registro)[6]);
			
			Long disIdASerUsada = disSubsId != null ? disSubsId : disId;
			String tempoAulaId = disIdASerUsada + "-" + credPratico + "-" + credTeorico + "-" + semana + "-" + turma;
			
			Map<Long,Integer> disToAlnMap = tempoAulaIdToDisAlnMap.get(tempoAulaId);
			if (disToAlnMap == null) {
				disToAlnMap = new HashMap<Long, Integer>();
				tempoAulaIdToDisAlnMap.put(tempoAulaId,disToAlnMap);
			}
			disToAlnMap.put(disId,totalAlunos*(credTeorico+credPratico));
			
			Integer total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			total = (total == null) ? 0 : total;
			tempoAulaIdToTotalAlnMap.put(tempoAulaId, total + totalAlunos);
		}
		
		Map<Long,Double> disIdToTotalCred = new HashMap<Long, Double>();
		for (Entry<String,Map<Long,Integer>> entry : tempoAulaIdToDisAlnMap.entrySet()) {
			String tempoAulaId = entry.getKey();
			int total = tempoAulaIdToTotalAlnMap.get(tempoAulaId);
			for (Entry<Long,Integer> entry2 : entry.getValue().entrySet()) {
				Long disId = entry2.getKey();
				int parcialAlunos = entry2.getValue();
				double contribTotalAlunos = (double)parcialAlunos / (double)total;
				
				Double value = disIdToTotalCred.get(disId);
				value = (value == null) ? 0.0 : value;
				disIdToTotalCred.put(disId, value + contribTotalAlunos);
			}
		}
		
		for (Entry<Long, Double> entry : disIdToTotalCred.entrySet()) {
			Long disId = entry.getKey();
			if (disciplinasIDs.contains(disId)) {
				countCreditos += entry.getValue();
			}
		}

		return countCreditos;
	}
		
	public static double calcReceita(
			InstituicaoEnsino instituicaoEnsino, Campus campus, List< Disciplina > disciplinas )
	{
		Set<Long> disciplinasIDs = new HashSet<Long>(disciplinas.size());
		for (Disciplina d : disciplinas) {
			disciplinasIDs.add(d.getId());
		}
		
		Query q2 = entityManager().createNativeQuery(
				" SELECT o.dis_id, SUM(att_quantidade*of.ofe_receita), o.att_cred_teorico, o.att_cred_pratico, o.semana" +
				" FROM atendimento_tatico o, ofertas of " +
				" WHERE o.ins_id = :instituicaoEnsino AND o.ofe_id=of.ofe_id" +
				" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)" +
				" GROUP BY o.dis_id, o.att_cred_teorico, o.att_cred_pratico, o.semana");

		q2.setParameter( "campus", campus.getId() );
		q2.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		double receitaSemanalTotal = 0.0;
		for (Object registro : q2.getResultList()) {
			Long disId = ((BigInteger)((Object[])registro)[0]).longValue();
			if (disciplinasIDs.contains(disId)) {
				double receitaSemanal = ((Double)((Object[])registro)[1]).doubleValue();
				int countCreditos = ((Integer)((Object[])registro)[2]).intValue() + ((Integer)((Object[])registro)[3]).intValue();
				receitaSemanalTotal += receitaSemanal * countCreditos;
			}
		}

		return receitaSemanalTotal * 4.5 * 6.0;
	}
	
	public static double calcReceita(InstituicaoEnsino instituicaoEnsino, Campus campus) {
		Query q = entityManager().createNativeQuery(
			" SELECT SUM(o.att_quantidade*(o.att_cred_pratico+o.att_cred_teorico)*of.ofe_receita) FROM atendimento_tatico o, ofertas of " +
			" WHERE o.ins_id = :instituicaoEnsino AND o.ofe_id=of.ofe_id" +
			" AND o.ofe_id IN (select f.ofe_id from ofertas f where f.cam_id = :campus)");

		q.setParameter( "campus", campus.getId() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino.getId() );
		
		double receita = 0.0;
		for (Object registro : q.getResultList()) {
			double receitaLocal = (Double)registro;
			receita += receitaLocal;
		}

		return receita * 4.5 * 6.0;
	}

	public static int countSalasDeAula(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT count ( * ) FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.sala.tipoSala.id = 1 " +
			" AND o.cenario = :cenario GROUP BY o.sala " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList().size();
	}

	@SuppressWarnings( "unchecked" )
	public static int countSalasDeAula(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT count ( * ) FROM AtendimentoTatico o " +
			" WHERE o.sala.tipoSala.id = 1 " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus GROUP BY o.sala " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		List< AtendimentoTatico > list = q.getResultList();
		return list.size();
	}

	public static int countLaboratorios(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT count ( * ) FROM AtendimentoTatico o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.sala.tipoSala.id = 2 " +
			" AND o.cenario = :cenario GROUP BY o.sala" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList().size();
	}

	public static int countLaboratorios(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT count ( * ) FROM AtendimentoTatico o " +
			" WHERE o.sala.tipoSala.id = 2 " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.oferta.campus = :campus GROUP BY o.sala " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList().size();
	}

	public static int countDemandas(
		InstituicaoEnsino instituicaoEnsino , Cenario cenario)
	{
		Query q = entityManager().createQuery(
			" SELECT sum ( o.quantidadeAlunos ) " +
			" FROM AtendimentoTatico o " +
			" WHERE o.cenario = :cenario " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Object sr = q.getSingleResult();
		return ( sr == null ? 0 : ( (Number) q.getSingleResult() ).intValue() );
	}

	public static int countDemandas(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT sum ( o.quantidadeAlunos ) " +
			" FROM AtendimentoTatico o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Object sr = q.getSingleResult();
		return ( sr == null ? 0 : ( (Number) q.getSingleResult() ).intValue() );
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findAllByDemanda(
		InstituicaoEnsino instituicaoEnsino, Demanda demanda )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.oferta = :oferta " +
			" AND o.disciplina = :disciplina " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "oferta", demanda.getOferta() );
		q.setParameter( "disciplina", demanda.getDisciplina() );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findAllByCampus(
		InstituicaoEnsino instituicaoEnsino, Campus campus )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.oferta.campus = :campus " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findAllBy(
		InstituicaoEnsino instituicaoEnsino, Curso curso )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.oferta.curriculo.curso = :curso " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "curso", curso );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< AtendimentoTatico > findAllBy(
		InstituicaoEnsino instituicaoEnsino, Oferta oferta )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM AtendimentoTatico o " +
			" WHERE o.oferta = :oferta " +
			" AND o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "oferta", oferta );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< Object > findAllAlunosCargaHorariaTatico(
		InstituicaoEnsino instituicaoEnsino )
	{
		
		Query q = entityManager().createQuery(
				" SELECT a.aluno.matricula, at.semana, " +
				" SUM(at.creditosTeorico+at.creditosPratico)*at.horarioAula.semanaLetiva.tempo " +
				" FROM AlunoDemanda a JOIN a.atendimentosTatico at " +
				" WHERE at.instituicaoEnsino = :instituicaoEnsino" +
				" GROUP BY a.aluno, at.semana" );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
	}

	public String getTurma()
	{
		return this.turma;
	}

	public void setTurma( String turma )
	{
		this.turma = turma;
	}

	public Sala getSala()
	{
		return this.sala;
	}

	public void setSala( Sala sala )
	{
		this.sala = sala;
	}

	public Semanas getSemana()
	{
		return this.semana;
	}

	public void setSemana( Semanas semana )
	{
		this.semana = semana;
	}

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
	
	public Disciplina getDisciplinaSubstituta()
	{
		return this.disciplinaSubstituta;
	}

	public void setDisciplinaSubstituta( Disciplina disciplinaSubstituta )
	{
		this.disciplinaSubstituta = disciplinaSubstituta;
	}
	
	
	public HorarioAula getHorarioAula()
	{
		return this.horarioAula;
	}

	public void setHorarioAula( HorarioAula horarioAula )
	{
		this.horarioAula = horarioAula;
	}

	public Integer getQuantidadeAlunos()
	{
		return this.quantidadeAlunos;
	}

	public void setQuantidadeAlunos( Integer quantidadeAlunos )
	{
		this.quantidadeAlunos = quantidadeAlunos;
	}

	public Integer getCreditosTeorico()
	{
		return this.creditosTeorico;
	}

	public void setCreditosTeorico( Integer creditosTeorico )
	{
		this.creditosTeorico = creditosTeorico;
	}

	public Integer getCreditosPratico()
	{
		return this.creditosPratico;
	}

	public void setCreditosPratico( Integer creditosPratico )
	{
		this.creditosPratico = creditosPratico;
	}

	public Integer getTotalCreditos()
	{
		return ( this.getCreditosPratico() + this.getCreditosTeorico() );
	}
	
	public void setAlunosDemanda(Set<AlunoDemanda> alunosDemanda){
		this.alunosDemanda = alunosDemanda;
	}
	
	public Set<AlunoDemanda> getAlunosDemanda(){
		return this.alunosDemanda;
	}

//	public String getNaturalKey()
//	{
//		Oferta oferta = getOferta();
//		Curriculo curriculo = oferta.getCurriculo();
//
//		Integer periodo = curriculo.getPeriodo(this.getDisciplina());
//
//		return oferta.getCampus().getId()
//			+ "-" + oferta.getTurno().getId()
//			+ "-" + curriculo.getCurso().getId()
//			+ "-" + curriculo.getId()
//			+ "-" + periodo
//			+ "-" + getDisciplina().getId()
//			+ "-" + getTurma()
//			+ "-" + ( getCreditosTeorico() > 0 );
//	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof AtendimentoTatico ) )
		{
			return false;
		}

		AtendimentoTatico other = ( AtendimentoTatico ) obj;

		boolean validaTurma = ( this.getTurma().equals( other.getTurma() ) );
		boolean validaSala = ( this.getSala().equals( other.getSala() ) ); 
		boolean validaSemana = ( this.getSemana().equals( other.getSemana() ) );
		boolean validaOferta = ( this.getOferta().equals( other.getOferta() ) );
		boolean validaDisciplina = ( this.getDisciplina().equals( other.getDisciplina() ) );
		boolean validaCreditoTeorico = ( this.getCreditosTeorico().equals( other.getCreditosTeorico() ) );
		boolean validaCreditoPratico = ( this.getCreditosPratico().equals( other.getCreditosPratico() ) );
		boolean validaInstituicaoEnsino = ( this.getInstituicaoEnsino().equals( other.getInstituicaoEnsino() ) );

		return ( validaTurma && validaSala && validaSemana && validaOferta && validaDisciplina
			&&  validaCreditoTeorico && validaCreditoPratico && validaInstituicaoEnsino );
	}
}
