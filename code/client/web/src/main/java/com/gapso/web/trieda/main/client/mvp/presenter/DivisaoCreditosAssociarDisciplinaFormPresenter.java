package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DivisoesCreditosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DivisaoCreditosAssociarDisciplinaFormPresenter	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		DisciplinaAutoCompleteBox getDisciplinaComboBox();
		NumberField getDia1NumberField();
		NumberField getDia2NumberField();
		NumberField getDia3NumberField();
		NumberField getDia4NumberField();
		NumberField getDia5NumberField();
		NumberField getDia6NumberField();
		NumberField getDia7NumberField();
		DivisaoCreditoDTO getDivisaoCreditoDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private SimpleGrid< DivisaoCreditoDTO > gridPanel;
	private Display display;

	public DivisaoCreditosAssociarDisciplinaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< DivisaoCreditoDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;

		this.setListeners();
	}

	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if( isValid() )
				{
					final DivisoesCreditosServiceAsync service = Services.divisaoCreditos();

					service.saveWithDisciplina( getDTO(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							if (caught instanceof TriedaException)
								MessageBox.alert("ERRO!", caught.getMessage(), null);
							else
								MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							gridPanel.updateList();

							Info.display( "Salvo",
								"Item salvo com sucesso!" );
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!", "Verifique os campos digitados<br />"
						+ "A soma dos créditos deve ser igual ao total de créditos da disciplina" , null );
				}
			}
		});
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}
	
	private DivisaoCreditoDTO getDTO()
	{
		DivisaoCreditoDTO divisaoCreditoDTO
			= this.display.getDivisaoCreditoDTO();

		divisaoCreditoDTO.setInstituicaoEnsinoId(
			this.instituicaoEnsinoDTO.getId() );
		divisaoCreditoDTO.setDisciplinaId( display.getDisciplinaComboBox().getValue().getId() );

		Number value1 = this.display.getDia1NumberField().getValue();
		int dia1 = ( value1 == null ? 0 : value1.intValue() );

		Number value2 = this.display.getDia2NumberField().getValue();
		int dia2 = ( value2 == null ? 0 : value2.intValue() );

		Number value3 = this.display.getDia3NumberField().getValue();
		int dia3 = ( value3 == null ? 0 : value3.intValue() );

		Number value4 = this.display.getDia4NumberField().getValue();
		int dia4 = ( value4 == null ? 0 : value4.intValue() );

		Number value5 = this.display.getDia5NumberField().getValue();
		int dia5 = ( value5 == null ? 0 : value5.intValue() );

		Number value6 = this.display.getDia6NumberField().getValue();
		int dia6 = ( value6 == null ? 0 : value6.intValue() );

		Number value7 = this.display.getDia7NumberField().getValue();
		int dia7 = ( value7 == null ? 0 : value7.intValue() );

		divisaoCreditoDTO.setDia1( dia1 );
		divisaoCreditoDTO.setDia2( dia2 );
		divisaoCreditoDTO.setDia3( dia3 );
		divisaoCreditoDTO.setDia4( dia4 );
		divisaoCreditoDTO.setDia5( dia5 );
		divisaoCreditoDTO.setDia6( dia6 );
		divisaoCreditoDTO.setDia7( dia7 );

		return divisaoCreditoDTO;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
