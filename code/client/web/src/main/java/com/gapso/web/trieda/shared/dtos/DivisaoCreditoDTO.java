package com.gapso.web.trieda.shared.dtos;

public class DivisaoCreditoDTO extends AbstractDTO<String>
	implements Comparable<DivisaoCreditoDTO>
{
	private static final long serialVersionUID = 7603464268140278420L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_TOTAL_CREDITOS = "totalCreditos";
	public static final String PROPERTY_DIA1 = "dia1";
	public static final String PROPERTY_DIA2 = "dia2";
	public static final String PROPERTY_DIA3 = "dia3";
	public static final String PROPERTY_DIA4 = "dia4";
	public static final String PROPERTY_DIA5 = "dia5";
	public static final String PROPERTY_DIA6 = "dia6";
	public static final String PROPERTY_DIA7 = "dia7";

	public DivisaoCreditoDTO()
	{
		super();
	}

	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}

	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID, value);
	}
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
	}
	
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}
	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	
	public void setDisciplinaString(String value) {
		set(PROPERTY_DISCIPLINA_STRING, value);
	}
	public String getDisciplinaString() {
		return get(PROPERTY_DISCIPLINA_STRING);
	}

	public void setTotalCreditos(Integer value){ set(PROPERTY_TOTAL_CREDITOS, value); }
	public Integer getTotalCreditos() 		   { return get(PROPERTY_TOTAL_CREDITOS); }
	
	public void setDia1(Integer value) { set(PROPERTY_DIA1, value); }
	public Integer getDia1() 		   { return get(PROPERTY_DIA1); }
	
	public void setDia2(Integer value) { set(PROPERTY_DIA2, value); }
	public Integer getDia2() 		   { return get(PROPERTY_DIA2); }
	
	public void setDia3(Integer value) { set(PROPERTY_DIA3, value); }
	public Integer getDia3() 		   { return get(PROPERTY_DIA3); }
	
	public void setDia4(Integer value) { set(PROPERTY_DIA4, value); }
	public Integer getDia4() 		   { return get(PROPERTY_DIA4); }
	
	public void setDia5(Integer value) { set(PROPERTY_DIA5, value); }
	public Integer getDia5() 		   { return get(PROPERTY_DIA5); }
	
	public void setDia6(Integer value) { set(PROPERTY_DIA6, value); }
	public Integer getDia6() 		   { return get(PROPERTY_DIA6); }
	
	public void setDia7(Integer value) { set(PROPERTY_DIA7, value); }
	public Integer getDia7() 		   { return get(PROPERTY_DIA7); }

	@Override
	public String getNaturalKey() {
		return getId() + "-" + getCenarioId() + "-" + getDisciplinaString() + "-" + getTotalCreditos();
	}
	
	@Override
	public int compareTo(DivisaoCreditoDTO o) {
		return getTotalCreditos().compareTo(o.getTotalCreditos());
	}
}
