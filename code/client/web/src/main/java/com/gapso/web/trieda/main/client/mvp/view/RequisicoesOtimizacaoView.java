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
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.command.ICommand;
import com.gapso.web.trieda.main.client.command.util.CommandFactory;
import com.gapso.web.trieda.main.client.command.util.CommandSelectionListener;
import com.gapso.web.trieda.main.client.mvp.presenter.RequisicoesOtimizacaoPresenter;
import com.gapso.web.trieda.main.client.mvp.view.gateways.IRequisicoesOtimizacaoViewGateway;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO;
import com.gapso.web.trieda.shared.dtos.RequisicaoOtimizacaoDTO.StatusRequisicaoOtimizacao;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RequisicoesOtimizacaoView extends MyComposite implements IRequisicoesOtimizacaoViewGateway {

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
		window.setHeadingHtml("Requisições de Otimização");
		window.setScrollMode(Style.Scroll.AUTO);
		window.setModal(true);
		window.setAutoWidth(true);
		window.setAutoHeight(true);
		window.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeConsultaRequisicao16()));
		/*window.addWindowListener(new WindowListener() {
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
		});*/
		
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
			"<p style='margin: 5px 5px 10px'><b>Modo Otimização:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_MODO_OTIMIZACAO + "}</p>"	+
			//"<p style='margin: 5px 5px 10px'><b>Otimizar por:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_OTIMIZAR_POR + "}</p>" +
			//"<p style='margin: 5px 5px 10px'><b>Função Objetivo:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_FUNCAO_OBJETIVO + "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Campi Selecionados:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_CAMPI_SELECIONADOS + "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Turnos Selecionados:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TURNOS_SELECIONADOS + "}</p>" + 
			"<p style='margin: 5px 5px 10px'><b>Total de Campi:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TOTAL_CAMPI + "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Total de Turnos:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TOTAL_TURNOS + "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Total de Ambientes:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TOTAL_AMBIENTES + "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Total de Alunos:</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TOTAL_ALUNOS+ "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Total de Demandas de Alunos (P1):</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TOTAL_ALUNODEMANDA_P1+ "}</p>" +
			"<p style='margin: 5px 5px 10px'><b>Total de Demandas de Alunos (P2):</b>{" + RequisicaoOtimizacaoDTO.PROPERTY_TOTAL_ALUNODEMANDA_P2+ "}</p>"
		));
		
		ColumnConfig statusColumn = new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_STATUS_INDEX,"Status",45);
		statusColumn.setRenderer(new GridCellRenderer<RequisicaoOtimizacaoDTO>() {
			@Override
			public Object render(RequisicaoOtimizacaoDTO model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<RequisicaoOtimizacaoDTO> store, Grid<RequisicaoOtimizacaoDTO> grid) {
				Integer statusIndex = model.getStatusIndex();
				StatusRequisicaoOtimizacao status = StatusRequisicaoOtimizacao.values()[statusIndex];
				AbstractImagePrototype icon = null;
				if (StatusRequisicaoOtimizacao.AGUARDANDO.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.horarioAula16());
				} else if (StatusRequisicaoOtimizacao.EXECUTANDO.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.ajax16());
				} else if (StatusRequisicaoOtimizacao.FINALIZADA_COM_RESULTADO.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.save16());
				} else if (StatusRequisicaoOtimizacao.FINALIZADA_SEM_RESULTADO.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.error16());
				} else if (StatusRequisicaoOtimizacao.CANCELADA.equals(status)) {
					icon = AbstractImagePrototype.create(Resources.DEFAULTS.cancel16());
				}
				
				Button iconButton = new Button("",icon);
				iconButton.setToolTip(status.name());
				iconButton.setScale(ButtonScale.SMALL);
				iconButton.setIconAlign(IconAlign.TOP);
				
				return iconButton;
			}
		});
		statusColumn.setAlignment(HorizontalAlignment.CENTER);
		ColumnConfig acoesColumn = new ColumnConfig("","Ações",90);
		acoesColumn.setRenderer(new GridCellRenderer<RequisicaoOtimizacaoDTO>() {
			@Override
			public Object render(RequisicaoOtimizacaoDTO model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<RequisicaoOtimizacaoDTO> store, Grid<RequisicaoOtimizacaoDTO> grid) {
				Button carregarSolucaoBtn = new Button("",AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeCarregaSolucao16()));
				carregarSolucaoBtn.setToolTip("Carregar Solução");
				Button cancelarRequisicaoBtn = new Button("",AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeMataRequisicao16()));
				cancelarRequisicaoBtn.setToolTip("Cancelar Requisição de Otimização");
				Button descartarSolucaoBtn = new Button("",AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeDescartaSolucao16()));
				descartarSolucaoBtn.setToolTip("Remover Requisição");
				
				Integer statusIndex = model.getStatusIndex();
				StatusRequisicaoOtimizacao status = StatusRequisicaoOtimizacao.values()[statusIndex];
				if (StatusRequisicaoOtimizacao.FINALIZADA_COM_RESULTADO.equals(status)) {
					ICommand cmd1 = CommandFactory.createCarregarSolucaoCommand(model,RequisicoesOtimizacaoView.this);
					carregarSolucaoBtn.addSelectionListener(CommandSelectionListener.<ButtonEvent>create(cmd1));
					ICommand cmd2 = CommandFactory.createRemoverRequisicaoOtimizacaoCommand(model,RequisicoesOtimizacaoView.this,RequisicoesOtimizacaoView.this);
					descartarSolucaoBtn.addSelectionListener(CommandSelectionListener.<ButtonEvent>create(cmd2));
					
					cancelarRequisicaoBtn.setEnabled(false);
					carregarSolucaoBtn.setEnabled(true);
					descartarSolucaoBtn.setEnabled(true);
				} else if (StatusRequisicaoOtimizacao.EXECUTANDO.equals(status) || StatusRequisicaoOtimizacao.AGUARDANDO.equals(status)) {
					ICommand cmd = CommandFactory.createCancelarRequisicaoOtimizacaoCommand(model,RequisicoesOtimizacaoView.this,RequisicoesOtimizacaoView.this);
					cancelarRequisicaoBtn.addSelectionListener(CommandSelectionListener.<ButtonEvent>create(cmd));
					
					cancelarRequisicaoBtn.setEnabled(true);
					carregarSolucaoBtn.setEnabled(false);
					descartarSolucaoBtn.setEnabled(false);
				} else {
					ICommand cmd = CommandFactory.createRemoverRequisicaoOtimizacaoCommand(model,RequisicoesOtimizacaoView.this,RequisicoesOtimizacaoView.this);
					descartarSolucaoBtn.addSelectionListener(CommandSelectionListener.<ButtonEvent>create(cmd));
					
					cancelarRequisicaoBtn.setEnabled(false);
					carregarSolucaoBtn.setEnabled(false);
					descartarSolucaoBtn.setEnabled(true);
				}
				
				ToolBar acoesToolBar = new ToolBar();
				acoesToolBar.setAutoWidth(true);
				acoesToolBar.add(cancelarRequisicaoBtn);
				acoesToolBar.add(carregarSolucaoBtn);
				acoesToolBar.add(descartarSolucaoBtn);
				
				return acoesToolBar;
			}
		});
		List<ColumnConfig> requisicoesColumns = new ArrayList<ColumnConfig>();
		requisicoesColumns.add(requisicoesRowExpander);
		requisicoesColumns.add(statusColumn);
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_POSICAO_FILA,"Status",100));
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_ROUND,"Requisição ID",125));
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_CENARIO_NOME,"Cenário [ID] Nome",100));
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_USER_NAME,"Usuário Criação",90));
		//requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_DURACAO,Duração",80)); // hh:mm:ss (calculado a partir das duas colunas abaixo)
		//requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_DURACAO,Início",100)); // dd/mm/aa hh:mm:ss
		//requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_DURACAO,Término",100)); // dd/mm/aa hh:mm:ss
		//requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_USER_NAME_CANCELAMENTO,"Usuário Cancelamento",90));
		requisicoesColumns.add(acoesColumn);
		ColumnModel requisicoesColumnModel = new ColumnModel(requisicoesColumns);
		
		requisicoesGrid = new Grid<RequisicaoOtimizacaoDTO>(requisicoesListStore,requisicoesColumnModel);
		requisicoesGrid.addPlugin(requisicoesRowExpander);
		requisicoesGrid.setBorders(true);
		requisicoesGrid.setWidth(800);
		requisicoesGrid.setHeight(500);
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
//		contentPanel.setScrollMode(Style.Scroll.AUTO);
//		contentPanel.setAutoHeight(true);
//		contentPanel.setAutoWidth(true);
//		contentPanel.add(requisicoesGrid);
//		window.add(contentPanel);
		window.add(requisicoesGrid);
	}
	
	public void show() {
		requisicoesGrid.getStore().getLoader().load();
	}

	@Override
	public void removeRequisicaoOtimizacaoFromGrid(RequisicaoOtimizacaoDTO requisicaoOtimizacaoDTO) {
		requisicoesGrid.getStore().remove(requisicaoOtimizacaoDTO);
	}
	
	@Override
	public void updateRequisicaoOtimizacaoGrid() {
		show();
	}
}