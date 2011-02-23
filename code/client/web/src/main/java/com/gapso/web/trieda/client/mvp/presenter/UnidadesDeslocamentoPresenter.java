package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.UnidadesServiceAsync;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class UnidadesDeslocamentoPresenter implements Presenter {

	public interface Display {
		Button getSaveButton();
		Button getCancelButton();
		Button getSimetricaButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		CampusComboBox getCampusComboBox();
		DeslocamentoGrid<DeslocamentoUnidadeDTO> getGrid();
		Component getComponent();
		CampusDTO getCampusDTO();
	}
	private Display display; 
	private GTab gTab;
	
	public UnidadesDeslocamentoPresenter(Display display) {
		this.display = display;
//		configureProxy();
		setListeners();
	}
	
//	private void configureProxy() {
//		UnidadesServiceAsync service = Services.unidades();
//		CampusDTO campusDTO = display.getCampusDTO();
//		
//		final FutureResult<List<DeslocamentoUnidadeDTO>> futureDeslocamentoUnidadeDTOList = new FutureResult<List<DeslocamentoUnidadeDTO>>();
//		service.getDeslocamento(campusDTO, futureDeslocamentoUnidadeDTOList);
//		
//		FutureSynchronizer synch = new FutureSynchronizer(futureDeslocamentoUnidadeDTOList);
//		synch.addCallback(new AsyncCallback<Boolean>() {
//			@Override
//			public void onFailure(Throwable caught) {
//				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
//			}
//			@Override
//			public void onSuccess(Boolean result) {
//				display.getGrid().updateList(futureDeslocamentoUnidadeDTOList.result());	
//			}
//		});
//	}
	
	private void setListeners() {
		display.getSaveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DeslocamentoUnidadeDTO> list = display.getGrid().getStore().getModels();
				UnidadesServiceAsync service = Services.unidades();
				service.saveDeslocamento(display.getCampusDTO(), list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
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
		display.getCampusComboBox().addSelectionChangedListener(new SelectionChangedListener<CampusDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
				CampusDTO campusDTO = se.getSelectedItem();
				UnidadesServiceAsync service = Services.unidades();
				service.getDeslocamento(campusDTO, new AsyncCallback<List<DeslocamentoUnidadeDTO>>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}

					@Override
					public void onSuccess(List<DeslocamentoUnidadeDTO> result) {
						display.getGrid().updateList(result);
					}
				});
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
