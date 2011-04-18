package com.gapso.web.trieda.main.client.mvp.presenter;

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
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelSalaFormView;
import com.gapso.web.trieda.main.client.mvp.view.SalaFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class SalasPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getDisciplinasAssociadasButton();
		Button getGruposDeSalasButton();
		UnidadeComboBox getUnidadeCB();
		CampusComboBox getCampusCB();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<SalaDTO> getGrid();
		GTabItem getGTabItem();
		Button getDisponibilidadeButton();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<SalaDTO>> proxy);
	}

	private Display display;

	public SalasPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final SalasServiceAsync service = Services.salas();
		RpcProxy<PagingLoadResult<SalaDTO>> proxy = new RpcProxy<PagingLoadResult<SalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<SalaDTO>> callback) {
				CampusDTO campusDTO = display.getCampusCB().getValue();
				UnidadeDTO unidadeDTO = display.getUnidadeCB().getValue();
				service.getList(campusDTO, unidadeDTO, (PagingLoadConfig) loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}

	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new SalaFormPresenter(new SalaFormView(new SalaDTO(), null, null, null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final SalaDTO salaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final FutureResult<CampusDTO> futureCampusDTO = new FutureResult<CampusDTO>();
				final FutureResult<UnidadeDTO> futureUnidadeDTO = new FutureResult<UnidadeDTO>();
				final FutureResult<TipoSalaDTO> futureSalaDTO = new FutureResult<TipoSalaDTO>();
				Services.campi().getCampus(salaDTO.getCampusId(), futureCampusDTO);
				Services.unidades().getUnidade(salaDTO.getUnidadeId(), futureUnidadeDTO);
				Services.salas().getTipoSala(salaDTO.getTipoId(), futureSalaDTO);

				FutureSynchronizer synch = new FutureSynchronizer(futureCampusDTO, futureUnidadeDTO, futureSalaDTO);
				synch.addCallback(new AsyncCallback<Boolean>() {
					public void onFailure(Throwable caught) { MessageBox.alert("ERRO!", "Deu falha na conexão", null); }
					public void onSuccess(Boolean result) {
						CampusDTO campusDTO = futureCampusDTO.result();
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();
						TipoSalaDTO tipoSalaDTO = futureSalaDTO.result();
						Presenter presenter = new SalaFormPresenter(new SalaFormView(salaDTO, campusDTO, unidadeDTO, tipoSalaDTO), display.getGrid());
						presenter.go(null);
					}
				});

			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final List<SalaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final SalasServiceAsync service = Services.salas();
				service.remove(list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display(display.getI18nConstants().informacao(), display.getI18nMessages().sucessoRemoverDoBD(list.toString()));
					}
				});
			}
		});
		display.getImportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				ImportExcelFormView importExcelFormView = new ImportExcelFormView(ExcelInformationType.SALAS,display.getGrid());
				importExcelFormView.show();
			}
		});
		display.getExportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(ExcelInformationType.SALAS,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
			}
		});
		display.getDisponibilidadeButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final SalaDTO salaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final FutureResult<UnidadeDTO> futureUnidadeDTO = new FutureResult<UnidadeDTO>();
				final FutureResult<List<HorarioDisponivelCenarioDTO>> futureHorarioDisponivelCenarioDTOList = new FutureResult<List<HorarioDisponivelCenarioDTO>>();
				
				Services.unidades().getUnidade(salaDTO.getUnidadeId(), futureUnidadeDTO);
				Services.salas().getHorariosDisponiveis(salaDTO, futureHorarioDisponivelCenarioDTOList);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureUnidadeDTO, futureHorarioDisponivelCenarioDTOList);
				synch.addCallback(new AsyncCallback<Boolean>() {
					public void onFailure(Throwable caught) { MessageBox.alert("ERRO!", "Deu falha na conexão", null); }
					public void onSuccess(Boolean result) {
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();
						List<HorarioDisponivelCenarioDTO> horarioDisponivelCenarioDTOList = futureHorarioDisponivelCenarioDTOList.result();
						Presenter presenter = new HorarioDisponivelSalaFormPresenter(unidadeDTO, new HorarioDisponivelSalaFormView(salaDTO, horarioDisponivelCenarioDTOList));
						presenter.go(null);
					}
				});
			}
		});
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getUnidadeCB().setValue(null);
				display.getCampusCB().setValue(null);
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
		GTab tab = (GTab) widget;
		tab.add((GTabItem) display.getComponent());
	}
}