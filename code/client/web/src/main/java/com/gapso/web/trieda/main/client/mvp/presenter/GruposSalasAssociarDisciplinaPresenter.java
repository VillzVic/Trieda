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
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.GruposSalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class GruposSalasAssociarDisciplinaPresenter
	implements Presenter
{
	public interface Display
	extends ITriedaI18nGateway
	{
		DisciplinaComboBox getDisciplinaComboBox();
		ListView< GrupoSalaDTO > getNaoVinculadaList();
		ListView< GrupoSalaDTO > getVinculadaList();
		Button getAdicionaBT();
		Button getRemoveBT();
		DisciplinaDTO getDisciplinaDTO();
		Component getComponent();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	
	public GruposSalasAssociarDisciplinaPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
	
		this.display = display;
		configureProxy();
		setListeners();
		setProxy();
	}
	
	private void configureProxy() {
		final GruposSalasServiceAsync service = Services.gruposSalas();
		RpcProxy<List<GrupoSalaDTO>> proxyNaoVinculada = new RpcProxy<List<GrupoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<GrupoSalaDTO>> callback) {
				DisciplinaDTO disciplinaDTO = display.getDisciplinaComboBox().getValue();
				service.getListNaoVinculadas(disciplinaDTO, callback);
			}
		};
		display.getNaoVinculadaList().setStore(new ListStore<GrupoSalaDTO>(new BaseListLoader<ListLoadResult<GrupoSalaDTO>>(proxyNaoVinculada)));
		RpcProxy<List<GrupoSalaDTO>> proxyVinculada = new RpcProxy<List<GrupoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<GrupoSalaDTO>> callback) {
				DisciplinaDTO disciplinaDTO = display.getDisciplinaComboBox().getValue();
				service.getListVinculadas(disciplinaDTO, callback);
			}
		};
		display.getVinculadaList().setStore(new ListStore<GrupoSalaDTO>(new BaseListLoader<ListLoadResult<GrupoSalaDTO>>(proxyVinculada)));
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
	
		display.getDisciplinaComboBox().addSelectionChangedListener(new SelectionChangedListener<DisciplinaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<DisciplinaDTO> se) {
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
				List<GrupoSalaDTO> gruposSalaDTOList = display.getNaoVinculadaList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoVinculadaList(), display.getVinculadaList(), gruposSalaDTOList);
				DisciplinaDTO disciplinaDTO = display.getDisciplinaComboBox().getValue();
				Services.gruposSalas().vincula(disciplinaDTO, gruposSalaDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
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
				List<GrupoSalaDTO> gruposSalaDTOList = display.getVinculadaList().getSelectionModel().getSelectedItems();
				transfere(display.getVinculadaList(), display.getNaoVinculadaList(), gruposSalaDTOList);
				DisciplinaDTO disciplinaDTO = display.getDisciplinaComboBox().getValue();
				Services.gruposSalas().desvincula(disciplinaDTO, gruposSalaDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						Info.display("Removido", "Sala removida com sucesso");
					}
				});
			}
		});
	}
	
	public void setProxy() {
		if(display.getDisciplinaDTO().getId() != null) {
			display.getNaoVinculadaList().getStore().getLoader().load();
			display.getVinculadaList().getStore().getLoader().load();
		}
	}
	
	private void transfere(ListView<GrupoSalaDTO> origem, ListView<GrupoSalaDTO> destino, List<GrupoSalaDTO> salaDTOList) {
		ListStore<GrupoSalaDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<GrupoSalaDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(salaDTOList);
		storeVinculadas.sort("codigo", SortDir.ASC);
		destino.refresh();
		
		for(GrupoSalaDTO grupoSalaDTO: salaDTOList) {
			storeNaoVinculadas.remove(grupoSalaDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}
}
