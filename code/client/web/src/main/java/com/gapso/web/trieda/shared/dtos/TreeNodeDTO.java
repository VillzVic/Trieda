package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class TreeNodeDTO extends BaseModel implements Comparable<TreeNodeDTO> {

	private static final long serialVersionUID = -8890847882508721586L;
	
	// Propriedades
	public static final String PROPERTY_TEXT = "text";
	public static final String PROPERTY_PATH = "path";
	public static final String PROPERTY_LEAF = "leaf";
	public static final String PROPERTY_EMPTY = "empty";
	
	private AbstractDTO<?> content;
	private TreeNodeDTO parent;
	
	public TreeNodeDTO() {
	}
	
	public TreeNodeDTO(AbstractDTO<?> content, String text, String path, Boolean leaf, Boolean empty, TreeNodeDTO parent) {
		this.content = content;
		setText(text);
		setPath(path);
		setLeaf(leaf);
		setEmpty(empty);
		setParent(parent);
	}

	public TreeNodeDTO(AbstractDTO<?> content) {
		this(content,content.getDisplayText(),(content.getDisplayText()+"/"),false,true,null);
	}
	
	public TreeNodeDTO(AbstractDTO<?> content, TreeNodeDTO parent) {
		this(content,content.getDisplayText(),"",false,true,parent);
		if (parent != null) {
			setPath(parent.getPath()+content.getDisplayText()+"/");
			parent.setEmpty(false);
			parent.setLeaf(false);
		}
	}
	
	public AbstractDTO<?> getContent() {
		return content;
	}
	public void setContent(AbstractDTO<?> content) {
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
	
	public TreeNodeDTO getParent() {
		return parent;
	}
	public void setParent(TreeNodeDTO parent) {
		this.parent = parent;
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
		TreeNodeDTO other = (TreeNodeDTO) obj;
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

	@Override
	public int compareTo(TreeNodeDTO o) {
		return getText().compareTo(o.getText());
	}
}