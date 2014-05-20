package com.gapso.trieda.domain;

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
@RooEntity( identifierColumn = "DIP_ID" )
@Table( name = "PROFESSORES_DISCIPLINAS" )
public class ProfessorDisciplina
	implements java.io.Serializable, Clonable< ProfessorDisciplina >
{
	private static final long serialVersionUID = -6254398976446496178L;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Professor.class)
	@JoinColumn(name = "PRF_ID")
	private Professor professor;

	@NotNull
	@ManyToOne(cascade = { CascadeType.PERSIST, CascadeType.MERGE,
			CascadeType.REFRESH }, targetEntity = Disciplina.class)
	@JoinColumn(name = "DIS_ID")
	private Disciplina disciplina;

	@NotNull
	@Column(name = "PRF_NOTA")
	@Min(0L)
	@Max(100L)
	private Integer nota;

	@NotNull
	@Column(name = "PRF_PREFERENCIA")
	@Min(0L)
	@Max(10L)
	private Integer preferencia;

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "DIP_ID")
	private Long id;

	@Version
	@Column(name = "version")
	private Integer version;

	public Long getId() {
		return this.id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Integer getVersion() {
		return this.version;
	}

	public void setVersion(Integer version) {
		this.version = version;
	}

	@Transactional
	public void detach() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.detach(this);
	}

	@Transactional
	public void persist() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.persist(this);
	}

	@Transactional
	public void remove() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		if (this.entityManager.contains(this)) {
			this.entityManager.remove(this);
		} else {
			ProfessorDisciplina attached = this.entityManager.find(
					this.getClass(), this.id);
			this.entityManager.remove(attached);
		}
	}

	@Transactional
	public void flush() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		this.entityManager.flush();
	}

	@Transactional
	public ProfessorDisciplina merge() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		ProfessorDisciplina merged = this.entityManager.merge(this);
		this.entityManager.flush();
		return merged;
	}
	
	@Transactional
	public ProfessorDisciplina mergeWithoutFlush() {
		if (this.entityManager == null)
			this.entityManager = entityManager();
		ProfessorDisciplina merged = this.entityManager.merge(this);
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new ProfessorDisciplina().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		return entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " +
			" WHERE o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino ).getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		return entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " +
			" WHERE o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professor.cenario = :cenario " +
			" AND o.disciplina.cenario = :cenario ")
			.setParameter( "instituicaoEnsino", instituicaoEnsino )
			.setParameter( "cenario", cenario ).getResultList();
	}

	public static ProfessorDisciplina find(
		Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		ProfessorDisciplina pd = entityManager().find( ProfessorDisciplina.class, id );

		if ( pd != null && pd.getProfessor() != null && pd.getDisciplina() != null
			&& pd.getProfessor().getTipoContrato() != null
			&& pd.getDisciplina().getTipoDisciplina() != null
			&& pd.getProfessor().getTipoContrato().getInstituicaoEnsino() != null
			&& pd.getDisciplina().getTipoDisciplina().getInstituicaoEnsino() != null
			&& pd.getProfessor().getTipoContrato().getInstituicaoEnsino() == instituicaoEnsino
			&& pd.getDisciplina().getTipoDisciplina().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return pd;
		}

		return null;
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " +
			" WHERE o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino )
			.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
	}

	public static int count(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Professor professor, Disciplina disciplina,
		String cpf, String nome,String  operadorPreferencia, Integer preferencia,String  operadorNotaDesempenho, Integer notaDesempenho)
	{
		String where = " o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professor.cenario = :cenario " +
			" AND o.disciplina.cenario = :cenario AND ";
		
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );

		if ( cpf != null )
		{
			where += " o.professor.cpf = :cpf AND ";
		}
		
		if ( professor != null )
		{
			where += ( " o.professor = :professor AND " );
		}

		if ( disciplina != null )
		{
			where += ( " o.disciplina = :disciplina AND " );
		}
		
		if(preferencia != null){
			if(operadorPreferencia != null)
				where += "  o.preferencia " + operadorPreferencia + " :preferencia and ";
			else
				where += "  o.preferencia = :preferencia and ";
		}
		
		if(notaDesempenho != null){
			if(operadorNotaDesempenho != null)
				where += "  o.nota " + operadorNotaDesempenho + " :notaDesempenho and ";
			else
				where += "  o.nota = :notaDesempenho and ";
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM ProfessorDisciplina o " +
					" where LOWER ( o.professor.nome ) LIKE LOWER ( :nome ) and "	+
					where + " 1=1 ");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "nome", nome );

		if ( cpf != null )
		{
			q.setParameter( "cpf", cpf );
		}
		
		if ( professor != null )
		{
			q.setParameter( "professor", professor );
		}

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}
		
		if(preferencia != null){
			q.setParameter( "preferencia", preferencia );
		}
		
		if(notaDesempenho != null){
			q.setParameter( "notaDesempenho", notaDesempenho );
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, Professor professor,
		Disciplina disciplina, 
		String cpf, String nome,String  operadorPreferencia,Integer preferencia,String  operadorNotaDesempenho, Integer notaDesempenho,
		int firstResult, int maxResults,	String orderBy )
	{
		String where = " o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professor.cenario = :cenario " +
			" AND o.disciplina.cenario = :cenario AND ";
		
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		
		if ( cpf != null )
		{
			where += " o.professor.cpf = :cpf AND ";
		}

		if ( professor != null )
		{
			where += ( " o.professor = :professor AND " );
		}

		if ( disciplina != null )
		{
			where += ( " o.disciplina = :disciplina AND " );
		}
		
		if(preferencia != null){
			if(operadorPreferencia != null)
				where += "  o.preferencia " + operadorPreferencia + " :preferencia and ";
			else
				where += "  o.preferencia = :preferencia and ";
		}
		
		if(notaDesempenho != null){
			if(operadorNotaDesempenho != null)
				where += "  o.nota " + operadorNotaDesempenho + " :notaDesempenho and ";
			else
				where += "  o.nota = :notaDesempenho and ";
		}

		if ( orderBy != null )
		{
			if ( orderBy.contains("professorCpf") )
			{
				orderBy = orderBy.replace("professorCpf", " ORDER BY o.professor.cpf");
			}
			else if ( orderBy.contains("professorString") )
			{
				orderBy = orderBy.replace("professorString", " ORDER BY o.professor.nome");
			}
			else if ( orderBy.contains("disciplinaString") )
			{
				orderBy = orderBy.replace("disciplinaString", " ORDER BY o.disciplina.codigo");
			}
			else
			{
				orderBy = " ORDER BY o." + orderBy;
			}
		}
		else
		{
			orderBy = "";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " +
				" where LOWER ( o.professor.nome ) LIKE LOWER ( :nome ) and "	+ where + " 1=1 " +orderBy );

		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "nome", nome );

		if ( professor != null )
		{
			q.setParameter( "professor", professor );
		}
		
		if ( cpf != null )
		{
			q.setParameter( "cpf", cpf );
		}

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}
		
		if(preferencia != null){
			q.setParameter( "preferencia", preferencia );
		}
		
		if(notaDesempenho != null){
			q.setParameter( "notaDesempenho", notaDesempenho );
		}

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< ProfessorDisciplina > findBy(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario, Disciplina disciplina )
	{
		String where = " o.professor.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.disciplina.tipoDisciplina.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.professor.cenario = :cenario " +
			" AND o.disciplina.cenario = :cenario AND ";

		if ( disciplina != null )
		{
			where += ( " o.disciplina = :disciplina AND " );
		}

		if ( where.length() > 1 )
		{
			where = ( " WHERE " + where.substring( 0, where.length() - 4 ) );
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM ProfessorDisciplina o " + where );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		if ( disciplina != null )
		{
			q.setParameter( "disciplina", disciplina );
		}

		return q.getResultList();
	}

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Professor: " ).append( getProfessor() ).append( ", " );
		sb.append( "Disciplina: " ).append( getDisciplina() ).append( ", " );
		sb.append( "Nota: ").append( getNota() ).append( ", " );
		sb.append( "Preferencia: " ).append( getPreferencia() );

		return sb.toString();
	}

	public static Map< String, ProfessorDisciplina > buildCursoNaturalKeyToProfessorDisciplinaMap(
		List< ProfessorDisciplina > pds )
	{
		Map< String, ProfessorDisciplina > map
			= new HashMap< String, ProfessorDisciplina >();

		for ( ProfessorDisciplina pd : pds )
		{
			map.put( pd.getNaturalKeyString(), pd );
		}

		return map;
	}

	public Professor getProfessor()
	{
		return this.professor;
	}

	public void setProfessor( Professor professor )
	{
		this.professor = professor;
	}

	public Disciplina getDisciplina()
	{
		return this.disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}

	public Integer getNota()
	{
		return this.nota;
	}

	public void setNota( Integer nota )
	{
		this.nota = nota;
	}

	public Integer getPreferencia()
	{
		return this.preferencia;
	}

	public void setPreferencia( Integer preferencia )
	{
		this.preferencia = preferencia;
	}

	public String getNaturalKeyString()
	{
		Professor professor = getProfessor();

		return professor.getCpf()
			+ "-" + getDisciplina().getCodigo();
	}

	public ProfessorDisciplina clone(CenarioClone novoCenario) {
		ProfessorDisciplina clone = new ProfessorDisciplina();
		clone.setDisciplina(novoCenario.getEntidadeClonada(this.getDisciplina()));
		clone.setNota(this.getNota());
		clone.setPreferencia(this.getPreferencia());
		clone.setProfessor(novoCenario.getEntidadeClonada(this.getProfessor()));
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario,
			ProfessorDisciplina entidadeClone) {
		
	}
}
