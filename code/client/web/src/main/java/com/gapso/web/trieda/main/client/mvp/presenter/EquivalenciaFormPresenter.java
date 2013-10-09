package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.BaseEvent;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class EquivalenciaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		DisciplinaAutoCompleteBox getDisciplinaCursouComboBox();
		DisciplinaAutoCompleteBox getDisciplinaEliminaComboBox();
		CheckBox getEquivalenciaGeralCheckBox();
		EquivalenciaDTO getEquivalenciaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
		ListView<CursoDTO> getCursosList();
		ListView<CursoDTO> getCursosPertencesList();
		Button getAdicionaCursoBT();
		Button getRemoveCursoBT();
		ContentPanel getAssociadosListPanel();
		ContentPanel getCursosListPanel();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO; 
	private SimpleGrid< EquivalenciaDTO > gridPanel;
	private Display display;
	private CenarioDTO cenarioDTO;

	public EquivalenciaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO,
		Display display, SimpleGrid< EquivalenciaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;
		this.cenarioDTO = cenarioDTO;

		setListeners();
		populaListas();
	}

	private void populaListas() {
		final FutureResult<ListLoadResult<CursoDTO>> futureCursoDTOList = new FutureResult<ListLoadResult<CursoDTO>>();
		Services.cursos().getListAll(cenarioDTO, futureCursoDTOList);
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
					try {
						Services.equivalencias().save(getDTO(), getCursosSelecionados(), new AsyncCallback<Void>() {
							@Override
							public void onFailure(Throwable caught) {
								if (caught instanceof TriedaException)
									MessageBox.alert("ERRO!", caught.getMessage(), null);
								else
									MessageBox.alert("ERRO!", "Deu falha na conexão", null);
							}
							@Override
							public void onSuccess(Void result) {
								display.getSimpleModal().hide();
								gridPanel.updateList();
								Info.display("Salvo", "Item salvo com sucesso!");
							}
						});
					} catch (Exception e) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
				}
			}
		});
		
		
		display.getEquivalenciaGeralCheckBox().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				boolean value = ((CheckBox)be.getSource()).getValue();
				display.getCursosListPanel().setEnabled(!value);
				display.getAssociadosListPanel().setEnabled(!value);
				display.getRemoveCursoBT().setEnabled(!value);
				display.getAdicionaCursoBT().setEnabled(!value);
			}
		});
		
		display.getAdicionaCursoBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursoListSelecionado = display.getCursosList().getSelectionModel().getSelectedItems();
				List<CursoDTO> cursoListInserido = display.getCursosPertencesList().getStore().getModels();

				for ( CursoDTO c : cursoListInserido ) {
					if ( cursoListSelecionado.contains(c) ) {
						Info.display("Erro", "Algum curso(s) selecionado já está associado");
						return;
					}
				}
				display.getCursosPertencesList().getStore().add(cursoListSelecionado);
				display.getCursosPertencesList().getStore().sort("codigo", SortDir.ASC);
				display.getCursosPertencesList().refresh();
				
				Info.display("Atualizado", "Curso(s) adicionado(s) à lista!");
			}
		});
		
		display.getRemoveCursoBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> cursoList = display.getCursosPertencesList().getSelectionModel().getSelectedItems();
				
				for(CursoDTO c : cursoList) {
					display.getCursosPertencesList().getStore().remove(c);
				}
				display.getCursosPertencesList().refresh();
				
				Info.display("Atualizado", "Curso(s) removido(s) da lista!");
			}
		});
	}

	private boolean isValid()
	{
		if ( (getCursosSelecionados() == null || getCursosSelecionados().isEmpty())	
				&& !display.getEquivalenciaGeralCheckBox().getValue() )
			return false;
		return display.isValid();
	}

	private EquivalenciaDTO getDTO()
	{
		EquivalenciaDTO equivalenciaDTO = display.getEquivalenciaDTO();

		equivalenciaDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		equivalenciaDTO.setCursouId( display.getDisciplinaCursouComboBox().getValue().getId() );
		equivalenciaDTO.setCursouString( display.getDisciplinaCursouComboBox().getValue().getCodigo() );
		equivalenciaDTO.setEliminaId( display.getDisciplinaEliminaComboBox().getValue().getId() );
		equivalenciaDTO.setEliminaString( display.getDisciplinaEliminaComboBox().getValue().getCodigo() );
		equivalenciaDTO.setEquivalenciaGeral( display.getEquivalenciaGeralCheckBox().getValue() );

		return equivalenciaDTO;
	}
	
	private List< CursoDTO > getCursosSelecionados()
	{
		return display.getCursosPertencesList().getStore().getModels();
	}
	
	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
