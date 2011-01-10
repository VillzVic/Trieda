package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class HorarioDisponivelProfessorFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		SimpleModal getSimpleModal();
		ProfessorDTO getProfessorDTO();
		void setProxy(RpcProxy<ListLoadResult<HorarioDisponivelCenarioDTO>> proxy);
		ListStore<HorarioDisponivelCenarioDTO> getStore();
	}
	private Display display;
	private CenarioDTO cenario;
	
	public HorarioDisponivelProfessorFormPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<ListLoadResult<HorarioDisponivelCenarioDTO>> proxy = new RpcProxy<ListLoadResult<HorarioDisponivelCenarioDTO>>() {
			@Override
			protected void load(Object loadConfig, AsyncCallback<ListLoadResult<HorarioDisponivelCenarioDTO>> callback) {
				Services.semanasLetiva().getHorariosDisponiveisByCenario(cenario, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getStore().commitChanges();
				List<HorarioDisponivelCenarioDTO> hdcDTOList = display.getStore().getModels();
				
				Services.professores().saveHorariosDisponiveis(getDTO(), hdcDTOList, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(Void result) {
						Info.display("Atualizado", "Horários atualizados com sucesso!");
						display.getSimpleModal().hide();
					}
					
				});
			}
		});
		
	}

	
	private ProfessorDTO getDTO() {
		return display.getProfessorDTO();
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}
}