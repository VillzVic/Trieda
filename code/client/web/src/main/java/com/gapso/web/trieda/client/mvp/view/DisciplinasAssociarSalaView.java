package com.gapso.web.trieda.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BaseTreeLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.data.TreeLoader;
import com.extjs.gxt.ui.client.dnd.DND.Operation;
import com.extjs.gxt.ui.client.dnd.TreePanelDragSource;
import com.extjs.gxt.ui.client.dnd.TreePanelDropTarget;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.DNDEvent;
import com.extjs.gxt.ui.client.event.DNDListener;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.TabPanel;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel;
import com.extjs.gxt.ui.client.widget.treepanel.TreePanel.TreeNode;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.presenter.DisciplinasAssociarSalaPresenter;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.GrupoSalaComboBox;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class DisciplinasAssociarSalaView extends MyComposite implements DisciplinasAssociarSalaPresenter.Display {

	private ContentPanel panel;
	private ContentPanel panelLists;
	
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	
	private UnidadeComboBox unidadeSalaCB;
	private SimpleComboBox<String> andarCB;
	private SalaComboBox salaCB;
	
	private UnidadeComboBox unidadeGrupoSalaCB = new UnidadeComboBox();
	private GrupoSalaComboBox grupoSalaCB = new GrupoSalaComboBox();
	
	private TreeStore<FileModel> storeDisciplina;
	private TreeStore<FileModel> storeSala;
	
	private TreePanel<FileModel> disciplinasList;
	private TreePanel<FileModel> salasList;
	
	private ToolButton removeButton;
	
	private TabPanel tabs;
	
	private GTabItem tabItem;
	
	public DisciplinasAssociarSalaView() {
		initUI();
		createTabItem();
		createForm();
		initComponent(tabItem);
	}

	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Associação de Disciplinas à Salas");
	}

	private void createTabItem() {
		tabItem = new GTabItem("Associação de Disciplinas à Salas", Resources.DEFAULTS.disciplina16());
		tabItem.setContent(panel);
	}
	
	private void createForm() {
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		formPanel.setAutoHeight(true);
		
		campusCB = new CampusComboBox() {
			@Override
			protected void onBlur(ComponentEvent ce) {
				super.onBlur(ce);
				if(campusCB.getValue() == null) {
					turnoCB.setValue(null);
					turnoCB.disable();
				}
			}
		};
		campusCB.setFieldLabel("Campus");
		formPanel.add(campusCB, formData);
		
		turnoCB = new TurnoComboBox() {
			@Override
			protected void onBlur(ComponentEvent ce) {
				super.onBlur(ce);
				if(turnoCB.getValue() == null) {
					setTabEnabled(false);
					getDisciplinasList().getStore().removeAll();
					getDisciplinasList().disable();
				}
			}
		};
		turnoCB.setFieldLabel("Turno");
		turnoCB.disable();
		turnoCB.setDisplayField("nome");
		formPanel.add(turnoCB, formData);
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(0, 0, 0, 5));
		bld.setCollapsible(true);
		
		tabs = new TabPanel();
		tabs.disable();
		
		TabItem salaTabItem = new TabItem();  
		salaTabItem.setBorders(false);
		salaTabItem.setText("Sala");  
		salaTabItem.setLayout(new FormLayout());
		
		unidadeSalaCB = new UnidadeComboBox() {
			@Override
			protected void onBlur(ComponentEvent ce) {
				super.onBlur(ce);
				if(unidadeSalaCB.getValue() == null) {
					andarCB.setValue(null);
					andarCB.disable();
					salaCB.setValue(null);
					salaCB.disable();
				}
			}
		};
		unidadeSalaCB.setFieldLabel("Unidade");
		unidadeSalaCB.disable();
		salaTabItem.add(unidadeSalaCB, formData);
		andarCB = new SimpleComboBox<String>() {
			@Override
			protected void onBlur(ComponentEvent ce) {
				super.onBlur(ce);
				if(andarCB.getValue() == null) {
					salaCB.setValue(null);
					salaCB.disable();
				}
			}
		};
		andarCB.setFieldLabel("Andar");
		andarCB.disable();
		salaTabItem.add(andarCB, formData);
		salaCB = new SalaComboBox();
		salaCB.setFieldLabel("Sala");
		salaCB.disable();
		salaTabItem.add(salaCB, formData);
		
		TabItem grupoSalaTabItem = new TabItem();  
		grupoSalaTabItem.setText("Grupo de Sala");  
		grupoSalaTabItem.setLayout(new FormLayout());
		
		unidadeGrupoSalaCB.setFieldLabel("Unidade");
		grupoSalaTabItem.add(unidadeGrupoSalaCB, formData);
		grupoSalaCB.setFieldLabel("Grupo de Sala");
		grupoSalaTabItem.add(grupoSalaCB, formData);
		
		tabs.add(salaTabItem);
		tabs.add(grupoSalaTabItem);
		
		formPanel.add(tabs, formData);
		
		panel.setTopComponent(formPanel);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel disciplinasListPanel = new ContentPanel(new FitLayout());
		disciplinasListPanel.setHeading("Disciplina(s)");
		disciplinasList = new TreePanel<FileModel>(getStoreDisciplina()) {
			@Override
			protected boolean hasChildren(FileModel model) {
				return !model.getFolha();
			}
		};
		TreePanelDragSource source = new TreePanelDragSource(disciplinasList);
		disciplinasList.getStyle().setLeafIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		disciplinasList.disable();
		disciplinasList.setDisplayProperty("name");
		disciplinasList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		disciplinasListPanel.add(disciplinasList);
		
		source.setStatusText("{0} Selecionado(s)");
		
		ContentPanel salasListPanel = new ContentPanel(new FitLayout());
		salasListPanel.getHeader().addTool(getRemoveButton());
		salasListPanel.setHeading("Salas(s)");
		salasList = new TreePanel<FileModel>(getStoreSala()) {
			@Override
			protected boolean hasChildren(FileModel model) {
				return !model.getFolha();
			}
		};
		TreePanelDropTarget target = new TreePanelDropTarget(salasList) {
			@Override
			protected void onDragDrop(DNDEvent event) { }
		};
		salasList.getStyle().setLeafIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		salasList.disable();
		salasList.setDisplayProperty("name");
		salasList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		salasList.setCaching(false);
		
		salasListPanel.add(salasList);
		
		target.setOperation(Operation.COPY);
		target.addDNDListener(new DNDListener() {
			@SuppressWarnings("rawtypes")
			@Override
			public void dragMove(DNDEvent e) {
				TreeNode overItem = salasList.findNode(e.getTarget());
				if (overItem != null && overItem.getModel() instanceof FileModel) {
					FileModel fileModel = (FileModel) overItem.getModel();
					if(!(fileModel instanceof SalaDTO) && !(fileModel instanceof GrupoSalaDTO)) {
						e.setCancelled(true);
						e.getStatus().setStatus(false);
						return;
					}
				}
				super.dragMove(e);
			}
			@SuppressWarnings({ "unchecked", "rawtypes" })
			@Override
			public void dragDrop(DNDEvent e) {
				TreePanel<FileModel> treePanel = (TreePanel) e.getComponent();
				FileModel sourceFileModel = treePanel.getSelectionModel().getSelectedItem();
				OfertaDTO ofertaDTO = null;
				Integer periodo = null;
				CurriculoDisciplinaDTO cdDTO = null;
				if(sourceFileModel instanceof OfertaDTO) {
					ofertaDTO = (OfertaDTO)sourceFileModel;
				} else if(sourceFileModel instanceof CurriculoDisciplinaDTO) {
					FileModel sourceFileModelParent = treePanel.getStore().getParent(sourceFileModel);
					if(sourceFileModelParent instanceof OfertaDTO) {
						ofertaDTO = (OfertaDTO)sourceFileModelParent;
						periodo = ((CurriculoDisciplinaDTO)sourceFileModel).getPeriodo();
					} else {
						ofertaDTO = (OfertaDTO)treePanel.getStore().getParent(sourceFileModelParent);
						periodo = ((CurriculoDisciplinaDTO)sourceFileModelParent).getPeriodo();
						cdDTO = (CurriculoDisciplinaDTO)sourceFileModel;
					}
				}
				
				FileModel fileModelTarget = salasList.findNode(e.getTarget()).getModel();
				
				if(fileModelTarget instanceof SalaDTO) {
					SalaDTO salaDTO = (SalaDTO)fileModelTarget;
					DisciplinasServiceAsync disciplinasService = Services.disciplinas();
					disciplinasService.saveDisciplinaToSala(ofertaDTO, periodo, cdDTO, salaDTO, new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							Info.display("Salvo", "Disciplinas associadas com sucesso!");
						}
					});
				} else {
					GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO)fileModelTarget;
					DisciplinasServiceAsync disciplinasService = Services.disciplinas();
					disciplinasService.saveDisciplinaToSala(ofertaDTO, periodo, cdDTO, grupoSalaDTO, new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							Info.display("Salvo", "Disciplinas associadas com sucesso!");
						}
					});
				}
			}
		});
		
		ContentPanel blankListPanel = new ContentPanel(new BorderLayout());
		blankListPanel.setHeaderVisible(false);
		blankListPanel.setBodyBorder(false);
		
		panelLists.add(disciplinasListPanel, new RowData(.5, 1, new Margins(0, 0, 10, 10)));
		panelLists.add(blankListPanel, new RowData(10, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(salasListPanel, new RowData(.5, 1, new Margins(0, 10, 10, 0)));
		
		bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		panel.setBodyBorder(false);
		panelLists.setBodyStyle("background-color: #DFE8F6;");
		panel.add(panelLists, bld);
	}
	
	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public UnidadeComboBox getUnidadeSalaComboBox() {
		return unidadeSalaCB;
	}
	
	@Override
	public UnidadeComboBox getUnidadeGrupoSalaComboBox() {
		return unidadeGrupoSalaCB;
	}

	@Override
	public TreePanel<FileModel> getDisciplinasList() {
		return disciplinasList;
	}

	@Override
	public TreePanel<FileModel> getSalasList() {
		return salasList;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public SimpleComboBox<String> getAndarComboBox() {
		return andarCB;
	}

	@Override
	public SalaComboBox getSalaComboBox() {
		return salaCB;
	}

	@Override
	public GrupoSalaComboBox getGrupoSalasComboBox() {
		return grupoSalaCB;
	}
	
	@Override
	public void setTabEnabled(boolean flag) {
		if(!flag) {
			unidadeSalaCB.setValue(null);
			unidadeSalaCB.disable();
			unidadeGrupoSalaCB.setValue(null);
			unidadeGrupoSalaCB.disable();
			andarCB.setValue(null);
			andarCB.disable();
			salaCB.setValue(null);
			salaCB.disable();
			grupoSalaCB.setValue(null);
			grupoSalaCB.disable();
			tabs.setEnabled(flag);
		} else if(flag != tabs.isEnabled()) {
			tabs.setEnabled(flag);
			unidadeSalaCB.enable();
			unidadeGrupoSalaCB.enable();
			andarCB.disable();
			salaCB.disable();
			grupoSalaCB.disable();
		}
	}
	
	public ToolButton getRemoveButton() {
		if(removeButton == null) {
			removeButton = new ToolButton("x-tool-close");
		}
		return removeButton;
	}
	
	public void setStoreDisciplina(TreeStore<FileModel> storeDisciplina) {
		this.storeDisciplina = storeDisciplina;
	}
	
	// TODO passar para presenter
	public TreeStore<FileModel> getStoreDisciplina() {
		if(this.storeDisciplina == null) {
			final DisciplinasServiceAsync service = Services.disciplinas();
			RpcProxy<List<FileModel>> proxy = new RpcProxy<List<FileModel>>() {
				@Override
				protected void load(Object loadConfig, AsyncCallback<List<FileModel>> callback) {
					service.getFolderChildren((FileModel) loadConfig, callback);
				}
			};
			final TreeLoader<FileModel> loader = new BaseTreeLoader<FileModel>(proxy);
			TreeStore<FileModel> store = new TreeStore<FileModel>(loader);
			setStoreDisciplina(store);
		}
		return this.storeDisciplina;
	}

	public void setStoreSala(TreeStore<FileModel> storeSala) {
		this.storeSala = storeSala;
	}
	
	// TODO passar para presenter
	public TreeStore<FileModel> getStoreSala() {
		if(this.storeSala == null) {
			final DisciplinasServiceAsync service = Services.disciplinas();
			RpcProxy<List<FileModel>> proxy = new RpcProxy<List<FileModel>>() {
				@Override
				protected void load(Object loadConfig, AsyncCallback<List<FileModel>> callback) {
					service.getFolderChildrenSala((FileModel) loadConfig, callback);
				}
			};
			final TreeLoader<FileModel> loader = new BaseTreeLoader<FileModel>(proxy);
			TreeStore<FileModel> store = new TreeStore<FileModel>(loader);
			setStoreSala(store);
		}
		return this.storeSala;
	}
	
}
