package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosDemandaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ResumoMatriculasPresenter 
	implements Presenter{

	public interface Display
		extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		TextField< String > getAlunoBuscaTextField();
		TextField< String > getMatriculaBuscaTextField();
		CampusComboBox getCampusBuscaComboBox();
		CursoComboBox getCursoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();		
		SimpleGrid< ResumoMatriculaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< ResumoMatriculaDTO > > proxy );
	}
	
	private Display display; 
	
	public ResumoMatriculasPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenario, Display display )
	{
			this.display = display;

			configureProxy();
			setListeners();
	}
	
	private void configureProxy()
	{
		final AlunosDemandaServiceAsync service = Services.alunosDemanda();

		RpcProxy< PagingLoadResult< ResumoMatriculaDTO > > proxy =
			new RpcProxy< PagingLoadResult< ResumoMatriculaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< ResumoMatriculaDTO > > callback )
			{
				String aluno = display.getAlunoBuscaTextField().getValue();
				String matricula = display.getMatriculaBuscaTextField().getValue();

				CampusDTO campusDTO
					= display.getCampusBuscaComboBox().getValue();
				
				CursoDTO cursoDTO
					= display.getCursoBuscaComboBox().getValue();

				service.getResumoList( aluno, matricula, campusDTO,
					cursoDTO, (PagingLoadConfig) loadConfig, callback );
			}
		};

		this.display.setProxy( proxy );
	}
	
	private void setListeners()
	{
		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getAlunoBuscaTextField().setValue( null );
				display.getMatriculaBuscaTextField().setValue( null );
				display.getCampusBuscaComboBox().setValue( null );
				display.getCursoBuscaComboBox().setValue( null );
				
				display.getGrid().updateList();
			}
		});
		
		this.display.getSubmitBuscaButton().addSelectionListener(
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
	public void go(Widget widget)
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}

}
