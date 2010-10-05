
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
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.mvp.view.CursoFormView;
import com.gapso.web.trieda.client.services.CursosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TiposCursosServiceAsync;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.TipoCursoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CursosPresenter implements Presenter {

	public interface Display {
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		Button getExportExcelButton();
		
		TextField<String> getNomeBuscaTextField();
		TextField<String> getCodigoBuscaTextField();
		TipoCursoComboBox getTipoCursoBuscaComboBox();
		
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		
		SimpleGrid<CursoDTO> getGrid();
		Component getComponent();
		void setProxy(RpcProxy<PagingLoadResult<CursoDTO>> proxy);
	}
	private Display display; 
	private GTab gTab;
	
	public CursosPresenter(Display display) {
		this.display = display;
		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		final CursosServiceAsync service = Services.cursos();
		RpcProxy<PagingLoadResult<CursoDTO>> proxy = new RpcProxy<PagingLoadResult<CursoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<CursoDTO>> callback) {
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				TipoCursoDTO tipoCursoDTO = display.getTipoCursoBuscaComboBox().getValue();
				service.getBuscaList(nome, codigo, tipoCursoDTO, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				Presenter presenter = new CursoFormPresenter(new CursoFormView(new CursoDTO(), null), display.getGrid());
				presenter.go(null);
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final CursoDTO cursoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final TiposCursosServiceAsync service = Services.tiposCursos();
				service.getTipoCurso(cursoDTO.getTipoId(), new AsyncCallback<TipoCursoDTO>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(TipoCursoDTO tipoCursoDTO) {
						Presenter presenter = new CursoFormPresenter(new CursoFormView(cursoDTO, tipoCursoDTO), display.getGrid());
						presenter.go(null);
					}
				});
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final CursosServiceAsync service = Services.cursos();
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
				display.getTipoCursoBuscaComboBox().setValue(null);
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