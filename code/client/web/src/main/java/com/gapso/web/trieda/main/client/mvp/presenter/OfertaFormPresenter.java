package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.OfertasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class OfertaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TurnoComboBox getTurnoComboBox();
		CampusComboBox getCampusComboBox();
		CurriculoComboBox getCurriculoComboBox();
		OfertaDTO getOfertaDTO();
		NumberField getReceitaNumberField();
		boolean isValid();

		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< OfertaDTO > gridPanel;
	private Display display;

	public OfertaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, SimpleGrid< OfertaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected(ButtonEvent ce)
				{
					if ( isValid() )
					{
						final OfertasServiceAsync service = Services.ofertas();

						service.save( getDTO(), new AsyncCallback< Void >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
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
						MessageBox.alert( "ERRO!", "Verifique os campos digitados", null );
					}
				}
		});
	}

	private boolean isValid()
	{
		return display.isValid();
	}

	private OfertaDTO getDTO()
	{
		OfertaDTO ofertaDTO = display.getOfertaDTO();

		ofertaDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		ofertaDTO.setTurnoId( display.getTurnoComboBox().getSelection().get( 0 ).getId() );
		ofertaDTO.setTurnoString( display.getTurnoComboBox().getSelection().get( 0 ).getNome() );
		ofertaDTO.setCampusId( display.getCampusComboBox().getSelection().get( 0 ).getId() );
		ofertaDTO.setCampusString( display.getCampusComboBox().getSelection().get( 0 ).getCodigo() );
		ofertaDTO.setMatrizCurricularId( display.getCurriculoComboBox().getSelection().get( 0 ).getId() );
		ofertaDTO.setMatrizCurricularString( display.getCurriculoComboBox().getSelection().get( 0 ).getCodigo() );
		ofertaDTO.setReceita( TriedaUtil.parseTriedaCurrency(
			display.getReceitaNumberField().getValue().doubleValue() ) );

		return ofertaDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
