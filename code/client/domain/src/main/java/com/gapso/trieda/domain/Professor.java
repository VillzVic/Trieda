package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
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
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
import javax.validation.constraints.Digits;
import javax.validation.constraints.Max;
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
@RooEntity( identifierColumn = "PRF_ID" )
@Table( name = "PROFESSORES", uniqueConstraints =
@UniqueConstraint( columnNames = { "PRF_CPF", "CEN_ID" } ) )
public class Professor
	implements Serializable, Comparable< Professor >, Clonable < Professor >
{
	private static final long serialVersionUID = 265242535107921721L;

	@NotNull
	@ManyToOne( targetEntity = Cenario.class )
	@JoinColumn( name = "CEN_ID" )
	private Cenario cenario;

	@NotNull
	@ManyToOne( targetEntity = TipoContrato.class )
	@JoinColumn( name = "TCO_ID" )
	private TipoContrato tipoContrato;

	@NotNull
	@ManyToOne( targetEntity = Titulacao.class )
	@JoinColumn( name = "TIT_ID" )
	private Titulacao titulacao;

	@ManyToOne( targetEntity = AreaTitulacao.class )
	@JoinColumn( name = "ATI_ID" )
	private AreaTitulacao areaTitulacao;

	@NotNull
	@Column( name = "PRF_CPF" )
	@Size( min = 14, max = 14 )
	private String cpf;

	@NotNull
	@Column( name = "PRF_NOME" )
	@Size( min = 3, max = 255 )
	private String nome;

	@Column( name = "PRF_CH_MIN" )
	@Max( 999L )
	private Integer cargaHorariaMin;

	@Column( name = "PRF_CH_MAX" )
	@Max( 999L )
	private Integer cargaHorariaMax;

	@Column( name = "PRF_CRED_ANTERIOR" )
	@Max( 999L )
	private Integer creditoAnterior;

	@Column( name = "PRF_VALOR_CREDITO" )
	@Digits( integer = 6, fraction = 2 )
	private Double valorCredito;
	
	@Column( name = "PRF_MAX_DIAS_SEMANA" )
	@Max( 7L )
	private Integer maxDiasSemana;
	
	@Column( name = "PRF_MIN_CRED_DIA" )
	@Max( 100L )
	private Integer minCreditosDia;

	@ManyToMany
	private Set< Campus > campi = new HashSet< Campus >();

	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE }, mappedBy = "professores" )
	private Set< HorarioDisponivelCenario > horarios = new HashSet< HorarioDisponivelCenario >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "professor" )
	private Set< ProfessorDisciplina > disciplinas = new HashSet< ProfessorDisciplina >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "professor" )
	private Set< AtendimentoOperacional > atendimentosOperacionais = new HashSet< AtendimentoOperacional >();

	@OneToMany( cascade = CascadeType.ALL, mappedBy = "professor" )
	private Set< Usuario > usuario = new HashSet< Usuario >();
	
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "professor" )
	private Set< MotivoUsoProfessorVirtual > motivosUsoProfessorVirtual = new HashSet< MotivoUsoProfessorVirtual >();
	
	@OneToMany( cascade = CascadeType.ALL, mappedBy = "professor" )
	private Set< DicaEliminacaoProfessorVirtual > dicasEliminacaoProfessorVirtual = new HashSet< DicaEliminacaoProfessorVirtual >();
	
    @OneToMany( mappedBy="professor" )
    private Set< Aula > aulas =  new HashSet< Aula >();

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Cenario: " ).append( getCenario() ).append( ", " );
		sb.append( "TipoContrato: " ).append( getTipoContrato() ).append( ", " );
		sb.append( "Titulacao: " ).append( getTitulacao() ).append( ", " );
		sb.append( "AreaTitulacao: " ).append( getAreaTitulacao() ).append( ", " );
		sb.append( "Cpf: " ).append( getCpf() ).append( ", " );
		sb.append( "Nome: " ).append( getNome() ).append( ", " );
		sb.append( "CargaHorariaMin: " ).append( getCargaHorariaMin() ).append( ", " );
		sb.append( "CargaHorariaMax: " ).append( getCargaHorariaMax() ).append( ", " );
		sb.append( "CreditoAnterior: " ).append( getCreditoAnterior() ).append( ", " );
		sb.append( "ValorCredito: " ).append( getValorCredito() ).append( ", " );
		sb.append( "Campi: " ).append( getCampi() == null ? "null" : getCampi().size() ).append( ", " );
		sb.append( "Horarios: " ).append( getHorarios() == null ? "null" : getHorarios().size() ).append( ", " );
		sb.append( "Disciplinas: ").append( getDisciplinas() == null ? "null" : getDisciplinas().size() ).append( ", " );
		sb.append( "Atendimentos Operacionais: " ).append( getAtendimentosOperacionais() == null ?
			"null" : getAtendimentosOperacionais().size() ).append( ", " );
		sb.append( "Usuario: " ).append( getUsuario() == null ? "null" : getUsuario().size() );

		return sb.toString();
	}

	public Cenario getCenario()
	{
		return this.cenario;
	}

	public void setCenario( Cenario cenario )
	{
		this.cenario = cenario;
	}

	public TipoContrato getTipoContrato()
	{
		return this.tipoContrato;
	}

	public void setTipoContrato( TipoContrato tipoContrato )
	{
		this.tipoContrato = tipoContrato;
	}

	public Titulacao getTitulacao()
	{
		return this.titulacao;
	}

	public void setTitulacao( Titulacao titulacao )
	{
		this.titulacao = titulacao;
	}

	public AreaTitulacao getAreaTitulacao()
	{
		return this.areaTitulacao;
	}

	public void setAreaTitulacao( AreaTitulacao areaTitulacao )
	{
		this.areaTitulacao = areaTitulacao;
	}

	public String getCpf()
	{
		return this.cpf;
	}

	public void setCpf( String cpf )
	{
		this.cpf = cpf;
	}

	public String getNome()
	{
		return this.nome;
	}

	public void setNome( String nome )
	{
		this.nome = nome;
	}

	public Integer getCargaHorariaMin()
	{
		return this.cargaHorariaMin;
	}

	public void setCargaHorariaMin( Integer cargaHorariaMin )
	{
		this.cargaHorariaMin = cargaHorariaMin;
	}

	public Integer getCargaHorariaMax()
	{
		return this.cargaHorariaMax;
	}

	public void setCargaHorariaMax( Integer cargaHorariaMax )
	{
		this.cargaHorariaMax = cargaHorariaMax;
	}

	public Integer getCreditoAnterior()
	{
		return this.creditoAnterior;
	}

	public void setCreditoAnterior( Integer creditoAnterior )
	{
		this.creditoAnterior = creditoAnterior;
	}

	public Double getValorCredito()
	{
		return this.valorCredito;
	}

	public void setValorCredito( Double valorCredito )
	{
		this.valorCredito = valorCredito;
	}
	
	public Integer getMaxDiasSemana()
	{
		return this.maxDiasSemana;
	}

	public void setMaxDiasSemana( Integer maxDiaSemana )
	{
		this.maxDiasSemana = maxDiaSemana;
	}
	
	public Integer getMinCreditosDia()
	{
		return this.minCreditosDia;
	}

	public void setMinCreditosDia( Integer minCreditosDia )
	{
		this.minCreditosDia = minCreditosDia;
	}

	public Set< Campus > getCampi()
	{
		return this.campi;
	}

	public void setCampi( Set< Campus > campi )
	{
		this.campi = campi;
	}

	public Set< HorarioDisponivelCenario > getHorarios()
	{
		return this.horarios;
	}

	public void setHorarios(
		Set< HorarioDisponivelCenario > horarios )
	{
		this.horarios = horarios;
	}

	public Set< ProfessorDisciplina > getDisciplinas()
	{
		return this.disciplinas;
	}

	public void setDisciplinas(
		Set< ProfessorDisciplina > disciplinas )
	{
		this.disciplinas = disciplinas;
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

	public Set< Usuario > getUsuario()
	{
		return usuario;
	}

	public void setUsuario( Set< Usuario > usuario )
	{
		this.usuario = usuario;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "PRF_ID" )
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
	public void persistAndPreencheHorarios()
	{
		persist();
		preencheHorarios();
	}
	
	@Transactional
	static public void preencheHorariosDosProfessores(List<Professor> professores, List<SemanaLetiva> semanasLetivas) {
		int count = 0;
		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
				for (HorarioDisponivelCenario hdc : HorarioDisponivelCenario.findBy(semanaLetiva.getInstituicaoEnsino(), horarioAula)) {
					hdc.getProfessores().addAll(professores);
					hdc.merge();
					count++;if (count == 100) {System.out.println("   100 hor�rios de professores processados"); count = 0;}
				}
			}
		}
	}
	
	@Transactional
	static public void atualizaHorariosDosProfessores(Map<Professor, List<TriedaTrio<Semanas,Calendar,Calendar>>> professoreToDisponibilidadesMap, List<SemanaLetiva> semanasLetivas) {
		// coleta os professores disponíveis por dia da semana e tempo de aula
		int count = 0;
		Map<HorarioDisponivelCenario, Set<Professor>> hdcToProfessorMap = new HashMap<HorarioDisponivelCenario, Set<Professor>>();
		for (Entry<Professor, List<TriedaTrio<Semanas,Calendar,Calendar>>> entry : professoreToDisponibilidadesMap.entrySet()) {
			List<TriedaTrio<Semanas,Calendar,Calendar>> disponibilidades = entry.getValue();
			Professor professor = entry.getKey();
			for (SemanaLetiva semanaLetiva : semanasLetivas) {
				// para cada tempo de aula
				for (HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
					for (TriedaTrio<Semanas,Calendar,Calendar> trio : disponibilidades){
						// verifica se o intervalo de horas é compatível
						boolean horarioAulaEstahContidoEmDisponibilidade = horarioAula.estahContidoEm(trio.getSegundo(),trio.getTerceiro()); 
						// para cada dia da semana
						for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
							Set<Professor> professoresDisponiveisNoDiaEHorario = hdcToProfessorMap.get(hdc);
							if (professoresDisponiveisNoDiaEHorario == null) {
								professoresDisponiveisNoDiaEHorario = new HashSet<Professor>();
								hdcToProfessorMap.put(hdc,professoresDisponiveisNoDiaEHorario);
							}
							
							// verifica se o dia da semana é compatível
							if (horarioAulaEstahContidoEmDisponibilidade && hdc.getDiaSemana().equals(trio.getPrimeiro())) {
								professoresDisponiveisNoDiaEHorario.add(professor);
							}
						}
					}
					count++;if (count == 100) {System.out.println("   100 horários de professores processados"); count = 0;}
				}
			}
		}
		
		// atualiza disponibilidades de professores
		count = 0;
		for (Entry<HorarioDisponivelCenario, Set<Professor>> entry : hdcToProfessorMap.entrySet()) {
			HorarioDisponivelCenario hdc = entry.getKey();
			Set<Professor> professoresDisponiveisNoDiaEHorario = entry.getValue();
			
			hdc.getProfessores().clear();
			hdc.getProfessores().addAll(professoresDisponiveisNoDiaEHorario);
			hdc.merge();
			
			count++;if (count == 100) {System.out.println("   100 horários de professores processados"); count = 0;}
		}
	}

	public void preencheHorarios()
	{
		List< SemanaLetiva > listDomains = SemanaLetiva.findByCenario(
			this.getTipoContrato().getInstituicaoEnsino(), this.getCenario() );

		for ( SemanaLetiva semanaLetiva : listDomains )
		{
			for ( HorarioAula horarioAula : semanaLetiva.getHorariosAula() )
			{
				for ( HorarioDisponivelCenario hdc :
						horarioAula.getHorariosDisponiveisCenario() )
				{
					hdc.getProfessores().add( this );
					hdc.merge();
				}
			}
		}
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
			this.entityManager.remove( this );
		}
		else
		{
			Professor attached = this.entityManager.find(
				this.getClass(), this.id );

			attached.removeHorariosDisponivelCenario();
			this.entityManager.remove( attached );
		}
	}

	@Transactional
	public void removeHorariosDisponivelCenario()
	{
		Set< HorarioDisponivelCenario > horarios = this.getHorarios();

		for ( HorarioDisponivelCenario horario : horarios )
		{
			horario.getProfessores().remove( this );
			horario.merge();
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
	public Professor merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Professor merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	@Transactional
	public Professor mergeWithoutFlush()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		Professor merged = this.entityManager.merge( this );
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new Professor().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Professor > findAll(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Professor > findByCampus(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Campus campus)
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o, IN (o.campi) cmp " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND cmp = :campus ");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campus", campus );
		q.setParameter( "cenario", cenario );

		return q.getResultList();
	}

	@SuppressWarnings("unchecked")
	public static List< Professor > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List< Professor > findByCenarioFetchAtendimentos(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT o FROM Professor o " +
			" LEFT JOIN FETCH o.atendimentosOperacionais " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}
	
	public static int count( InstituicaoEnsino instituicaoEnsino, Cenario cenario, String cpf, String nome,
		TipoContrato tipoContrato, Titulacao titulacao, AreaTitulacao areaTitulacao,
		String operadorCargaHorariaMin,
		Integer cargaHorariaMin, String operadorCargaHorariaMax,
		Integer cargaHorariaMax, String operadorNotaDesempenho,
		Double notaDesempenho, String operadorCargaHorariaAnterior,
		Integer cargaHorariaAnterior, String operadorCustoCreditoSemanal,
		Double custoCreditoSemanal, String operadorMaxDiasSemana,
		Integer maxDiasSemana, String operadorMinCreditosSemanais,
		Integer minCreditosSemanais, String operadorTotalCreditosSemanais,
		Long totalCreditosSemanais, String operadorCargaHorariaSemanal,
		Long cargaHorariaSemanal )
	{
		String where = " o.tipoContrato.instituicaoEnsino = :instituicaoEnsino AND" +
				" o.cenario = :cenario AND";
		
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );

		if ( cpf != null )
		{
			where += " o.cpf = :cpf AND ";
		}

		if ( tipoContrato != null )
		{
			where += " o.tipoContrato = :tipoContrato AND ";
		}

		if ( titulacao != null )
		{
			where += " o.titulacao = :titulacao AND ";
		}
		
		if ( areaTitulacao != null )
		{
			where += " o.areaTitulacao = :areaTitulacao AND ";
		}
		
		if(cargaHorariaMin != null){
			if(operadorCargaHorariaMin != null)
				where += "  o.cargaHorariaMin " + operadorCargaHorariaMin + " :cargaHorariaMin and ";
			else
				where += "  o.cargaHorariaMin = :cargaHorariaMin and ";
		}
		if(cargaHorariaMax != null){
			if(operadorCargaHorariaMax != null)
				where += "  o.cargaHorariaMax " + operadorCargaHorariaMax + " :cargaHorariaMax and ";
			else
				where += "  o.cargaHorariaMax = :cargaHorariaMax and ";
		}
		
		if(notaDesempenho != null){
			if(operadorNotaDesempenho != null)
				where += "  (select AVG(nota) from ProfessorDisciplina p where p.professor = o) " + operadorNotaDesempenho + " :notaDesempenho and ";
			else
				where += "  (select AVG(nota) from ProfessorDisciplina p where p.professor = o) = :notaDesempenho and ";
		}
		
		if(cargaHorariaAnterior != null){
			if(operadorCargaHorariaAnterior != null)
				where += "  o.creditoAnterior " + operadorCargaHorariaAnterior + " :creditoAnterior and ";
			else
				where += "  o.creditoAnterior = :creditoAnterior and ";
		}
		
		if(custoCreditoSemanal != null){
			if(operadorCustoCreditoSemanal != null)
				where += "  o.valorCredito " + operadorCustoCreditoSemanal + " :valorCredito and ";
			else
				where += "  o.valorCredito = :valorCredito and ";
		}
		
		if(maxDiasSemana != null){
			if(operadorMaxDiasSemana != null)
				where += "  o.maxDiasSemana " + operadorMaxDiasSemana + " :maxDiasSemana and ";
			else
				where += "  o.maxDiasSemana = :maxDiasSemana and ";
		}
		
		if(minCreditosSemanais != null){
			if(operadorMinCreditosSemanais != null)
				where += "  o.minCreditosDia " + operadorMinCreditosSemanais + " :minCreditosDia and ";
			else
				where += "  o.minCreditosDia = :minCreditosDia and ";
		}
		
		if(totalCreditosSemanais != null){
			if(operadorTotalCreditosSemanais != null)
				where += " (select count(p) from AtendimentoOperacional p where p.professor = o )  " + operadorTotalCreditosSemanais + " :totalCreditosSemanais and ";
			else
				where += " (select count(p) from AtendimentoOperacional p where p.professor = o ) = :totalCreditosSemanais and ";
		}
		
		if(cargaHorariaSemanal != null){
			if(operadorCargaHorariaSemanal != null)
				where += " (select sum(a.HorarioDisponivelCenario.horarioAula.semanaLetiva.tempo) from AtendimentoOperacional a where a.professor = o )  " + operadorCargaHorariaSemanal + " :cargaHorariaSemanal and ";
			else
				where += " (select sum(a.HorarioDisponivelCenario.horarioAula.semanaLetiva.tempo) from AtendimentoOperacional a where a.professor = o ) = :cargaHorariaSemanal and ";
		}

		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Professor o " +
			" where LOWER ( o.nome ) LIKE LOWER ( :nome ) and "
			 + where + " 1 = 1 ");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "nome", nome );

		if ( cpf != null )
		{
			q.setParameter( "cpf", cpf );
		}
		
		if ( tipoContrato != null )
		{
			q.setParameter( "tipoContrato", tipoContrato );
		}

		if ( titulacao != null )
		{
			q.setParameter( "titulacao", titulacao );
		}

		if ( areaTitulacao != null )
		{
			q.setParameter( "areaTitulacao", areaTitulacao );
		}
		
		if ( areaTitulacao != null )
		{
			q.setParameter( "areaTitulacao", areaTitulacao );
		}
		
		if(cargaHorariaMin != null){
			q.setParameter("cargaHorariaMin", cargaHorariaMin);
		}
		
		if(cargaHorariaMax != null){
			q.setParameter("cargaHorariaMax", cargaHorariaMax);
		}
		
		if(notaDesempenho != null){
			q.setParameter("notaDesempenho", notaDesempenho);
		}

		if(cargaHorariaAnterior != null){
			q.setParameter("creditoAnterior", cargaHorariaAnterior);
		}
		
		if(custoCreditoSemanal != null){
			q.setParameter("valorCredito", custoCreditoSemanal);
		}
		
		if(maxDiasSemana != null){
			q.setParameter("maxDiasSemana", maxDiasSemana);
		}
		
		if(minCreditosSemanais != null){
			q.setParameter("minCreditosDia", minCreditosSemanais);
		}
		
		if(totalCreditosSemanais != null){
			q.setParameter("totalCreditosSemanais", totalCreditosSemanais);
		}
		
		if(cargaHorariaSemanal != null){
			q.setParameter("cargaHorariaSemanal", cargaHorariaSemanal);
		}

		return ( (Number) q.getSingleResult() ).intValue();
	}

	@SuppressWarnings( "unchecked" )
	public static List< Object > findBy( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String cpf, String nome, TipoContrato tipoContrato, Titulacao titulacao,
		AreaTitulacao areaTitulacao,
		String operadorCargaHorariaMin, Integer cargaHorariaMin, 
		String operadorCargaHorariaMax, Integer cargaHorariaMax, 
		String operadorNotaDesempenho, Double notaDesempenho, 
		String operadorCargaHorariaAnterior, Integer cargaHorariaAnterior, 
		String operadorCustoCreditoSemanal, Double custoCreditoSemanal, 
		String operadorMaxDiasSemana,Integer maxDiasSemana, 
		String operadorMinCreditosSemanais,	Integer minCreditosSemanais, 
		String operadorTotalCreditosSemanais,	Long totalCreditosSemanais, 
		String operadorCargaHorariaSemanal,	Long cargaHorariaSemanal, 
		int firstResult, int maxResults, String orderBy )
	{
		String where = " o.tipoContrato.instituicaoEnsino = :instituicaoEnsino AND" +
				" o.cenario = :cenario AND ";

		nome = ( ( nome == null ) ? "" : nome );
		nome = ( "%" + nome.replace( '*', '%' ) + "%" );
		
		if ( cpf != null )
		{
			where += " o.cpf = :cpf AND ";
		}

		if ( tipoContrato != null )
		{
			where += " o.tipoContrato = :tipoContrato AND ";
		}

		if ( titulacao != null )
		{
			where += " o.titulacao = :titulacao AND ";
		}

		if ( areaTitulacao != null )
		{
			where += " o.areaTitulacao = :areaTitulacao AND ";
		}
		
		if(cargaHorariaMin != null){
			if(operadorCargaHorariaMin != null)
				where += " o.cargaHorariaMin " + operadorCargaHorariaMin + " :cargaHorariaMin and ";
			else
				where += "  o.cargaHorariaMin = :cargaHorariaMin and ";
		}
		if(cargaHorariaMax != null){
			if(operadorCargaHorariaMax != null)
				where += "  o.cargaHorariaMax " + operadorCargaHorariaMax + " :cargaHorariaMax and ";
			else
				where += "  o.cargaHorariaMax = :cargaHorariaMax and ";
		}
		
		
		if(notaDesempenho != null){
			if(operadorNotaDesempenho != null)
				where += "  (select AVG(nota) from ProfessorDisciplina p where p.professor = o) " + operadorNotaDesempenho + " :notaDesempenho and ";
			else
				where += "  (select AVG(nota) from ProfessorDisciplina p where p.professor = o) = :notaDesempenho and ";
		}
		
		if(cargaHorariaAnterior != null){
			if(operadorCargaHorariaAnterior != null)
				where += "  o.creditoAnterior " + operadorCargaHorariaAnterior + " :creditoAnterior and ";
			else
				where += "  o.creditoAnterior = :creditoAnterior and ";
		}
		
		if(custoCreditoSemanal != null){
			if(operadorCustoCreditoSemanal != null)
				where += "  o.valorCredito " + operadorCustoCreditoSemanal + " :valorCredito and ";
			else
				where += "  o.valorCredito = :valorCredito and ";
		}
		
		if(maxDiasSemana != null){
			if(operadorMaxDiasSemana != null)
				where += "  o.maxDiasSemana " + operadorMaxDiasSemana + " :maxDiasSemana and ";
			else
				where += "  o.maxDiasSemana = :maxDiasSemana and ";
		}
		
		if(minCreditosSemanais != null){
			if(operadorMinCreditosSemanais != null)
				where += "  o.minCreditosDia " + operadorMinCreditosSemanais + " :minCreditosDia and ";
			else
				where += "  o.minCreditosDia = :minCreditosDia and ";
		}
		
		if(totalCreditosSemanais != null){
			if(operadorTotalCreditosSemanais != null)
				where += " (select count(p) from AtendimentoOperacional p where p.professor = o )  " + operadorTotalCreditosSemanais + " :totalCreditosSemanais and ";
			else
				where += " (select count(p) from AtendimentoOperacional p where p.professor = o ) = :totalCreditosSemanais and ";
		}
		
		if(cargaHorariaSemanal != null){
			if(operadorCargaHorariaSemanal != null)
				where += " (select sum(a.HorarioDisponivelCenario.horarioAula.semanaLetiva.tempo) from AtendimentoOperacional a where a.professor = o )  " + operadorCargaHorariaSemanal + " :cargaHorariaSemanal and ";
			else
				where += " (select sum(a.HorarioDisponivelCenario.horarioAula.semanaLetiva.tempo) from AtendimentoOperacional a where a.professor = o ) = :cargaHorariaSemanal and ";
		}

		if ( orderBy != null )
		{
			if( orderBy.contains("notaDesempenho") )
				orderBy = orderBy.replace("notaDesempenho", "ORDER BY AVG(d.nota)");
			else
				orderBy = " ORDER BY o." + orderBy.replace("String", "");
		}
		else
		{
			orderBy = "";
		}

		Query q;
		
/*		if(cenario.getAtendimentosOperacionais().isEmpty())
		{
			q = entityManager().createQuery(
					" SELECT o, SUM(0), SUM(0) FROM Professor o " +
					" where LOWER ( o.nome ) LIKE LOWER ( :nome ) and "
					+ where + " 1 = 1 "+  " GROUP BY o " +orderBy );
		}
		else
		{*/
			q = entityManager().createQuery(
				" SELECT o, count(p), sum(sl.tempo) FROM Professor o" +
				" LEFT JOIN o.atendimentosOperacionais p LEFT JOIN p.HorarioDisponivelCenario h LEFT JOIN h.horarioAula ha " +
				" LEFT JOIN ha.semanaLetiva sl " +
				" where LOWER ( o.nome ) LIKE LOWER ( :nome ) and "
				+ where + " 1 = 1 "+  " GROUP BY o " +orderBy );
		//}

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "nome", nome );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		if ( cpf != null )
		{
			q.setParameter( "cpf", cpf );
		}

		if ( tipoContrato != null )
		{
			q.setParameter( "tipoContrato", tipoContrato );
		}

		if ( titulacao != null )
		{
			q.setParameter( "titulacao", titulacao );
		}

		if ( areaTitulacao != null )
		{
			q.setParameter( "areaTitulacao", areaTitulacao );
		}
		
		if ( notaDesempenho != null )
		{
			q.setParameter( "notaDesempenho", notaDesempenho );
		}
		
		if(cargaHorariaMin != null){
			q.setParameter("cargaHorariaMin", cargaHorariaMin);
		}
		
		if(cargaHorariaMax != null){
			q.setParameter("cargaHorariaMax", cargaHorariaMax);
		}

		if(cargaHorariaAnterior != null){
			q.setParameter("creditoAnterior", cargaHorariaAnterior);
		}
		
		if(custoCreditoSemanal != null){
			q.setParameter("valorCredito", custoCreditoSemanal);
		}
		
		if(maxDiasSemana != null){
			q.setParameter("maxDiasSemana", maxDiasSemana);
		}
		
		if(minCreditosSemanais != null){
			q.setParameter("minCreditosDia", minCreditosSemanais);
		}
		
		if(totalCreditosSemanais != null){
			q.setParameter("totalCreditosSemanais", totalCreditosSemanais);
		}
		
		if(cargaHorariaSemanal != null){
			q.setParameter("cargaHorariaSemanal", cargaHorariaSemanal);
		}
		
		
		
		return q.getResultList();
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< Professor > findBy( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String nome, String cpf, int firstResult, int maxResults )
	{
	
		String where = "";
		
		if ( nome != null )
		{
			where += " AND o.nome LIKE LOWER (:nome)";
		}
		if ( cpf != null )
		{
			where += " AND o.cpf LIKE LOWER (:cpf)";
		}

		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino" +
			" AND o.cenario = :cenario" +
			where 
			);

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setFirstResult( firstResult );
		q.setMaxResults( maxResults );

		if ( cpf != null )
		{
			q.setParameter( "cpf", "%" + cpf + "%" );
		}
		
		if ( nome != null )
		{
			q.setParameter( "nome", "%" + nome + "%" );
		}

		return q.getResultList();
	}
	
	public static Professor findByNomeCpf( InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		String nome, String cpf)
	{
		if ( cpf == null && nome == null ) return null;
		else if ( cpf.isEmpty() && nome.isEmpty() ) return null;
		
		nome = ( ( nome == null ) ? "" : nome );
		nome = ( nome.replace( '*', '%' ) );
		
		String cpfQuery = "";
		
		if ( cpf != null )
		{
			if( !cpf.isEmpty())
				cpfQuery += " AND  LOWER ( o.cpf ) LIKE LOWER ( :cpf )";
		}

		String nomeQuery = "";
		
		if ( nome != null )
		{
			if( !nome.isEmpty())
				nomeQuery +=  " AND LOWER ( o.nome ) LIKE LOWER ( :nome )";
		}
		
		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o" +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino" +
			" AND o.cenario = :cenario" +
			nomeQuery +
			cpfQuery);
		

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );

		if ( cpf != null )
		{
			if( !cpf.isEmpty())
				q.setParameter( "cpf", cpf );
		}

		if ( nome != null )
		{
			if( !nome.isEmpty())
				q.setParameter( "nome", nome );	
		}

		return (Professor) q.getSingleResult();
	}

	public static Professor find( Long id, InstituicaoEnsino instituicaoEnsino )
	{
		if ( id == null || instituicaoEnsino == null )
		{
			return null;
		}

		Professor professor = entityManager().find( Professor.class, id );

		if ( professor != null && professor.getTipoContrato() != null
			&& professor.getTipoContrato().getInstituicaoEnsino() != null
			&& professor.getTipoContrato().getInstituicaoEnsino() == instituicaoEnsino )
		{
			return professor;
		}

		return null;
	}

	@SuppressWarnings( "unchecked" )
	public static List< Professor > find(
		InstituicaoEnsino instituicaoEnsino,
		int firstResult, int maxResults )
	{
		return entityManager().createQuery(
			" SELECT o FROM Professor o " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " )
			.setParameter( "instituicaoEnsino", instituicaoEnsino )
			.setFirstResult( firstResult ).setMaxResults( maxResults ).getResultList();
	}

	@SuppressWarnings( "unchecked" )
	public List< HorarioDisponivelCenario > getHorarios(
		InstituicaoEnsino instituicaoEnsino )
	{
		Query q = entityManager().createQuery(
			" SELECT o FROM HorarioDisponivelCenario o, IN ( o.professores ) c " +
			" WHERE c.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND c = :professor " );

		q.setParameter( "professor", this );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}

	public static Map< String, Professor > buildProfessorCpfToProfessorMap(
		List< Professor > professores )
	{
		Map< String, Professor > professoresMap
			= new HashMap< String, Professor >();

		for ( Professor professor : professores )
		{
			professoresMap.put( professor.getCpf(), professor );
		}

		return professoresMap;
	}

	public static boolean checkCodigoUnique( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, String cpf )
	{
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM Professor o " +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND o.cpf = :cpf " );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cpf", cpf );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}

	//@Override
	public int compareTo( Professor o )
	{
		return this.getNome().compareTo( o.getNome() );
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( obj == null || !( obj instanceof Professor ) )
		{
			return false;
		}

		Professor other = (Professor) obj;

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
	
	static public Map<Long,Professor> getProfessoresMap(Collection<Professor> professores) {
		Map<Long,Professor> map = new HashMap<Long, Professor>();
		for (Professor professor : professores) {
			map.put(professor.getId(), professor);
		}
		return map;
	}

	@SuppressWarnings("unchecked")
	public static List<Professor> findProfessoresUteis(
			InstituicaoEnsino instituicaoEnsino, Cenario cenario,
			Campus campus) {
		
		String campusQuery = campus == null ? "" : " AND cmp = :campus ";
		String campusQuery2 = campus == null ? "" : ", IN (o.campi) cmp ";

		Query q = entityManager().createQuery(
			" SELECT o FROM Professor o " + campusQuery2 +
			" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			campusQuery +
			" AND o.disciplinas IS NOT EMPTY " +
			" AND o.horarios IS NOT EMPTY ");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		
		if (campus != null)
		{
			q.setParameter( "campus", campus );
		}

		return q.getResultList();
	}

	public Set< MotivoUsoProfessorVirtual > getMotivosUsoProfessorVirtual() {
		return motivosUsoProfessorVirtual;
	}

	public void setMotivosUsoProfessorVirtual(Set< MotivoUsoProfessorVirtual > motivosUsoProfessorVirtual) {
		this.motivosUsoProfessorVirtual = motivosUsoProfessorVirtual;
	}

	public Set< DicaEliminacaoProfessorVirtual > getDicasEliminacaoProfessorVirtual() {
		return dicasEliminacaoProfessorVirtual;
	}

	public void setDicasEliminacaoProfessorVirtual(	Set< DicaEliminacaoProfessorVirtual > dicasEliminacaoProfessorVirtual) {
		this.dicasEliminacaoProfessorVirtual = dicasEliminacaoProfessorVirtual;
	}

	public Set< Aula > getAulas() {
		return aulas;
	}
	
	public Set<Aula> getAulas(Semanas semana) {
		Set<Aula> aulas = new HashSet<Aula>();
		for (Aula aula : getAulas())
		{
			if(aula.getHorarioDisponivelCenario().getDiaSemana().equals(semana))
			{
				aulas.add(aula);
			}
		}
		return aulas;
	}

	public void setAulas(Set< Aula > aulas) {
		this.aulas = aulas;
	}
	
	public static Professor findProx(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, Professor professor, String order) {
		
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Professor o " +
    		" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " + 
    		" AND o.cenario = :cenario " +
    		" AND o > :professor " +
    		" ORDER BY o." + order);

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "professor", professor );
		q.setMaxResults(1);

        return (Professor)q.getSingleResult();
	}
	
	public static Professor findAnt(InstituicaoEnsino instituicaoEnsino,
			Cenario cenario, Professor professor, String order) {
		
		Query q = entityManager().createQuery(
	       	" SELECT o FROM Professor o " +
    		" WHERE o.tipoContrato.instituicaoEnsino = :instituicaoEnsino " + 
    		" AND o.cenario = :cenario " +
    		" AND o < :professor " +
    		" ORDER BY o." + order + " DESC");

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "cenario", cenario );
		q.setParameter( "professor", professor );
		q.setMaxResults(1);

        return (Professor)q.getSingleResult();
	}

	public Professor clone(CenarioClone novoCenario) {
		Professor clone = new Professor();
		clone.setAreaTitulacao(novoCenario.getEntidadeClonada(this.getAreaTitulacao()));
		clone.setCargaHorariaMax(this.getCargaHorariaMax());
		clone.setCargaHorariaMin(this.getCargaHorariaMin());
		clone.setCenario(novoCenario.getCenario());
		clone.setCpf(this.getCpf());
		clone.setCreditoAnterior(this.getCreditoAnterior());
		clone.setMaxDiasSemana(this.getMaxDiasSemana());
		clone.setMinCreditosDia(this.getMinCreditosDia());
		clone.setNome(this.getNome());
		clone.setTipoContrato(novoCenario.getEntidadeClonada(this.getTipoContrato()));
		clone.setTitulacao(novoCenario.getEntidadeClonada(this.getTitulacao()));
		clone.setValorCredito(this.getValorCredito());
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario, Professor entidadeClone) {
		for (Campus campus : this.getCampi())
		{
			entidadeClone.getCampi().add(novoCenario.getEntidadeClonada(campus));
		}
		
		for (ProfessorDisciplina professorDisciplina : this.getDisciplinas())
		{
			entidadeClone.getDisciplinas().add(novoCenario.clone(professorDisciplina));
		}
		
		for (HorarioDisponivelCenario horarioDisponivel : this.getHorarios())
		{
			entidadeClone.getHorarios().add(novoCenario.getEntidadeClonada(horarioDisponivel));
			novoCenario.getEntidadeClonada(horarioDisponivel).getProfessores().add(entidadeClone);
		}
		
	}
}
