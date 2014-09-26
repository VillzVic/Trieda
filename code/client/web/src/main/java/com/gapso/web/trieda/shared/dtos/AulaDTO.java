package com.gapso.web.trieda.shared.dtos;

import java.util.Set;

public class AulaDTO
	extends AbstractDTO< String >
	implements Comparable< AulaDTO >
{
	
	private static final long serialVersionUID = -4405054287109634889L;

	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_ATENDIMENTOS_IDS = "atendimentosIds";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CREDITOS_TEORICOS = "creditosTeoricos";
	public static final String PROPERTY_CREDITOS_PRATICOS = "creditosPraticos";
	public static final String PROPERTY_HORARIO_DISPONIVEL_CENARIO_ID = "horarioDiponivelCenarioId";
	public static final String PROPERTY_HORARIO_AULA_ID = "horarioAulaId";
	public static final String PROPERTY_HORARIO_STRING = "horarioString";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_NOME = "professorNome";
	public static final String PROPERTY_PROFESSOR_VIRTUAL_ID = "professorVirtualId";
	public static final String PROPERTY_SEMANA = "semana";
	public static final String PROPERTY_SEMANA_STRING = "semanaString";
	

	public AulaDTO()
	{
		super();
	}
	
	public void setId( Long value )
	{
		set( PROPERTY_ID, value );
	}

	public Long getId()
	{
		return get( PROPERTY_ID );
	}
	
	public void setAtendimentosIds( Set<Long> value )
	{
		set( PROPERTY_ATENDIMENTOS_IDS, value );
	}

	public Set<Long> getAtendimentosIds()
	{
		return get( PROPERTY_ATENDIMENTOS_IDS );
	}

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}
	
	public void setCreditosPraticos( Integer value )
	{
		set( PROPERTY_CREDITOS_PRATICOS, value );
	}

	public Integer getCreditosPraticos()
	{
		return get( PROPERTY_CREDITOS_PRATICOS );
	}
	
	public void setCreditosTeoricos( Integer value )
	{
		set( PROPERTY_CREDITOS_TEORICOS, value );
	}

	public Integer getCreditosTeoricos()
	{
		return get( PROPERTY_CREDITOS_TEORICOS );
	}
	
	public void setHorarioDisponivelCenarioId( Long value )
	{
		set( PROPERTY_HORARIO_DISPONIVEL_CENARIO_ID, value );
	}

	public Long getHorarioDisponivelCenarioId()
	{
		return get( PROPERTY_HORARIO_DISPONIVEL_CENARIO_ID );
	}
	
	public void setHorarioAulaId( Long value )
	{
		set( PROPERTY_HORARIO_AULA_ID, value );
	}

	public Long getHorarioAulaId()
	{
		return get( PROPERTY_HORARIO_AULA_ID );
	}
	
	public void setHorarioString( String value )
	{
		set( PROPERTY_HORARIO_STRING, value );
	}

	public String getHorarioString()
	{
		return get( PROPERTY_HORARIO_STRING );
	}
	
	public void setSalaId( Long value )
	{
		set( PROPERTY_SALA_ID, value );
	}

	public Long getSalaId()
	{
		return get( PROPERTY_SALA_ID );
	}
	
	public void setSalaString( String value )
	{
		set( PROPERTY_SALA_STRING, value );
	}

	public String getSalaString()
	{
		return get( PROPERTY_SALA_STRING );
	}
	
	public void setProfessorId( Long value )
	{
		set( PROPERTY_PROFESSOR_ID, value );
	}

	public Long getProfessorId()
	{
		return get( PROPERTY_PROFESSOR_ID );
	}
	
	public void setProfessorNome( String value )
	{
		set( PROPERTY_PROFESSOR_NOME, value );
	}

	public String getProfessorNome()
	{
		return get( PROPERTY_PROFESSOR_NOME );
	}
	
	public void setProfessorVirtualId( Long value )
	{
		set( PROPERTY_PROFESSOR_VIRTUAL_ID, value );
	}

	public Long getProfessorVirtualId()
	{
		return get( PROPERTY_PROFESSOR_VIRTUAL_ID );
	}
	
	public void setSemana( Integer value )
	{
		set( PROPERTY_SEMANA, value );
	}

	public Integer getSemana()
	{
		return get( PROPERTY_SEMANA );
	}
	
	public void setSemanaString( String value )
	{
		set( PROPERTY_SEMANA_STRING, value );
	}

	public String getSemanaString()
	{
		return get( PROPERTY_SEMANA_STRING );
	}
	
	@Override
	public int compareTo(AulaDTO o) {
		return this.getNaturalKey().compareTo(o.getNaturalKey());
	}

	@Override
	public String getNaturalKey() {
		return this.getHorarioAulaId() + "-" + this.getSalaId() + "-" + this.getSemana();
	}
	
	public String getIdKey() {
		
		if (this.getId() != null)
		{
			return this.getId().toString();
		}
		else if (this.getAtendimentosIds() != null)
		{
			String idKey = "";
			for (Long id : this.getAtendimentosIds())
			{
				idKey += id.toString() + "-";
			}
			return idKey;
		}
		else
		{
			return "";
		}
	}

}
