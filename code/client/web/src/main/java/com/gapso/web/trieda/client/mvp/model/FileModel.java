package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class FileModel extends BaseModel {

	private static final long serialVersionUID = 7303710319701186022L;

	public FileModel() {
		set("folha", false);
	}

	public FileModel(String name) {
		this(name, name, false);
	}
	public FileModel(String name, Boolean folha) {
		this(name, name, folha);
	}
	public FileModel(String name, String path) {
		this(name, path, false);
	}
	public FileModel(String name, String path, Boolean folha) {
		setName(name);
		setPath(path);
		setFolha(folha);
	}

	public void setName(String name) {
		set("name", name);
	}
	public String getName() {
		return get("name");
	}

	public void setPath(String path) {
		set("path", path);
	}
	public String getPath() {
		return (String) ((get("path") != null) ? get("path") : get("name"));
	}

	public void setFolha(Boolean folha) {
		set("folha", folha);
	}
	public Boolean getFolha() {
		return get("folha");
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((getName() == null) ? 0 : getName().hashCode());
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
		FileModel other = (FileModel) obj;
		if (getName() == null) {
			if (other.getName() != null)
				return false;
		} else if (!getName().equals(other.getName()))
			return false;
		if (getPath() == null) {
			if (other.getPath() != null)
				return false;
		} else if (!getPath().equals(other.getPath()))
			return false;
		return true;
	}
}
