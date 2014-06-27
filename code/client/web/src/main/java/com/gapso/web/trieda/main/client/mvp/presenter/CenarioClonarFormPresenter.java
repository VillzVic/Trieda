package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.CenarioEditarFormPresenter.Display;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CenariosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CenarioClonarFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		CheckBox getOficialCheckBox();
		TextField< String > getNomeTextField();
		NumberField getAnoTextField();
		NumberField getSemestreTextField();
		TextField< String > getComentarioTextField();
		CenarioDTO getCenarioDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
		CheckBox getClonarSolucaoCheckBox();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< CenarioDTO > gridPanel;
	private Display display;

	public CenarioClonarFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, SimpleGrid< CenarioDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if ( isValid() )
				{
					final CenariosServiceAsync service = Services.cenarios();
					new AcompanhamentoPanelPresenter("chaveClonarCenario", new AcompanhamentoPanelView());
					service.clonar( display.getCenarioDTO(), getClone(), display.getClonarSolucaoCheckBox().getValue(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!",
								"Deu falha na conexão", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							gridPanel.updateList();
							Info.display( "Salvo", "Item salvo com sucesso!" );
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!",
						"Verifique os campos digitados", null );
				}
			}
		});
	}
	
	private CenarioDTO getClone() {
		CenarioDTO clone = new CenarioDTO();
		clone.setNome(display.getNomeTextField().getValue());
		clone.setOficial(display.getOficialCheckBox().getValue());
		clone.setAno(display.getAnoTextField().getValue().intValue());
		clone.setSemestre(display.getSemestreTextField().getValue().intValue());
		clone.setComentario(display.getComentarioTextField().getValue());
		
		return clone;
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
