package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosDemandaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DemandasPorAlunoPresenter
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
		Button getAssociarAlunosDemanda();
		Button getImportExcelAlunosDemandaBT();
		MenuItem getExportXlsExcelAlunosDemandaBT();
		MenuItem getExportXlsxExcelAlunosDemandaBT();
		CampusComboBox getCampusBuscaComboBox();
		CursoComboBox getCursoBuscaComboBox();
		CurriculoComboBox getCurriculoBuscaComboBox();
		TurnoComboBox getTurnoBuscaComboBox();
		DisciplinaAutoCompleteBox getDisciplinaBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< AlunoDemandaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< AlunoDemandaDTO > > proxy );
	}
	
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private GTab gTab;
	private CenarioDTO cenarioDTO;
	
	public DemandasPorAlunoPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenarioDTO = cenarioDTO;
		configureProxy();
		setListeners();
	}
	
	private void configureProxy()
	{
		final AlunosDemandaServiceAsync service = Services.alunosDemanda();
	
		RpcProxy< PagingLoadResult< AlunoDemandaDTO > > proxy =
			new RpcProxy< PagingLoadResult< AlunoDemandaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< AlunoDemandaDTO > > callback )
			{
				CampusDTO campus = display.getCampusBuscaComboBox().getValue();
				CursoDTO curso = display.getCursoBuscaComboBox().getValue();
				CurriculoDTO curriculo = display.getCurriculoBuscaComboBox().getValue();
				TurnoDTO turno = display.getTurnoBuscaComboBox().getValue();
				DisciplinaDTO disciplina = display.getDisciplinaBuscaComboBox().getValue();
	
				service.getAlunosDemandaList( cenarioDTO, campus, curso, curriculo, turno,
					disciplina, (PagingLoadConfig) loadConfig, callback );
			}
		};
	
		this.display.setProxy( proxy );
	}
	
	private void setListeners()
	{	
		this.display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.DEMANDAS_POR_ALUNO, instituicaoEnsinoDTO );
	
				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros,display.getGrid() );
	
				importExcelFormView.show();
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
					ExcelInformationType.DEMANDAS_POR_ALUNO, instituicaoEnsinoDTO, fileExtension );
	
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
					ExcelInformationType.DEMANDAS_POR_ALUNO, instituicaoEnsinoDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
	
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
	
		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getCampusBuscaComboBox().setValue( null );
				display.getCursoBuscaComboBox().setValue( null );
				display.getCurriculoBuscaComboBox().setValue( null );
				display.getTurnoBuscaComboBox().setValue( null );
				display.getDisciplinaBuscaComboBox().setValue( null );
	
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
	public void go( Widget widget )
	{
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
