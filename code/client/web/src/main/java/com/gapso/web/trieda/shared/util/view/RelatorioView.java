package com.gapso.web.trieda.shared.util.view;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.extjs.gxt.ui.client.widget.treegrid.WidgetTreeGridCellRenderer;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public abstract class RelatorioView extends MyComposite
	implements RelatorioPresenter.Display
{

	protected FormPanel formPanel;
	protected TreeStore< RelatorioDTO > store = new TreeStore< RelatorioDTO >();
	protected TreeGrid< RelatorioDTO > tree;
	protected ContentPanel panel;
	private GTabItem tabItem;
	private Button submitBt;
	private Button resetBt;
	LayoutContainer left;
	LayoutContainer right;
	LayoutContainer center;
	private int numFiltros;
	protected CenarioDTO cenarioDTO;
	protected List<Button> histogramasBt = new ArrayList<Button>();
	
	protected RelatorioView(CenarioDTO cenarioDTO){
		this.cenarioDTO = cenarioDTO;
		initUI();
	}
	
	protected abstract String getHeadingPanel();
	protected abstract GTabItem createGTabItem();
	
	private void initUI()
	{
		this.panel = new ContentPanel(new BorderLayout());
		this.panel.setHeadingHtml(getHeadingPanel());

		createFilter();
		createGrid();
		createTabItem();
		initComponent( tabItem );
	}
	
	private void createTabItem(){
		this.tabItem = createGTabItem();

		this.tabItem.setContent(this.panel);
	}

	private void createFilter(){
		formPanel = new FormPanel();
		
		formPanel.setHeaderVisible(true);
		formPanel.setHeadingHtml("Filtros");
		
		final LayoutContainer main = new LayoutContainer(new ColumnLayout());
		left = new LayoutContainer(new FormLayout(LabelAlign.LEFT));
		right = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		center = new LayoutContainer(new FormLayout(LabelAlign.RIGHT));
		LayoutContainer submit = new LayoutContainer(new FormLayout());
		LayoutContainer reset = new LayoutContainer(new FormLayout());
		setFilter();
		
		for(Field<?> campo : getFiltersList()) addFilter(campo);
		
		this.submitBt = new Button("Gerar Relatório", AbstractImagePrototype.create(Resources.DEFAULTS.resumoFaixaDemanda16()));
		this.resetBt = new Button( "Limpar Filtro", AbstractImagePrototype.create( Resources.DEFAULTS.filterClean16() ) );
		submit.add(submitBt, new HBoxLayoutData(new Margins(0, 0, 0, 5)));
		reset.add(resetBt, new HBoxLayoutData(new Margins(0, 0, 0, 5)));
		
		main.add(left, new ColumnData(0.31));
		main.add(center, new ColumnData(0.26));
		main.add(right, new ColumnData(0.26));
		main.add(reset, new ColumnData(0.08));
		main.add(submit, new ColumnData(0.09));
		
		formPanel.add(main, new FormData("100%"));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
		int numLinhas = (int) Math.ceil(numFiltros/3.0);
		bld.setSize(48 + numLinhas * 26);
		
		this.panel.add(formPanel, bld);
	}
	
	protected abstract void setFilter();
	
	protected abstract List<Field<?>> getFiltersList();
	
	protected void createHistogramasButton(int numHistogramas)
	{
		for (int i = 0; i<numHistogramas; i++)
		{
			Button bt = new Button();
			histogramasBt.add(bt);
		}
	}
	
	protected Button getHistogramaButton(RelatorioDTO model)
	{
		if (histogramasBt.size() < model.getButtonIndex() + 1)
		{
			Button bt = new Button();
			bt.setText(model.getButtonText());
			histogramasBt.add(bt);
			return bt;
		}
		else
		{
			histogramasBt.get(model.getButtonIndex()).setText(model.getButtonText());
			return histogramasBt.get(model.getButtonIndex());
		}
	}
	
	private void createGrid()
	{
	    ColumnConfig text = new ColumnConfig(RelatorioDTO.PROPERTY_TEXT, "Texto", 800);  
	    text.setRenderer(new WidgetTreeGridCellRenderer<RelatorioDTO>(){  
			@Override
			public Widget getWidget(RelatorioDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex, ListStore<RelatorioDTO> store,
					Grid<RelatorioDTO> grid) {
				if (model.getButtonIndex() != null)
				{
			        return getHistogramaButton(model);
				}
				return null;
			}
	    });
		tree = new TreeGrid< RelatorioDTO >( getStore(),  new ColumnModel( Arrays.asList(text) )  ){
			  @Override
			  protected void onAfterRenderView() {
			    super.onAfterRenderView();
			    this.getView().getHeader().hide();
			  }
		};
		tree.setAutoExpandColumn(RelatorioDTO.PROPERTY_TEXT);
		tree.getView().setAutoFill(true);

	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible( false );
	    contentPanel.add( tree );

	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );
	    panel.add( contentPanel, bld );
	}
	
	@Override
	public void setStore( TreeStore< RelatorioDTO > store )
	{
		this.store = store;
	}

	public TreeStore< RelatorioDTO > getStore()
	{
		return store;
	}

	@Override
	public TreeGrid< RelatorioDTO > getTree()
	{
		return tree;
	}
	
	@Override
	public Button getSubmitBuscaButton()
	{
		return submitBt;
	}
	
	@Override
	public Button getResetBuscaButton()
	{
		return resetBt;
	}
	
	@Override
	public List<Button> getHistogramasButton()
	{
		return histogramasBt;
	}
	
	protected void addFilter(Field<?> field){
		switch (numFiltros%3){
		case 0:
			left.add(field, new FormData("100%"));
			break;
		case 1:
			center.add(field, new FormData("100%"));
			break;
		case 2:
			right.add(field, new FormData("100%"));
			break;
		}
		numFiltros++;
	}
}
