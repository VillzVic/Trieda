package com.gapso.web.trieda.shared.dtos;

public class RelatorioDTO extends AbstractTreeDTO<String> implements
	Comparable<RelatorioDTO> {
	
	private static final long serialVersionUID = -4952776873486318384L;

	// Propriedades
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_TEXT = "text";
	public static final String PROPERTY_BUTTON_INDEX = "buttonIndex";
	public static final String PROPERTY_BUTTON_TEXT = "buttonText";
	
	private boolean hasChildren = false;

	public RelatorioDTO() {
		super();
	}
	
	public RelatorioDTO( String text ) {
		super();
		setText(text);
	}
	
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}

	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	
	public void setText(String value) {
		set(PROPERTY_TEXT, value);
	}

	public String getText() {
		return get(PROPERTY_TEXT);
	}
	
	public void setButtonIndex(Integer value) {
		set(PROPERTY_BUTTON_INDEX, value);
	}

	public Integer getButtonIndex() {
		return get(PROPERTY_BUTTON_INDEX);
	}
	
	public void setButtonText(String value) {
		set(PROPERTY_BUTTON_TEXT, value);
	}

	public String getButtonText() {
		return get(PROPERTY_BUTTON_TEXT);
	}
	
	public boolean hasChildren() {
		return hasChildren;
	}

	public void hasChildren(boolean hasChildren) {
		this.hasChildren = hasChildren;
	}
	
	@Override
	public int compareTo(RelatorioDTO o) {
		return getNaturalKey().compareTo(o.getNaturalKey());
	}

	@Override
	public String getNaturalKey() {
		String key = getText();
		RelatorioDTO parent = (RelatorioDTO) this.getParent();
		while (parent != null)
		{
			key += "-" + parent.getText();
			parent = (RelatorioDTO) parent.getParent();
		}
		return key;
	}

}
