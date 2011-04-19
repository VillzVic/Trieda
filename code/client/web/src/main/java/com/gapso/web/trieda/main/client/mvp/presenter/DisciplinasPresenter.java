
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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.DivisaoCreditoDisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelDisciplinaFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoDisciplinaComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinasPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		Button getDivisaoCreditoButton();
		Button getDisponibilidadeButton();
		
		TextField<String> getNomeBuscaTextField();
		TextField<String> getCodigoBuscaTextField();
		TipoDisciplinaComboBox getTipoDisciplinaBuscaComboBox();
		
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		
		SimpleGrid<DisciplinaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<DisciplinaDTO>> proxy);
	}
	private CenarioDTO cenario;
	private Display display; 
	private GTab gTab;
	
	public DisciplinasPresenter(CenarioDTO cenario, Display display) {
		this.cenario = cenario;
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final DisciplinasServiceAsync service = Services.disciplinas();
		RpcProxy<PagingLoadResult<DisciplinaDTO>> proxy = new RpcProxy<PagingLoadResult<DisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<DisciplinaDTO>> callback) {
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				TipoDisciplinaDTO tipoDisciplinaDTO = display.getTipoDisciplinaBuscaComboBox().getValue();
				service.getBuscaList(nome, codigo, tipoDisciplinaDTO, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new DisciplinaFormPresenter(cenario, new DisciplinaFormView(new DisciplinaDTO(), null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final DisciplinaDTO disciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final DisciplinasServiceAsync service = Services.disciplinas();
				service.getTipoDisciplina(disciplinaDTO.getTipoId(), new AsyncCallback<TipoDisciplinaDTO>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(TipoDisciplinaDTO tipoDisciplinaDTO) {
						Presenter presenter = new DisciplinaFormPresenter(cenario, new DisciplinaFormView(disciplinaDTO, tipoDisciplinaDTO), display.getGrid());
						presenter.go(null);
					}
				});
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DisciplinaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final DisciplinasServiceAsync service = Services.disciplinas();
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
		display.getImportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				ImportExcelFormView importExcelFormView = new ImportExcelFormView(ExcelInformationType.DISCIPLINAS,display.getGrid());
				importExcelFormView.show();
			}
		});
		display.getExportExcelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(ExcelInformationType.DISCIPLINAS,display.getI18nConstants(),display.getI18nMessages());
				e.submit();
			}
		});
		display.getDisponibilidadeButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final DisciplinaDTO disciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				SemanaLetivaDTO semanaLetivaDTO = new SemanaLetivaDTO();
				semanaLetivaDTO.setId(cenario.getSemanaLetivaId());
				
				Services.disciplinas().getHorariosDisponiveis(disciplinaDTO, semanaLetivaDTO, new AsyncCallback<List<HorarioDisponivelCenarioDTO>>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(List<HorarioDisponivelCenarioDTO> result) {
						SemanaLetivaDTO semanaLetiva = new SemanaLetivaDTO();
						semanaLetiva.setId(cenario.getSemanaLetivaId());
						Presenter presenter = new HorarioDisponivelDisciplinaFormPresenter(cenario, semanaLetiva, new HorarioDisponivelDisciplinaFormView(disciplinaDTO, result));
						presenter.go(null);
					}
				});
			}
		});
		display.getDivisaoCreditoButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final DisciplinaDTO disciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Services.disciplinas().getDivisaoCredito(disciplinaDTO, new AbstractAsyncCallbackWithDefaultOnFailure<DivisaoCreditoDTO>(display) {
					@Override
					public void onSuccess(DivisaoCreditoDTO result) {
						Presenter presenter = new DivisaoCreditoDisciplinaFormPresenter(cenario, new DivisaoCreditoDisciplinaFormView(result, disciplinaDTO));
						presenter.go(null);
					}
				});
			}
		});
		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getNomeBuscaTextField().setValue(null);
				display.getCodigoBuscaTextField().setValue(null);
				display.getTipoDisciplinaBuscaComboBox().setValue(null);
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
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
