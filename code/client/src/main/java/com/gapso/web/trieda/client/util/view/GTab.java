package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;

public class GTab extends TabPanel {
	
	public GTab() {
	}

	@Override
	public boolean add(TabItem item) {
		boolean flag = super.add(item);
		if(flag) {
			setSelection(item);
		}
		return flag;
	}
	
}
