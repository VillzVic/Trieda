package com.gapso.trieda.domain;

import java.io.Serializable;
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
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
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
@RooEntity( identifierColumn = "DIC_PRV_ID" )
@Table( name = "DICAS_ELIMINACAO_PROF_VIRTUAL" )
public class DicaEliminacaoProfessorVirtual
	implements Serializable, Clonable< DicaEliminacaoProfessorVirtual >
{
	private static final long serialVersionUID = 5135175685795497437L;

	@PersistenceContext
	transient EntityManager entityManager;
	
	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "DIC_PRV_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;
	
	@NotNull
	@Column(name = "DIC_PRV_DICA_ELIMINACAO")
	@Type(type="text")
	private String dicaEliminacao;

	@NotNull
    @ManyToOne( cascade = { CascadeType.PERSIST, CascadeType.MERGE,
    	CascadeType.REFRESH }, targetEntity = Professor.class )
    @JoinColumn( name = "PRF_ID" )
    private Professor professor;
    
    @ManyToMany(cascade = {CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH, CascadeType.DETACH })
	@JoinTable(name="ATENDIMENTO_OPERACIONAL_DICAS_ELIMINACAO",
	joinColumns=@JoinColumn(name="DIC_PRV_ID"),
	inverseJoinColumns=@JoinColumn(name="ATP_ID"))
    private Set<AtendimentoOperacional> atendimentosOperacional = new HashSet<AtendimentoOperacional>();

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

	public String getDicaEliminacao() {
		return dicaEliminacao;
	}

	public void setDicaEliminacao(String dicaEliminacao) {
		this.dicaEliminacao = dicaEliminacao;
	}

	public Professor getProfessor() {
		return professor;
	}

	public void setProfessor(Professor professor) {
		this.professor = professor;
	}

	public Set<AtendimentoOperacional> getAtendimentosOperacional() {
		return atendimentosOperacional;
	}

	public void setAtendimentosOperacional(Set<AtendimentoOperacional> atendimentosOperacional) {
		this.atendimentosOperacional = atendimentosOperacional;
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
	
	public static final EntityManager entityManager()
	{
		EntityManager em = new DicaEliminacaoProfessorVirtual().entityManager;

		if ( em == null )
		{
			throw new IllegalStateException(
				" Entity manager has not been injected (is the Spring " +
				" Aspects JAR configured as an AJC/AJDT aspects library?) " );
		}

		return em;
	}
	
	@SuppressWarnings( "unchecked" )
	public static List< DicaEliminacaoProfessorVirtual > findByCenario(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario )
	{
		Query q = entityManager().createQuery(
			" SELECT DISTINCT ( o )" +
			" FROM DicaEliminacaoProfessorVirtual o, IN (o.atendimentosOperacional) at " +
			" WHERE at.cenario = :cenario " +
			" AND at.instituicaoEnsino = :instituicaoEnsino" );

		q.setParameter( "cenario", cenario );
		q.setParameter( "instituicaoEnsino", instituicaoEnsino );

		return q.getResultList();
	}


	public DicaEliminacaoProfessorVirtual clone(CenarioClone novoCenario) {
		DicaEliminacaoProfessorVirtual clone = new DicaEliminacaoProfessorVirtual();
		clone.setDicaEliminacao(this.getDicaEliminacao());
		clone.setProfessor(novoCenario.getEntidadeClonada(this.getProfessor()));
		
		return clone;
	}

	public void cloneChilds(CenarioClone novoCenario,
			DicaEliminacaoProfessorVirtual entidadeClone) {
		
	}
}
