package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class EquivalenciaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		CursoComboBox getCursoComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		ListView<CursoDTO> getCursosList();
		ListView<DisciplinaDTO> getDisciplinasNaoPertencesList();
		ListView<DisciplinaDTO> getDisciplinasPertencesList();
		Button getAtualizaDisciplinasDoCursoBT();
		Button getAdicionaDisciplinasBT();
		Button getRemoveDisciplinasBT();
		EquivalenciaDTO getEquivalenciaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<EquivalenciaDTO> gridPanel;
	private Display display;
	
	public EquivalenciaFormPresenter(Display display, SimpleGrid<EquivalenciaDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
		populaListas();
	}

	private void populaListas() {
		final FutureResult<ListLoadResult<CursoDTO>> futureCursoDTOList = new FutureResult<ListLoadResult<CursoDTO>>();
		Services.cursos().getListAll(futureCursoDTOList);
		FutureSynchronizer synch = new FutureSynchronizer(futureCursoDTOList);
		synch.addCallback(new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(Boolean result) {
				ListLoadResult<CursoDTO> cursoDTOList = futureCursoDTOList.result();
				
				ListStore<CursoDTO> storeCurso = display.getCursosList().getStore();  
				storeCurso.add(cursoDTOList.getData());
				display.getCursosList().setStore(storeCurso);
				display.getCursosList().refresh();
			}
		});
	}
	
	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					Services.equivalencias().save(getDTO(), getDisciplinasSelecionadas(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							gridPanel.updateList();
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
				}
			}
		});
		
		display.getAtualizaDisciplinasDoCursoBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursoSelecionadoList = display.getCursosList().getSelectionModel().getSelectedItems();
				
				final FutureResult<ListLoadResult<DisciplinaDTO>> futureDisciplinaDTOList = new FutureResult<ListLoadResult<DisciplinaDTO>>();
				Services.disciplinas().getListByCursos(cursoSelecionadoList, futureDisciplinaDTOList);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureDisciplinaDTOList);
				synch.addCallback(new AsyncCallback<Boolean>() {

					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}

					@Override
					public void onSuccess(Boolean result) {
						ListLoadResult<DisciplinaDTO> disciplinaDTOList = futureDisciplinaDTOList.result();
						
						display.getDisciplinasNaoPertencesList().getStore().removeAll();
						display.getDisciplinasNaoPertencesList().getStore().add(disciplinaDTOList.getData());
						display.getDisciplinasNaoPertencesList().refresh();
						
						Info.display("Atualizado", "Lista de disciplinas atualizada!");
					}
					
				});
			}
		});
		
		display.getAdicionaDisciplinasBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> disciplinaList = display.getDisciplinasNaoPertencesList().getSelectionModel().getSelectedItems();

				display.getDisciplinasPertencesList().getStore().add(disciplinaList);
				display.getDisciplinasPertencesList().getStore().sort("codigo", SortDir.ASC);
				display.getDisciplinasPertencesList().refresh();
				
				for(DisciplinaDTO d : disciplinaList) {
					display.getDisciplinasNaoPertencesList().getStore().remove(d);
				}
				display.getDisciplinasNaoPertencesList().refresh();
				
				Info.display("Atualizado", "Salas adicionadas a lista!");
			}
		});
		
		display.getRemoveDisciplinasBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> disciplinaList = display.getDisciplinasPertencesList().getSelectionModel().getSelectedItems();

				display.getDisciplinasNaoPertencesList().getStore().add(disciplinaList);
				display.getDisciplinasNaoPertencesList().getStore().sort("codigo", SortDir.ASC);
				display.getDisciplinasNaoPertencesList().refresh();
				
				for(DisciplinaDTO d : disciplinaList) {
					display.getDisciplinasPertencesList().getStore().remove(d);
				}
				display.getDisciplinasPertencesList().refresh();
				
				Info.display("Atualizado", "Salas removidas a lista!");
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}
	
	private EquivalenciaDTO getDTO() {
		EquivalenciaDTO equivalenciaDTO = display.getEquivalenciaDTO();
		equivalenciaDTO.setCursouId(display.getDisciplinaComboBox().getValue().getId());
		equivalenciaDTO.setCursouString(display.getDisciplinaComboBox().getValue().getCodigo());
		return equivalenciaDTO;
	}
	
	private List<DisciplinaDTO> getDisciplinasSelecionadas() {
		return display.getDisciplinasPertencesList().getStore().getModels();
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
