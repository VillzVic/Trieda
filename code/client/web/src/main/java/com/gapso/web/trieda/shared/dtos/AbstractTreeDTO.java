package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseTreeModel;

public abstract class AbstractTreeDTO<NKType> extends BaseTreeModel {

	private static final long serialVersionUID = 1173587744183607571L;
	
	// Propriedades
	public static final String PROPERTY_DISPLAY_TEXT = "displayText";
	
	public abstract NKType getNaturalKey();
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((getNaturalKey() == null) ? 0 : getNaturalKey().hashCode());
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
		AbstractTreeDTO<NKType> other = (AbstractTreeDTO<NKType>) obj;
		if (getNaturalKey() == null) {
			if (other.getNaturalKey() != null)
				return false;
		} else if (!getNaturalKey().equals(other.getNaturalKey()))
			return false;
		return true;
	}
	
}