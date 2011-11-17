package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class DivisaoCreditoDisciplinaFormPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		Button getClearButton();
		NumberField getDia1NumberField();
		NumberField getDia2NumberField();
		NumberField getDia3NumberField();
		NumberField getDia4NumberField();
		NumberField getDia5NumberField();
		NumberField getDia6NumberField();
		NumberField getDia7NumberField();
		DivisaoCreditoDTO getDivisaoCreditoDTO();
		DisciplinaDTO getDisciplinaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;

	public DivisaoCreditoDisciplinaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
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
					Services.disciplinas().salvarDivisaoCredito( display.getDisciplinaDTO(),
						getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
					{
						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							Info.display( "Salvo", "Item salvo com sucesso!" );
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!", "Verifique os campos digitados<br />"
						+ "A soma dos créditos deve ser igual a "
						+ display.getDisciplinaDTO().getTotalCreditos(), null );
				}
			}
		});

		this.display.getClearButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.disciplinas().removeDivisaoCredito( display.getDisciplinaDTO(),
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getSimpleModal().hide();

						Info.display( "Atualizado",
							"A disciplina não mais possui regra de divisão de créditos." );
					}

					@Override
					public void onFailure( Throwable caught )
					{
						Info.display( "Erro",
							"Nao foi possível remover a divisão de crédito." );
					}
				});
			}
		});
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}
	
	private DivisaoCreditoDTO getDTO()
	{
		DivisaoCreditoDTO divisaoCreditoDTO = this.display.getDivisaoCreditoDTO();

		divisaoCreditoDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		divisaoCreditoDTO.setCenarioId( this.cenario.getId() );

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
