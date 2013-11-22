package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.ProfessorFormView;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.HorarioDisponivelProfessorFormPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.view.HorarioDisponivelProfessorFormView;
import com.gapso.web.trieda.shared.services.AreasTitulacaoServiceAsync;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public abstract class ProfessoresListPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getEditButton();
		Button getRemoveButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		TextField< String > getCpfBuscaTextField();
		TipoContratoComboBox getTipoContratoBuscaComboBox();
		TitulacaoComboBox getTitulacaoBuscaComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		Button getDisponibilidadeButton();
		SimpleGrid< ProfessorDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< ProfessorDTO > > proxy );
		RelatorioProfessorFiltro getProfessorFiltro();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	protected CenarioDTO cenario;
	protected Long campusDTO;
	protected Display display;
	private GTab gTab;

	public ProfessoresListPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Long campusDTO, Display display )
	{
		this.instituicaoEnsinoDTO  = instituicaoEnsinoDTO;
		this.campusDTO = campusDTO;
		this.cenario = cenarioDTO;
		this.display = display;

		configureProxy();
		setListeners();
	}

	protected abstract void configureProxy();

	protected void setListeners()
	{
		display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDTO professorDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final ProfessoresServiceAsync professoresService = Services.professores();
				final AreasTitulacaoServiceAsync areasTitulacaoService = Services.areasTitulacao();

				final FutureResult< TipoContratoDTO > futureTipoContratoDTO = new FutureResult< TipoContratoDTO >();
				final FutureResult< TitulacaoDTO > futureTitulacaoDTO = new FutureResult< TitulacaoDTO >();
				final FutureResult< AreaTitulacaoDTO > futureAreaTitulacaoDTO = new FutureResult< AreaTitulacaoDTO >();

				professoresService.getTipoContrato(
					professorDTO.getTipoContratoId(), futureTipoContratoDTO );

				professoresService.getTitulacao(
					professorDTO.getTitulacaoId(), futureTitulacaoDTO );

				areasTitulacaoService.getAreaTitulacao(
					professorDTO.getAreaTitulacaoId(), futureAreaTitulacaoDTO );

				FutureSynchronizer synch = new FutureSynchronizer(
					futureTipoContratoDTO, futureTitulacaoDTO, futureAreaTitulacaoDTO );

				synch.addCallback( new AsyncCallback< Boolean >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
					}

					@Override
					public void onSuccess( Boolean result )
					{
						TipoContratoDTO tipoContratoDTO = futureTipoContratoDTO.result();
						TitulacaoDTO titulacaoDTO = futureTitulacaoDTO.result();
						AreaTitulacaoDTO areaTitulacaoDTO = futureAreaTitulacaoDTO.result();

						Presenter presenter = new ProfessorFormPresenter(
							instituicaoEnsinoDTO, cenario,
							new ProfessorFormView( professorDTO, tipoContratoDTO,
							titulacaoDTO, areaTitulacaoDTO, cenario ), display.getGrid() );

						presenter.go( null );
					}
				});
			}
		});

		display.getDisponibilidadeButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDTO professorDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Services.professores().getHorariosDisponiveis( professorDTO,
					new AsyncCallback< List< HorarioDisponivelCenarioDTO > >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi posssível exibir as tela de disponiblidade", null );
					}

					@Override
					public void onSuccess(
						List< HorarioDisponivelCenarioDTO > result )
					{
						Presenter presenter = new HorarioDisponivelProfessorFormPresenter(
							instituicaoEnsinoDTO, cenario, 
							new HorarioDisponivelProfessorFormView( professorDTO, result ) );

						presenter.go( null );
					}
				});
			}
		});

		display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessoresServiceAsync service = Services.professores();

				List< ProfessorDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover o(s) professor(es)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();

						Info.display( "Removido", "Item removido com sucesso!" );
					}
				});
			}
		});

		display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getCpfBuscaTextField().setValue( null );
				display.getTipoContratoBuscaComboBox().setValue( null );
				display.getTitulacaoBuscaComboBox().setValue( null );
				display.getAreaTitulacaoBuscaComboBox().setValue( null );
				display.getGrid().updateList();
			}
		});

		display.getSubmitBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getGrid().updateList();
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
