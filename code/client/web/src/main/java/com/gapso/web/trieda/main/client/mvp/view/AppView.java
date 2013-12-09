package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.ButtonScale;
import com.extjs.gxt.ui.client.Style.IconAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Viewport;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.AppPresenter;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CenarioPanel;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class AppView extends MyComposite implements AppPresenter.Display {

	private Viewport viewport;
	private ContentPanel panel;
	
	private GTab tab;
	private CenarioPanel cenarioPanel;
	
	private Button cenariosBt;
	private Button administracaoBt;
	private Button usuariosBt;
	
	public AppView() {
		initUI();
	}
	
	private void initUI() {
		TriedaI18nConstants i18nConstants = GWT.create(TriedaI18nConstants.class);
		
		viewport = new Viewport();
		viewport.setLayout(new FitLayout());
		panel = new ContentPanel(new BorderLayout());
		panel.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.logo()));
		panel.getHeader().addTool(createCenariosButton());
		panel.getHeader().getTool(0).setStyleAttribute("padding-right", "10px");
		panel.getHeader().addTool(createAdministracaoButton());
		panel.getHeader().getTool(1).setStyleAttribute("padding-right", "10px");
		panel.getHeader().addTool(createUsuariosButton());
		panel.getHeader().setTitle(i18nConstants.triedaVersion());
		viewport.add(panel);
		
		createWest();
		createCenter();
		
		initComponent(viewport);
	}
		
	private void createWest() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.WEST);
	    bld.setMargins(new Margins(5, 0, 5, 5));
	    bld.setCollapsible(true);
	    bld.setFloatable(true);
	    
	    cenarioPanel = new CenarioPanel();
	    //panel.add(cenarioPanel, bld);
	}
	
	private void createCenter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    tab = new GTab();
	    panel.add(tab, bld);
	}

	public Button createCenariosButton() {
		cenariosBt = new Button();
		cenariosBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.cenario16()));
		cenariosBt.setText("Cenários");
		cenariosBt.setToolTip("Cenários");
		cenariosBt.setScale(ButtonScale.MEDIUM);
		cenariosBt.setIconAlign(IconAlign.RIGHT);
		cenariosBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		cenariosBt.setHeight(24);
		Menu menu = new Menu();
		menu.add(new MenuItem("Gerenciar Cenários", AbstractImagePrototype.create(Resources.DEFAULTS.cenario16()) ));
		menu.add(new MenuItem("Gerenciar Requisições de Otimização", AbstractImagePrototype.create(Resources.DEFAULTS.gerarGradeConsultaRequisicao16()) ));
		cenariosBt.setMenu(menu);
		return cenariosBt;
	}
	
	public Button createUsuariosButton() {
		usuariosBt = new Button();
		usuariosBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.professor16()));
		usuariosBt.setToolTip("Menu do Usuário");
		usuariosBt.setScale(ButtonScale.MEDIUM);
		usuariosBt.setIconAlign(IconAlign.LEFT);
		usuariosBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		usuariosBt.setHeight(24);
		Menu menu = new Menu();
		MenuItem menuItem = new MenuItem("Nome");
		menuItem.disable();
		menuItem.addStyleName("disabled-color");
		menu.add(menuItem);
		menu.add(new MenuItem("Alterar Senha", AbstractImagePrototype.create(Resources.DEFAULTS.senha16()) ));
		menu.add(new MenuItem("Sair", AbstractImagePrototype.create(Resources.DEFAULTS.logout16()) ));
		usuariosBt.setMenu(menu);
		return usuariosBt;
	}

	public Button createAdministracaoButton() {
		administracaoBt = new Button();
		administracaoBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.chave16()));
		administracaoBt.setToolTip("Administração");
		administracaoBt.setScale(ButtonScale.MEDIUM);
		administracaoBt.setIconAlign(IconAlign.LEFT);
		administracaoBt.setArrowAlign(ButtonArrowAlign.BOTTOM);
		administracaoBt.setHeight(24);
		Menu menu = new Menu();
		menu.add(new MenuItem("Usuários", AbstractImagePrototype.create(Resources.DEFAULTS.campiTrabalho16()) ));
		administracaoBt.setMenu(menu);
		return administracaoBt;
	}
	
	@Override
	public ContentPanel getPanel() {
		return panel;
	}
	
	@Override
	public GTab getGTab() {
		return tab;
	}

	@Override
	public Widget asWidget() {
		return this;
	}

	@Override
	public CenarioPanel getCenarioPanel() {
		return cenarioPanel;
	}
	
	@Override
	public MenuItem getGerenciarCenariosButton() {
		return (MenuItem) cenariosBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getGerenciarRequisicoesCenariosButton() {
		return (MenuItem) cenariosBt.getMenu().getItem(1);
	}
		
	@Override
	public MenuItem getListarUsuariosButton() {
		return (MenuItem) administracaoBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getUsuariosNomeButton() {
		return (MenuItem) usuariosBt.getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getUsuariosAlterarSenhaButton() {
		return (MenuItem) usuariosBt.getMenu().getItem(1);
	}
	
	@Override
	public MenuItem getUsuariosSairButton() {
		return (MenuItem) usuariosBt.getMenu().getItem(2);
	}
}
