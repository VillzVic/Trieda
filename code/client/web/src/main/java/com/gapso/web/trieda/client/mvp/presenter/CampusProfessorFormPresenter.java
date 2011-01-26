package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorCampusDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CampusProfessorFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		CampusComboBox getCampusComboBox();
		ListView<ProfessorDTO> getProfessorNaoAssociadoList();
		ListView<ProfessorDTO> getProfessorAssociadoList();
		ProfessorCampusDTO getProfessorCampusDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private CenarioDTO cenario;
	private SimpleGrid<ProfessorCampusDTO> gridPanel;
	private Display display;
	
	public CampusProfessorFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<ProfessorCampusDTO> gridPanel) {
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<ListLoadResult<ProfessorDTO>> proxyNaoVinculada = new RpcProxy<ListLoadResult<ProfessorDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<ProfessorDTO>> callback) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				Services.professores().getList(callback);
			}
		};
		display.getProfessorNaoAssociadoList().setStore(new ListStore<ProfessorDTO>(new BaseListLoader<ListLoadResult<ProfessorDTO>>(proxyNaoVinculada)));
		RpcProxy<ListLoadResult<ProfessorDTO>> proxyVinculada = new RpcProxy<ListLoadResult<ProfessorDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<ProfessorDTO>> callback) {
				CampusDTO campusDTO = display.getCampusComboBox().getValue();
				Services.professores().getList(callback);
			}
		};
		display.getProfessorAssociadoList().setStore(new ListStore<ProfessorDTO>(new BaseListLoader<ListLoadResult<ProfessorDTO>>(proxyNaoVinculada)));
	}
	
	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
//				if(isValid()) {
//					final TurnosServiceAsync service = Services.turnos();
//					service.save(getDTO(), new AsyncCallback<Void>() {
//						@Override
//						public void onFailure(Throwable caught) {
//							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
//						}
//						@Override
//						public void onSuccess(Void result) {
//							display.getSimpleModal().hide();
//							gridPanel.updateList();
//							+
//							Info.display("Salvo", "Item salvo com sucesso!");
//						}
//					});
//				} else {
//					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
//				}
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}
	
//	private TurnoDTO getDTO() {
//		TurnoDTO turnoDTO = display.getTurnoDTO();
//		turnoDTO.setCenarioId(cenario.getId());
//		turnoDTO.setNome(display.getNomeTextField().getValue());
//		turnoDTO.setTempo(display.getTempoTextField().getValue().intValue());
//		return turnoDTO;
//	}

	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
