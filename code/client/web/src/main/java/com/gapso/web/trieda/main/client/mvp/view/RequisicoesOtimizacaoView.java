package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.core.XTemplate;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.store.StoreListener;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.WidgetComponent;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.TextArea;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.RowExpander;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
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
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

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
		window.setModal(true);
		window.setWidth(900);
		window.setHeight(600);
		window.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeConsultaRequisicao16()));
		final ContentPanel panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setWidth(880);
		panel.setHeight(570);
		FieldSet legenda = new FieldSet();
		legenda.setLayout(new ColumnLayout());
		legenda.setHeadingHtml("Legenda");
		legenda.setCollapsible(true);
		legenda.setBorders(false);
		legenda.collapse();
		legenda.setStyleAttribute("margin-top", "5px");
		legenda.setStyleAttribute("margin-left", "5px");
		legenda.setStyleAttribute("margin-right", "5px");
		
		legenda.add(createLegenda(AbstractImagePrototype.create(Resources.DEFAULTS.horarioAula16()), "AGUARDANDO: A requisição de otimização foi enviada com sucesso e está aguardando em" +
				" uma fila o resolvedor matemático ser liberado para que o seu processo de otimização seja" +
				" iniciado (ou seja, há outra requisição de otimização em processo de otimização)."));
		legenda.add(createLegenda(AbstractImagePrototype.create(Resources.DEFAULTS.ajax16()), "EXECUTANDO: A requisição de otimização foi enviada com sucesso e encontra-se em processo de otimização pelo resolvedor matemático."));
		legenda.add(createLegenda(AbstractImagePrototype.create(Resources.DEFAULTS.save16()), "FINALIZADA_COM_RESULTADO: A requisição de otimização foi finalizada e um arquivo com o resultado desta requisição foi gerado."));
		legenda.add(createLegenda(AbstractImagePrototype.create(Resources.DEFAULTS.error16()), "FINALIZADA_SEM_RESULTADO: A requisição de otimização foi finalizada, porém, não foi gerado um arquivo com o resultado desta requisição o que indica que provavelmente ocorreu um erro no resolvedor matemático."));
		legenda.add(createLegenda(AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()), "CANCELADA: A requisição de otimização que estava em andamento foi cancelada por algum usuário."));
		panel.setTopComponent(legenda);
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
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_DURACAO_REQUISICAO,"Duração da Requisição",120)); // hh:mm:ss (calculado a partir das duas colunas abaixo)
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_DURACAO_OTIMIZACAO,"Duração da Otimização",120));
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_INSTANTE_INICIO_REQUISICAO,"Início da Requisição",140)); // dd/mm/aa hh:mm:ss
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_INSTANTE_INICIO_OTIMIZACAO,"Início da Otimização",140));
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_INSTANTE_TERMINO,"Término",100)); // dd/mm/aa hh:mm:ss
		requisicoesColumns.add(new ColumnConfig(RequisicaoOtimizacaoDTO.PROPERTY_USER_NAME_CANCEL,"Usuário Cancelamento",90));
		requisicoesColumns.add(acoesColumn);
		ColumnModel requisicoesColumnModel = new ColumnModel(requisicoesColumns);
		
		requisicoesGrid = new Grid<RequisicaoOtimizacaoDTO>(requisicoesListStore,requisicoesColumnModel);
		requisicoesGrid.addPlugin(requisicoesRowExpander);
		requisicoesGrid.setBorders(true);
		//requisicoesGrid.setWidth(800);
		//requisicoesGrid.setHeight(500);
		requisicoesGrid.getView().setEmptyText("Não há requisições de otimização em andamento ou ainda não carregadas.");
		
		requisicoesListStore.addStoreListener(new StoreListener<RequisicaoOtimizacaoDTO>() {
			@SuppressWarnings("unchecked")
			@Override
			public void storeDataChanged(StoreEvent<RequisicaoOtimizacaoDTO> se) {
				if (se != null && se.getStore() != null && se.getStore().getModels() != null) {
					boolean reqEmAndamento = false;
					for (RequisicaoOtimizacaoDTO requisicoes: se.getStore().getModels())
					{
						if (requisicoes.getStatusIndex() == StatusRequisicaoOtimizacao.AGUARDANDO.ordinal()
								|| requisicoes.getStatusIndex() == StatusRequisicaoOtimizacao.EXECUTANDO.ordinal())
						{
							reqEmAndamento = true;
						}
					}
					if (showEmpty || reqEmAndamento) {
						presenter.setRequisicoesDeOtimizacao((List<RequisicaoOtimizacaoDTO>)se.getStore().getModels());
						window.show();
					}
				}
			}
		});
		
		final ContentPanel contentPanel = new ContentPanel(new FitLayout());
		contentPanel.setHeaderVisible(false);
		BorderLayoutData bldCenter = new BorderLayoutData( LayoutRegion.CENTER );
		bldCenter.setMargins( new Margins( 5, 5, 5, 5 ) );
		contentPanel.add(requisicoesGrid);
		contentPanel.setBorders(false);
		panel.add(contentPanel, bldCenter);
		window.add(panel);
		
		legenda.addListener(Events.Collapse, new Listener<ComponentEvent>() {
		@Override
			public void handleEvent(ComponentEvent fe) {
			panel.syncSize();
		    }
		});
		
		legenda.addListener(Events.Expand, new Listener<ComponentEvent>() {
		@Override
			public void handleEvent(ComponentEvent fe) {
			panel.syncSize();
		    }
		});
	}
	
	private Widget createLegenda(AbstractImagePrototype icone, String texto) {
		Button button = new Button("",icone);
		button.setScale(ButtonScale.SMALL);
		LayoutContainer legendaContainer = new LayoutContainer(new ColumnLayout());
		LayoutContainer legendaText = new LayoutContainer();
		legendaText.addText(texto);
		legendaText.setStyleAttribute("margin-top", "2px");
		legendaText.setStyleAttribute("margin-left", "5px");
		legendaText.setWidth(832);
		legendaContainer.add(button);
		legendaContainer.add(legendaText);
		legendaContainer.setStyleAttribute("margin-bottom", "5px");
		
		return legendaContainer;
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