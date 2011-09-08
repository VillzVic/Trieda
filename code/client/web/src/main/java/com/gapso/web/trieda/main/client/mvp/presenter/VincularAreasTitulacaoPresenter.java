package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.LoadListener;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AreasTitulacaoServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class VincularAreasTitulacaoPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		CursoComboBox getCursoComboBox();
		ListView<AreaTitulacaoDTO> getNaoVinculadaList();
		ListView<AreaTitulacaoDTO> getVinculadaList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Component getComponent();
	}

	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;

	public VincularAreasTitulacaoPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final AreasTitulacaoServiceAsync service = Services.areasTitulacao();
		RpcProxy<List<AreaTitulacaoDTO>> proxyNaoVinculada = new RpcProxy<List<AreaTitulacaoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<AreaTitulacaoDTO>> callback) {
				CursoDTO cursoDTO = display.getCursoComboBox().getValue();
				service.getListNaoVinculadas(cursoDTO, callback);
			}
		};
		display.getNaoVinculadaList().setStore(new ListStore<AreaTitulacaoDTO>(new BaseListLoader<ListLoadResult<AreaTitulacaoDTO>>(proxyNaoVinculada)));
		RpcProxy<List<AreaTitulacaoDTO>> proxyVinculada = new RpcProxy<List<AreaTitulacaoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<AreaTitulacaoDTO>> callback) {
				CursoDTO cursoDTO = display.getCursoComboBox().getValue();
				service.getListVinculadas(cursoDTO, callback);
			}
		};
		display.getVinculadaList().setStore(new ListStore<AreaTitulacaoDTO>(new BaseListLoader<ListLoadResult<AreaTitulacaoDTO>>(proxyVinculada)));
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

		display.getCursoComboBox().addSelectionChangedListener(new SelectionChangedListener<CursoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CursoDTO> se) {
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
				List<AreaTitulacaoDTO> areaTitulacaoDTOList = display.getNaoVinculadaList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoVinculadaList(), display.getVinculadaList(), areaTitulacaoDTOList);
				CursoDTO cursoDTO = display.getCursoComboBox().getValue();
				Services.areasTitulacao().vincula(cursoDTO, areaTitulacaoDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Cadastrado", "Área Titulação vinculada com sucesso");
					}
				});
			}
		});
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<AreaTitulacaoDTO> areaTitulacaoDTOList = display.getVinculadaList().getSelectionModel().getSelectedItems();
				transfere(display.getVinculadaList(), display.getNaoVinculadaList(), areaTitulacaoDTOList);
				CursoDTO cursoDTO = display.getCursoComboBox().getValue();
				Services.areasTitulacao().desvincula(cursoDTO, areaTitulacaoDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Removido", "Área Titulação removida com sucesso");
					}
				});
			}
		});
		display.getImportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.CURSO_AREAS_TITULACAO, instituicaoEnsinoDTO );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, null );

				importExcelFormView.show();
			}
		});

		display.getExportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.CURSO_AREAS_TITULACAO, instituicaoEnsinoDTO );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
			}
		});
	}

	private void transfere(ListView<AreaTitulacaoDTO> origem, ListView<AreaTitulacaoDTO> destino, List<AreaTitulacaoDTO> areaTitulacaoDTOList) {
		ListStore<AreaTitulacaoDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<AreaTitulacaoDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(areaTitulacaoDTOList);
		storeVinculadas.sort("codigo", SortDir.ASC);
		destino.refresh();
		
		for(AreaTitulacaoDTO areaTitulacaoDTO: areaTitulacaoDTOList) {
			storeNaoVinculadas.remove(areaTitulacaoDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
