package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Calendar;
import java.util.Collections;
import java.util.Date;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EntityManager;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
import javax.validation.constraints.NotNull;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "ROP_ID")
@Table(name = "REQUISICAO_OTIMIZACAO", uniqueConstraints =
@UniqueConstraint(columnNames = {"USERNAME_CRIACAO","ROP_ROUND","PAR_ID","CEN_ID"}))
public class RequisicaoOtimizacao implements Serializable, Comparable<RequisicaoOtimizacao> {

	private static final long serialVersionUID = 1L;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "ROP_ID")
	private Long id;

	@Version
	@Column(name = "VERSION")
	private Integer version;
	
	@NotNull
    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USERNAME_CRIACAO")
    private Usuario usuario;
	
    @ManyToOne(targetEntity = Usuario.class)
    @JoinColumn(name = "USERNAME_CANCELAMENTO")
    private Usuario usuarioCancel;
	
	@NotNull
	@ManyToOne(targetEntity = Cenario.class)
	@JoinColumn(name = "CEN_ID")
	private Cenario cenario;
	
	@NotNull
	@Column(name = "ROP_ROUND")
	private Long round;
	
	@NotNull
	@OneToOne(targetEntity = Parametro.class)
	@JoinColumn(name = "PAR_ID")
	private Parametro parametro;
	
    @Column( name = "ROP_INSTANTE_INICIO_REQUISICAO" )
    @Temporal( TemporalType.TIMESTAMP )
    @DateTimeFormat( style = "--" )
    private Date instanteInicioRequisicao;
    
    @Column( name = "ROP_INSTANTE_INICIO_OTIMIZACAO" )
    @Temporal( TemporalType.TIMESTAMP )
    @DateTimeFormat( style = "--" )
    private Date instanteInicioOtimizacao;
    
    @Column( name = "ROP_INSTANTE_TERMINO" )
    @Temporal( TemporalType.TIMESTAMP )
    @DateTimeFormat( style = "--" )
    private Date instanteTermino;
    
	@Column(name = "ROP_STATUS_COD")
	private Integer statusCodigo;
	
	@Column(name = "ROP_STATUS_TEXTO")
	private String statusTexto;
	
	@Column(name = "ROP_TOTAL_CAMPI")
	private Integer totalCampi;
	
	@Column(name = "ROP_CAMPI_SELECIONADOS")
	private String campiSelecionados;
	
	@Column(name = "ROP_TOTAL_TURNOS")
	private Integer totalTurnos;
	
	@Column(name = "ROP_TURNOS_SELECIONADOS")
	private String turnosSelecionados;
	
	@Column(name = "ROP_TOTAL_ALUNOS")
	private Integer totalAlunos;
	
	@Column(name = "ROP_TOTAL_ALUNOS_DEMANDAS_P1")
	private Integer totalAlunosDemandasP1;
	
	@Column(name = "ROP_TOTAL_ALUNOS_DEMANDAS_P2")
	private Integer totalAlunosDemandasP2;
	
	@Column(name = "ROP_TOTAL_AMBIENTES")
	private Integer totalAmbientes;
	
	@Column(name = "ROP_TOTAL_PROFESSORES")
	private Integer totalProfessores;
	
	@PersistenceContext
	transient EntityManager entityManager;
	
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
	
	public Usuario getUsuario() {
		return this.usuario;
	}

	public void setUsuario(Usuario usuario) {
		this.usuario = usuario;
	}
	
	public Cenario getCenario() {
		return this.cenario;
	}

	public void setCenario(Cenario cenario) {
		this.cenario = cenario;
	}
	
	public Long getRound() {
		return this.round;
	}

	public void setRound(Long round) {
		this.round = round;
	}
	
	public Parametro getParametro() {
		return this.parametro;
	}

	public void setParametro(Parametro parametro) {
		this.parametro = parametro;
	}
	
	//@Override
	public int compareTo(RequisicaoOtimizacao o) {
		return getId().compareTo(o.getId());
	}
	
	@Transactional
	public void persist() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}
		this.entityManager.persist(this);
	}
	
	@Transactional
	public RequisicaoOtimizacao merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		RequisicaoOtimizacao merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	@Transactional
	public void remove() {
		if (this.entityManager == null) {
			this.entityManager = entityManager();
		}

		if (this.entityManager.contains(this)) {
			this.entityManager.remove(this);
		} else {
			RequisicaoOtimizacao attached = this.entityManager.find(this.getClass(),this.id);
			if (attached != null) {
				this.entityManager.remove(attached);
			}
		}
	}
	
	public static RequisicaoOtimizacao findBy(Usuario usuario, Cenario cenario, Parametro parametro, Long round) {
		Query q = entityManager().createQuery(
			" SELECT o FROM RequisicaoOtimizacao o " +
			" WHERE o.cenario.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.cenario = :cenario " +
			" AND o.parametro = :parametro " +
			" AND o.round = :round "
		);

		q.setParameter("instituicaoEnsino",usuario.getInstituicaoEnsino());
		q.setParameter("cenario",cenario);
		q.setParameter("parametro",parametro);
		q.setParameter("round",round);

		return (RequisicaoOtimizacao)q.getSingleResult();
	}
	
	@SuppressWarnings("unchecked")
	public static List<RequisicaoOtimizacao> findBy(Usuario usuario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM RequisicaoOtimizacao o " +
			" WHERE o.cenario.instituicaoEnsino = :instituicaoEnsino " +
			" AND YEAR(o.instanteInicioRequisicao) = :ano " +
			" ORDER BY o.instanteInicioRequisicao DESC"
		);

		q.setParameter("instituicaoEnsino", usuario.getInstituicaoEnsino());
		q.setParameter("ano", Calendar.getInstance().get(Calendar.YEAR));

		return q.getResultList();
	}
	
	@SuppressWarnings("unchecked")
	public static List<RequisicaoOtimizacao> findBy(List<Long> ids) {
		if (!ids.isEmpty()) {
			Query q = entityManager().createQuery(
				" SELECT o FROM RequisicaoOtimizacao o " +
				" WHERE o.id IN (:ids) "
			);
	
			q.setParameter("ids",ids);
	
			return q.getResultList();
		} else {
			return Collections.<RequisicaoOtimizacao>emptyList();
		}
	}
	
	public static RequisicaoOtimizacao findBy(Long round) {
		Query q = entityManager().createQuery(
			" SELECT o FROM RequisicaoOtimizacao o " +
			" WHERE o.round = (:round) "
		);

		q.setParameter("round",round);
		
		if (!q.getResultList().isEmpty())
			return (RequisicaoOtimizacao)q.getResultList().get(0);
		else
			return null;
	}
	
	@SuppressWarnings("unchecked")
	public static List<RequisicaoOtimizacao> findAll() {
		Query q = entityManager().createQuery("SELECT o FROM RequisicaoOtimizacao o");
		return q.getResultList();
	}
	
	public static final EntityManager entityManager() {
		EntityManager em = new RequisicaoOtimizacao().entityManager;
		if (em == null){
			throw new IllegalStateException(" Entity manager has not been injected (is the Spring " + " Aspects JAR configured as an AJC/AJDT aspects library?) " );
		}
		return em;
	}

	public Usuario getUsuarioCancel() {
		return usuarioCancel;
	}

	public void setUsuarioCancel(Usuario usuarioCancel) {
		this.usuarioCancel = usuarioCancel;
	}

	public Date getInstanteInicioRequisicao() {
		return instanteInicioRequisicao;
	}

	public void setInstanteInicioRequisicao(Date instanteInicioRequisicao) {
		this.instanteInicioRequisicao = instanteInicioRequisicao;
	}
	
	public Date getInstanteInicioOtimizacao() {
		return instanteInicioOtimizacao;
	}

	public void setInstanteInicioOtimizacao(Date instanteInicioOtimizacao) {
		this.instanteInicioOtimizacao = instanteInicioOtimizacao;
	}

	public Date getInstanteTermino() {
		return instanteTermino;
	}

	public void setInstanteTermino(Date instanteTermino) {
		this.instanteTermino = instanteTermino;
	}

	public Integer getStatusCodigo() {
		return statusCodigo;
	}

	public void setStatusCodigo(Integer statusCodigo) {
		this.statusCodigo = statusCodigo;
	}

	public String getStatusTexto() {
		return statusTexto;
	}

	public void setStatusTexto(String statusTexto) {
		this.statusTexto = statusTexto;
	}

	public Integer getTotalCampi() {
		return totalCampi;
	}

	public void setTotalCampi(Integer totalCampi) {
		this.totalCampi = totalCampi;
	}

	public String getCampiSelecionados() {
		return campiSelecionados;
	}

	public void setCampiSelecionados(String campiSelecionados) {
		this.campiSelecionados = campiSelecionados;
	}

	public Integer getTotalTurnos() {
		return totalTurnos;
	}

	public void setTotalTurnos(Integer totalTurnos) {
		this.totalTurnos = totalTurnos;
	}

	public String getTurnosSelecionados() {
		return turnosSelecionados;
	}

	public void setTurnosSelecionados(String turnosSelecionados) {
		this.turnosSelecionados = turnosSelecionados;
	}

	public Integer getTotalAlunos() {
		return totalAlunos;
	}

	public void setTotalAlunos(Integer totalAlunos) {
		this.totalAlunos = totalAlunos;
	}

	public Integer getTotalAlunosDemandasP1() {
		return totalAlunosDemandasP1;
	}

	public void setTotalAlunosDemandasP1(Integer totalAlunosDemandasP1) {
		this.totalAlunosDemandasP1 = totalAlunosDemandasP1;
	}

	public Integer getTotalAlunosDemandasP2() {
		return totalAlunosDemandasP2;
	}

	public void setTotalAlunosDemandasP2(Integer totalAlunosDemandasP2) {
		this.totalAlunosDemandasP2 = totalAlunosDemandasP2;
	}

	public Integer getTotalAmbientes() {
		return totalAmbientes;
	}

	public void setTotalAmbientes(Integer totalAmbientes) {
		this.totalAmbientes = totalAmbientes;
	}

	public Integer getTotalProfessores() {
		return totalProfessores;
	}

	public void setTotalProfessores(Integer totalProfessores) {
		this.totalProfessores = totalProfessores;
	}
}