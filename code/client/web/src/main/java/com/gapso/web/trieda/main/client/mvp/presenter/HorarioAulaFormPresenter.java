package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.user.client.ui.Widget;

public class HorarioAulaFormPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getSalvarButton();
		SemanaLetivaComboBox getSemanaLetivaComboBox();
		TurnoComboBox getTurnoComboBox();
		TextField<String> getHorarioInicioTextField();
		HorarioAulaDTO getHorarioAulaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private SimpleGrid<HorarioAulaDTO> gridPanel;
	private Display display;
	
	public HorarioAulaFormPresenter(Display display, SimpleGrid<HorarioAulaDTO> gridPanel) {
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					Services.horariosAula().save(getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
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
	
	private HorarioAulaDTO getDTO() {
		HorarioAulaDTO dto = display.getHorarioAulaDTO();
		dto.setSemanaLetivaId(display.getSemanaLetivaComboBox().getSelection().get(0).getId());
		dto.setTurnoId(display.getTurnoComboBox().getSelection().get(0).getId());
		DateTimeFormat df = DateTimeFormat.getFormat("HH:mm");
		dto.setInicio(df.parse(display.getHorarioInicioTextField().getValue()));
		return dto;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
