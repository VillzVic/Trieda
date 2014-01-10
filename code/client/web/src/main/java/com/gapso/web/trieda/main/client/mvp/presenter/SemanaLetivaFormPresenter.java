package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SemanaLetivaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField< String > getCodigoTextField();
		TextField< String > getDescricaoTextField();
		NumberField getTempoAulaNumberField();
		CheckBox getPermiteIntervaloCheckBox();
		SemanaLetivaDTO getSemanaLetivaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< SemanaLetivaDTO > gridPanel;
	private Display display;
	private CenarioDTO cenarioDTO;

	public SemanaLetivaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO,
		Display display, SimpleGrid< SemanaLetivaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;
		this.cenarioDTO = cenarioDTO;

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
					final SemanasLetivaServiceAsync service = Services.semanasLetiva();

					service.save( getDTO(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!",
								"Erro ao salvar a semana letiva", null );
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

	private boolean isValid()
	{
		return this.display.isValid();
	}

	private SemanaLetivaDTO getDTO()
	{
		SemanaLetivaDTO dto = this.display.getSemanaLetivaDTO();

		dto.setCenarioId( this.cenarioDTO.getId() );
		dto.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		dto.setCodigo( this.display.getCodigoTextField().getValue() );
		dto.setDescricao( this.display.getDescricaoTextField().getValue() );
		dto.setTempo( this.display.getTempoAulaNumberField().getValue().intValue() );
		dto.setPermiteIntervaloAula( this.display.getPermiteIntervaloCheckBox().getValue().booleanValue() );

		return dto;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
