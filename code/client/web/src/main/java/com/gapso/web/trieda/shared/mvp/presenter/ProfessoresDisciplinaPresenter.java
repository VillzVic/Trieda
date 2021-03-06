package com.gapso.web.trieda.shared.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.view.ProfessorDisciplinaFormView;
import com.gapso.web.trieda.shared.mvp.view.ProfessorDisciplinasFormView;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.ProfessoresDisciplinaServiceAsync;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class ProfessoresDisciplinaPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		ProfessorComboBox getProfessorBuscaComboBox();
		DisciplinaAutoCompleteBox getDisciplinaBuscaComboBox();
		TextField< String > getCpfBuscaTextField();
		TextField<String> getNomeBuscaTF();
		NumberField getPreferenciaField();
		OperadorComboBox getPreferenciaOperadorCB();
		NumberField getNotaDesempenhoBuscaField();
		OperadorComboBox getNotaDesempenhoOperadorCB();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< ProfessorDisciplinaDTO > getGrid();
		Component getComponent();
		Button getAssociarMassaButton();
		Button getHabilitarEquivalenciasButton();
		void setProxy( RpcProxy< PagingLoadResult< ProfessorDisciplinaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private GTab gTab;
	private Display display;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor;
	private CenarioDTO cenarioDTO;

	public ProfessoresDisciplinaPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, UsuarioDTO usuario, Display display, boolean isVisaoProfessor )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.usuario = usuario;
		this.isVisaoProfessor = isVisaoProfessor;
		this.cenarioDTO = cenarioDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		RpcProxy< PagingLoadResult< ProfessorDisciplinaDTO > > proxy
			= new RpcProxy< PagingLoadResult< ProfessorDisciplinaDTO > >()
		{
			@Override
			public void load( Object loadConfig, AsyncCallback< PagingLoadResult< ProfessorDisciplinaDTO > > callback )
			{
				String cpf = null;
				String nome = null;
				ProfessorDTO professorDTO = null;
				DisciplinaDTO disciplinaDTO = null;
				String operadorPreferencia = null;
				Integer preferencia = null;
				String operadorNotaDesempenho = null; 
				Integer notaDesempenho = null;

				if ( usuario.isAdministrador() )
				{
					professorDTO = display.getProfessorBuscaComboBox().getValue();
					disciplinaDTO = display.getDisciplinaBuscaComboBox().getValue();
					cpf = display.getCpfBuscaTextField().getValue();
					nome = display.getNomeBuscaTF().getValue();
					operadorPreferencia = (display.getPreferenciaOperadorCB().getValue()==null)?null: display.getPreferenciaOperadorCB().getValue().getValue().getOperadorSQL();
					preferencia = display.getPreferenciaField().getValue() == null?null:display.getPreferenciaField().getValue().intValue();
					operadorNotaDesempenho = (display.getNotaDesempenhoOperadorCB().getValue()==null)?null: display.getNotaDesempenhoOperadorCB().getValue().getValue().getOperadorSQL();
					 notaDesempenho = display.getNotaDesempenhoBuscaField().getValue() == null?null:display.getNotaDesempenhoBuscaField().getValue().intValue();
				}
				else
				{
					professorDTO = new ProfessorDTO();
					professorDTO.setId( usuario.getProfessorId() );
					cpf = null;
					nome = null;
					disciplinaDTO = null;
				}

				Services.professoresDisciplina().getBuscaList(
					cenarioDTO, professorDTO, disciplinaDTO, 
					cpf, nome,  operadorPreferencia, preferencia, operadorNotaDesempenho,notaDesempenho,
					(PagingLoadConfig) loadConfig, callback );
			}
		};

		display.setProxy( proxy );
	}

	private void setListeners()
	{
		if ( usuario.isAdministrador() )
		{
			display.getNewButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new ProfessorDisciplinaFormPresenter( instituicaoEnsinoDTO,
						usuario, new ProfessorDisciplinaFormView( cenarioDTO, usuario,
							new ProfessorDisciplinaDTO(), null, null ), display.getGrid() );

					presenter.go( null );
				}
			});
		}

		display.getEditButton().addSelectionListener( new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDisciplinaDTO professorDisciplinaDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final ProfessoresServiceAsync professoresService = Services.professores();
				final DisciplinasServiceAsync disciplinasService = Services.disciplinas();

				final FutureResult< ProfessorDTO > futureProfessorDTO = new FutureResult< ProfessorDTO >();
				final FutureResult< DisciplinaDTO > futureDisciplinaDTO = new FutureResult< DisciplinaDTO >();

				professoresService.getProfessor( professorDisciplinaDTO.getProfessorId(), futureProfessorDTO );
				disciplinasService.getDisciplina( professorDisciplinaDTO.getDisciplinaId(), futureDisciplinaDTO );

				FutureSynchronizer synch = new FutureSynchronizer( futureProfessorDTO, futureDisciplinaDTO );

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
						ProfessorDTO professorDTO = futureProfessorDTO.result();
						DisciplinaDTO disciplinaDTO = futureDisciplinaDTO.result();

						Presenter presenter = new ProfessorDisciplinaFormPresenter( instituicaoEnsinoDTO,
							usuario, new ProfessorDisciplinaFormView( cenarioDTO, usuario, professorDisciplinaDTO,
								professorDTO, disciplinaDTO), display.getGrid() );

						presenter.go( null );
					}
				});
			}
		});

		if ( usuario.isAdministrador() )
		{
			display.getRemoveButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					List< ProfessorDisciplinaDTO > list
						= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

					final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();

					service.remove( list, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
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

			display.getResetBuscaButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					display.getProfessorBuscaComboBox().setValue( null );
					display.getDisciplinaBuscaComboBox().setValue( null );
					display.getCpfBuscaTextField().setValue( null );
					display.getNomeBuscaTF().setValue( null );
					display.getPreferenciaField().setValue( null );
					display.getPreferenciaOperadorCB().setValue( null );
					display.getNotaDesempenhoBuscaField().setValue( null );
					display.getNotaDesempenhoOperadorCB().setValue( null );
					display.getGrid().updateList();
				}
			});

			display.getSubmitBuscaButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					display.getGrid().updateList();
				}
			});

			display.getExportXlsExcelButton().addSelectionListener( new SelectionListener< MenuEvent >()
			{
				@Override
				public void componentSelected( MenuEvent ce )
				{
					String fileExtension = "xls";
					
					ExcelParametros parametros = new ExcelParametros(
							ExcelInformationType.HABILITACAO_PROFESSORES, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros,	display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});
			
			display.getExportXlsxExcelButton().addSelectionListener( new SelectionListener< MenuEvent >()
			{
				@Override
				public void componentSelected( MenuEvent ce )
				{
					String fileExtension = "xlsx";
					
					ExcelParametros parametros = new ExcelParametros(
							ExcelInformationType.HABILITACAO_PROFESSORES, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros,	display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});

			display.getImportExcelButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.HABILITACAO_PROFESSORES, instituicaoEnsinoDTO, cenarioDTO );

					ImportExcelFormView importExcelFormView
						= new ImportExcelFormView( parametros, display.getGrid() );

					importExcelFormView.show();
				}
			});
			
			display.getAssociarMassaButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new ProfessorDisciplinasFormPresenter( instituicaoEnsinoDTO, cenarioDTO,
						usuario, new ProfessorDisciplinasFormView( cenarioDTO, usuario ), display.getGrid() );

					presenter.go( null );
				}
			});
			
			display.getHabilitarEquivalenciasButton().addSelectionListener(new SelectionListener<ButtonEvent>() {

				@Override
				public void componentSelected(ButtonEvent ce) {
					final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
					
					service.habilitarEquivalenciasProfessores(cenarioDTO, new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Deu falha na conexão", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getGrid().updateList();

							Info.display( "Sucesso", "Habilitação das equivalências foi realizado com sucesso!" );
						}
					});
					
					
				}
				
			});
		}
	}

	public boolean isVisaoProfessor()
	{
		return isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@Override
	public void go(Widget widget)
	{
		this.gTab = (GTab)widget;
		this.gTab.add( (GTabItem) display.getComponent() );
	}
}
