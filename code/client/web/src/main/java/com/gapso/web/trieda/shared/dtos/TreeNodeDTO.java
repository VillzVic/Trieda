package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class TreeNodeDTO<DTOType extends AbstractDTO<?>> extends BaseModel {

	private static final long serialVersionUID = -8890847882508721586L;
	
	// Propriedades
	public static final String PROPERTY_TEXT = "text";
	public static final String PROPERTY_PATH = "path";
	public static final String PROPERTY_LEAF = "leaf";
	public static final String PROPERTY_EMPTY = "empty";
	
	private DTOType content;
	
	public TreeNodeDTO() {
	}
	
	public TreeNodeDTO(DTOType content) {
		this.content = content;
		setText(content.getDisplayText());
		setPath(content.getDisplayText());
		setLeaf(false);
	}
	
	public DTOType getContent() {
		return content;
	}
	public void setContent(DTOType content) {
		this.content = content;
	}

	public void setText(String text) {
		set(PROPERTY_TEXT, text);
	}
	public String getText() {
		return get(PROPERTY_TEXT);
	}

	public void setPath(String path) {
		set(PROPERTY_PATH, path);
	}
	public String getPath() {
		return get(PROPERTY_PATH);
	}

	public void setLeaf(Boolean leaf) {
		set(PROPERTY_LEAF, leaf);
	}
	public Boolean getLeaf() {
		return get(PROPERTY_LEAF);
	}
	
	public void setEmpty(Boolean empty) {
		set(PROPERTY_EMPTY, empty);
	}
	public Boolean getEmpty() {
		return get(PROPERTY_EMPTY);
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((getText() == null) ? 0 : getText().hashCode());
		result = prime * result + ((getPath() == null) ? 0 : getPath().hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		@SuppressWarnings("unchecked")
		TreeNodeDTO<DTOType> other = (TreeNodeDTO<DTOType>) obj;
		if (getText() == null) {
			if (other.getText() != null)
				return false;
		} else if (!getText().equals(other.getText()))
			return false;
		if (getPath() == null) {
			if (other.getPath() != null)
				return false;
		} else if (!getPath().equals(other.getPath()))
			return false;
		return true;
	}
}