package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.client.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
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
	
	public CampiDeslocamentoPresenter(Display display) {
		this.display = display;
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
				service.saveDeslocamento(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
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
