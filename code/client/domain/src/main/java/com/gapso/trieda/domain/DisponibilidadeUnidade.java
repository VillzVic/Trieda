package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Query;
import javax.persistence.Table;
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
@RooEntity( identifierColumn = "DPU_ID" )
@Table( name = "DISPONIBILIDADE_UNIDADES" )
public class DisponibilidadeUnidade extends Disponibilidade implements Serializable, Clonable<DisponibilidadeUnidade>
{
	private static final long serialVersionUID = 7249377105170783187L;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "DPU_ID" )
	private Long id;
	
	@NotNull
	@ManyToOne( targetEntity = Unidade.class )
	@JoinColumn( name = "UNI_ID" )
	private Unidade DisponibilidadeUnidade;
	
	public Long getId() {
		return id;
	}
	
	public Long getEntidadeId() {
		return DisponibilidadeUnidade.getId();
	}
	
	public String getEntidadeTipo()
	{
		return "DisponibilidadeUnidade";
	}
	
	public DisponibilidadeUnidade clone(CenarioClone novoCenario) {
		DisponibilidadeUnidade clone = new DisponibilidadeUnidade();
		clone.setUnidade(novoCenario.getEntidadeClonada(this.getUnidade()));
		this.clone(clone);
		return clone;
	}
	
	public void cloneChilds(CenarioClone novoCenario, DisponibilidadeUnidade entidadeClone) {}
	
	public Unidade getUnidade() {
		return DisponibilidadeUnidade;
	}
	
	public void setUnidade(Unidade unidade) {
		this.DisponibilidadeUnidade = unidade;
	}
	
	@Transactional
	public DisponibilidadeUnidade merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}
	
		DisponibilidadeUnidade merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	public static DisponibilidadeUnidade find(Long id)
	{
		if ( id == null )
		{
			return null;
		}
	
		DisponibilidadeUnidade disponibilidade = entityManager().find( DisponibilidadeUnidade.class, id );
	
		if ( disponibilidade != null )
		{
			return disponibilidade;
		}
	
		return null;
	}
	
	@SuppressWarnings("unchecked")
	public static List<DisponibilidadeUnidade> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeUnidade o " +
			" WHERE o.DisponibilidadeUnidade.campus.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}
}