package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.client.services.ProfessoresDisciplinaServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.ProfessorComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ProfessorDisciplinaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		ProfessorComboBox getProfessorComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		NumberField getPreferenciaNumberField();
		NumberField getNotaDesempenhoNumberField();
		ProfessorDisciplinaDTO getProfessorDisciplinaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<ProfessorDisciplinaDTO> gridPanel;
	private Display display;
	
	public ProfessorDisciplinaFormPresenter(Display display, SimpleGrid<ProfessorDisciplinaDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
					service.save(getDTO(), new AsyncCallback<Void>() {
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
	
	private ProfessorDisciplinaDTO getDTO() {
		ProfessorDisciplinaDTO unidadeDTO = display.getProfessorDisciplinaDTO();
		unidadeDTO.setProfessorId(display.getProfessorComboBox().getValue().getId());
		unidadeDTO.setProfessorString(display.getProfessorComboBox().getValue().getNome());
		unidadeDTO.setProfessorCpf(display.getProfessorComboBox().getValue().getCpf());
		unidadeDTO.setDisciplinaId(display.getDisciplinaComboBox().getValue().getId());
		unidadeDTO.setDisciplinaString(display.getDisciplinaComboBox().getValue().getCodigo());
		
		unidadeDTO.setPreferencia(display.getPreferenciaNumberField().getValue().intValue());
		unidadeDTO.setNotaDesempenho(display.getNotaDesempenhoNumberField().getValue().intValue());
		return unidadeDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
