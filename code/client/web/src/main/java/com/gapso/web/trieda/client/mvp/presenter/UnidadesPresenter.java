package com.gapso.web.trieda.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.view.SalasView;
import com.gapso.web.trieda.client.mvp.view.UnidadeFormView;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.UnidadesServiceAsync;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class UnidadesPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getDeslocamentoUnidadesButton();
		Button getSalasButton();
		TextField<String> getNomeBuscaTextField();
		TextField<String> getCodigoBuscaTextField();
		CampusComboBox getCampusBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<UnidadeDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<UnidadeDTO>> proxy);
	}
	private Display display; 
	private GTab gTab;
	
	public UnidadesPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final UnidadesServiceAsync service = Services.unidades();
		RpcProxy<PagingLoadResult<UnidadeDTO>> proxy = new RpcProxy<PagingLoadResult<UnidadeDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<UnidadeDTO>> callback) {
//				service.getList((PagingLoadConfig)loadConfig, callback);
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				CampusDTO campusDTO = display.getCampusBuscaComboBox().getValue();
				service.getBuscaList(campusDTO, nome, codigo, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new UnidadeFormPresenter(new UnidadeFormView(new UnidadeDTO(), null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final UnidadeDTO unidadeDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final CampiServiceAsync campus = Services.campi();
				campus.getCampus(unidadeDTO.getCampusId(), new AsyncCallback<CampusDTO>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(CampusDTO campusDTO) {
						Presenter presenter = new UnidadeFormPresenter(new UnidadeFormView(unidadeDTO, campusDTO), display.getGrid());
						presenter.go(null);
					}
				});
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<UnidadeDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final UnidadesServiceAsync service = Services.unidades();
				service.remove(list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
					}
				});
			}
		});
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getNomeBuscaTextField().setValue(null);
				display.getCodigoBuscaTextField().setValue(null);
				display.getCampusBuscaComboBox().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
		display.getSalasButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SalasPresenter(new SalasView());
				presenter.go(gTab);
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
