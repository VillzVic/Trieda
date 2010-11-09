package com.gapso.web.trieda.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.CurriculosServiceAsync;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class DisciplinasAssociarSalaPresenter implements Presenter {

	public interface Display {
		CampusComboBox getCampusComboBox();
		SimpleComboBox<TurnoDTO> getTurnoComboBox();
		UnidadeComboBox getUnidadeSalaComboBox();
		UnidadeComboBox getUnidadeGrupoSalaComboBox();
		SimpleComboBox<String> getAndarComboBox();
		SimpleComboBox<SalaDTO> getSalaComboBox();
		TreePanel<FileModel> getDisciplinasList();
		TreePanel<FileModel> getSalasList();
		SimpleComboBox<GrupoSalaDTO> getGrupoSalasComboBox();
		void setTabEnabled(boolean flag);
		Component getComponent();
	}
	private Display display;
	private Map<String, List<SalaDTO>> andaresSalasMap;
	
	public DisciplinasAssociarSalaPresenter(Display display) {
		this.display = display;
		setListeners();
//		populaListas();
	}

//	private void populaListas() {
//		final FutureResult<ListLoadResult<CursoDTO>> futureCursoDTOList = new FutureResult<ListLoadResult<CursoDTO>>();
//		
//		CursosServiceAsync cursosService = Services.cursos();
//		
//		cursosService.getListByCampus(campusDTO, callback);
//		
//		FutureSynchronizer synch = new FutureSynchronizer(futureSalaDTOList, futureSalaPertencesDTOList);
//		synch.addCallback(new AsyncCallback<Boolean>() {
//			@Override
//			public void onFailure(Throwable caught) {
//				caught.printStackTrace();
//			}
//			@Override
//			public void onSuccess(Boolean result) {
//				ListLoadResult<SalaDTO> salaDTOList = futureSalaDTOList.result();
//				ListLoadResult<SalaDTO> salaDTOPertencesList = futureSalaPertencesDTOList.result();
//				
//				ListStore<SalaDTO> storeAndares = display.getAndaresList().getStore();  
//				storeAndares.add(salaDTOList.getData());
//				display.getAndaresList().setStore(storeAndares);
//				display.getAndaresList().refresh();
//				
//				ListStore<SalaDTO> storePertences = display.getSalasPertencesList().getStore();
//				storePertences.add(salaDTOPertencesList.getData());
//				display.getSalasPertencesList().setStore(storePertences);
//				display.getSalasPertencesList().refresh();
//			}
//		});
//	}
	
	private void setListeners() {

		display.getCampusComboBox().addSelectionChangedListener(new SelectionChangedListener<CampusDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
				final CampusDTO campusDTO = se.getSelectedItem();
				
				final FutureResult<ListLoadResult<TurnoDTO>> futureTurnoDTOList = new FutureResult<ListLoadResult<TurnoDTO>>();
				
				TurnosServiceAsync turnosService = Services.turnos();
				
				turnosService.getListByCampus(campusDTO, futureTurnoDTOList);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureTurnoDTOList);
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						display.getTurnoComboBox().disable();
						caught.printStackTrace();
					}

					@Override
					public void onSuccess(Boolean result) {
						ListLoadResult<TurnoDTO> turnoDTOList = futureTurnoDTOList.result();
						display.getTurnoComboBox().setEnabled(!turnoDTOList.getData().isEmpty());
						display.getTurnoComboBox().getStore().removeAll();
						display.getTurnoComboBox().add(turnoDTOList.getData());
						display.getUnidadeSalaComboBox().setCampusId(campusDTO.getId());
						display.getUnidadeGrupoSalaComboBox().setCampusId(campusDTO.getId());
					}
					
				});
			}
		});
		
		display.getTurnoComboBox().addSelectionChangedListener(new SelectionChangedListener<SimpleComboValue<TurnoDTO>>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SimpleComboValue<TurnoDTO>> se) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				TurnoDTO turnoDTO = se.getSelectedItem().getValue();
				
				final FutureResult<ListLoadResult<FileModel>> futureCursoDTOList = new FutureResult<ListLoadResult<FileModel>>();
				
				CurriculosServiceAsync curriculosService = Services.curriculos();
				
				curriculosService.getListByCampusAndTurno(campusDTO, turnoDTO, futureCursoDTOList);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureCursoDTOList);
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						display.getTurnoComboBox().disable();
						caught.printStackTrace();
					}

					@Override
					public void onSuccess(Boolean result) {
						ListLoadResult<FileModel> cursoDTOList = futureCursoDTOList.result();
						TreeStore<FileModel> store = display.getDisciplinasList().getStore();
						store.removeAll();
						store.add(cursoDTOList.getData(), true);
						display.setTabEnabled(!cursoDTOList.getData().isEmpty());
						display.getDisciplinasList().setEnabled(!cursoDTOList.getData().isEmpty());
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
							if(result != null) for(String andar : result.keySet()) {
								andares.add(andar);
							}
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
				display.getSalaComboBox().setValue(null);
				display.getSalaComboBox().setEnabled(se.getSelectedItem() != null);
				if(se.getSelectedItem() != null) {
					display.getSalaComboBox().getStore().removeAll();
					display.getSalaComboBox().add(andaresSalasMap.get(se.getSelectedItem().getValue()));
				}
			}
		});
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
							display.getGrupoSalasComboBox().getStore().removeAll();
							display.getGrupoSalasComboBox().add(result);
							display.getGrupoSalasComboBox().setEnabled(!result.isEmpty());
						}
					});
				}
			}
		});
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SimpleComboValue<SalaDTO>>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SimpleComboValue<SalaDTO>> se) {
				if(se.getSelectedItem() != null) {
					SalaDTO salaDTO = se.getSelectedItem().getValue();
					display.getSalasList().enable();
					display.getSalasList().getStore().removeAll();
					display.getSalasList().getStore().add(salaDTO, true);
				}
			}
		});
