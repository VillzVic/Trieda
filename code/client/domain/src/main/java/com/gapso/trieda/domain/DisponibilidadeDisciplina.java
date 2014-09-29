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
@RooEntity( identifierColumn = "DPD_ID" )
@Table( name = "DISPONIBILIDADE_DISCIPLINAS" )
public class DisponibilidadeDisciplina extends Disponibilidade implements Serializable, Clonable<Disponibilidade>
{
	private static final long serialVersionUID = 3900149414072122210L;
	
	@Id
    @GeneratedValue( strategy = GenerationType.AUTO )
    @Column( name = "DPD_ID" )
    private Long id;

	@NotNull
    @ManyToOne( targetEntity = Disciplina.class )
    @JoinColumn( name = "DIS_ID" )
    private Disciplina DisponibilidadeDisciplina;

	public Long getId() {
		return id;
	}
	
	public Long getEntidadeId() {
		return DisponibilidadeDisciplina.getId();
	}
	
	public String getEntidadeTipo()
	{
		return "DisponibilidadeDisciplina";
	}

	public DisponibilidadeDisciplina clone(CenarioClone novoCenario) {
		// TODO Auto-generated method stub
		return null;
	}

	public Disciplina getDisciplina() {
		return DisponibilidadeDisciplina;
	}

	public void setDisciplina(Disciplina disciplina) {
		this.DisponibilidadeDisciplina = disciplina;
	}

	@Transactional
	public DisponibilidadeDisciplina merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}

		DisponibilidadeDisciplina merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	public static DisponibilidadeDisciplina find(Long id)
	{
		if ( id == null )
		{
			return null;
		}

		DisponibilidadeDisciplina disponibilidade = entityManager().find( DisponibilidadeDisciplina.class, id );

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
	public static List<DisponibilidadeDisciplina> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeDisciplina o " +
			" WHERE o.DisponibilidadeDisciplina.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}
}
