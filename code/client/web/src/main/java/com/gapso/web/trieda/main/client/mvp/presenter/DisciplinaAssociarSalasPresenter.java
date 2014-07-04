package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.LoadListener;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinaAssociarSalasPresenter
	implements Presenter
{
	public interface Display
	extends ITriedaI18nGateway
	{
		SalaAutoCompleteBox getSalaComboBox();
		ListView< DisciplinaDTO > getNaoVinculadaList();
		ListView< DisciplinaDTO > getVinculadaList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		SalaDTO getSalaDTO();
		Component getComponent();
		Button getAssociarDisciplinasPraticasLaboratoriosBT();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	
	public DisciplinaAssociarSalasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		this.display = display;
		configureProxy();
		setListeners();
		setProxy();
	}
	
	private void configureProxy() {
		final SalasServiceAsync service = Services.salas();
		RpcProxy<List<DisciplinaDTO>> proxyNaoVinculada = new RpcProxy<List<DisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<DisciplinaDTO>> callback) {
				SalaDTO salaDTO = display.getSalaComboBox().getValue();
				service.getListDisciplinasNaoVinculadas(cenarioDTO, salaDTO, callback);
			}
		};
		display.getNaoVinculadaList().setStore(new ListStore<DisciplinaDTO>(new BaseListLoader<ListLoadResult<SalaDTO>>(proxyNaoVinculada)));
		RpcProxy<List<DisciplinaDTO>> proxyVinculada = new RpcProxy<List<DisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<DisciplinaDTO>> callback) {
				SalaDTO salaDTO = display.getSalaComboBox().getValue();
				service.getListDisciplinasVinculadas(salaDTO, callback);
			}
		};
		display.getVinculadaList().setStore(new ListStore<DisciplinaDTO>(new BaseListLoader<ListLoadResult<DisciplinaDTO>>(proxyVinculada)));
		addLoadingListener();
	}
	
	private void addLoadingListener() {
		display.getNaoVinculadaList().getStore().getLoader().addLoadListener(new LoadListener() {
			@Override
			public void loaderBeforeLoad(LoadEvent le) {
				display.getNaoVinculadaList().mask(display.getI18nMessages().loading(), "loading");
				display.getVinculadaList().mask(display.getI18nMessages().loading(), "loading");
			}
			@Override
			public void loaderLoad(LoadEvent le) {
				display.getNaoVinculadaList().unmask();
				display.getVinculadaList().unmask();
			}
		});
	}
	
	private void setListeners() {
	
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SalaDTO> se) {
				display.getNaoVinculadaList().getStore().getLoader().load();
				display.getVinculadaList().getStore().getLoader().load();
				display.getNaoVinculadaList().setEnabled(true);
				display.getVinculadaList().setEnabled(true);
				display.getAdicionaBT().setEnabled(true);
				display.getRemoveBT().setEnabled(true);
			}
		});
	
		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> salasDTOList = display.getNaoVinculadaList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoVinculadaList(), display.getVinculadaList(), salasDTOList);
				SalaDTO salaDTO = display.getSalaComboBox().getValue();
				Services.salas().vincula(salaDTO, salasDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Cadastrado", "Sala vinculada com sucesso");
					}
				});
			}
		});
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> disciplinasDTOList = display.getVinculadaList().getSelectionModel().getSelectedItems();
				transfere(display.getVinculadaList(), display.getNaoVinculadaList(), disciplinasDTOList);
				SalaDTO salaDTO = display.getSalaComboBox().getValue();
				Services.salas().desvincula(salaDTO, disciplinasDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Removido", "Sala removida com sucesso");
					}
				});
			}
		});
		display.getImportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DISCIPLINAS_SALAS, instituicaoEnsinoDTO, cenarioDTO );
	
				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, null );
	
				importExcelFormView.show();
			}
		});
	
		display.getExportXlsExcelButton().addSelectionListener(new SelectionListener<MenuEvent>(){
			@Override
			public void componentSelected(MenuEvent ce) {
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DISCIPLINAS_SALAS, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
	
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		display.getExportXlsxExcelButton().addSelectionListener(new SelectionListener<MenuEvent>(){
			@Override
			public void componentSelected(MenuEvent ce) {
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.CURSO_AREAS_TITULACAO, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
	
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.display.getAssociarDisciplinasPraticasLaboratoriosBT().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				DisciplinasServiceAsync service = Services.disciplinas();
				service.associarDisciplinasSemLaboratorioATodosLaboratorios(cenarioDTO, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display(display.getI18nConstants().informacao(),"A associação de disciplinas que exigem laboratórios com salas de aulas do tipo laboratório foi efetuada com sucesso!");
					}
				});
			}
		});
	}
	
	public void setProxy() {
		if(display.getSalaDTO().getId() != null) {
			display.getNaoVinculadaList().getStore().getLoader().load();
			display.getVinculadaList().getStore().getLoader().load();
		}
	}
	
	private void transfere(ListView<DisciplinaDTO> origem, ListView<DisciplinaDTO> destino, List<DisciplinaDTO> disciplinasDTOList) {
		ListStore<DisciplinaDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<DisciplinaDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(disciplinasDTOList);
		storeVinculadas.sort("codigo", SortDir.ASC);
		destino.refresh();
		
		for(DisciplinaDTO disciplinaDTO: disciplinasDTOList) {
			storeNaoVinculadas.remove(disciplinaDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}
}
