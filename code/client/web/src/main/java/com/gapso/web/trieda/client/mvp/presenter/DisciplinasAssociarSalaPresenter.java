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
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.OfertasServiceAsync;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.GrupoSalaComboBox;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
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
		SalaComboBox getSalaComboBox();
		TreePanel<FileModel> getDisciplinasList();
		TreePanel<FileModel> getSalasList();
		GrupoSalaComboBox getGrupoSalasComboBox();
		ToolButton getRemoveButton();
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
				display.getTurnoComboBox().setEnabled(campusDTO != null);
				if(campusDTO != null) {
					display.getTurnoComboBox().setCampusDTO(campusDTO);
				}
				
//				final FutureResult<ListLoadResult<TurnoDTO>> futureTurnoDTOList = new FutureResult<ListLoadResult<TurnoDTO>>();
//				
//				TurnosServiceAsync turnosService = Services.turnos();
//				
//				turnosService.getListByCampus(campusDTO, futureTurnoDTOList);
//				
//				FutureSynchronizer synch = new FutureSynchronizer(futureTurnoDTOList);
//				synch.addCallback(new AsyncCallback<Boolean>() {
//					@Override
//					public void onFailure(Throwable caught) {
//						display.getTurnoComboBox().disable();
//						caught.printStackTrace();
//					}
//
//					@Override
//					public void onSuccess(Boolean result) {
//						ListLoadResult<TurnoDTO> turnoDTOList = futureTurnoDTOList.result();
//						display.getTurnoComboBox().setEnabled(!turnoDTOList.getData().isEmpty());
//						display.getTurnoComboBox().getStore().removeAll();
//						display.getTurnoComboBox().getStore().add(turnoDTOList.getData());
//						display.getUnidadeSalaComboBox().setCampusId(campusDTO.getId());
//						display.getUnidadeGrupoSalaComboBox().setCampusId(campusDTO.getId());
//					}
//					
//				});
			}
		});
		
		display.getTurnoComboBox().addSelectionChangedListener(new SelectionChangedListener<TurnoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<TurnoDTO> se) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				TurnoDTO turnoDTO = se.getSelectedItem();
				
				final FutureResult<ListLoadResult<FileModel>> futureOfertaDTOList = new FutureResult<ListLoadResult<FileModel>>();
				
				OfertasServiceAsync ofertasService = Services.ofertas();
				
				ofertasService.getListByCampusAndTurno(campusDTO, turnoDTO, futureOfertaDTOList);
				
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
				display.getSalaComboBox().setValue(null);
				display.getSalaComboBox().setEnabled(se.getSelectedItem() != null);
				if(se.getSelectedItem() != null) {
					display.getSalaComboBox().getStore().removeAll();
					display.getSalaComboBox().updateList(andaresSalasMap.get(se.getSelectedItem().getValue()));
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
							display.getGrupoSalasComboBox().getStore().add(result);
							display.getGrupoSalasComboBox().setEnabled(!result.isEmpty());
						}
					});
				}
			}
		});
		display.getSalaComboBox().addSelectionChangedListener(new SelectionChangedListener<SalaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<SalaDTO> se) {
				if(se.getSelectedItem() != null) {
					SalaDTO salaDTO = se.getSelectedItem();
					display.getSalasList().enable();
					display.getSalasList().getStore().removeAll();
					display.getSalasList().getStore().add(salaDTO, true);
				}
			}
		});
		display.getGrupoSalasComboBox().addSelectionChangedListener(new SelectionChangedListener<GrupoSalaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<GrupoSalaDTO> se) {
				if(se.getSelectedItem() != null) {
					GrupoSalaDTO grupoSalaDTO = se.getSelectedItem();
					display.getSalasList().enable();
					display.getSalasList().getStore().removeAll();
					display.getSalasList().getStore().add(grupoSalaDTO, true);
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
				
				if(fileModel instanceof CurriculoDisciplinaDTO) {
					cdDTO = (CurriculoDisciplinaDTO)fileModel;
					TurnoDTO turnoDTO = (TurnoDTO)display.getSalasList().getStore().getParent(cdDTO);
					CurriculoDTO curriculoDTO = (CurriculoDTO)display.getSalasList().getStore().getParent(turnoDTO);
					FileModel fileModelRoot = display.getSalasList().getStore().getParent(curriculoDTO);
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
