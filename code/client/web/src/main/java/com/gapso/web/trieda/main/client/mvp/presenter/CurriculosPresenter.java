package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.CurriculoDisciplinasView;
import com.gapso.web.trieda.main.client.mvp.view.CurriculoFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.CursosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CurriculosPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getAssociarDisciplinasButton();
		CursoComboBox getCursoBuscaComboBox();
		TextField< String > getCodigoBuscaTextField();
		TextField< String > getDescricaoBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< CurriculoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< CurriculoDTO > > proxy );
	}

	private CenarioDTO cenario;
	private Display display; 
	private GTab gTab;

	public CurriculosPresenter( CenarioDTO cenario, Display display )
	{
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final CurriculosServiceAsync service = Services.curriculos();

		RpcProxy< PagingLoadResult< CurriculoDTO > > proxy = new RpcProxy<PagingLoadResult<CurriculoDTO>>()
		{
			@Override
			public void load( Object loadConfig, AsyncCallback< PagingLoadResult< CurriculoDTO > > callback )
			{
				String codigo = display.getCodigoBuscaTextField().getValue();
				String descricao = display.getDescricaoBuscaTextField().getValue();
				CursoDTO cursoDTO = display.getCursoBuscaComboBox().getValue();

				service.getBuscaList( cursoDTO, codigo, descricao, (PagingLoadConfig) loadConfig, callback );
			}
		};

		display.setProxy( proxy );
	}
	
	private void setListeners()
	{
		display.getNewButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					Presenter presenter = new CurriculoFormPresenter( cenario,
						new CurriculoFormView( new CurriculoDTO(), null, cenario ), display.getGrid() );

					presenter.go( null );
				}
		});

		display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final CurriculoDTO curriculoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
					final CursosServiceAsync campus = Services.cursos();

					campus.getCurso( curriculoDTO.getCursoId(), new AsyncCallback< CursoDTO >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							caught.printStackTrace();
						}

						@Override
						public void onSuccess( CursoDTO cursoDTO )
						{
							Presenter presenter = new CurriculoFormPresenter( cenario,
								new CurriculoFormView( curriculoDTO, cursoDTO, cenario ), display.getGrid() );

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
					List< CurriculoDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
					final CurriculosServiceAsync service = Services.curriculos();

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

		display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ImportExcelFormView importExcelFormView = new ImportExcelFormView(
						ExcelInformationType.CURRICULOS,display.getGrid() );

					importExcelFormView.show();
				}
		});

		display.getExportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						ExcelInformationType.CURRICULOS,display.getI18nConstants(),
						display.getI18nMessages() );

					e.submit();
				}
		});

		display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					display.getCodigoBuscaTextField().setValue( null );
					display.getDescricaoBuscaTextField().setValue( null );
					display.getCursoBuscaComboBox().setValue( null );
					display.getGrid().updateList();
				}
		});

		display.getSubmitBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected(ButtonEvent ce )
				{
					display.getGrid().updateList();
				}
		});

		display.getAssociarDisciplinasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					CurriculoDTO curriculoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
					Presenter presenter = new CurriculoDisciplinasPresenter( new CurriculoDisciplinasView( curriculoDTO ) );
					presenter.go( gTab );
				}
		});
	}

	@Override
	public void go( Widget widget )
	{
		gTab = (GTab)widget;
		gTab.add( (GTabItem)display.getComponent() );
	}
}
