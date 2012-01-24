package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.IconButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class DisciplinasAssociarSalaPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		UnidadeComboBox getUnidadeSalaComboBox();
		UnidadeComboBox getUnidadeGrupoSalaComboBox();
		SimpleComboBox< String> getAndarComboBox();
		TreePanel< TreeNodeDTO > getDisciplinasList();
		TreePanel< TreeNodeDTO > getSalasList();
		ToolButton getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		void setTabEnabled( boolean flag );
		Component getComponent();
		Button getAssociarDisciplinasPraticasLaboratoriosBT();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private Map< String, List< SalaDTO > > andaresSalasMap;

	public DisciplinasAssociarSalaPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		this.display.getCampusComboBox().addSelectionChangedListener(
			new SelectionChangedListener< CampusDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< CampusDTO > se )
			{
				// Obtém o campo selecionado pelo usuário na combobox
				final CampusDTO selectedCampus = se.getSelectedItem();

				// Verifica se há turnos associados ao campus
				// selecionado ( a associação entre turnos e campus
				// ocorre através do cadastro de uma oferta )
				Services.turnos().getListByCampus( selectedCampus,
					new AbstractAsyncCallbackWithDefaultOnFailure< ListLoadResult< TurnoDTO > >( display )
				{
					@Override
					public void onSuccess( ListLoadResult< TurnoDTO > result )
					{
						// Exibe uma msg de alerta caso não haja
						// turnos associados ao campus selecionado
						if ( result.getData().isEmpty() )
						{
							MessageBox.alert( display.getI18nConstants().mensagemAlerta(),
								display.getI18nMessages().ofertasNaoCadastradas( selectedCampus.getNome() ), null );
						}
					}
				});
			}
		});

		this.display.getTurnoComboBox().addSelectionChangedListener(
			new SelectionChangedListener< TurnoDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< TurnoDTO > se )
			{
				display.getDisciplinasList().mask(
					display.getI18nMessages().loading(), "loading" );

				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				TurnoDTO turnoDTO = se.getSelectedItem();

				final FutureResult< ListLoadResult< TreeNodeDTO > > futureOfertaDTOList
					= new FutureResult< ListLoadResult< TreeNodeDTO > >();

				Services.ofertas().getListByCampusAndTurno(
					campusDTO, turnoDTO, futureOfertaDTOList );

				FutureSynchronizer synch = new FutureSynchronizer( futureOfertaDTOList );
				synch.addCallback( new AsyncCallback< Boolean >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						display.getTurnoComboBox().disable();
						caught.printStackTrace();
						display.getDisciplinasList().unmask();
					}

					@Override
					public void onSuccess( Boolean result )
					{
						ListLoadResult< TreeNodeDTO > ofertaDTOList
							= futureOfertaDTOList.result();

						TreeStore< TreeNodeDTO > treeStore
							= display.getDisciplinasList().getStore();

						treeStore.removeAll();
						treeStore.add( ofertaDTOList.getData(), true );

						boolean existeOferta = !ofertaDTOList.getData().isEmpty();
						
						display.setTabEnabled( existeOferta );
						display.getDisciplinasList().setEnabled( existeOferta );
						display.getDisciplinasList().unmask();
					}
				});
			}
		});

		this.display.getUnidadeSalaComboBox().addSelectionChangedListener(
			new SelectionChangedListener< UnidadeDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< UnidadeDTO > se )
			{
				UnidadeDTO unidadeDTO = se.getSelectedItem();

				if ( unidadeDTO != null )
				{
					SalasServiceAsync salasService = Services.salas();

					salasService.getSalasEAndareMap( unidadeDTO.getId(),
						new AsyncCallback< Map< String, List< SalaDTO > > >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							caught.printStackTrace();
						}

						@Override
						public void onSuccess( Map< String, List< SalaDTO > > result )
						{
							andaresSalasMap = result;
							display.getAndarComboBox().getStore().removeAll();

							List< String > andares = new ArrayList< String >();
							andares.addAll( result.keySet() );
							display.getAndarComboBox().add( andares );
							display.getAndarComboBox().setEnabled( !andares.isEmpty() );
						}
					});
				}
			}
		});

		this.display.getAndarComboBox().addSelectionChangedListener(
			new SelectionChangedListener< SimpleComboValue< String > >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< SimpleComboValue< String > > se )
			{
				display.getSalasList().mask(
					display.getI18nMessages().loading(), "loading" );

				display.getSalasList().setEnabled( se.getSelectedItem() != null );

				if ( se.getSelectedItem() != null )
				{
					display.getSalasList().getStore().removeAll();

					List< SalaDTO > salaDTOList = andaresSalasMap.get(
						se.getSelectedItem().getValue() );

					List< TreeNodeDTO > treeNodesList
						= new ArrayList< TreeNodeDTO >( salaDTOList.size() );

					for ( SalaDTO salaDTO : salaDTOList )
					{
						TreeNodeDTO nodeDTO = new TreeNodeDTO( salaDTO );

						nodeDTO.setEmpty( !salaDTO .getContainsCurriculoDisciplina() );
						treeNodesList.add( nodeDTO );
					}

					Collections.sort( treeNodesList );

					display.getSalasList().getStore().add( treeNodesList, true );
					display.getSalasList().unmask();
				}
			}
		});

		this.display.getUnidadeGrupoSalaComboBox().addSelectionChangedListener(
			new SelectionChangedListener< UnidadeDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< UnidadeDTO > se )
			{
				display.getSalasList().mask(
					display.getI18nMessages().loading(), "loading" );

				UnidadeDTO unidadeDTO = se.getSelectedItem();

				if ( unidadeDTO != null )
				{
					SalasServiceAsync salasService = Services.salas();

					salasService.getGruposDeSalas( unidadeDTO.getId(),
						new AsyncCallback< List< GrupoSalaDTO > >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							caught.printStackTrace();
							display.getSalasList().unmask();
						}

						@Override
						public void onSuccess( List< GrupoSalaDTO > result )
						{
							display.getSalasList().enable();
							display.getSalasList().getStore().removeAll();

							for ( GrupoSalaDTO grupoSalaDTO : result )
							{
								TreeNodeDTO nodeDTO = new TreeNodeDTO( grupoSalaDTO );

								display.getSalasList().getStore().add( nodeDTO, true );
								display.getSalasList().unmask();
							}
						}
					});
				}
			}
		});

		this.display.getRemoveButton().addListener( Events.Select,
			new SelectionListener< IconButtonEvent >()
		{
			@Override
			public void componentSelected( IconButtonEvent ce )
			{
				TreeNodeDTO selectedNodeDTO
					= display.getSalasList().getSelectionModel().getSelectedItem();

				TreeNodeDTO cdTreeNodeDTO = null;
				CurriculoDisciplinaDTO cdDTO = null;
				SalaDTO salaDTO = null;
				GrupoSalaDTO grupoSalaDTO = null;

				if ( selectedNodeDTO.getLeaf() )
				{
					cdTreeNodeDTO = selectedNodeDTO;
					cdDTO = (CurriculoDisciplinaDTO) selectedNodeDTO.getContent();

					AbstractDTO< ? > nodeContent = selectedNodeDTO.getParent()
						.getParent().getParent().getContent();

					if ( nodeContent instanceof SalaDTO )
					{
						salaDTO = (SalaDTO) nodeContent;
					}
					else
					{
						grupoSalaDTO = (GrupoSalaDTO) nodeContent;
					}
				}
				else
				{
					Info.display( "Erro", "Selecione uma disciplina!" );
				}

				final TreeNodeDTO cdTreeNodeDTORemove = cdTreeNodeDTO;

				if ( cdDTO != null )
				{
					DisciplinasServiceAsync salasService = Services.disciplinas();

					if ( salaDTO != null )
					{
						salasService.removeDisciplinaToSala( salaDTO, cdDTO,
							new AsyncCallback< Void >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								caught.printStackTrace();
							}

							@Override
							public void onSuccess( Void result )
							{
								display.getSalasList().getStore().remove( cdTreeNodeDTORemove );

								Info.display( "Removido",
									"Disciplinas removidas com sucesso!" );
							}
						});
					}
					else
					{
						salasService.removeDisciplinaToSala( grupoSalaDTO, cdDTO,
							new AsyncCallback< Void >()
						{
							@Override
							public void onFailure( Throwable caught )
							{
								caught.printStackTrace();
							}

							@Override
							public void onSuccess( Void result )
							{
								display.getSalasList().getStore().remove( cdTreeNodeDTORemove );

								Info.display( "Removido",
									"Disciplinas removidas com sucesso!" );
							}
						});
					}
				}
			}
		});

		this.display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
				public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DISCIPLINAS_SALAS, instituicaoEnsinoDTO );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, null );

				importExcelFormView.show();
			}
		});

		this.display.getExportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DISCIPLINAS_SALAS, instituicaoEnsinoDTO );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
			}
		});
		
		this.display.getAssociarDisciplinasPraticasLaboratoriosBT().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				DisciplinasServiceAsync service = Services.disciplinas();
				service.associarDisciplinasALaboratorios(new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display(display.getI18nConstants().informacao(),"A associação de disciplinas que exigem laboratórios com salas de aulas do tipo laboratório foi efetuada com sucesso!");
					}
				});
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		GTab gTab = (GTab) widget;
		gTab.add( (GTabItem) this.display.getComponent() );
	}
}
