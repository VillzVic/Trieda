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
@RooEntity( identifierColumn = "DPS_ID" )
@Table( name = "DISPONIBILIDADE_SALAS" )
public class DisponibilidadeSala  extends Disponibilidade implements Serializable, Clonable<DisponibilidadeSala>
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
	
	public DisponibilidadeSala clone(CenarioClone novoCenario) {
		DisponibilidadeSala clone = new DisponibilidadeSala();
		clone.setSala(novoCenario.getEntidadeClonada(this.getSala()));
		this.clone(clone);
		return clone;
	}
	
	public void cloneChilds(CenarioClone novoCenario, DisponibilidadeSala entidadeClone) {}
	
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
	
	@SuppressWarnings("unchecked")
	public static List<DisponibilidadeSala> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeSala o " +
			" WHERE o.DisponibilidadeSala.tipoSala.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}
	
	public static Map<Sala, List<Disponibilidade>> findDisponibilidadesPorAmbiente(Cenario cenario) {
		Map<Sala, List<Disponibilidade>> salaMapDisponibilidade = new HashMap<Sala, List<Disponibilidade>>();
		List<DisponibilidadeSala> disponibilidadesSalas = findBy(cenario);
		for (DisponibilidadeSala dispSal : disponibilidadesSalas) {
			List<Disponibilidade> disponibilidades = salaMapDisponibilidade.get(dispSal.getSala());
			if (disponibilidades == null) {
				disponibilidades = new ArrayList<Disponibilidade>();
				salaMapDisponibilidade.put(dispSal.getSala(), disponibilidades);
			}
			disponibilidades.add(dispSal);
		}
		return salaMapDisponibilidade;
	}
	
	public static Map<Sala, List<DisponibilidadeGenerica>> transformaEmDisponibilidadesCompactas(Map<Sala, List<Disponibilidade>> salasMapDisponibilidade) {
		Map<Sala, List<DisponibilidadeGenerica>> dispCompactasMap = new HashMap<Sala, List<DisponibilidadeGenerica>>();
		for (Entry<Sala, List<Disponibilidade>> entry : salasMapDisponibilidade.entrySet()) {
			Sala sala = entry.getKey();
			List<Disponibilidade> disponibilidadesOriginais = entry.getValue();
			List<DisponibilidadeGenerica> disponibilidadesCompactas = Disponibilidade.geraDisponibilidadesCompactadasPorDiaSemana(disponibilidadesOriginais);
			dispCompactasMap.put(sala, disponibilidadesCompactas);
		}
		return dispCompactasMap;
	}
}