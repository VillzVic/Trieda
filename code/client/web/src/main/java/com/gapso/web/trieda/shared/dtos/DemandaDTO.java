package com.gapso.web.trieda.shared.dtos;

public class DemandaDTO
	extends AbstractDTO< String >
	implements Comparable< DemandaDTO >
{
	private static final long serialVersionUID = 5815525344760896272L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_OFERTA_ID = "ofertaId";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_DEMANDA = "quantidade";
	public static final String PROPERTY_DEMANDA_REAL = "quantidadeReal";
	public static final String PROPERTY_DEMANDA_VIRTUAL = "quantidadeVirtual";
	public static final String PROPERTY_QUANTIDADE_DEMANDA_ENABLE = "quantidadeDemandaEnable";
	public static final String PROPERTY_INSTITUICAO_ENSINO_ID = "instituicaoEnsinoId";
	public static final String PROPERTY_INSTITUICAO_ENSINO_STRING = "instituicaoEnsinoString";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_EXIGE_EQUIVALENCIA_FORCADA = "exigeEquivalenciaForcada";
	public static final String PROPERTY_PRIORIDADE_ALUNOS = "prioridadeAlunos";
	public static final String PROPERTY_QUANTIDADE_NAO_ATENDIDA = "quantidadeNaoAtendida";
	

	public DemandaDTO()
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

	public void setOfertaId( Long value )
	{
		set(PROPERTY_OFERTA_ID, value);
	}

	public Long getOfertaId()
	{
		return get( PROPERTY_OFERTA_ID );
	}

	public void setCampusId( Long value )
	{
		set( PROPERTY_CAMPUS_ID, value );
	}

	public Long getCampusId()
	{
		return get( PROPERTY_CAMPUS_ID );
	}

	public void setCampusString( String value )
	{
		set( PROPERTY_CAMPUS_STRING, value );
	}

	public String getCampusString()
	{
		return get( PROPERTY_CAMPUS_STRING );
	}

	public void setCursoId( Long value )
	{
		set( PROPERTY_CURSO_ID, value );
	}

	public Long getCursoId()
	{
		return get( PROPERTY_CURSO_ID );
	}

	public void setCursoString( String value )
	{
		set( PROPERTY_CURSO_STRING, value );
	}

	public String getCursoString()
	{
		return get( PROPERTY_CURSO_STRING );
	}

	public void setCurriculoId( Long value )
	{
		set( PROPERTY_CURRICULO_ID, value );
	}

	public Long getCurriculoId()
	{
		return get( PROPERTY_CURRICULO_ID );
	}

	public void setCurriculoString( String value )
	{
		set( PROPERTY_CURRICULO_STRING, value );
	}

	public String getCurriculoString()
	{
		return get( PROPERTY_CURRICULO_STRING );
	}

	public void setTurnoId( Long value )
	{
		set( PROPERTY_TURNO_ID, value );
	}

	public Long getTurnoId()
	{
		return get( PROPERTY_TURNO_ID );
	}

	public void setTurnoString( String value )
	{
		set( PROPERTY_TURNO_STRING, value );
	}

	public String getTurnoString() {
		return get(PROPERTY_TURNO_STRING);
	}

	public void setDisciplinaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_ID, value );
	}

	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}

	public void setDisciplinaString( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING, value );
	}

	public String getDisciplinaString()
	{
		return get( PROPERTY_DISCIPLINA_STRING );
	}

	public void setDemanda( Integer value )
	{
		set( PROPERTY_DEMANDA, value );
	}

	public Integer getDemanda()
	{
		return get( PROPERTY_DEMANDA );
	}
	
	public void setDemandaReal( Integer value )
	{
		set( PROPERTY_DEMANDA_REAL, value );
	}

	public Integer getDemandaReal()
	{
		return get( PROPERTY_DEMANDA_REAL );
	}
	
	public void setDemandaVirtual( Integer value )
	{
		set( PROPERTY_DEMANDA_VIRTUAL, value );
	}

	public Integer getDemandaVirtual()
	{
		return get( PROPERTY_DEMANDA_VIRTUAL );
	}
	
	public void setPeriodo( Integer value )
	{
		set( PROPERTY_PERIODO, value );
	}

	public Integer getPeriodo()
	{
		return get( PROPERTY_PERIODO );
	}

	public void setQuantidadeDemandaEnable( Boolean value )
	{
		set( PROPERTY_QUANTIDADE_DEMANDA_ENABLE, value );
	}

	public Boolean getQuantidadeDemandaEnable()
	{
		return get( PROPERTY_QUANTIDADE_DEMANDA_ENABLE );
	}
	
	public void setExigeEquivalenciaForcada( Boolean value )
	{
		set( PROPERTY_EXIGE_EQUIVALENCIA_FORCADA, value );
	}

	public Boolean getExigeEquivalenciaForcada()
	{
		return get( PROPERTY_EXIGE_EQUIVALENCIA_FORCADA );
	}
	
	public void setPrioridadeAlunos( Integer value )
	{
		set( PROPERTY_PRIORIDADE_ALUNOS, value );
	}
	
	public Integer getPrioridadeAlunos()
	{
		return get( PROPERTY_PRIORIDADE_ALUNOS );
	}
	
	public void setQuantidadeNaoAtendida( Integer value )
	{
		set( PROPERTY_QUANTIDADE_NAO_ATENDIDA, value );
	}
	
	public Integer getQuantidadeNaoAtendida()
	{
		return get( PROPERTY_QUANTIDADE_NAO_ATENDIDA );
	}

	@Override
	public String getNaturalKey()
	{
		return getCampusString() + "-" + getTurnoString()
			+ "-" + getCursoString() + "-" + getCurriculoString()
			+ "-" + getDisciplinaString();
	}

	@Override
	public int compareTo( DemandaDTO o )
	{
		int result = getCampusString().compareTo( o.getCampusString() );

		if ( result == 0 )
		{
			result = getTurnoString().compareTo( o.getTurnoString() );

			if ( result == 0 )
			{
				result = getCursoString().compareTo( o.getCursoString() );

				if ( result == 0 )
				{
					result = getCurriculoString().compareTo( o.getCurriculoString() );

					if ( result == 0 )
					{
						result = getDisciplinaString().compareTo( o.getDisciplinaString() );
					}
				}
			}
		}

		return result;
	}
}
