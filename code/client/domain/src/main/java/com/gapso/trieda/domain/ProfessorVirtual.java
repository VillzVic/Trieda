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
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.PersistenceContext;
import javax.persistence.Query;
import javax.persistence.Table;
import javax.persistence.Version;
import javax.validation.constraints.Max;

import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.roo.addon.entity.RooEntity;
import org.springframework.roo.addon.javabean.RooJavaBean;
import org.springframework.roo.addon.tostring.RooToString;
import org.springframework.transaction.annotation.Transactional;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity(identifierColumn = "PRV_ID")
@Table(name = "PROFESSORES_VIRTUAIS")
public class ProfessorVirtual implements Serializable
{
	private static final long serialVersionUID = 265242535107921721L;

	@ManyToOne(targetEntity = Titulacao.class)
	@JoinColumn(name = "TIT_ID")
	private Titulacao titulacao;

	@ManyToOne(targetEntity = AreaTitulacao.class)
	@JoinColumn(name = "ATI_ID")
	private AreaTitulacao areaTitulacao;

	@Column(name = "PRF_CH_MIN")
	@Max(999L)
	private Integer cargaHorariaMin;

	@Column(name = "PRF_CH_MAX")
	@Max(999L)
	private Integer cargaHorariaMax;

	@ManyToMany( cascade = { CascadeType.PERSIST, CascadeType.MERGE, CascadeType.REFRESH } )
	private Set< Disciplina > disciplinas = new HashSet< Disciplina >();

	@OneToMany(mappedBy = "professorVirtual", cascade = CascadeType.ALL)
	private Set< AtendimentoOperacional > atendimentos = new HashSet< AtendimentoOperacional >();

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		sb.append( "Id: " ).append( getId() ).append( ", " );
		sb.append( "Version: " ).append( getVersion() ).append( ", " );
		sb.append( "Titulacao: " ).append( getTitulacao() ).append( ", " );
		sb.append( "AreaTitulacao: " ).append( getAreaTitulacao() ).append( ", " );
		sb.append( "CargaHorariaMin: " ).append( getCargaHorariaMin() ).append( ", " );
		sb.append( "CargaHorariaMax: " ).append( getCargaHorariaMax() ).append( ", " );
		sb.append( "Disciplinas: " ).append( getDisciplinas() == null ? "null" : getDisciplinas().size() );
		sb.append( "Atendimentos: " ).append( getAtendimentos() == null ? "null" : getAtendimentos().size() );

		return sb.toString();
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

	public Set< Disciplina > getDisciplinas()
	{
		return this.disciplinas;
	}

	public void setDisciplinas( Set< Disciplina > disciplinas )
	{
		this.disciplinas = disciplinas;
	}

	public Set< AtendimentoOperacional > getAtendimentos()
	{
		return atendimentos;
	}

	public void setAtendimentos( Set< AtendimentoOperacional > atendimentos )
	{
		this.atendimentos = atendimentos;
	}

	@PersistenceContext
	transient EntityManager entityManager;

	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	@Column(name = "PRF_ID")
	private Long id;

	@Version
	@Column(name = "version")
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
			ProfessorVirtual attached
				= this.entityManager.find( this.getClass(), this.id );

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
	public ProfessorVirtual merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		ProfessorVirtual merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}

	public static final EntityManager entityManager()
	{
		EntityManager em = new ProfessorVirtual().entityManager;
		if ( em == null )
		{
			throw new IllegalStateException(
					"Entity manager has not been injected (is the Spring Aspects JAR configured as an AJC/AJDT aspects library?)");
		}

		return em;
	}

	public static ProfessorVirtual find(Long id)
	{
		if (id == null)
		{
			return null;
		}

		return entityManager().find( ProfessorVirtual.class, id );
	}

	@SuppressWarnings("unchecked")
	public static List<ProfessorVirtual> findBy( Campus campus )
	{
		Query q = entityManager().createQuery(
						"SELECT DISTINCT o.professorVirtual FROM AtendimentoOperacional o WHERE o.oferta.campus = :campus" );

		q.setParameter( "campus", campus );
		return q.getResultList();
	}
}
