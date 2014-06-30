package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ProgressBar;
import com.extjs.gxt.ui.client.widget.WidgetComponent;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.SituacaoCadastrosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SituacaoCadastrosView
	extends MyComposite
	implements SituacaoCadastrosPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CenarioDTO cenarioDTO;
	
	public SituacaoCadastrosView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		String title = "Status dos Cadastros para o Cenário (" + cenarioDTO.getNome() + ")" ;
	
		this.simpleModal = new SimpleModal(
			null, null, title, Resources.DEFAULTS.detalhes16() );
		this.simpleModal.setHeight( 160 );
		this.simpleModal.setWidth(350);
		createForm();
	
		this.simpleModal.setContent( this.formPanel );
	}
	
	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
	
		ProgressBar progresso = new ProgressBar();
		progresso.updateProgress(0.75, "75%");
		
		formPanel.add(progresso);
		LayoutContainer container = new LayoutContainer(new ColumnLayout());
		WidgetComponent info = new WidgetComponent(AbstractImagePrototype.create(Resources.DEFAULTS.warning16()).createImage());
		info.setStyleAttribute("margin-right", "10px");
		info.setStyleAttribute("float", "left");
		container.add(info);
		LayoutContainer text = new LayoutContainer(new ColumnLayout());
		text.addText("Já é possível obter uma solução, porém faltam cadastros importantes para uma solução de boa qualidade.");
		text.setStyleAttribute("float", "none");
		container.setStyleAttribute("margin-top", "10px");
		container.add(text);
		formPanel.add( container );
		
		Button detalhesBt = new Button( "Detalhes",
			AbstractImagePrototype.create( Resources.DEFAULTS.detalhes16() ) );
		simpleModal.addButton( detalhesBt );
	
		this.simpleModal.setFocusWidget( progresso );
	}
	
	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}
	
	@Override
	public CenarioDTO getCenarioDTO()
	{
		return this.cenarioDTO;
	}
}