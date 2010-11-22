package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class FileModel extends BaseModel {

	private static final long serialVersionUID = 7303710319701186022L;

	public FileModel() {
		set("folha", false);
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
	public boolean equals(Object obj) {
		if (obj != null && obj instanceof FileModel) {
			FileModel mobj = (FileModel) obj;
			if (mobj.getName() != null && getName() != null) {
				return getName().equals(mobj.getName())
						&& getPath().equals(mobj.getPath());
			}
		}
		return super.equals(obj);
	}

}
