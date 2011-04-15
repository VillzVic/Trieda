package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.services.UnidadesServiceAsync;
import com.gapso.web.trieda.main.client.util.view.CampusComboBox;
import com.gapso.web.trieda.main.client.util.view.SimpleGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class UnidadeFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getNomeTextField();
		TextField<String> getCodigoTextField();
		CampusComboBox getCampusComboBox();
		UnidadeDTO getUnidadeDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
		
		TriedaI18nConstants getI18nConstants();
		TriedaI18nMessages getI18nMessages();
	}
	private SimpleGrid<UnidadeDTO> gridPanel;
	private Display display;
	
	public UnidadeFormPresenter(Display display) {
		this(display, null);
	}
	public UnidadeFormPresenter(Display display, SimpleGrid<UnidadeDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final UnidadesServiceAsync service = Services.unidades();
					final UnidadeDTO dto = getDTO();
					service.save(dto, new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							if(gridPanel != null) {
								gridPanel.updateList();
							}
							Info.display(
								display.getI18nConstants().informacao(),
								display.getI18nMessages().sucessoSalvarNoBD(dto.getCodigo())
							);
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
	
	private UnidadeDTO getDTO() {
		UnidadeDTO unidadeDTO = display.getUnidadeDTO();
		unidadeDTO.setNome(display.getNomeTextField().getValue());
		unidadeDTO.setCodigo(display.getCodigoTextField().getValue());
		unidadeDTO.setCampusId(display.getCampusComboBox().getSelection().get(0).getId());
		unidadeDTO.setCampusString(display.getCampusComboBox().getSelection().get(0).getCodigo());
		return unidadeDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
