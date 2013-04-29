package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoAtendimentosFaixaDemandaFormPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class ResumoAtendimentosFaixaDemandaFormView 	
	extends MyComposite
	implements ResumoAtendimentosFaixaDemandaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private NumberField acimaNF;
	private NumberField abaixoNF;
	private Button addNovaFaixaBt;
	private LayoutContainer left;
	private LayoutContainer right;
	
	public ResumoAtendimentosFaixaDemandaFormView() {
		initUI();
	}
	
	private void initUI()
	{
		String title = ( "Editar faixas de demanda" );

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.demanda16() );

		this.simpleModal.setHeight( 395 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}
	
	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		formPanel.setLabelWidth(88);
		this.formPanel.setHeaderVisible( false );
		this.formPanel.setScrollMode(Scroll.AUTO);
		
		this.acimaNF = new NumberField();
		this.acimaNF.setName( "acima" );
		this.acimaNF.setFieldLabel("Limite superior (acima de)");
		this.acimaNF.setLabelStyle("width: 85");
		this.acimaNF.setAllowBlank( false );
		this.acimaNF.setAllowDecimals( false );
		this.acimaNF.setMaxValue( 999 );
		formPanel.add( this.acimaNF, formData );
		
		LayoutContainer main = new LayoutContainer();
	    main.setLayout(new ColumnLayout());
	    
	    left = new LayoutContainer();
	    FormLayout layout = new FormLayout();
	    layout.setLabelWidth(88);
	    left.setLayout(layout);
	    
	    right = new LayoutContainer();
	    right.setStyleAttribute("paddingLeft", "10px");
	    layout = new FormLayout();
	    layout.setLabelSeparator("");
	    layout.setLabelWidth(20);
	    right.setLayout(layout);
		
	    main.add(left, new ColumnData(.6));
	    main.add(right, new ColumnData(.4));
	  
	    formPanel.add(main, new FormData("100%"));
	    
	    this.addNovaFaixaBt = new Button("Adicionar nova faixa");
	    
	    formPanel.add(addNovaFaixaBt);
	    
		this.abaixoNF = new NumberField();
		this.abaixoNF.setName( "abaixo" );
		this.abaixoNF.setFieldLabel("Limite inferior (abaixo de)");
		this.abaixoNF.setAllowBlank( false );
		this.abaixoNF.setAllowDecimals( false );
		this.abaixoNF.setMaxValue( 999 );
		formPanel.add( this.abaixoNF, formData );
	}
	
	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}
	
	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}
	
	@Override
	public NumberField getAcimaNumberField()
	{
		return this.acimaNF;
	}
	
	@Override
	public NumberField getAbaixoNumberField()
	{
		return this.abaixoNF;
	}
	
	@Override
	public Button getAddNovaFaixaButton()
	{
		return this.addNovaFaixaBt;
	}
	
	@Override 
	public LayoutContainer getLeft()
	{
		return this.left;
	}
	
	@Override 
	public LayoutContainer getRight()
	{
		return this.right;
	}
	
	@Override
	public FormPanel getFormPanel()
	{
		return this.formPanel;
	}
}
