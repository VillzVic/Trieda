package com.gapso.web.trieda.client.mvp.presenter;

import java.util.ArrayList;
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
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
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
		TreePanel<FileModel> getDisciplinasList();
		TreePanel<FileModel> getSalasList();
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
				
				final FutureResult<ListLoadResult<FileModel>> futureOfertaDTOList = new FutureResult<ListLoadResult<FileModel>>();
				
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
						ListLoadResult<FileModel> ofertaDTOList = futureOfertaDTOList.result();
						TreeStore<FileModel> store = display.getDisciplinasList().getStore();
						store.removeAll();
						store.add(ofertaDTOList.getData(), true);
						display.setTabEnabled(!ofertaDTOList.getData().isEmpty());
						display.getDisciplinasList().setEnabled(!ofertaDTOList.getData().isEmpty());
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
					List<FileModel> fileModelList = new ArrayList<FileModel>(salaDTOList.size());
					for(SalaDTO salaDTO : salaDTOList) {
						fileModelList.add(salaDTO);
					}
					display.getSalasList().getStore().add(fileModelList, true);
					
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
							for(GrupoSalaDTO grupoSala : result) {
								display.getSalasList().getStore().add(grupoSala, true);
							}
						}
					});
				}
			}
		});
		display.getRemoveButton().addListener(Events.Select, new SelectionListener<IconButtonEvent>(){
			@Override
			public void componentSelected(IconButtonEvent ce) {
				FileModel fileModel = display.getSalasList().getSelectionModel().getSelectedItem();
				CurriculoDisciplinaDTO cdDTO = null;
				SalaDTO salaDTO = null;
				GrupoSalaDTO grupoSalaDTO = null;
				
				if(fileModel.getFolha()) {
					cdDTO = (CurriculoDisciplinaDTO)fileModel;
					CurriculoDisciplinaDTO curriculoDisciplinaDTO = (CurriculoDisciplinaDTO)display.getSalasList().getStore().getParent(cdDTO);
					OfertaDTO ofertaDTO = (OfertaDTO)display.getSalasList().getStore().getParent(curriculoDisciplinaDTO);
					FileModel fileModelRoot = display.getSalasList().getStore().getParent(ofertaDTO);
					if(fileModelRoot instanceof SalaDTO) {
						salaDTO = (SalaDTO)fileModelRoot;
					} else {
						grupoSalaDTO = (GrupoSalaDTO)fileModelRoot;
					}
				} else {
					Info.display("Erro", "Selecione uma disciplina!");
				}
				
				final CurriculoDisciplinaDTO cdDTORemove = cdDTO;
				
				if(cdDTO != null) {
					DisciplinasServiceAsync salasService = Services.disciplinas();
					if(salaDTO != null) {
						salasService.removeDisciplinaToSala(salaDTO, cdDTO, new AsyncCallback<Void>() {
							@Override
							public void onFailure(Throwable caught) {
								caught.printStackTrace();
							}
							@Override
							public void onSuccess(Void result) {
								display.getSalasList().getStore().remove(cdDTORemove);
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
								display.getSalasList().getStore().remove(cdDTORemove);
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
