package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class AlunosView
	extends MyComposite
	implements AlunosPresenter.Display
{
	private SimpleToolBar toolBar;
	private SimpleGrid< AlunoDTO > gridPanel;
	private SimpleFilter filter;
	private TextField< String > nomeBuscaTextField;
	private TextField< String > cpfBuscaTextField;
	private ContentPanel panel;
	private GTabItem tabItem;

	public AlunosView()
	{
		initUI();
	}

	private void initUI()
	{
		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeading( "Master Data » Alunos" );

		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem(
			"Alunos", Resources.DEFAULTS.professor16() );

		this.tabItem.setContent( this.panel );
	}

	private void createToolBar()
	{
		this.toolBar = new SimpleToolBar(
			true, true, true, false, false, this );

		this.panel.setTopComponent( this.toolBar );
	}

	private void createGrid()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5, 5, 5, 5 ) );

	    this.gridPanel = new SimpleGrid< AlunoDTO >( getColumnList(), this );

	    this.panel.add( this.gridPanel, bld );
	}

	private List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list
			= new ArrayList< ColumnConfig >();

		list.add( new ColumnConfig( AlunoDTO.PROPERTY_ALUNO_NOME, "Nome", 250 ) );
		list.add( new ColumnConfig( AlunoDTO.PROPERTY_ALUNO_CPF, "CPF", 150 ) );

		return list;
	}

	private void createFilter()
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.EAST );
		bld.setMargins( new Margins( 5, 5, 5, 0 ) );
		bld.setCollapsible( true );

		this.filter = new SimpleFilter();

		this.nomeBuscaTextField = new TextField< String >();
		this.nomeBuscaTextField.setFieldLabel( "Nome" );

		this.cpfBuscaTextField = new TextField< String >();
		this.cpfBuscaTextField.setFieldLabel( "CPF" );

		this.filter.addField( this.nomeBuscaTextField );
		this.filter.addField( this.cpfBuscaTextField );

		this.panel.add( this.filter, bld );
	}
	
	@Override
	public Button getNewButton()
	{
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton()
	{
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton()
	{
		return toolBar.getRemoveButton();
	}

	@Override
	public SimpleGrid< AlunoDTO > getGrid()
	{
		return gridPanel;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< AlunoDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public TextField< String > getNomeBuscaTextField()
	{
		return this.nomeBuscaTextField;
	}

	@Override
	public TextField< String > getCpfBuscaTextField()
	{
		return this.cpfBuscaTextField;
	}

	@Override
	public Button getSubmitBuscaButton()
	{
		return this.filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton()
	{
		return this.filter.getResetButton();
	}
}
