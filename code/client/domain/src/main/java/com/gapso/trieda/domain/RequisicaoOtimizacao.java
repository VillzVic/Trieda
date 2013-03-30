package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.Collections;
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
import javax.persistence.UniqueConstraint;
import javax.persistence.Version;
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
@RooEntity(identifierColumn = "ROP_ID")
@Table(name = "REQUISICAO_OTIMIZACAO", uniqueConstraints =
@UniqueConstraint(columnNames = {"USERNAME","ROP_ROUND","PAR_ID","CEN_ID"}))
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
    @JoinColumn(name = "USERNAME")
    private Usuario usuario;
	
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
			" WHERE o.usuario = :usuario " +
			" AND o.cenario = :cenario " +
			" AND o.parametro = :parametro " +
			" AND o.round = :round "
		);

		q.setParameter("usuario",usuario);
		q.setParameter("cenario",cenario);
		q.setParameter("parametro",parametro);
		q.setParameter("round",round);

		return (RequisicaoOtimizacao)q.getSingleResult();
	}
	
	@SuppressWarnings("unchecked")
	public static List<RequisicaoOtimizacao> findBy(Usuario usuario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM RequisicaoOtimizacao o " +
			" WHERE o.usuario = :usuario "
		);

		q.setParameter("usuario",usuario);

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
}