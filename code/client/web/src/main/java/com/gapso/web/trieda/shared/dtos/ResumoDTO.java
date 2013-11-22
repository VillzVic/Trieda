package com.gapso.web.trieda.shared.dtos;

public class ResumoDTO  extends AbstractDTO< String >
	implements Comparable< ResumoDTO >
{
	private static final long serialVersionUID = 3757070364869952924L;

	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_LABEL = "label";
	public static final String PROPERTY_VALOR = "valor";
	
	public ResumoDTO()
	{
		super();
	}
	
	public ResumoDTO(String label, String valor)
	{
		super();
		setLabel(label);
		setValor(valor);
	}
	
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	
	public void setLabel(String value) {
		set(PROPERTY_LABEL, value);
	}
	
	public String getLabel() {
		return get(PROPERTY_LABEL);
	}
	
	public void setValor(String value) {
		set(PROPERTY_VALOR, value);
	}
	
	public String getValor() {
		return get(PROPERTY_VALOR);
	}
	
	@Override
	public String getDisplayText() {
		return ((String)get(PROPERTY_LABEL) + (String)get(PROPERTY_VALOR));
	}

	@Override
	public int compareTo(ResumoDTO o) {
		return getLabel().compareTo(o.getLabel());
	}

	@Override
	public String getNaturalKey() {
		return getLabel();
	}
}
