package com.gapso.trieda.domain;

import java.io.Serializable;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
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
@RooEntity( identifierColumn = "DPS_ID" )
@Table( name = "DISPONIBILIDADE_SALAS" )
public class DisponibilidadeSala  extends Disponibilidade implements Serializable, Clonable<Disponibilidade>
{
	private static final long serialVersionUID = 4534696857174619653L;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "DPS_ID" )
	private Long id;
	
	@NotNull
	@ManyToOne( targetEntity = Sala.class )
	@JoinColumn( name = "SAL_ID" )
	private Sala DisponibilidadeSala;
	
	public Long getId() {
		return id;
	}
	
	public Long getEntidadeId() {
		return DisponibilidadeSala.getId();
	}
	
	public String getEntidadeTipo()
	{
		return "DisponibilidadeSala";
	}
	
	public DisponibilidadeDisciplina clone(CenarioClone novoCenario) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public Sala getSala() {
		return DisponibilidadeSala;
	}
	
	public void setSala(Sala sala) {
		this.DisponibilidadeSala = sala;
	}
	
	@Transactional
	public DisponibilidadeSala merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}
	
		DisponibilidadeSala merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	public static DisponibilidadeSala find(Long id)
	{
		if ( id == null )
		{
			return null;
		}
	
		DisponibilidadeSala disponibilidade = entityManager().find( DisponibilidadeSala.class, id );
	
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
}