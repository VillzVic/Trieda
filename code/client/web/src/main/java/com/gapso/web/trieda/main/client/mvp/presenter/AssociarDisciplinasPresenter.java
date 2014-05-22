package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.BaseEvent;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AssociarDisciplinasPresenter implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		CheckBox getSalaCheckBox();
		CheckBox getLaboratorioCheckBox();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	private SimpleGrid< SalaDTO > gridPanel;
	private Display display;
	private String errorMessage;

	public AssociarDisciplinasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO,
		Display display, SimpleGrid< SalaDTO > gridPanel )
	{
		this.gridPanel = gridPanel;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		this.errorMessage = "";

		setListeners();
	}

	private void setListeners()
	{
		
		display.getSalvarButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
				{
					@Override
					public void componentSelected( ButtonEvent ce )
					{
						final SalasServiceAsync service = Services.salas();
						
						service.associarDisciplinas(cenarioDTO, 
								display.getSalaCheckBox().getValue(), 
								display.getLaboratorioCheckBox().getValue(), new AsyncCallback< Void >()
								{
									@Override
									public void onFailure( Throwable caught )
									{
										MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
									}

									@Override
									public void onSuccess( Void result )
									{
										//display.getGrid().updateList();

										Info.display( "Sucesso", "Associação das discipilnas à ambientes foi realizada com sucesso!" );
									}
								});
						
					}
				});
	}


	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
