package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.services.CampiServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.main.client.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.ui.Widget;

public class CampiDeslocamentoPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getSaveButton();
		Button getCancelButton();
		Button getSimetricaButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		DeslocamentoGrid<DeslocamentoCampusDTO> getGrid();
		Component getComponent();
	}
	private Display display; 
	private GTab gTab;
	private CenarioDTO cenario;
	
	public CampiDeslocamentoPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		this.cenario = cenario;
//		configureProxy();
		setListeners();
	}
	
//	private void configureProxy() {
//		CampiServiceAsync service = Services.campi();
//		final FutureResult<List<DeslocamentoCampusDTO>> futureDeslocamentoCampusDTOList = new FutureResult<List<DeslocamentoCampusDTO>>();
//		service.getDeslocamentos(futureDeslocamentoCampusDTOList);
//		
//		FutureSynchronizer synch = new FutureSynchronizer(futureDeslocamentoCampusDTOList);
//		synch.addCallback(new AsyncCallback<Boolean>() {
//			@Override
//			public void onFailure(Throwable caught) {
//				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
//			}
//			@Override
//			public void onSuccess(Boolean result) {
//				display.getGrid().updateList(futureDeslocamentoCampusDTOList.result());	
//			}
//		});
//	}
	
	private void setListeners() {
		display.getSaveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DeslocamentoCampusDTO> list = display.getGrid().getStore().getModels();
				CampiServiceAsync service = Services.campi();
				service.saveDeslocamento(cenario, list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().getStore().commitChanges();
						Info.display("Salvo", "Deslocamentos atualizado com sucesso");
					}
				});
			}
		});
		display.getCancelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().getStore().rejectChanges();
				Info.display("Cancelado", "Alterações canceladas com sucesso");
			}
		});
		display.getSimetricaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().igualarOrigemDestino();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
