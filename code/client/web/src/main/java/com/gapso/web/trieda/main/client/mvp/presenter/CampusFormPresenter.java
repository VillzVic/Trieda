package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.TriedaCurrency;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class CampusFormPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getSalvarButton();
		TextField<String> getNomeTextField();
		TextField<String> getCodigoTextField();
		NumberField getValorCreditoNumberField();
		EstadoComboBox getEstadoComboBox();
		TextField<String> getMunicipioTextField();
		TextField<String> getBairroTextField();
		CheckBox getPublicadoCheckBox();
		CampusDTO getCampusDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private CenarioDTO cenario;
	private SimpleGrid<CampusDTO> gridPanel;
	private Display display;
	
	public CampusFormPresenter(CenarioDTO cenario, Display display) {
		this(cenario, display, null);
	}
	public CampusFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<CampusDTO> gridPanel) {
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
					final CampiServiceAsync service = Services.campi();
					service.save(getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							if(gridPanel != null) {
								gridPanel.updateList();
							}
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
	
	private CampusDTO getDTO() {
		CampusDTO campusDTO = display.getCampusDTO();
		campusDTO.setCenarioId(cenario.getId());
		campusDTO.setNome(display.getNomeTextField().getValue());
		campusDTO.setCodigo(display.getCodigoTextField().getValue());
		campusDTO.setValorCredito( new TriedaCurrency( display.getValorCreditoNumberField().getValue().doubleValue() ) );
		campusDTO.setEstado(display.getEstadoComboBox().getValue().getValue().name());
		campusDTO.setMunicipio(display.getMunicipioTextField().getValue());
		campusDTO.setBairro(display.getBairroTextField().getValue());
		campusDTO.setPublicado(display.getPublicadoCheckBox().getValue());
		return campusDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
