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
@RooEntity( identifierColumn = "CNF_ID" )
@Table( name = "PARAMETROS_CONFIGURACAO" )
public class ParametroConfiguracao
	implements Serializable
{
	
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 7944630009255400760L;

	@PersistenceContext
	transient EntityManager entityManager;
	
	@NotNull
	@ManyToOne( targetEntity = InstituicaoEnsino.class )
	@JoinColumn( name = "INS_ID" )
	private InstituicaoEnsino instituicaoEnsino;
	
	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "CNF_ID" )
	private Long id;

	@Version
	@Column( name = "version" )
	private Integer version;
	
	@NotNull
	@Column( name = "CNF_URL_OTI")
	private String urlOtimizacao;
	
	@NotNull
	@Column( name = "CNF_NOME_OTI")
	private String nomeOtimizacao;
	
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
	
	public String getUrlOtimizacao() {
		return urlOtimizacao;
	}

	public void setUrlOtimizacao(String urlOtimizacao) {
		this.urlOtimizacao = urlOtimizacao;
	}
	
	public String getNomeOtimizacao() {
		return nomeOtimizacao;
	}

	public void setNomeOtimizacao(String nomeOtimizacao) {
		this.nomeOtimizacao = nomeOtimizacao;
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
			ParametroConfiguracao attached = this.entityManager.find(
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
	public ParametroConfiguracao merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		ParametroConfiguracao merged = this.entityManager.merge( this );
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
	
	@Transactional
	public static ParametroConfiguracao findConfiguracoes(InstituicaoEnsino instituicaoEnsino) {
		ParametroConfiguracao config = findBy(instituicaoEnsino);
		
		if (config == null)
		{
			ParametroConfiguracao novaConfig = new ParametroConfiguracao();
			novaConfig.setInstituicaoEnsino(instituicaoEnsino);
			novaConfig.setNomeOtimizacao("trieda");
			novaConfig.setUrlOtimizacao("http://localhost:8080/SolverWS");
			
			novaConfig.persist();
			return novaConfig;
		}
		else
		{
			return config;
		}
	}
	
	@SuppressWarnings("unchecked")
	public static ParametroConfiguracao findBy(InstituicaoEnsino instituicaoEnsino) {

		Query q = entityManager().createQuery(
			" SELECT o " +
			" FROM ParametroConfiguracao o " +
			" WHERE o.instituicaoEnsino = :instituicaoEnsino " );

		q.setParameter( "instituicaoEnsino", instituicaoEnsino );
		
		List<ParametroConfiguracao> result =  q.getResultList();
		
		if (!result.isEmpty())
		{
			return result.get(0);
		}
		else
		{
			return null;
		}

	}
	
}
