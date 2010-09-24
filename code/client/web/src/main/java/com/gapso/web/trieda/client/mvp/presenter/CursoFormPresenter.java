package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.services.CursosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoCursoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CursoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getNomeTextField();
		TextField<String> getCodigoTextField();
		TipoCursoComboBox getTipoCursoComboBox();
		NumberField getNumMinDoutoresTextField();
		NumberField getNumMinMestresTextField();
		NumberField getMaxDisciplinasPeloProfessorTextField();
		CheckBox getAdmMaisDeUmDisciplinaCheckBox();
		CursoDTO getCursoDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<CursoDTO> gridPanel;
	private Display display;
	
	public CursoFormPresenter(Display display, SimpleGrid<CursoDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CursosServiceAsync service = Services.cursos();
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
	
	private CursoDTO getDTO() {
		CursoDTO cursoDTO = display.getCursoDTO();
		cursoDTO.setNome(display.getNomeTextField().getValue());
		cursoDTO.setCodigo(display.getCodigoTextField().getValue());
		cursoDTO.setNumMinDoutores(display.getNumMinDoutoresTextField().getValue().intValue());
		cursoDTO.setNumMinMestres(display.getNumMinMestresTextField().getValue().intValue());
		cursoDTO.setMaxDisciplinasPeloProfessor(display.getMaxDisciplinasPeloProfessorTextField().getValue().intValue());
		cursoDTO.setAdmMaisDeUmDisciplina(display.getAdmMaisDeUmDisciplinaCheckBox().getValue());
		cursoDTO.setTipoId(display.getTipoCursoComboBox().getSelection().get(0).getId());
		cursoDTO.setTipoString(display.getTipoCursoComboBox().getSelection().get(0).getCodigo());
		return cursoDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
