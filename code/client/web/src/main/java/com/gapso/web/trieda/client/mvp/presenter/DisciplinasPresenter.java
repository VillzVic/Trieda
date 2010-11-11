
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
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.view.DisciplinaFormView;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.TipoDisciplinaComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinasPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		
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
