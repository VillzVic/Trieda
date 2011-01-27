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
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorCampusDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CampusProfessorFormPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getSalvarButton();
		CampusComboBox getCampusComboBox();
		ListView<ProfessorDTO> getProfessorNaoAssociadoList();
		ListView<ProfessorDTO> getProfessorAssociadoList();
		Button getAdicionaBT();
		Button getRemoveBT();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
//	private CenarioDTO cenario;
	private SimpleGrid<ProfessorCampusDTO> gridPanel;
	private Display display;
	
	public CampusProfessorFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<ProfessorCampusDTO> gridPanel) {
//		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<List<ProfessorDTO>> proxyNaoVinculada = new RpcProxy<List<ProfessorDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<ProfessorDTO>> callback) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				Services.professores().getProfessoresNaoEmCampus(campusDTO, callback);
			}
		};
		display.getProfessorNaoAssociadoList().setStore(new ListStore<ProfessorDTO>(new BaseListLoader<ListLoadResult<ProfessorDTO>>(proxyNaoVinculada)));
		RpcProxy<List<ProfessorDTO>> proxyVinculada = new RpcProxy<List<ProfessorDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<List<ProfessorDTO>> callback) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				Services.professores().getProfessoresEmCampus(campusDTO, callback);
			}
		};
		display.getProfessorAssociadoList().setStore(new ListStore<ProfessorDTO>(new BaseListLoader<ListLoadResult<ProfessorDTO>>(proxyVinculada)));
		
		if(display.getCampusComboBox().getValue() != null) {
			display.getProfessorNaoAssociadoList().setEnabled(true);
			display.getProfessorAssociadoList().setEnabled(true);
			display.getAdicionaBT().setEnabled(true);
			display.getRemoveBT().setEnabled(true);
			display.getProfessorNaoAssociadoList().getStore().getLoader().load();
			display.getProfessorAssociadoList().getStore().getLoader().load();
		}
	}
	
	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					CampusDTO campusDTO = display.getCampusComboBox().getValue();
					List<ProfessorDTO> professorDTOList = display.getProfessorAssociadoList().getStore().getModels();
					Services.professores().salvarProfessorCampus(campusDTO, professorDTOList, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							gridPanel.updateList();
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", "Nenhum professor ou campus foi selecionado", null);
				}
			}
		});
		
		display.getCampusComboBox().addSelectionChangedListener(new SelectionChangedListener<CampusDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
				display.getProfessorNaoAssociadoList().setEnabled(true);
				display.getProfessorAssociadoList().setEnabled(true);
				display.getAdicionaBT().setEnabled(true);
				display.getRemoveBT().setEnabled(true);
				display.getProfessorNaoAssociadoList().getStore().getLoader().load();
				display.getProfessorAssociadoList().getStore().getLoader().load();
			}
		});
		
		display.getAdicionaBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<ProfessorDTO> professorDTOList = display.getProfessorNaoAssociadoList().getSelectionModel().getSelectedItems();
				transfere(display.getProfessorNaoAssociadoList(), display.getProfessorAssociadoList(), professorDTOList);
			}
		});
		
		display.getRemoveBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<ProfessorDTO> professorDTOList = display.getProfessorAssociadoList().getSelectionModel().getSelectedItems();
				transfere(display.getProfessorAssociadoList(), display.getProfessorNaoAssociadoList(), professorDTOList);
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}
	
	private void transfere(ListView<ProfessorDTO> origem, ListView<ProfessorDTO> destino, List<ProfessorDTO> professorDTOList) {
		ListStore<ProfessorDTO> storeNaoVinculadas = origem.getStore();  
		ListStore<ProfessorDTO> storeVinculadas = destino.getStore();
		
		storeVinculadas.add(professorDTOList);
		storeVinculadas.sort("nome", SortDir.ASC);
		destino.refresh();
		
		for(ProfessorDTO professorDTO: professorDTOList) {
			storeNaoVinculadas.remove(professorDTO);
		}
		origem.refresh();
	}

	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
