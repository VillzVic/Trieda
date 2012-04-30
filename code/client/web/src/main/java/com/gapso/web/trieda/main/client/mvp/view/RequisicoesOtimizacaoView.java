package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.core.XTemplate;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.WindowEvent;
import com.extjs.gxt.ui.client.event.WindowListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.store.StoreListener;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.RowExpander;
import com.gapso.web.trieda.main.client.command.ICommand;
import com.gapso.web.trieda.main.client.command.util.CommandFactory;
import com.gapso.web.trieda.main.client.command.util.CommandSelectionListener;
import com.gapso.web.trieda.main.client.mvp.presenter.RequisicoesOtimizacaoPresenter;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO.StatusRequisicaoOtimizacao;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RequisicoesOtimizacaoView extends MyComposite {

	private boolean showEmpty;
	private RequisicoesOtimizacaoPresenter presenter;
	private Window window;
	private Grid<RequisicaoOtimizacaoDTO> requisicoesGrid;
	
	public RequisicoesOtimizacaoView(boolean showEmpty, RequisicoesOtimizacaoPresenter presenter) {
		this.showEmpty = showEmpty;
		this.presenter = presenter;
		this.presenter.setI18NGateway(this);
		criaComponentes();
	}
	
	private void criaComponentes() {
		window = new Window();
		window.setHeading("Requisições de Otimização");
		window.setModal(true);
		window.setAutoWidth(true);
		window.setAutoHeight(true);
		window.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeConsultaRequisicao16()));
		window.addWindowListener(new WindowListener() {
			@Override
			public void windowDeactivate(WindowEvent we) {
				super.windowDeactivate(we);
				presenter.removeRequisicoesDeOtimizacaoPorStatus(StatusRequisicaoOtimizacao.FINALIZADA_SEM_OUTPUT);
			}

			@Override
			public void windowHide(WindowEvent we) {
				super.windowHide(we);
				presenter.removeRequisicoesDeOtimizacaoPorStatus(StatusRequisicaoOtimizacao.FINALIZADA_SEM_OUTPUT);
			}
		});
		
		RpcProxy<List<RequisicaoOtimizacaoDTO>> requisicoesProxy = new RpcProxy<List<RequisicaoOtimizacaoDTO>>() {
			@Override
			protected void load(Object loadConfig,AsyncCallback<List<RequisicaoOtimizacaoDTO>> callback) {
				presenter.consultaRequisicoesDeOtimizacao(callback);
			}
		};
		ListLoader<ListLoadResult<RequisicaoOtimizacaoDTO>> requisicoesListLoader = new BaseListLoader<ListLoadResult<RequisicaoOtimizacaoDTO>>(requisicoesProxy);
		ListStore<RequisicaoOtimizacaoDTO> requisicoesListStore = new ListStore<RequisicaoOtimizacaoDTO>(requisicoesListLoader);
		
		RowExpander requisicoesRowExpander = new RowExpander();
		requisicoesRowExpander.setTemplate(XTemplate.create(
			"<table>" +
				"<tr> <td><b>Modo Otimização</b></td> <td>{"+RequisicaoOtimizacaoDTO.PROPERTY_MODO_OTIMIZACAO+"}</td> </tr>" +
				"<tr> <td><b>Otimizar por</b></td> <td>{"+RequisicaoOtimizacaoDTO.PROPERTY_OTIMIZAR_POR+"}</td> </tr>" +
				"<tr> <td><b>Função Objetivo</b></td> <td>{"+RequisicaoOtimizacaoDTO.PROPERTY_FUNCAO_OBJETIVO+"}</td> </tr>" +
				"<tr> <td><b>Campi Selecionados</b></td> <td>{"+RequisicaoOtimizacaoDTO.PROPERTY_CAMPI_SELECIONADOS+"}</td> </tr>" +
				"<tr> <td><b>Turno</b></td> <td>{"+RequisicaoOtimizacaoDTO.PROPERTY_TURNO+"}</td> </tr>" +
			"</table>"
		));
		ColumnConfig statusColumn = new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_STATUS_INDEX,"Status",50);
		statusColumn.setRenderer(new GridCellRenderer<RequisicaoOtimizacaoDTO>() {
			@Override
			public Object render(RequisicaoOtimizacaoDTO model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<RequisicaoOtimizacaoDTO> store, Grid<RequisicaoOtimizacaoDTO> grid) {
				Integer statusIndex = model.getStatusIndex();
				StatusRequisicaoOtimizacao status = StatusRequisicaoOtimizacao.values()[statusIndex];
				AbstractImagePrototype icon = null;
				if (StatusRequisicaoOtimizacao.EM_ANDAMENTO.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.ajax16());
				} else if (StatusRequisicaoOtimizacao.FINALIZADA_COM_OUTPUT.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.save16());
				} else if (StatusRequisicaoOtimizacao.FINALIZADA_SEM_OUTPUT.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.error16());
				}
				
				Button iconButton = new Button("",icon);
				iconButton.setToolTip(status.name());
				iconButton.setScale(ButtonScale.SMALL);
				iconButton.setIconAlign(IconAlign.TOP);
				
				return iconButton;
			}
		});
		statusColumn.setAlignment(HorizontalAlignment.CENTER);
		ColumnConfig acoesColumn = new ColumnConfig("","Ações",150);
		acoesColumn.setRenderer(new GridCellRenderer<RequisicaoOtimizacaoDTO>() {
			@Override
			public Object render(RequisicaoOtimizacaoDTO model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<RequisicaoOtimizacaoDTO> store, Grid<RequisicaoOtimizacaoDTO> grid) {
				Button carregarSolucaoBtn = new Button("Carregar Solução");
				
				Integer statusIndex = model.getStatusIndex();
				StatusRequisicaoOtimizacao status = StatusRequisicaoOtimizacao.values()[statusIndex];
				if (StatusRequisicaoOtimizacao.FINALIZADA_COM_OUTPUT.equals(status)) {
					carregarSolucaoBtn.setEnabled(true);
					ICommand cmd = CommandFactory.createCarregarSolucaoCommand(model,RequisicoesOtimizacaoView.this);
					carregarSolucaoBtn.addSelectionListener(CommandSelectionListener.<ButtonEvent>create(cmd));
				} else {
					carregarSolucaoBtn.setEnabled(false);
				}
				
				return carregarSolucaoBtn;
			}
		});
		List<ColumnConfig> requisicoesColumns = new ArrayList<ColumnConfig>();
		requisicoesColumns.add(requisicoesRowExpander);
		requisicoesColumns.add(statusColumn);
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_USER_NAME,"Usuário",100));
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_ROUND,"Round",150));
		requisicoesColumns.add(acoesColumn);
		ColumnModel requisicoesColumnModel = new ColumnModel(requisicoesColumns);
		
		requisicoesGrid = new Grid<RequisicaoOtimizacaoDTO>(requisicoesListStore,requisicoesColumnModel);
		requisicoesGrid.addPlugin(requisicoesRowExpander);
		requisicoesGrid.setBorders(true);
		requisicoesGrid.setWidth(500);
		requisicoesGrid.setHeight(400);
		requisicoesGrid.getView().setEmptyText("Não há requisições de otimização em andamento ou ainda não carregadas.");
		
		requisicoesListStore.addStoreListener(new StoreListener<RequisicaoOtimizacaoDTO>() {
			@SuppressWarnings("unchecked")
			@Override
			public void storeDataChanged(StoreEvent<RequisicaoOtimizacaoDTO> se) {
				if (se != null && se.getStore() != null && se.getStore().getModels() != null) {
					if (showEmpty || !se.getStore().getModels().isEmpty()) {
						presenter.setRequisicoesDeOtimizacao((List<RequisicaoOtimizacaoDTO>)se.getStore().getModels());
						window.show();
					}
				}
			}
		});
		
		ContentPanel contentPanel = new ContentPanel();
		contentPanel.setScrollMode(Style.Scroll.AUTO);
		contentPanel.setAutoHeight(true);
		contentPanel.setAutoWidth(true);
		contentPanel.add(requisicoesGrid);
		window.add(contentPanel);
	}
	
	public void show() {
		requisicoesGrid.getStore().getLoader().load();
	}
}
