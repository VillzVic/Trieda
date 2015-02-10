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
@RooEntity( identifierColumn = "DPC_ID" )
@Table( name = "DISPONIBILIDADE_CAMPI" )
public class DisponibilidadeCampus extends Disponibilidade implements Serializable, Clonable<DisponibilidadeCampus>
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
		DisponibilidadeCampus clone = new DisponibilidadeCampus();
		clone.setCampus(novoCenario.getEntidadeClonada(this.getCampus()));
		this.clone(clone);
		return clone;
	}
	
	public void cloneChilds(CenarioClone novoCenario, DisponibilidadeCampus entidadeClone) {}
	
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
	
	@SuppressWarnings("unchecked")
	public static List<DisponibilidadeCampus> findBy(Cenario cenario) {
		Query q = entityManager().createQuery(
			" SELECT o FROM DisponibilidadeCampus o " +
			" WHERE o.DisponibilidadeCampus.cenario.id = :cenarioId " +
			" ORDER BY o.horarioInicio, o.horarioFim" );
		
		q.setParameter( "cenarioId", cenario.getId() );

		return q.getResultList();
	}
	
	public static Map<Campus, List<Disponibilidade>> findDisponibilidadesPorCampus(Cenario cenario) {
		Map<Campus, List<Disponibilidade>> campusMapDisponibilidade = new HashMap<Campus, List<Disponibilidade>>();
		List<DisponibilidadeCampus> disponibilidadesCampi = findBy(cenario);
		for (DisponibilidadeCampus dispCam : disponibilidadesCampi) {
			List<Disponibilidade> disponibilidades = campusMapDisponibilidade.get(dispCam.getCampus());
			if (disponibilidades == null) {
				disponibilidades = new ArrayList<Disponibilidade>();
				campusMapDisponibilidade.put(dispCam.getCampus(), disponibilidades);
			}
			disponibilidades.add(dispCam);
		}
		return campusMapDisponibilidade;
	}
	
	public static Map<Campus, List<DisponibilidadeGenerica>> transformaEmDisponibilidadesCompactas(Map<Campus, List<Disponibilidade>> campusMapDisponibilidade) {
		Map<Campus, List<DisponibilidadeGenerica>> dispCompactasMap = new HashMap<Campus, List<DisponibilidadeGenerica>>();
		for (Entry<Campus, List<Disponibilidade>> entry : campusMapDisponibilidade.entrySet()) {
			Campus campus = entry.getKey();
			List<Disponibilidade> disponibilidadesOriginais = entry.getValue();
			List<DisponibilidadeGenerica> disponibilidadesCompactas = Disponibilidade.geraDisponibilidadesCompactadasPorDiaSemana(disponibilidadesOriginais);
			dispCompactasMap.put(campus, disponibilidadesCompactas);
		}
		return dispCompactasMap;
	}
}