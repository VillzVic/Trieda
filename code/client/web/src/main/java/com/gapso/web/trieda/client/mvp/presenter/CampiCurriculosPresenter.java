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
import com.gapso.web.trieda.client.mvp.model.CampusCurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.view.CampusCurriculoFormView;
import com.gapso.web.trieda.client.services.CampiCurriculosServiceAsync;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.CurriculosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class CampiCurriculosPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		TurnoComboBox getTurnoBuscaComboBox();
		CampusComboBox getCampusBuscaComboBox();
		CursoComboBox getCursoBuscaComboBox();
		CurriculoComboBox getCurriculoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<CampusCurriculoDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<CampusCurriculoDTO>> proxy);
	}
	private Display display; 
	
	public CampiCurriculosPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CampiCurriculosServiceAsync service = Services.campiCurriculos();
		RpcProxy<PagingLoadResult<CampusCurriculoDTO>> proxy = new RpcProxy<PagingLoadResult<CampusCurriculoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<CampusCurriculoDTO>> callback) {
				TurnoDTO turnoDTO = display.getTurnoBuscaComboBox().getValue();
				CampusDTO campusDTO = display.getCampusBuscaComboBox().getValue();
				CursoDTO cursoDTO = display.getCursoBuscaComboBox().getValue();
				CurriculoDTO curriculoDTO = display.getCurriculoBuscaComboBox().getValue();
				service.getBuscaList(turnoDTO, campusDTO, cursoDTO, curriculoDTO, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CampusCurriculoFormPresenter(new CampusCurriculoFormView(new CampusCurriculoDTO(), null, null, null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final CampusCurriculoDTO campusCurriculoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final TurnosServiceAsync turnosService = Services.turnos();
				final CampiServiceAsync campiService = Services.campi();
				final CurriculosServiceAsync curriculosService = Services.curriculos();
				
				final FutureResult<TurnoDTO> futureTurnoDTO = new FutureResult<TurnoDTO>();
				final FutureResult<CampusDTO> futureCampusDTO = new FutureResult<CampusDTO>();
				final FutureResult<CurriculoDTO> futureCurriculoDTO = new FutureResult<CurriculoDTO>();
				
				turnosService.getTurno(campusCurriculoDTO.getTurnoId(), futureTurnoDTO);
				campiService.getCampus(campusCurriculoDTO.getCampusId(), futureCampusDTO);
				curriculosService.getCurriculo(campusCurriculoDTO.getMatrizCurricularId(), futureCurriculoDTO);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureTurnoDTO, futureCampusDTO, futureCurriculoDTO);
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Boolean result) {
						TurnoDTO turnoDTO = futureTurnoDTO.result();
						CampusDTO campusDTO = futureCampusDTO.result();
						CurriculoDTO curriculoDTO = futureCurriculoDTO.result();
						Presenter presenter = new CampusCurriculoFormPresenter(new CampusCurriculoFormView(campusCurriculoDTO, turnoDTO, campusDTO, curriculoDTO), display.getGrid());
						presenter.go(null);	
					}
				});
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CampusCurriculoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final CampiCurriculosServiceAsync service = Services.campiCurriculos();
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
				display.getTurnoBuscaComboBox().setValue(null);
				display.getCampusBuscaComboBox().setValue(null);
				display.getCursoBuscaComboBox().setValue(null);
				display.getCurriculoBuscaComboBox().setValue(null);
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
		GTab gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