//		display.getAtualizaCursosBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				CursosServiceAsync cursoService = Services.cursos();
//				CampusDTO campusDTO = display.getCampusComboBox().getValue();
//				cursoService.getListByCampus(campusDTO, new AsyncCallback<ListLoadResult<CursoDTO>>() {
//					@Override
//					public void onFailure(Throwable caught) {
//						caught.printStackTrace();
//					}
//					@Override
//					public void onSuccess(ListLoadResult<CursoDTO> result) {
//						ListStore<CursoDTO> storeCursos = display.getCursosList().getStore();
//						storeCursos.removeAll();
//						storeCursos.add(result.getData());
//						display.getCursosList().refresh();
//					}
//				});
//			}
//		});
//		display.getAtualizaDisciplinasDoCursoBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				DisciplinasServiceAsync disciplinaService = Services.disciplinas();
//				List<CursoDTO> cursos = display.getCursosList().getStore().getModels();
//				disciplinaService.getListByCursos(cursos, new AsyncCallback<ListLoadResult<DisciplinaDTO>>(){
//					@Override
//					public void onFailure(Throwable caught) {
//						caught.printStackTrace();
//					}
//					@Override
//					public void onSuccess(ListLoadResult<DisciplinaDTO> result) {
//						ListStore<DisciplinaDTO> storeDisciplinas = display.getDisciplinasNaoPertencesList().getStore();
//						storeDisciplinas.removeAll();
//						storeDisciplinas.add(result.getData());
//						display.getDisciplinasNaoPertencesList().refresh();
//					}
//				});
//			}
//		});
		

//		display.getCampusComboBox().addSelectionChangedListener(new SelectionChangedListener<CampusDTO>(){
//			@Override
//			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
//			}
//		});
		
