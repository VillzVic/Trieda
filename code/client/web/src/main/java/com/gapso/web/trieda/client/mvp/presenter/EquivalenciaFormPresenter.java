package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.EquivalenciaDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class EquivalenciaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		CursoComboBox getCursoComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		ListView<CursoDTO> getCursosList();
		ListView<DisciplinaDTO> getDisciplinasNaoPertencesList();
		ListView<DisciplinaDTO> getDisciplinasPertencesList();
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
