package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.services.CursosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SelecionarCursosPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		CampusComboBox getCampusComboBox();
		ListView<CursoDTO> getNaoSelecionadoList();
		ListView<CursoDTO> getSelecionadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		Button getFecharBT();
		Component getComponent();
		
		SimpleModal getSimpleModal();
	}
	
	private Display display;
	private List<CursoDTO> cursos;
	
	public SelecionarCursosPresenter(List<CursoDTO> cursos, Display display) {
		this.display = display;
		this.cursos = cursos;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CursosServiceAsync service = Services.cursos();
		RpcProxy<ListLoadResult<CursoDTO>> proxyNaoSelecionado = new RpcProxy<ListLoadResult<CursoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CursoDTO>> callback) {
				List<CursoDTO> cursosSelecionados = display.getSelecionadoList().getStore().getModels();
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				service.getListByCampus(campusDTO, cursosSelecionados, callback);
			}
		};
		display.getNaoSelecionadoList().setStore(new ListStore<CursoDTO>(new BaseListLoader<ListLoadResult<CursoDTO>>(proxyNaoSelecionado)));
		
		ListStore<CursoDTO> listStore = new ListStore<CursoDTO>();
		listStore.add(cursos);
		display.getSelecionadoList().setStore(listStore);
//		listStore.getLoader().load();
	}
	
	private void setListeners() {

		display.getCampusComboBox().addSelectionChangedListener(new SelectionChangedListener<CampusDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
				display.getNaoSelecionadoList().getStore().getLoader().load();
				display.getNaoSelecionadoList().setEnabled(true);
				display.getAdicionaBT().setEnabled(true);
				display.getRemoveBT().setEnabled(true);
			}
		});

		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursosDTOList = display.getNaoSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getNaoSelecionadoList(), display.getSelecionadoList(), cursosDTOList);
				cursos.addAll(cursosDTOList);
			}
		});
		
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursosDTOList = display.getSelecionadoList().getSelectionModel().getSelectedItems();
				transfere(display.getSelecionadoList(), display.getNaoSelecionadoList(), cursosDTOList);
				cursos.removeAll(cursosDTOList);
			}
		});
		
		display.getFecharBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getSimpleModal().hide();
			}
		});
		
	}

	private void transfere(ListView<CursoDTO> origem, ListView<CursoDTO> destino, List<CursoDTO> cursoDTOList) {
		ListStore<CursoDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<CursoDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(cursoDTOList);
		storeVinculadas.sort(CursoDTO.PROPERTY_DISPLAY_TEXT, SortDir.ASC);
		destino.refresh();
		
		for(CursoDTO cursoDTO: cursoDTOList) {
			storeNaoVinculadas.remove(cursoDTO);
		}
		origem.refresh();
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
