package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class CampusFormPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		TextField< String > getCodigoTextField();
		NumberField getValorCreditoNumberField();
		EstadoComboBox getEstadoComboBox();
		TextField< String > getMunicipioTextField();
		TextField< String > getBairroTextField();
		TextField< String > getQtdProfessorVirtualTextField();
		CheckBox getPublicadoCheckBox();
		CampusDTO getCampusDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private SimpleGrid< CampusDTO > gridPanel;
	private Display display;

	public CampusFormPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this( instituicaoEnsinoDTO, cenario, display, null );
	}

	public CampusFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,CenarioDTO cenario,
		Display display, SimpleGrid< CampusDTO > gridPanel )
	{
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

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
					final CampiServiceAsync service = Services.campi();
					final SemanasLetivaServiceAsync serviceSL = Services.semanasLetiva();
					
					serviceSL.getAllHorariosDisponiveisCenario(cenario, 
							new AbstractAsyncCallbackWithDefaultOnFailure<PagingLoadResult<HorarioDisponivelCenarioDTO>>( display )
					{
						@Override
						public void onSuccess( PagingLoadResult<HorarioDisponivelCenarioDTO> result )
						{
							if (result.getData().isEmpty())
							{
								MessageBox.confirm("Aviso!", "Não existem horarios cadastrados nas semanas letivas," +
										" deseja criar o campus sem disponibilidades?", new Listener<MessageBoxEvent>() {
									@Override
									public void handleEvent(MessageBoxEvent be) {
										if(be.getButtonClicked().getHtml().equalsIgnoreCase("yes") ||
												be.getButtonClicked().getHtml().equalsIgnoreCase("sim")) {
											service.save( getDTO(),
												new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
											{
												@Override
												public void onSuccess( Void result )
												{
													display.getSimpleModal().hide();
	
													if ( gridPanel != null )
													{
														gridPanel.updateList();
													}
	
													Info.display( "Salvo", "Campus salvo com sucesso!" );
												}
											});
										}
									}
								});
							}
							else
							{
							service.save( getDTO(),
									new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
								{
									@Override
									public void onSuccess( Void result )
									{
										display.getSimpleModal().hide();

										if ( gridPanel != null )
										{
											gridPanel.updateList();
										}

										Info.display( "Salvo", "Campus salvo com sucesso!" );
									}
								});
							}
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
		return this.display.isValid();
	}

	private CampusDTO getDTO()
	{
		CampusDTO campusDTO = this.display.getCampusDTO();

		campusDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		campusDTO.setCenarioId( this.cenario.getId() );
		campusDTO.setNome( this.display.getNomeTextField().getValue() );
		campusDTO.setCodigo(display.getCodigoTextField().getValue() );
		campusDTO.setValorCredito( new TriedaCurrency(
			this.display.getValorCreditoNumberField().getValue().doubleValue() ) );
		campusDTO.setEstado( this.display.getEstadoComboBox().getValue().getValue().name() );
		campusDTO.setMunicipio( this.display.getMunicipioTextField().getValue() );
		campusDTO.setBairro( this.display.getBairroTextField().getValue() );
		campusDTO.setPublicado( this.display.getPublicadoCheckBox().getValue() );
		campusDTO.setQtdProfessorVirtual( this.display.getQtdProfessorVirtualTextField().getValue() );

		return campusDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
