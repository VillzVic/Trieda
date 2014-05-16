package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.MotivosNaoAtendimentoView;
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
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
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
		Button getMotivosNaoAtendimentoButton();
		void setProxy( RpcProxy< PagingLoadResult< AlunoDemandaDTO > > proxy );
		NumberField getPeriodoField();
		TextField<String> getMatriculaBuscaTextField();
		TextField<String> getNomeTextField();
		NumberField getAlunoPrioridadeField();
		ComboBoxBoolean getAtentido();
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
				
				Integer periodo = ( display.getPeriodoField().getValue() == null ) ? null : display.getPeriodoField().getValue().intValue();
				String matricula = display.getMatriculaBuscaTextField().getValue();
				String nome = display.getNomeTextField().getValue();
				Integer alunoPrioridade = ( display.getAlunoPrioridadeField().getValue() == null ) ? null : display.getAlunoPrioridadeField().getValue().intValue();
				Boolean atendido = (display.getAtentido().getValue()==null)?null:display.getAtentido().getValue().getValue().getValue();
	
				service.getAlunosDemandaList( cenarioDTO, campus, curso, curriculo, turno,
					disciplina, periodo, matricula, nome, alunoPrioridade, atendido, (PagingLoadConfig) loadConfig, callback );
			}
		};
	
		this.display.setProxy( proxy );
	}
	
	private void setListeners()
	{	
		this.display.getRemoveButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final AlunosDemandaServiceAsync service = Services.alunosDemanda();

				List< AlunoDemandaDTO > list
					= display.getGrid().getSelectionModel().getSelectedItems();

				service.removeAlunosDemanda( list,
					new AbstractAsyncCallbackWithDefaultOnFailure< Void >( display )
				{
					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().getGrid().getStore().getLoader().load();
						Info.display( "Removido", "Item(ns) removido com sucesso!" );
					}
				});
			}
		});
		
		this.display.getImportExcelButton().addSelectionListener(new SelectionListener< ButtonEvent >()	{
			@Override
			public void componentSelected( ButtonEvent ce )	{
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.DEMANDAS_POR_ALUNO, instituicaoEnsinoDTO, cenarioDTO );
				ImportExcelFormView importExcelFormView	= new ImportExcelFormView( parametros,display.getGrid() );
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
					ExcelInformationType.DEMANDAS_POR_ALUNO, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
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
					ExcelInformationType.DEMANDAS_POR_ALUNO, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
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
				display.getPeriodoField().setValue( null );
				display.getMatriculaBuscaTextField().setValue( null );
				display.getNomeTextField().setValue( null );
				display.getAlunoPrioridadeField().setValue( null );
				display.getAtentido().setValue( null );
	
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
		
		this.display.getMotivosNaoAtendimentoButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final AlunoDemandaDTO alunoDemandaDTO = display.getGrid().getSelectionModel().getSelectedItem();
				
				Presenter presenter = new MotivosNaoAtendimentoPresenter(new MotivosNaoAtendimentoView(cenarioDTO, alunoDemandaDTO));
				presenter.go( gTab );
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
