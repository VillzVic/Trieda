package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.AlocacaoManualPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualView
	extends MyComposite
	implements AlocacaoManualPresenter.Display
{
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	private DemandaDTO demandaDTO;
	private DisciplinaDTO disciplinaDTO;

	public AlocacaoManualView( CenarioDTO cenarioDTO, DemandaDTO demandaDTO, DisciplinaDTO disciplinaDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.demandaDTO = demandaDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeadingHtml( cenarioDTO.getNome() + " » Alocação Manual" );

		createPanel();
		createTabItem();
		initComponent( this.tabItem );
	}
	
	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Alocação Manual", Resources.DEFAULTS.confirmacao16() );

		this.tabItem.setContent( this.panel );
	}
	
	private void createPanel()
	{
		BorderLayoutData bldNorth = new BorderLayoutData( LayoutRegion.NORTH );
		bldNorth.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldNorth.setCollapsible(true);
		bldNorth.setSize(100);
		BorderLayoutData bldEast = new BorderLayoutData( LayoutRegion.EAST );
		bldEast.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldEast.setCollapsible(true);
		BorderLayoutData bldWest = new BorderLayoutData( LayoutRegion.WEST );
		bldWest.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldWest.setCollapsible(true);
		BorderLayoutData bldCenter = new BorderLayoutData( LayoutRegion.CENTER );
		bldCenter.setMargins( new Margins( 5, 5, 5, 5 ) );
		bldCenter.setCollapsible(true);

	    ContentPanel topPanel = new ContentPanel();
	    topPanel.setHeadingHtml("Turma");
	    
	    ContentPanel leftPanel = new ContentPanel();
	    leftPanel.add(createDisciplinaPanel());
	    
	    ContentPanel rightPanel = new ContentPanel();
	    
	    ContentPanel centerPanel = new ContentPanel();
	    

	    this.panel.add( topPanel, bldNorth );
	    this.panel.add( leftPanel, bldWest );
	    this.panel.add( rightPanel, bldEast );
	    this.panel.add( centerPanel, bldCenter );
	}

	private Widget createDisciplinaPanel() {
		ContentPanel disciplinaPanel = new ContentPanel();
		disciplinaPanel.setHeadingHtml(" Disciplina (" + demandaDTO.getDisciplinaString() + ")");
		disciplinaPanel.addText(disciplinaDTO.getNome());
		
		LayoutContainer container = new LayoutContainer(new ColumnLayout());
		LabelField creditos = new LabelField();
		creditos.setValue("Créd: " + disciplinaDTO.getCreditosTeorico() + "Teo " + disciplinaDTO.getCreditosPratico() + "Pra");
		creditos.setStyleAttribute("margin-top", "2px");
		CheckBox cb = new CheckBox();
		cb.setHideLabel( true );
		cb.setBoxLabel( "Exige Lab?" );
		cb.setValue( disciplinaDTO.getLaboratorio() );
		cb.setReadOnly(true);
		
		container.add(creditos, new ColumnData(0.55));
		container.add(cb, new ColumnData(0.45));
		disciplinaPanel.add(container);
		
		disciplinaPanel.setHeight(70);
		
		return disciplinaPanel;
	}
}
