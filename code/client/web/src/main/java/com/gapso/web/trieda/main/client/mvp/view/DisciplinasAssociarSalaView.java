package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BaseTreeLoader;
import com.extjs.gxt.ui.client.data.ModelIconProvider;
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
import com.gapso.web.trieda.main.client.mvp.presenter.DisciplinasAssociarSalaPresenter;
import com.gapso.web.trieda.main.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.CampusComboBox;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.main.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.main.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class DisciplinasAssociarSalaView extends MyComposite implements DisciplinasAssociarSalaPresenter.Display {

	private ContentPanel panel;
	private ContentPanel panelLists;
	
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	
	private UnidadeComboBox unidadeSalaCB;
	private SimpleComboBox<String> andarCB;
	
	private UnidadeComboBox unidadeGrupoSalaCB;
	
	private TreeStore<TreeNodeDTO> storeDisciplina;
	private TreeStore<TreeNodeDTO> storeSala;
	
	private TreePanel<TreeNodeDTO> disciplinasList;
	private TreePanel<TreeNodeDTO> salasList;
	
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
		tabItem = new GTabItem("Associação de Disciplinas à Salas", Resources.DEFAULTS.associacaoDisciplinaSala16());
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
		
		campusCB = new CampusComboBox();
		campusCB.setFieldLabel("Campus");
		formPanel.add(campusCB, formData);
		
		turnoCB = new TurnoComboBox(campusCB) {
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
		
		tabs = new TabPanel();
		tabs.disable();
		
		TabItem salaTabItem = new TabItem();
		salaTabItem.setBorders(false);
		salaTabItem.setText("Sala");  
		salaTabItem.setLayout(new FormLayout());
		
		unidadeSalaCB = new UnidadeComboBox(campusCB) {
			@Override
			protected void onBlur(ComponentEvent ce) {
				super.onBlur(ce);
				if(unidadeSalaCB.getValue() == null) {
					andarCB.setValue(null);
					andarCB.disable();
				}
			}
		};
		unidadeSalaCB.setFieldLabel("Unidade");
		unidadeSalaCB.disable();
		salaTabItem.add(unidadeSalaCB, formData);
		andarCB = new SimpleComboBox<String>();
		andarCB.setFieldLabel("Andar");
		andarCB.disable();
		salaTabItem.add(andarCB, formData);

		TabItem grupoSalaTabItem = new TabItem();  
		grupoSalaTabItem.setText("Grupo de Sala");  
		grupoSalaTabItem.setLayout(new FormLayout());
		
		unidadeGrupoSalaCB = new UnidadeComboBox(campusCB);
		unidadeGrupoSalaCB.setFieldLabel("Unidade");
		grupoSalaTabItem.add(unidadeGrupoSalaCB, formData);
		
		tabs.add(salaTabItem);
		tabs.add(grupoSalaTabItem);
		
		formPanel.add(tabs, formData);
		
		panel.setTopComponent(formPanel);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel disciplinasListPanel = new ContentPanel(new FitLayout());
		disciplinasListPanel.setHeading("Matriz(es) Curricular(es)");
		disciplinasList = new TreePanel<TreeNodeDTO>(getStoreDisciplina()) {
			@Override
			protected boolean hasChildren(TreeNodeDTO model) {
				return !model.getLeaf();
			}
		};
		TreePanelDragSource source = new TreePanelDragSource(disciplinasList);
		disciplinasList.getStyle().setLeafIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		disciplinasList.disable();
		disciplinasList.setDisplayProperty(TreeNodeDTO.PROPERTY_TEXT);
		disciplinasList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		disciplinasListPanel.add(disciplinasList);
		
		source.setStatusText("{0} Selecionado(s)");
		
		ContentPanel salasListPanel = new ContentPanel(new FitLayout());
		salasListPanel.getHeader().addTool(getRemoveButton());
		salasListPanel.setHeading("Sala(s)");
		salasList = new TreePanel<TreeNodeDTO>(getStoreSala()) {
			@Override
			protected boolean hasChildren(TreeNodeDTO model) {
				return !model.getLeaf();
			}
		};
		salasList.setIconProvider(new ModelIconProvider<TreeNodeDTO>() {
			@Override
			public AbstractImagePrototype getIcon(TreeNodeDTO model) {
				if(model.getEmpty() && !model.getLeaf()) {
					return AbstractImagePrototype.create(Resources.DEFAULTS.folderEmpty16());
				}
				return null;
			}
		});
		TreePanelDropTarget target = new TreePanelDropTarget(salasList) {
			@Override
			protected void onDragDrop(DNDEvent event) { }
		};
		salasList.getStyle().setLeafIcon(AbstractImagePrototype.create(Resources.DEFAULTS.disciplina16()));
		salasList.disable();
		salasList.setDisplayProperty(TreeNodeDTO.PROPERTY_TEXT);
		salasList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		salasList.setCaching(false);
		
		salasListPanel.add(salasList);
		
		target.setOperation(Operation.COPY);
		target.addDNDListener(new DNDListener() {
			@SuppressWarnings("rawtypes")
			@Override
			public void dragMove(DNDEvent e) {
				TreeNode nodeTarget = salasList.findNode(e.getTarget());
				if (nodeTarget != null && (nodeTarget.getModel() instanceof TreeNodeDTO)) {
					TreeNodeDTO nodeTargetDTO = (TreeNodeDTO)nodeTarget.getModel();
					AbstractDTO<?> contentNodeTargetDTO = nodeTargetDTO.getContent();
					if(!(contentNodeTargetDTO instanceof SalaDTO) && !(contentNodeTargetDTO instanceof GrupoSalaDTO)) {
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
				TreePanel<TreeNodeDTO> treePanel = (TreePanel) e.getComponent();
				TreeNodeDTO selectedNode = treePanel.getSelectionModel().getSelectedItem();
				AbstractDTO<?> contentSelectedNode = selectedNode.getContent(); 
				
				OfertaDTO ofertaDTO = null;
				Integer periodo = null;
				CurriculoDisciplinaDTO cdDTO = null;
				
				if(contentSelectedNode instanceof OfertaDTO) {
					ofertaDTO = (OfertaDTO)contentSelectedNode;
				} else if(contentSelectedNode instanceof CurriculoDisciplinaDTO) {
					TreeNodeDTO parentNode = selectedNode.getParent();
					AbstractDTO<?> contentParentNode = parentNode.getContent();
					if(contentParentNode instanceof OfertaDTO) {
						ofertaDTO = (OfertaDTO)contentParentNode;
						periodo = ((CurriculoDisciplinaDTO)contentSelectedNode).getPeriodo();
					} else {
						ofertaDTO = (OfertaDTO)parentNode.getParent().getContent();
						cdDTO = (CurriculoDisciplinaDTO)contentSelectedNode;
						periodo = cdDTO.getPeriodo();
					}
				}
				
				final TreeNodeDTO targetNode = salasList.findNode(e.getTarget()).getModel();
				AbstractDTO<?> contentTargetNode = targetNode.getContent();
				
				if (contentTargetNode instanceof SalaDTO) {
					SalaDTO salaDTO = (SalaDTO)contentTargetNode;
					DisciplinasServiceAsync disciplinasService = Services.disciplinas();
					disciplinasService.saveDisciplinaToSala(ofertaDTO, periodo, cdDTO, salaDTO, new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							Info.display("Salvo", "Disciplinas associadas com sucesso!");
							addItemInFolderEmpty(targetNode, salasList);
						}
					});
				} else {
					GrupoSalaDTO grupoSalaDTO = (GrupoSalaDTO)contentTargetNode;
					DisciplinasServiceAsync disciplinasService = Services.disciplinas();
					disciplinasService.saveDisciplinaToSala(ofertaDTO, periodo, cdDTO, grupoSalaDTO, new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							Info.display("Salvo", "Disciplinas associadas com sucesso!");
							addItemInFolderEmpty(targetNode, salasList);
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
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		panel.setBodyBorder(false);
		panelLists.setBodyStyle("background-color: #DFE8F6;");
		panel.add(panelLists, bld);
	}
	
	public void addItemInFolderEmpty(TreeNodeDTO targetNode, TreePanel<TreeNodeDTO> list) {
		if(targetNode.getEmpty()) {
			salasList.setExpanded(targetNode, false);
			targetNode.setEmpty(false);
		}
		salasList.setExpanded(targetNode, true);
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
	public TreePanel<TreeNodeDTO> getDisciplinasList() {
		return disciplinasList;
	}

	@Override
	public TreePanel<TreeNodeDTO> getSalasList() {
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
	public void setTabEnabled(boolean flag) {
		if(!flag) {
			unidadeSalaCB.setValue(null);
			unidadeSalaCB.disable();
			unidadeGrupoSalaCB.setValue(null);
			unidadeGrupoSalaCB.disable();
			andarCB.setValue(null);
			andarCB.disable();
			tabs.setEnabled(flag);
		} else if(flag != tabs.isEnabled()) {
			tabs.setEnabled(flag);
			unidadeSalaCB.enable();
			unidadeGrupoSalaCB.enable();
			andarCB.disable();
		}
	}
	
	public ToolButton getRemoveButton() {
		if(removeButton == null) {
			removeButton = new ToolButton("x-tool-close");
		}
		return removeButton;
	}
	
	public void setStoreDisciplina(TreeStore<TreeNodeDTO> storeDisciplina) {
		this.storeDisciplina = storeDisciplina;
	}
	
	// TODO passar para presenter
	public TreeStore<TreeNodeDTO> getStoreDisciplina() {
		if(this.storeDisciplina == null) {
			final DisciplinasServiceAsync service = Services.disciplinas();
			RpcProxy<List<TreeNodeDTO>> proxy = new RpcProxy<List<TreeNodeDTO>>() {
				@Override
				protected void load(Object loadConfig, AsyncCallback<List<TreeNodeDTO>> callback) {
					service.getFolderChildren((TreeNodeDTO)loadConfig, callback);
				}
			};
			final TreeLoader<TreeNodeDTO> loader = new BaseTreeLoader<TreeNodeDTO>(proxy);
			TreeStore<TreeNodeDTO> store = new TreeStore<TreeNodeDTO>(loader);
			setStoreDisciplina(store);
		}
		return this.storeDisciplina;
	}

	public void setStoreSala(TreeStore<TreeNodeDTO> storeSala) {
		this.storeSala = storeSala;
	}
	
	// TODO passar para presenter
	public TreeStore<TreeNodeDTO> getStoreSala() {
		if(this.storeSala == null) {
			final DisciplinasServiceAsync service = Services.disciplinas();
			RpcProxy<List<TreeNodeDTO>> proxy = new RpcProxy<List<TreeNodeDTO>>() {
				@Override
				protected void load(Object loadConfig, AsyncCallback<List<TreeNodeDTO>> callback) {
					if (loadConfig != null) {
						TreeNodeDTO currentNode = (TreeNodeDTO)loadConfig;
						AbstractDTO<?> contentCurrentNode = currentNode.getContent(); 
						
						TreeNodeDTO salaTreeNodeDTO = null;
						TreeNodeDTO grupoSalaTreeNodeDTO = null;
						TreeNodeDTO ofertaTreeNodeDTO = null;
						TreeNodeDTO curriculoDisciplinaTreeNodeDTO = null;
						
						if (contentCurrentNode instanceof SalaDTO) {
							salaTreeNodeDTO = currentNode;//salaDTO = (SalaDTO)contentCurrentNode;
						} else if (contentCurrentNode instanceof GrupoSalaDTO) {
							grupoSalaTreeNodeDTO = currentNode;//grupoSalaDTO = (GrupoSalaDTO)contentCurrentNode;
						} else if (contentCurrentNode instanceof OfertaDTO) {
							TreeNodeDTO parentNode = currentNode.getParent();
							AbstractDTO<?> contentParentNode = parentNode.getContent();
							if (contentParentNode instanceof SalaDTO) {
								salaTreeNodeDTO = parentNode;//salaDTO = (SalaDTO)contentParentNode;
							} else {
								grupoSalaTreeNodeDTO = parentNode;//grupoSalaDTO = (GrupoSalaDTO)contentParentNode;
							}					
							ofertaTreeNodeDTO = currentNode;//ofertaDTO = (OfertaDTO)contentCurrentNode;
						} else if (contentCurrentNode instanceof CurriculoDisciplinaDTO) {
							TreeNodeDTO parentNode = currentNode.getParent();
							ofertaTreeNodeDTO = parentNode;//ofertaDTO = (OfertaDTO)parentNode.getContent();
							TreeNodeDTO parentParentNode = parentNode.getParent();
							AbstractDTO<?> contentparentParentNode = parentParentNode.getContent();
							if (contentparentParentNode instanceof SalaDTO) {
								//salaDTO = (SalaDTO)contentparentParentNode;
								salaTreeNodeDTO = parentParentNode;
							} else {
								grupoSalaTreeNodeDTO = parentParentNode;//grupoSalaDTO = (GrupoSalaDTO)contentparentParentNode;
							}
							curriculoDisciplinaTreeNodeDTO = currentNode;//curriculoDisciplinaDTO = (CurriculoDisciplinaDTO)contentCurrentNode;
						}
						if(salaTreeNodeDTO != null) {
							service.getDisciplinasByTreeSalas(salaTreeNodeDTO,ofertaTreeNodeDTO,curriculoDisciplinaTreeNodeDTO,callback);
						} else if(grupoSalaTreeNodeDTO != null) {
							service.getDisciplinasByTreeGrupoSalas(grupoSalaTreeNodeDTO,ofertaTreeNodeDTO,curriculoDisciplinaTreeNodeDTO,callback);
						}
					}
				}
			};
			final TreeLoader<TreeNodeDTO> loader = new BaseTreeLoader<TreeNodeDTO>(proxy);
			TreeStore<TreeNodeDTO> store = new TreeStore<TreeNodeDTO>(loader);
			setStoreSala(store);
		}
		return this.storeSala;
	}
	
}
