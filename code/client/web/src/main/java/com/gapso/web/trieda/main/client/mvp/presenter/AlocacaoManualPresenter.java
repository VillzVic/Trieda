package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurmaStatusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleUnpagedGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualPresenter 
	implements Presenter
{

	private Display display;
	
	public interface Display extends ITriedaI18nGateway {
		Component getComponent();
		SimpleUnpagedGrid< TurmaStatusDTO > getGrid();
		void setProxy( RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy );
		DemandaDTO getDemanda();
		Button getTurmasRemoveButton();
		void refreshDemandasPanel(int planejado, int naoPlanejado, int naoAtendido);
	}
	
	private CenarioDTO cenarioDTO;
	
	public AlocacaoManualPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.display = display;
		this.cenarioDTO = cenarioDTO;
		configureProxy();
		setListeners();
	}
	
 	private void setListeners()
 	{
		this.display.getTurmasRemoveButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final List< TurmaStatusDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
					final AtendimentosServiceAsync service = Services.atendimentos();
					service.deleteTurmasStatus(cenarioDTO, display.getDemanda(), list, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível remover a(s) Turma(s)", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getGrid().updateList();
							int planejadas = 0;
							int naoPlanejadas = 0;
							for (TurmaStatusDTO turmas : list)
							{
								if (turmas.getStatus().equals("Planejada"))
									planejadas -= turmas.getQtdeTotal();
								else if (turmas.getStatus().equals("Não Planejada"))
									naoPlanejadas -= turmas.getQtdeTotal();
							}
							display.refreshDemandasPanel(planejadas, naoPlanejadas, 0);
							Info.display( "Removido", "Turma(s) removida(s) com sucesso!" );
						}
					});

				}
			});
	}

	private void configureProxy()
 	{
 		final AtendimentosServiceAsync service = Services.atendimentos();

 		RpcProxy< ListLoadResult< TurmaStatusDTO > > proxy =
 			new RpcProxy< ListLoadResult< TurmaStatusDTO > >()
 		{
 			@Override
 			public void load( Object loadConfig,
 				AsyncCallback< ListLoadResult< TurmaStatusDTO > > callback)
 			{
 				service.getTurmasStatus(cenarioDTO, display.getDemanda(), callback);
 			}
 		};

 		display.setProxy( proxy );
 	}
	
	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
