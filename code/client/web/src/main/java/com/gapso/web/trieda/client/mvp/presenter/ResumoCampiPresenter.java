package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class ResumoCampiPresenter implements Presenter {

	public interface Display {
		void setStore(TreeStore<FileModel> store);
		TreePanel<FileModel> getTree();  
		Component getComponent();
	}
//	private CenarioDTO cenario;
	private Display display; 
	
	public ResumoCampiPresenter(CenarioDTO cenario, Display display) {
//		this.cenario = cenario;
		this.display = display;
	}

	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}