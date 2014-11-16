package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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

import com.gapso.trieda.misc.TriedaDomainUtil;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "DPP_ID" )
@Table( name = "DISPONIBILIDADE_PROFESSORES" )
public class DisponibilidadeProfessor extends Disponibilidade implements Serializable, Clonable<DisponibilidadeProfessor>
{
	private static final long serialVersionUID = 2992132361625378649L;

	@Id
	@GeneratedValue( strategy = GenerationType.AUTO )
	@Column( name = "DPP_ID" )
	private Long id;
	
	@NotNull
	@ManyToOne( targetEntity = Professor.class )
	@JoinColumn( name = "PRF_ID" )
	private Professor DisponibilidadeProfessor;
	
	public Long getId() {
		return id;
	}
	
	public Long getEntidadeId() {
		return DisponibilidadeProfessor.getId();
	}
	
	public String getEntidadeTipo()
	{
		return "DisponibilidadeProfessor";
	}
	
	public DisponibilidadeProfessor clone(CenarioClone novoCenario) {
		DisponibilidadeProfessor clone = new DisponibilidadeProfessor();
		clone.setProfessor(novoCenario.getEntidadeClonada(this.getProfessor()));
		this.clone(clone);
		return clone;
	}
	
	public void cloneChilds(CenarioClone novoCenario, DisponibilidadeProfessor entidadeClone) {}
	
	public Professor getProfessor() {
		return DisponibilidadeProfessor;
	}
	
	public void setProfessor(Professor professor) {
		this.DisponibilidadeProfessor = professor;
	}
	
	@Transactional
	public DisponibilidadeProfessor merge()
	{
		if ( this.entityManager == null )
		{
			this.entityManager = entityManager();
		}
	
		DisponibilidadeProfessor merged = this.entityManager.merge( this );
		this.entityManager.flush();
		return merged;
	}
	
	public static DisponibilidadeProfessor find(Long id)
	{
		if ( id == null )
		{
			return null;
		}
	
		DisponibilidadeProfessor disponibilidade = entityManager().find( DisponibilidadeProfessor.class, id );
	
		if ( disponibilidade != null )
		{
			return disponibilidade;
		}
	
		return null;
	}
	
	@SuppressWarnings("unchecked")
	public static List<DisponibilidadeProfessor> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeProfessor o " +
			" WHERE o.DisponibilidadeProfessor.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}

	@Override
	public String toString() {
		return this.DisponibilidadeProfessor.getCpf() + 
				" " + TriedaDomainUtil.shortTimeString(this.getHorarioInicio()) + ">" + TriedaDomainUtil.shortTimeString(this.getHorarioFim()) +
				" Seg:" + diaSemanaStr(this.getSegunda()) +
				" Ter:" + diaSemanaStr(this.getTerca()) +
				" Qua:" + diaSemanaStr(this.getQuarta()) +
				" Qui:" + diaSemanaStr(this.getQuinta()) +
				" Sex:" + diaSemanaStr(this.getSexta()) +
				" Sab:" + diaSemanaStr(this.getSabado()) +
				" Dom:" + diaSemanaStr(this.getDomingo());
	}
	
	private static String diaSemanaStr(boolean diaSem) {
		return diaSem ? "X" : "_";
	}
	
	public static Map<Professor, List<Disponibilidade>> findDisponibilidadesPorProfessor(Cenario cenario) {
		Map<Professor, List<Disponibilidade>> professorMapDisponibilidade = new HashMap<Professor, List<Disponibilidade>>();
		List<DisponibilidadeProfessor> disponibilidadesProfessores = findBy(cenario);
		for (DisponibilidadeProfessor dispProf : disponibilidadesProfessores) {
			List<Disponibilidade> disponibilidades = professorMapDisponibilidade.get(dispProf.getProfessor());
			if (disponibilidades == null) {
				disponibilidades = new ArrayList<Disponibilidade>();
				professorMapDisponibilidade.put(dispProf.getProfessor(), disponibilidades);
			}
			disponibilidades.add(dispProf);
		}
		return professorMapDisponibilidade;
	}
}
