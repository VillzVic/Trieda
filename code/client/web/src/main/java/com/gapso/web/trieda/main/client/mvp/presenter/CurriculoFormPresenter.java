package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.services.CurriculosServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.CursoComboBox;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CurriculoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getCodigoTextField();
		TextField<String> getDescricaoTextField();
		CursoComboBox getCursoComboBox();
		CurriculoDTO getCurriculoDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private CenarioDTO cenario;
	private SimpleGrid<CurriculoDTO> gridPanel;
	private Display display;
	
	public CurriculoFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<CurriculoDTO> gridPanel) {
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CurriculosServiceAsync service = Services.curriculos();
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
	
	private CurriculoDTO getDTO() {
		CurriculoDTO curriculoDTO = display.getCurriculoDTO();
		curriculoDTO.setCenarioId(cenario.getId());
		curriculoDTO.setCodigo(display.getCodigoTextField().getValue());
		curriculoDTO.setDescricao(display.getDescricaoTextField().getValue());
		curriculoDTO.setCursoId(display.getCursoComboBox().getSelection().get(0).getId());
		curriculoDTO.setCursoString(display.getCursoComboBox().getSelection().get(0).getCodigo());
		return curriculoDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
