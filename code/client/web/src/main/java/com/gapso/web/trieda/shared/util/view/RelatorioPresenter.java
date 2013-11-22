package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.ui.Widget;

public abstract class RelatorioPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		void setStore( TreeStore< RelatorioDTO > store );
		TreeGrid< RelatorioDTO > getTree();  
		Component getComponent();
		TreeStore< RelatorioDTO > getStore();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
	}
	
	protected InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	protected Display display;
	protected CenarioDTO cenarioDTO;
	protected GTab gTab;
	
	public RelatorioPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		this.display = display;
	}
	
	
	public Display getDisplay(){
		return this.display;
	}

	@Override
	public void go( Widget widget )
	{
		gTab = (GTab)widget;
		gTab.add( (GTabItem) this.display.getComponent() );
	}
}
