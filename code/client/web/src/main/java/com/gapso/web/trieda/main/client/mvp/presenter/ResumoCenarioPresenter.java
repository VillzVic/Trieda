package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class ResumoCenarioPresenter
	implements Presenter
{
	public interface Display
	{
		void setStore( TreeStore< TreeNodeDTO > store );
		TreePanel< TreeNodeDTO > getTree();
		Component getComponent();
	}

	@SuppressWarnings( "unused" )
	private CenarioDTO cenario;
	private Display display; 

	public ResumoCenarioPresenter(
		CenarioDTO cenario, Display display )
	{
		this.cenario = cenario;
		this.display = display;
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}

}
