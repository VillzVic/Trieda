package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.PesquisaPorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosDemandaServiceAsync;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AtendimentosPesquisaPorDisciplinaPresenter 
	implements Presenter
{
		public interface Display
		extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		TextField< String > getCodigoBuscaTextField();
		CampusComboBox getCampusBuscaComboBox();
		TextField< String > getTurmaBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();		
		SimpleGrid< PesquisaPorDisciplinaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< PesquisaPorDisciplinaDTO > > proxy );
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	
	public AtendimentosPesquisaPorDisciplinaPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display )
	{
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		configureProxy();
		setListeners();
	}
	
	private void configureProxy()
	{
		final AtendimentosServiceAsync service = Services.atendimentos();
	
		RpcProxy< PagingLoadResult< PesquisaPorDisciplinaDTO > > proxy =
			new RpcProxy< PagingLoadResult< PesquisaPorDisciplinaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
					AsyncCallback< PagingLoadResult< PesquisaPorDisciplinaDTO > > callback )
			{
				String codigo = display.getCodigoBuscaTextField().getValue();
	
				CampusDTO campusDTO
					= display.getCampusBuscaComboBox().getValue();
				
				String turma 
					= display.getTurmaBuscaTextField().getValue();
	
				service.getAtendimentosPesquisaPorDisciplinaList(cenarioDTO, codigo, campusDTO, turma, (PagingLoadConfig) loadConfig, callback);
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
				display.getCodigoBuscaTextField().setValue( null );
				display.getCampusBuscaComboBox().setValue( null );
				
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
		
		this.display.getExportXlsExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.ATENDIMENTOS_DISCIPLINA, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
				
		this.display.getExportXlsxExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.ATENDIMENTOS_DISCIPLINA, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
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
