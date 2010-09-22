package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TiposCursosServiceAsync;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class TipoCursoFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getCodigoTextField();
		TextField<String> getDescricaoTextField();
		TipoCursoDTO getTipoCursoDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<TipoCursoDTO> gridPanel;
	private Display display;
	
	public TipoCursoFormPresenter(Display display, SimpleGrid<TipoCursoDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final TiposCursosServiceAsync service = Services.tiposCursos();
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
	
	private TipoCursoDTO getDTO() {
		TipoCursoDTO tipoCursoDTO = display.getTipoCursoDTO();
		tipoCursoDTO.setCodigo(display.getCodigoTextField().getValue());
		tipoCursoDTO.setDescricao(display.getDescricaoTextField().getValue());
		return tipoCursoDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}