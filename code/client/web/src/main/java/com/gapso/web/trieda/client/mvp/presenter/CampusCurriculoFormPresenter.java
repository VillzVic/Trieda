package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CampusCurriculoDTO;
import com.gapso.web.trieda.client.services.CampiCurriculosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CampusCurriculoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TurnoComboBox getTurnoComboBox();
		CampusComboBox getCampusComboBox();
		CurriculoComboBox getCurriculoComboBox();
		CampusCurriculoDTO getCampusCurriculoDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<CampusCurriculoDTO> gridPanel;
	private Display display;
	
	public CampusCurriculoFormPresenter(Display display, SimpleGrid<CampusCurriculoDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CampiCurriculosServiceAsync service = Services.campiCurriculos();
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
	
	private CampusCurriculoDTO getDTO() {
		CampusCurriculoDTO campusCurriculoDTO = display.getCampusCurriculoDTO();
		campusCurriculoDTO.setTurnoId(display.getTurnoComboBox().getSelection().get(0).getId());
		campusCurriculoDTO.setTurnoString(display.getTurnoComboBox().getSelection().get(0).getNome());
		campusCurriculoDTO.setCampusId(display.getCampusComboBox().getSelection().get(0).getId());
		campusCurriculoDTO.setCampusString(display.getCampusComboBox().getSelection().get(0).getCodigo());
		campusCurriculoDTO.setMatrizCurricularId(display.getCurriculoComboBox().getSelection().get(0).getId());
		campusCurriculoDTO.setMatrizCurricularString(display.getCurriculoComboBox().getSelection().get(0).getCodigo());
		return campusCurriculoDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
