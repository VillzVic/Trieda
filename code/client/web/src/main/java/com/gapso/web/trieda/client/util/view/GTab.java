package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;

public class GTab extends TabPanel {
	
	public GTab() {
	}

	@Override
	public boolean add(TabItem item) {
		return add((GTabItem) item);
	}

	public boolean add(GTabItem item) {
		boolean flag = true;
		if(!containsTabItem(item, true)) {
			flag = super.add(item);
			setSelection(item);
		}
		return flag;
	}
	
	public boolean containsTabItem(GTabItem item) {
		return containsTabItem(item, false);
	}
	public boolean containsTabItem(GTabItem item, boolean open) {
		if(item.getIdTabItem() == null) return false;
		for(TabItem tabItem : getItems()) {
			GTabItem gTabItem = (GTabItem) tabItem;
			if(gTabItem.getIdTabItem() == null) continue;
			if(gTabItem.getIdTabItem().equals(item.getIdTabItem())) {
				if(open) setSelection(gTabItem);
				return true;
			}
		}
		return false;
	}
}
