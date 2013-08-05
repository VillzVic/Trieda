package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

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
@RooEntity( identifierColumn = "AFD_ID" )
@Table( name = "ATENDIMENTOS_FAIXA_DEMANDA" )
public class AtendimentoFaixaDemanda 
	implements Serializable
{
	private static final long serialVersionUID = -729417219296001563L;
	
	@PersistenceContext
	transient EntityManager entityManager;
	
	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "AFD_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;

	@NotNull
    @ManyToOne( targetEntity = Campus.class )
	@JoinColumn( name = "CAM_ID" )
	private Campus campus;

	@Column( name = "AFD_FAIXA_SUP" )
	@Min( 0L )
	@Max( 999L )
	private Integer faixaSuperior;
	
	@Column( name = "AFD_FAIXA_INF" )
	@Min( 0L )
	@Max( 999L )
	private Integer faixaInferior;
	
	@Column( name = "AFD_DEM_P1" )
	@Min( 0L )
	private Integer demandaP1;
	
	@Column( name = "AFD_ATEND_P1" )
	@Min( 0L )
	private Integer atendimentoP1;
	
	@Column( name = "AFD_ATEND_PERCENT_P1" )
	private Double atendimentoPercentP1;
	
	@Column( name = "AFD_ATEND_SOMA" )
	@Min( 0L )
	private Integer atendimentoSoma;
	
	@Column( name = "AFD_ATEND_SOMA_PERCENT" )
	private Double atendimentoSomaPercent;
	
	@Column( name = "AFD_TURMAS_ABERTAS" )
	@Min( 0L )
	private Double turmasAbertas;
	
	@Column( name = "AFD_MEDIA_TURMA" )
	private Double mediaTurma;
	
	@Column( name = "AFD_DEM_ACUM_P1" )
	@Min( 0L )
	private Integer demandaAcumP1;
	
	@Column( name = "AFD_CREDITOS_PAGOS" )
	@Min( 0L )
	private Double creditosPagos;
	
	@Column( name = "AFD_ATEND_SOMA_ACUM" )
	@Min( 0L )
	private Integer atendimentoSomaAcum;
	
	@Column( name = "AFD_ATEND_ACUM_PERCENT" )
	private Double atendimentoAcumPercent;
	
	@Column( name = "AFD_RECEITA_SEMANAL" )
	private Double receitaSemanal;
	
	@Column( name = "AFD_CUSTO_DOCENTE_SEMANAL" )
	private Double custoDocenteSemanal;
	
	@Column( name = "AFD_CUSTO_DOCENTE_PERCENT" )
	private Double custoDocentePercent;
	
	@Column( name = "AFD_RECEITA_ACUM" )
	private Double receitaAcumulada;
	
	@Column( name = "AFD_CUSTO_DOCENTE_ACUM" )
	private Double custoDocenteAcumulado;
	
	@Column( name = "AFD_CUSTO_DOCENTE_ACUM_PERCENT" )
	private Double custoDocentePorReceitaAcumuladoPercent;
	
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
			AtendimentoFaixaDemanda attached = this.entityManager.find(
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
	public AtendimentoFaixaDemanda merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		AtendimentoFaixaDemanda merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new AtendimentoFaixaDemanda().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				"Entity manager has not been injected (is the Spring " +
				"Aspects JAR configured as an AJC/AJDT aspects library?)" );
		}

		return em;
	}
	
	public static boolean isSalvo(InstituicaoEnsino instituicaoEnsino, Campus campus) {
		Query q = entityManager().createQuery(
			" SELECT COUNT ( o ) FROM AtendimentoFaixaDemanda o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus = :campus " );

		q.setParameter( "campus", campus );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		Number size = (Number) q.setMaxResults( 1 ).getSingleResult();
		return ( size.intValue() > 0 );
	}
	
	@SuppressWarnings( "unchecked" )
	public static List<AtendimentoFaixaDemanda> findByCampus(InstituicaoEnsino instituicaoEnsino,
			Campus campus) {

		Query q = entityManager().createQuery(
			" SELECT o " +
			" FROM AtendimentoFaixaDemanda o " +
			" WHERE o.campus.instituicaoEnsino = :instituicaoEnsino " +
			" AND o.campus = :campus " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		q.setParameter( "campus", campus );

		return q.getResultList();
	}
	
	@Transactional
	public static Integer deleteAllFromCampus(Campus campus) {
		Query q = entityManager().createQuery( "DELETE FROM AtendimentoFaixaDemanda WHERE campus = :campus");
		q.setParameter( "campus", campus );
		
		return q.executeUpdate();
	}


	public Campus getCampus() {
		return campus;
	}

	public void setCampus(Campus campus) {
		this.campus = campus;
	}

	public Long getId() {
		return id;
	}

	public void setId(Long id) {
		this.id = id;
	}

	public Integer getVersion() {
		return version;
	}

	public void setVersion(Integer version) {
		this.version = version;
	}

	public Integer getFaixaSuperior() {
		return faixaSuperior;
	}

	public void setFaixaSuperior(Integer faixaSuperior) {
		this.faixaSuperior = faixaSuperior;
	}

	public Integer getFaixaInferior() {
		return faixaInferior;
	}

	public void setFaixaInferior(Integer faixaInferior) {
		this.faixaInferior = faixaInferior;
	}

	public Integer getDemandaP1() {
		return demandaP1;
	}

	public void setDemandaP1(Integer demandaP1) {
		this.demandaP1 = demandaP1;
	}

	public Integer getAtendimentoP1() {
		return atendimentoP1;
	}

	public void setAtendimentoP1(Integer atendimentoP1) {
		this.atendimentoP1 = atendimentoP1;
	}

	public Double getAtendimentoPercentP1() {
		return atendimentoPercentP1;
	}

	public void setAtendimentoPercentP1(Double atendimentoPercentP1) {
		this.atendimentoPercentP1 = atendimentoPercentP1;
	}

	public Integer getAtendimentoSoma() {
		return atendimentoSoma;
	}

	public void setAtendimentoSoma(Integer atendimentoSoma) {
		this.atendimentoSoma = atendimentoSoma;
	}

	public Double getAtendimentoSomaPercent() {
		return atendimentoSomaPercent;
	}

	public void setAtendimentoSomaPercent(Double atendimentoSomaPercent) {
		this.atendimentoSomaPercent = atendimentoSomaPercent;
	}

	public Double getTurmasAbertas() {
		return turmasAbertas;
	}

	public void setTurmasAbertas(Double turmasAbertas) {
		this.turmasAbertas = turmasAbertas;
	}

	public Double getMediaTurma() {
		return mediaTurma;
	}

	public void setMediaTurma(Double mediaTurma) {
		this.mediaTurma = mediaTurma;
	}

	public Integer getDemandaAcumP1() {
		return demandaAcumP1;
	}

	public void setDemandaAcumP1(Integer demandaAcumP1) {
		this.demandaAcumP1 = demandaAcumP1;
	}

	public Double getCreditosPagos() {
		return creditosPagos;
	}

	public void setCreditosPagos(Double creditosPagos) {
		this.creditosPagos = creditosPagos;
	}

	public Integer getAtendimentoSomaAcum() {
		return atendimentoSomaAcum;
	}

	public void setAtendimentoSomaAcum(Integer atendimentoSomaAcum) {
		this.atendimentoSomaAcum = atendimentoSomaAcum;
	}

	public Double getAtendimentoAcumPercent() {
		return atendimentoAcumPercent;
	}

	public void setAtendimentoAcumPercent(Double atendimentoAcumPercent) {
		this.atendimentoAcumPercent = atendimentoAcumPercent;
	}

	public Double getReceitaSemanal() {
		return receitaSemanal;
	}

	public void setReceitaSemanal(Double receitaSemanal) {
		this.receitaSemanal = receitaSemanal;
	}

	public Double getCustoDocenteSemanal() {
		return custoDocenteSemanal;
	}

	public void setCustoDocenteSemanal(Double custoDocenteSemanal) {
		this.custoDocenteSemanal = custoDocenteSemanal;
	}

	public Double getCustoDocentePercent() {
		return custoDocentePercent;
	}

	public void setCustoDocentePercent(Double custoDocentePercent) {
		this.custoDocentePercent = custoDocentePercent;
	}

	public Double getCustoDocenteAcumulado() {
		return custoDocenteAcumulado;
	}

	public void setCustoDocenteAcumulado(Double custoDocenteAcumulado) {
		this.custoDocenteAcumulado = custoDocenteAcumulado;
	}

	public Double getCustoDocentePorReceitaAcumuladoPercent() {
		return custoDocentePorReceitaAcumuladoPercent;
	}

	public void setCustoDocentePorReceitaAcumuladoPercent(
			Double custoDocentePorReceitaAcumuladoPercent) {
		this.custoDocentePorReceitaAcumuladoPercent = custoDocentePorReceitaAcumuladoPercent;
	}

	public Double getReceitaAcumulada() {
		return receitaAcumulada;
	}

	public void setReceitaAcumulada(Double receitaAcumulada) {
		this.receitaAcumulada = receitaAcumulada;
	}

}
