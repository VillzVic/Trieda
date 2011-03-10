package com.gapso.web.trieda.client.mvp.presenter;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.IconButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class DisciplinasAssociarSalaPresenter implements Presenter {

	public interface Display {
		CampusComboBox getCampusComboBox();
		TurnoComboBox getTurnoComboBox();
		UnidadeComboBox getUnidadeSalaComboBox();
		UnidadeComboBox getUnidadeGrupoSalaComboBox();
		SimpleComboBox<String> getAndarComboBox();
		TreePanel<TreeNodeDTO> getDisciplinasList();
		TreePanel<TreeNodeDTO> getSalasList();
		ToolButton getRemoveButton();
		void setTabEnabled(boolean flag);
		Component getComponent();
	}
	private Display display;
	private Map<String, List<SalaDTO>> andaresSalasMap;
	
	public DisciplinasAssociarSalaPresenter(Display display) {
		this.display = display;
		setListeners();
	}

	private void setListeners() {

		display.getTurnoComboBox().addSelectionChangedListener(new SelectionChangedListener<TurnoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<TurnoDTO> se) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				TurnoDTO turnoDTO = se.getSelectedItem();
				
				final FutureResult<ListLoadResult<TreeNodeDTO>> futureOfertaDTOList = new FutureResult<ListLoadResult<TreeNodeDTO>>();
				
				Services.ofertas().getListByCampusAndTurno(campusDTO, turnoDTO, futureOfertaDTOList);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureOfertaDTOList);
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						display.getTurnoComboBox().disable();
						caught.printStackTrace();
					}

					@Override
					public void onSuccess(Boolean result) {
						ListLoadResult<TreeNodeDTO> ofertaDTOList = futureOfertaDTOList.result();
						TreeStore<TreeNodeDTO> treeStore = display.getDisciplinasList().getStore();
						treeStore.removeAll();
						treeStore.add(ofertaDTOList.getData(), true);
						boolean existeOferta = !ofertaDTOList.getData().isEmpty();
						display.setTabEnabled(existeOferta);
						display.getDisciplinasList().setEnabled(existeOferta);
					}
				});
			}

		});
		display.getUnidadeSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<UnidadeDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<UnidadeDTO> se) {
				UnidadeDTO unidadeDTO = se.getSelectedItem();
				if(unidadeDTO != null) {
					SalasServiceAsync salasService = Services.salas();
					salasService.getSalasEAndareMap(unidadeDTO.getId(), new AsyncCallback<Map<String, List<SalaDTO>>>() {
						@Override
						public void onFailure(Throwable caught) {
							caught.printStackTrace();
						}
						@Override
						public void onSuccess(Map<String, List<SalaDTO>> result) {
							andaresSalasMap = result;
							display.getAndarComboBox().getStore().removeAll();
							List<String> andares = new ArrayList<String>();
							andares.addAll(result.keySet());
							display.getAndarComboBox().add(andares);
							display.getAndarComboBox().setEnabled(!andares.isEmpty());
						}
					});
				}
			}
		});
		display.getAndarComboBox().addSelectionChangedListener(new SelectionChangedListener<SimpleComboValue<String>>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SimpleComboValue<String>> se) {
				display.getSalasList().setEnabled(se.getSelectedItem() != null);
				if(se.getSelectedItem() != null) {
					display.getSalasList().getStore().removeAll();
					List<SalaDTO> salaDTOList = andaresSalasMap.get(se.getSelectedItem().getValue());
					List<TreeNodeDTO> treeNodesList = new ArrayList<TreeNodeDTO>(salaDTOList.size());
					for(SalaDTO salaDTO : salaDTOList) {
						TreeNodeDTO nodeDTO = new TreeNodeDTO(salaDTO);
						treeNodesList.add(nodeDTO);
					}
					
					Collections.sort(treeNodesList);
					
					display.getSalasList().getStore().add(treeNodesList, true);
				}
			}
		});
//		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>(){
//			@Override
//			public void selectionChanged(SelectionChangedEvent<SalaDTO> se) {
//				if(se.getSelectedItem() != null) {
//					SalaDTO salaDTO = se.getSelectedItem();
//					display.getSalasList().enable();
//					display.getSalasList().getStore().removeAll();
//					display.getSalasList().getStore().add(salaDTO, true);
//				}
//			}
//		});
		display.getUnidadeGrupoSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<UnidadeDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<UnidadeDTO> se) {
				UnidadeDTO unidadeDTO = se.getSelectedItem();
				if(unidadeDTO != null) {
					SalasServiceAsync salasService = Services.salas();
					salasService.getGruposDeSalas(unidadeDTO.getId(), new AsyncCallback<List<GrupoSalaDTO>>() {
						@Override
						public void onFailure(Throwable caught) {
							caught.printStackTrace();
						}
						@Override
						public void onSuccess(List<GrupoSalaDTO> result) {
							display.getSalasList().enable();
							display.getSalasList().getStore().removeAll();
							for(GrupoSalaDTO grupoSalaDTO : result) {
								TreeNodeDTO nodeDTO = new TreeNodeDTO(grupoSalaDTO);
								display.getSalasList().getStore().add(nodeDTO, true);
							}
						}
					});
				}
			}
		});
		display.getRemoveButton().addListener(Events.Select, new SelectionListener<IconButtonEvent>(){
			@Override
			public void componentSelected(IconButtonEvent ce) {
				TreeNodeDTO selectedNodeDTO = display.getSalasList().getSelectionModel().getSelectedItem();
				
				TreeNodeDTO cdTreeNodeDTO = null;
				CurriculoDisciplinaDTO cdDTO = null;
				SalaDTO salaDTO = null;
				GrupoSalaDTO grupoSalaDTO = null;
				
				if (selectedNodeDTO.getLeaf()) {
					cdTreeNodeDTO = selectedNodeDTO;
					cdDTO = (CurriculoDisciplinaDTO)selectedNodeDTO.getContent();
					AbstractDTO<?> nodeContent = selectedNodeDTO.getParent().getParent().getParent().getContent();
					if (nodeContent instanceof SalaDTO) {
						salaDTO = (SalaDTO)nodeContent;
					} else {
						grupoSalaDTO = (GrupoSalaDTO)nodeContent;
					}
				} else {
					Info.display("Erro", "Selecione uma disciplina!");
				}
				
				final TreeNodeDTO cdTreeNodeDTORemove = cdTreeNodeDTO;
				
				if (cdDTO != null) {
					DisciplinasServiceAsync salasService = Services.disciplinas();
					if(salaDTO != null) {
						salasService.removeDisciplinaToSala(salaDTO, cdDTO, new AsyncCallback<Void>() {
							@Override
							public void onFailure(Throwable caught) {
								caught.printStackTrace();
							}
							@Override
							public void onSuccess(Void result) {
								display.getSalasList().getStore().remove(cdTreeNodeDTORemove);
								Info.display("Removido", "Disciplinas removidas com sucesso!");
							}
						});
					} else {
						salasService.removeDisciplinaToSala(grupoSalaDTO, cdDTO, new AsyncCallback<Void>() {
							@Override
							public void onFailure(Throwable caught) {
								caught.printStackTrace();
							}
							@Override
							public void onSuccess(Void result) {
								display.getSalasList().getStore().remove(cdTreeNodeDTORemove);
								Info.display("Removido", "Disciplinas removidas com sucesso!");
							}
						});
					}
				}
			}
		});

	}

	@Override
	public void go(Widget widget) {
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
