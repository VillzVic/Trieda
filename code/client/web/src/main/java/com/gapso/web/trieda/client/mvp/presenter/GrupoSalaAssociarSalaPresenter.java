package com.gapso.web.trieda.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.GruposSalasServiceAsync;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class GrupoSalaAssociarSalaPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		GrupoSalaDTO getGrupoSalaDTO();
		ListView<SalaDTO> getAndaresList();
		ListView<SalaDTO> getSalasNaoPertencesList();
		ListView<SalaDTO> getSalasPertencesList();
		Button getAtualizaSalasDoAndarBT();
		Button getAdicionaSalasBT();
		Button getRemoveSalasBT();
		SimpleModal getSimpleModal();
	}
	private Display display;
	SimpleGrid<GrupoSalaDTO> simpleGrid;
	
	public GrupoSalaAssociarSalaPresenter(Display display, SimpleGrid<GrupoSalaDTO> simpleGrid) {
		this.display = display;
		this.simpleGrid = simpleGrid;
		setListeners();
		populaListas();
	}

	private void populaListas() {
		final FutureResult<ListLoadResult<SalaDTO>> futureSalaDTOList = new FutureResult<ListLoadResult<SalaDTO>>();
		final FutureResult<ListLoadResult<SalaDTO>> futureSalaPertencesDTOList = new FutureResult<ListLoadResult<SalaDTO>>();
		
		SalasServiceAsync salasService = Services.salas();
		GruposSalasServiceAsync gruposSalasService = Services.gruposSalas();
		
		salasService.getAndaresList(futureSalaDTOList);
		gruposSalasService.getSalas(getDTO(), futureSalaPertencesDTOList);
		
		FutureSynchronizer synch = new FutureSynchronizer(futureSalaDTOList, futureSalaPertencesDTOList);
		synch.addCallback(new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(Boolean result) {
				ListLoadResult<SalaDTO> salaDTOList = futureSalaDTOList.result();
				ListLoadResult<SalaDTO> salaDTOPertencesList = futureSalaPertencesDTOList.result();
				
				ListStore<SalaDTO> storeAndares = display.getAndaresList().getStore();  
				storeAndares.add(salaDTOList.getData());
				storeAndares.sort("codigo", SortDir.ASC);
				display.getAndaresList().setStore(storeAndares);
				display.getAndaresList().refresh();
				
				ListStore<SalaDTO> storePertences = display.getSalasPertencesList().getStore();
				storePertences.add(salaDTOPertencesList.getData());
				storePertences.sort("codigo", SortDir.ASC);
				display.getSalasPertencesList().setStore(storePertences);
				display.getSalasPertencesList().refresh();
			}
		});
	}
	
	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<SalaDTO> salaDTOSelecionadasList = display.getSalasPertencesList().getStore().getModels();
				
				GruposSalasServiceAsync gruposSalasService = Services.gruposSalas();
				gruposSalasService.saveSalas(salaDTOSelecionadasList, getDTO(), new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(Void result) {
						Info.display("Atualizado", "Salas atualizadas com sucesso!");
						display.getSimpleModal().hide();
						simpleGrid.updateList();
					}
				});
			}
		});
		
		display.getAtualizaSalasDoAndarBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<SalaDTO> salaDTOAndaresList = display.getAndaresList().getSelectionModel().getSelectedItems();
				final List<SalaDTO> salaDTOSelecionadasList = display.getSalasPertencesList().getStore().getModels();
				List<String> salasStringList = new ArrayList<String>();
				for(SalaDTO sala : salaDTOAndaresList) {
					salasStringList.add(sala.getAndar());
				}
				
				final FutureResult<ListLoadResult<SalaDTO>> futureSalaDTOList = new FutureResult<ListLoadResult<SalaDTO>>();
				UnidadeDTO unidadeDTO = new UnidadeDTO();
				unidadeDTO.setId(display.getGrupoSalaDTO().getUnidadeId());
				Services.salas().getSalasDoAndareList(unidadeDTO, salasStringList, futureSalaDTOList);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureSalaDTOList);
				synch.addCallback(new AsyncCallback<Boolean>() {

					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}

					@Override
					public void onSuccess(Boolean result) {
						ListLoadResult<SalaDTO> salaDTOList = futureSalaDTOList.result();
						ListStore<SalaDTO> storeSalasDoAndarPertences = display.getSalasNaoPertencesList().getStore();
						storeSalasDoAndarPertences.removeAll();
						List<SalaDTO> removerSalaDTO = new ArrayList<SalaDTO>();
						for(SalaDTO o1 : salaDTOList.getData()) {
							for(SalaDTO o2 : salaDTOSelecionadasList) {
								if(o1.getId().equals(o2.getId())) {
									removerSalaDTO.add(o1);
									break;
								}
							}
						}
						salaDTOList.getData().removeAll(removerSalaDTO);
						storeSalasDoAndarPertences.add(salaDTOList.getData());
						storeSalasDoAndarPertences.sort("codigo", SortDir.ASC);
						display.getSalasNaoPertencesList().refresh();
						
						Info.display("Atualizado", "Lista de salas não associadas atualizada!");
					}
					
				});
			}
		});
		
		display.getAdicionaSalasBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<SalaDTO> salaDTOSelecionadasList = display.getSalasNaoPertencesList().getSelectionModel().getSelectedItems();

				ListStore<SalaDTO> storeSalasPertences = display.getSalasPertencesList().getStore();  
				storeSalasPertences.add(salaDTOSelecionadasList);
				storeSalasPertences.sort("codigo", SortDir.ASC);
				display.getSalasPertencesList().refresh();
				
				ListStore<SalaDTO> storeSalasNaoPertences = display.getSalasNaoPertencesList().getStore();
				for(SalaDTO o : salaDTOSelecionadasList) {
					storeSalasNaoPertences.remove(o);
				}
				display.getSalasNaoPertencesList().refresh();
				
				Info.display("Atualizado", "Salas adicionadas a lista!");
			}
		});
		
		display.getRemoveSalasBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<SalaDTO> salaDTOSelecionadasList = display.getSalasPertencesList().getSelectionModel().getSelectedItems();
				List<SalaDTO> salaDTOAndaresList = display.getAndaresList().getSelectionModel().getSelectedItems();
				
				ListStore<SalaDTO> storeSalasPertences = display.getSalasPertencesList().getStore();
				ListStore<SalaDTO> storeSalasNaoPertences = display.getSalasNaoPertencesList().getStore();
				
				for(SalaDTO o : salaDTOSelecionadasList) {
					for(SalaDTO a : salaDTOAndaresList) {
						if(o.getAndar().equals(a.getAndar())) {
							storeSalasNaoPertences.add(o);
							break;
						}
					}
				}
				storeSalasNaoPertences.sort("codigo", SortDir.ASC);
				display.getSalasNaoPertencesList().refresh();
				
				for(SalaDTO o : salaDTOSelecionadasList) {
					storeSalasPertences.remove(o);
				}
				display.getSalasPertencesList().refresh();
				
				Info.display("Removida", "Salas removidas da lista!");
			}
		});
		
	}

	
	private GrupoSalaDTO getDTO() {
		GrupoSalaDTO grupoSalaDTO = display.getGrupoSalaDTO();
		return grupoSalaDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
