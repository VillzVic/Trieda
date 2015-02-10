package com.gapso.trieda.domain;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

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

import com.gapso.trieda.misc.DisponibilidadeGenerica;

@Configurable
@Entity
@RooJavaBean
@RooToString
@RooEntity( identifierColumn = "DPD_ID" )
@Table( name = "DISPONIBILIDADE_DISCIPLINAS" )
public class DisponibilidadeDisciplina extends Disponibilidade implements Serializable, Clonable<DisponibilidadeDisciplina>
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
		DisponibilidadeDisciplina clone = new DisponibilidadeDisciplina();
		clone.setDisciplina(novoCenario.getEntidadeClonada(this.getDisciplina()));
		this.clone(clone);
		return clone;
	}
	
	public void cloneChilds(CenarioClone novoCenario, DisponibilidadeDisciplina entidadeClone) {}

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
	
	@SuppressWarnings("unchecked")
	public static List<DisponibilidadeDisciplina> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeDisciplina o " +
			" WHERE o.DisponibilidadeDisciplina.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}
	
	public static Map<Disciplina, List<Disponibilidade>> findDisponibilidadesPorDisciplina(Cenario cenario) {
		Map<Disciplina, List<Disponibilidade>> disciplinaMapDisponibilidade = new HashMap<Disciplina, List<Disponibilidade>>();
		List<DisponibilidadeDisciplina> disponibilidadesDisciplinas = findBy(cenario);
		for (DisponibilidadeDisciplina dispDisc : disponibilidadesDisciplinas) {
			List<Disponibilidade> disponibilidades = disciplinaMapDisponibilidade.get(dispDisc.getDisciplina());
			if (disponibilidades == null) {
				disponibilidades = new ArrayList<Disponibilidade>();
				disciplinaMapDisponibilidade.put(dispDisc.getDisciplina(), disponibilidades);
			}
			disponibilidades.add(dispDisc);
		}
		return disciplinaMapDisponibilidade;
	}
	
	public static Map<Disciplina, List<DisponibilidadeGenerica>> transformaEmDisponibilidadesCompactas(Map<Disciplina, List<Disponibilidade>> disciplinasMapDisponibilidade) {
		Map<Disciplina, List<DisponibilidadeGenerica>> dispCompactasMap = new HashMap<Disciplina, List<DisponibilidadeGenerica>>();
		for (Entry<Disciplina, List<Disponibilidade>> entry : disciplinasMapDisponibilidade.entrySet()) {
			Disciplina disciplina = entry.getKey();
			List<Disponibilidade> disponibilidadesOriginais = entry.getValue();
			List<DisponibilidadeGenerica> disponibilidadesCompactas = Disponibilidade.geraDisponibilidadesCompactadasPorDiaSemana(disponibilidadesOriginais);
			dispCompactasMap.put(disciplina, disponibilidadesCompactas);
		}
		return dispCompactasMap;
	}
}