//		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				List<SalaDTO> salaDTOSelecionadasList = display.getSalasPertencesList().getStore().getModels();
//				
//				GruposSalasServiceAsync gruposSalasService = Services.gruposSalas();
//				gruposSalasService.saveSalas(salaDTOSelecionadasList, getDTO(), new AsyncCallback<Void>() {
//					@Override
//					public void onFailure(Throwable caught) {
//						caught.printStackTrace();
//					}
//					@Override
//					public void onSuccess(Void result) {
//						Info.display("Atualizado", "Salas atualizadas com sucesso!");
//						display.getSimpleModal().hide();
//						simpleGrid.updateList();
//					}
//					
//				});
//			}
//		});
//		
//		display.getAtualizaSalasDoAndarBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				List<SalaDTO> salaDTOAndaresList = display.getAndaresList().getSelectionModel().getSelectedItems();
//				final List<SalaDTO> salaDTOSelecionadasList = display.getSalasPertencesList().getStore().getModels();
//				List<String> salasStringList = new ArrayList<String>();
//				for(SalaDTO sala : salaDTOAndaresList) {
//					salasStringList.add(sala.getAndar());
//				}
//				
//				final FutureResult<ListLoadResult<SalaDTO>> futureSalaDTOList = new FutureResult<ListLoadResult<SalaDTO>>();
//				SalasServiceAsync salasService = Services.salas();
//				salasService.getSalasDoAndareList(salasStringList, futureSalaDTOList);
//				
//				FutureSynchronizer synch = new FutureSynchronizer(futureSalaDTOList);
//				synch.addCallback(new AsyncCallback<Boolean>() {
//
//					@Override
//					public void onFailure(Throwable caught) {
//						caught.printStackTrace();
//					}
//
//					@Override
//					public void onSuccess(Boolean result) {
//						ListLoadResult<SalaDTO> salaDTOList = futureSalaDTOList.result();
//						ListStore<SalaDTO> storeSalasDoAndarPertences = display.getSalasNaoPertencesList().getStore();
//						storeSalasDoAndarPertences.removeAll();
//						List<SalaDTO> removerSalaDTO = new ArrayList<SalaDTO>();
//						for(SalaDTO o1 : salaDTOList.getData()) {
//							for(SalaDTO o2 : salaDTOSelecionadasList) {
//								if(o1.getId().equals(o2.getId())) {
//									removerSalaDTO.add(o1);
//									break;
//								}
//							}
//						}
//						salaDTOList.getData().removeAll(removerSalaDTO);
//						storeSalasDoAndarPertences.add(salaDTOList.getData());
//						storeSalasDoAndarPertences.sort("codigo", SortDir.ASC);
//						display.getSalasNaoPertencesList().refresh();
//						
//						Info.display("Atualizado", "Lista de salas não associadas atualizada!");
//					}
//					
//				});
//			}
//		});
//		
//		display.getAdicionaSalasBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				List<SalaDTO> salaDTOSelecionadasList = display.getSalasNaoPertencesList().getSelectionModel().getSelectedItems();
//
//				ListStore<SalaDTO> storeSalasPertences = display.getSalasPertencesList().getStore();  
//				storeSalasPertences.add(salaDTOSelecionadasList);
//				storeSalasPertences.sort("codigo", SortDir.ASC);
//				display.getSalasPertencesList().refresh();
//				
//				ListStore<SalaDTO> storeSalasNaoPertences = display.getSalasNaoPertencesList().getStore();
//				for(SalaDTO o : salaDTOSelecionadasList) {
//					storeSalasNaoPertences.remove(o);
//				}
//				display.getSalasNaoPertencesList().refresh();
//				
//				Info.display("Atualizado", "Salas adicionadas a lista!");
//			}
//		});
//		
//		display.getRemoveSalasBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
//			@Override
//			public void componentSelected(ButtonEvent ce) {
//				List<SalaDTO> salaDTOSelecionadasList = display.getSalasPertencesList().getSelectionModel().getSelectedItems();
//				List<SalaDTO> salaDTOAndaresList = display.getAndaresList().getSelectionModel().getSelectedItems();
//				
//				ListStore<SalaDTO> storeSalasPertences = display.getSalasPertencesList().getStore();
//				ListStore<SalaDTO> storeSalasNaoPertences = display.getSalasNaoPertencesList().getStore();
//				
//				for(SalaDTO o : salaDTOSelecionadasList) {
//					for(SalaDTO a : salaDTOAndaresList) {
//						if(o.getAndar().equals(a.getAndar())) {
//							storeSalasNaoPertences.add(o);
//							break;
//						}
//					}
//				}
//				storeSalasNaoPertences.sort("codigo", SortDir.ASC);
//				display.getSalasNaoPertencesList().refresh();
//				
//				for(SalaDTO o : salaDTOSelecionadasList) {
//					storeSalasPertences.remove(o);
//				}
//				display.getSalasPertencesList().refresh();
//				
//				Info.display("Removida", "Salas removidas da lista!");
//			}
//		});
	}

	
//	private GrupoSalaDTO getDTO() {
//		GrupoSalaDTO grupoSalaDTO = display.getGrupoSalaDTO();
//		return grupoSalaDTO;
//	}
	
	@Override
	public void go(Widget widget) {
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
