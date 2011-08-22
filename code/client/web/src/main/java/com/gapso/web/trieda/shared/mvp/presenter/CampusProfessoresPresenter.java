package com.gapso.web.trieda.shared.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.view.CampusProfessorFormView;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CampusProfessoresPresenter
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
		CampusComboBox getCampusBuscaComboBox();
		ProfessorComboBox getProfessorBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< ProfessorCampusDTO > getGrid();
		Component getComponent();
	}

	private CenarioDTO cenario;
	private UsuarioDTO usuario;
	private Display display; 

	public CampusProfessoresPresenter( CenarioDTO cenario,
		UsuarioDTO usuario, Display display )
	{
		this.cenario = cenario;
		this.usuario = usuario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		RpcProxy< PagingLoadResult< ProfessorCampusDTO > > proxy
			= new RpcProxy< PagingLoadResult< ProfessorCampusDTO > >()
		{
			@Override
			public void load( Object loadConfig, AsyncCallback< PagingLoadResult< ProfessorCampusDTO > > callback )
			{
				if ( usuario.isAdministrador() )
				{
					CampusDTO campus = display.getCampusBuscaComboBox().getValue();
					ProfessorDTO professor = display.getProfessorBuscaComboBox().getValue();
					Services.professores().getProfessorCampusList( campus, professor, callback );
				}
				else
				{
					Services.professores().getProfessorCampusByCurrentProfessor( callback );
				}
			}
		};

		display.getGrid().setProxy( proxy );
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
					Presenter presenter = new CampusProfessorFormPresenter(
						cenario, new CampusProfessorFormView( null ), display.getGrid() );

					presenter.go( null );
				}
			});

			display.getEditButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ProfessorCampusDTO pcDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
					CampusDTO campusDTO = new CampusDTO();

					campusDTO.setId( pcDTO.getCampusId() );
					campusDTO.setCodigo( pcDTO.getCampusString() );

					Presenter presenter = new CampusProfessorFormPresenter(
						cenario, new CampusProfessorFormView( campusDTO ), display.getGrid() );
					presenter.go( null );
				}
			});

			display.getRemoveButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					List< ProfessorCampusDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
					Services.professores().removeProfessorCampus( list, new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
					{
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
					display.getCampusBuscaComboBox().setValue( null );
					display.getProfessorBuscaComboBox().setValue( null );
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

			display.getImportExcelButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ImportExcelFormView importExcelFormView = new ImportExcelFormView(
						ExcelInformationType.CAMPI_TRABALHO,display.getGrid() );

					importExcelFormView.show();
				}
			});

			display.getExportExcelButton().addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ExportExcelFormSubmit e = new ExportExcelFormSubmit( ExcelInformationType.CAMPI_TRABALHO,
						display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
				}
			});
		}
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem)display.getComponent() );
	}
}
