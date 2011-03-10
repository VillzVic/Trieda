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
import com.gapso.web.trieda.client.mvp.view.EquivalenciaFormView;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class EquivalenciasPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		CampusComboBox getCampusComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid<EquivalenciaDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<EquivalenciaDTO>> proxy);
	}
	private Display display; 
	
	public EquivalenciasPresenter(CenarioDTO cenario, Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<PagingLoadResult<EquivalenciaDTO>> proxy = new RpcProxy<PagingLoadResult<EquivalenciaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<EquivalenciaDTO>> callback) {
				DisciplinaDTO disciplina = display.getDisciplinaComboBox().getValue();
				Services.equivalencias().getBuscaList(disciplina, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new EquivalenciaFormPresenter(new EquivalenciaFormView(new EquivalenciaDTO()), display.getGrid());
				presenter.go(null);
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<EquivalenciaDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				Services.equivalencias().remove(list, new AsyncCallback<Void>() {
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
				display.getCampusComboBox().setValue(null);
				display.getDisciplinaComboBox().setValue(null);
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
