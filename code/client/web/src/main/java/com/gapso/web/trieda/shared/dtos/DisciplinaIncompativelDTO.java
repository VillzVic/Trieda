package com.gapso.web.trieda.shared.dtos;

public class DisciplinaIncompativelDTO extends AbstractDTO<String>
	implements Comparable<DisciplinaIncompativelDTO>
{
	private static final long serialVersionUID = 2804545521094014835L;

	// Propriedades
	public static final String PROPERTY_DISCIPLINA1_ID = "disciplina1Id";
	public static final String PROPERTY_DISCIPLINA2_ID = "disciplina2Id";
	public static final String PROPERTY_DISCIPLINA1_STRING = "disciplina1String";
	public static final String PROPERTY_DISCIPLINA2_STRING = "disciplina2String";
	public static final String PROPERTY_INCOMPATIVEL = "incompativel";

	public DisciplinaIncompativelDTO() {
		super();
	}

	public void setDisciplina1Id(Long value) {
		set(PROPERTY_DISCIPLINA1_ID, value);
	}
	public Long getDisciplina1Id() {
		return get(PROPERTY_DISCIPLINA1_ID);
	}
	
	public void setDisciplina2Id(Long value) {
		set(PROPERTY_DISCIPLINA2_ID, value);
	}
	public Long getDisciplina2Id() {
		return get(PROPERTY_DISCIPLINA2_ID);
	}
	
	public void setDisciplina1String(String value) {
		set(PROPERTY_DISCIPLINA1_STRING, value);
	}
	public String getDisciplina1String() {
		return get(PROPERTY_DISCIPLINA1_STRING);
	}
	
	public void setDisciplina2String(String value) {
		set(PROPERTY_DISCIPLINA2_STRING, value);
	}
	public String getDisciplina2String() {
		return get(PROPERTY_DISCIPLINA2_STRING);
	}
	
	public Boolean getIncompativel() {
		return get(PROPERTY_INCOMPATIVEL);
	}
	public void setIncompativel(Boolean value) {
		set(PROPERTY_INCOMPATIVEL, value);
	}
	
	@Override
	public String getNaturalKey() {
		return getDisciplina1String() + "-" + getDisciplina2String();
	}

	@Override
	public int compareTo(DisciplinaIncompativelDTO o) {
		return getNaturalKey().compareTo(o.getNaturalKey());
	}
}
