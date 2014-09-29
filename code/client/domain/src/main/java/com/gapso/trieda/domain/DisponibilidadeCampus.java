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
@RooEntity( identifierColumn = "DPC_ID" )
@Table( name = "DISPONIBILIDADE_CAMPI" )
public class DisponibilidadeCampus extends Disponibilidade implements Serializable, Clonable<Disponibilidade>
{
	private static final long serialVersionUID = 4764994780282251456L;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "DPC_ID" )
	private Long id;
	
	@NotNull
	@ManyToOne( targetEntity = Campus.class )
	@JoinColumn( name = "CAM_ID" )
	private Campus DisponibilidadeCampus;
	
	public Long getId() {
		return id;
	}
	
	public Long getEntidadeId() {
		return DisponibilidadeCampus.getId();
	}
	
	public String getEntidadeTipo()
	{
		return "DisponibilidadeCampus";
	}
	
	public DisponibilidadeCampus clone(CenarioClone novoCenario) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public Campus getCampus() {
		return DisponibilidadeCampus;
	}
	
	public void setCampus(Campus campus) {
		this.DisponibilidadeCampus = campus;
	}
	
	@Transactional
	public DisponibilidadeCampus merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}
	
		DisponibilidadeCampus merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	public static DisponibilidadeCampus find(Long id)
	{
		if ( id == null )
		{
			return null;
		}
	
		DisponibilidadeCampus disponibilidade = entityManager().find( DisponibilidadeCampus.class, id );
	
		if ( disponibilidade != null )
		{
			return disponibilidade;
		}
	
		return null;
	}
	
	public void cloneChilds(CenarioClone novoCenario,
			Disponibilidade entidadeClone) {
		// TODO Auto-generated method stub
		
	}
	
	@SuppressWarnings("unchecked")
	public static List<DisponibilidadeCampus> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeCampus o " +
			" WHERE o.DisponibilidadeCampus.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}
}