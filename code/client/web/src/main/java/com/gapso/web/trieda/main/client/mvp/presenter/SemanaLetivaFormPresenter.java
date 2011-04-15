package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class SemanaLetivaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getCodigoTextField();
		TextField<String> getDescricaoTextField();
		SemanaLetivaDTO getSemanaLetivaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<SemanaLetivaDTO> gridPanel;
	private Display display;
	
	public SemanaLetivaFormPresenter(Display display, SimpleGrid<SemanaLetivaDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final SemanasLetivaServiceAsync service = Services.semanasLetiva();
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
	
	private SemanaLetivaDTO getDTO() {
		SemanaLetivaDTO dto = display.getSemanaLetivaDTO();
		dto.setCodigo(display.getCodigoTextField().getValue());
		dto.setDescricao(display.getDescricaoTextField().getValue());
		return dto;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
