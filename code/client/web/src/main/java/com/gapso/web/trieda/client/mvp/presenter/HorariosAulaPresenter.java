package com.gapso.web.trieda.client.mvp.presenter;

import java.util.Date;
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
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.view.HorarioAulaFormView;
import com.gapso.web.trieda.client.services.HorariosAulaServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CalendarioComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class HorariosAulaPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TextField<String> getHorarioBuscaTextField();
		CalendarioComboBox getCalendarioBuscaComboBox();
		TurnoComboBox getTurnoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<HorarioAulaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<HorarioAulaDTO>> proxy);
	}
	private Display display; 
	
	public HorariosAulaPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final HorariosAulaServiceAsync service = Services.horariosAula();
		RpcProxy<PagingLoadResult<HorarioAulaDTO>> proxy = new RpcProxy<PagingLoadResult<HorarioAulaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<HorarioAulaDTO>> callback) {
//				service.getList((PagingLoadConfig)loadConfig, callback);
				DateTimeFormat df = DateTimeFormat.getFormat("HH:mm");
				String horarioString = display.getHorarioBuscaTextField().getValue();
				Date horario = (horarioString == null)? null : df.parse(horarioString);
				TurnoDTO turnoDTO = display.getTurnoBuscaComboBox().getValue();
				CalendarioDTO calendarioDTO = display.getCalendarioBuscaComboBox().getValue();
				service.getBuscaList(calendarioDTO, turnoDTO, horario, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new HorarioAulaFormPresenter(new HorarioAulaFormView(new HorarioAulaDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				HorarioAulaDTO horarioAulaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new HorarioAulaFormPresenter(new HorarioAulaFormView(horarioAulaDTO), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<HorarioAulaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final HorariosAulaServiceAsync service = Services.horariosAula();
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
				display.getHorarioBuscaTextField().setValue(null);
				display.getCalendarioBuscaComboBox().setValue(null);
				display.getTurnoBuscaComboBox().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
