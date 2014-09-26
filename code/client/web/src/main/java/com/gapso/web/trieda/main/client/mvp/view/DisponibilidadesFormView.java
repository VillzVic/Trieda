package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.gapso.web.trieda.main.client.mvp.presenter.DisponibilidadesFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisponibilidadesGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TextFieldMask;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class DisponibilidadesFormView 
extends MyComposite
implements DisponibilidadesFormPresenter.Display
{
	private CenarioDTO cenarioDTO;
	private Long entidadeId;
	private String entidadeTipo;
	private String nomeEntidade;
	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private DisponibilidadesGrid< DisponibilidadeDTO > gridPanel;
	private TextField< String > horarioInicioTF;
	private TextField< String > horarioFimTF;
	private Button adicionarHorarioBT;
	
	public DisponibilidadesFormView(
		CenarioDTO cenarioDTO, Long entidadeId, String entidadeTipo, String nomeEntidade )
	{
		this.entidadeId = entidadeId;
		this.cenarioDTO = cenarioDTO;
		this.entidadeTipo = entidadeTipo;
		this.nomeEntidade = nomeEntidade;

		initUI();
		createGrid();
	}
	
	private void initUI()
	{
		String title = "Disponibilidades (" + nomeEntidade + ")";
		this.simpleModal = new SimpleModal(title,Resources.DEFAULTS.semanaLetiva16());

		this.simpleModal.setHeight( 580 );
		this.simpleModal.setWidth( 620 );
	}
	
	private void createGrid()
	{
		this.gridPanel = new DisponibilidadesGrid< DisponibilidadeDTO >(this);
		this.gridPanel.setBodyStyle( "background-color: #FFFFFF;" );

		FormPanel formPanel = new FormPanel();
		formPanel.setLayout( new HBoxLayout() );
		formPanel.setHeaderVisible( true );
		formPanel.setHeadingHtml( "Adicionar uma Disponibilidade" );
		formPanel.setButtonAlign( HorizontalAlignment.RIGHT );

		this.horarioInicioTF = new TextFieldMask( "99:99" );
		this.horarioInicioTF.setRegex( "([0-1][0-9]|2[0-4]):([0-5][0-9])$" );
		this.horarioInicioTF.setName( "horInicio" );
		this.horarioInicioTF.setAllowBlank( false );
		this.horarioInicioTF.setEmptyText( "Horário de início" );
		
		this.horarioFimTF = new TextFieldMask( "99:99" );
		this.horarioFimTF.setRegex( "([0-1][0-9]|2[0-4]):([0-5][0-9])$" );
		this.horarioFimTF.setName( "horFim" );
		this.horarioFimTF.setAllowBlank( false );
		this.horarioFimTF.setEmptyText( "Horário de fim" );

		formPanel.add( new Label( "Horário Início:" ),
			new HBoxLayoutData( new Margins( 5, 0, 0, 5 ) ) );

		formPanel.add( this.horarioInicioTF,
			new HBoxLayoutData( new Margins( 0, 20, 0, 5 ) ) );
		
		formPanel.add( new Label( "Horário Fim:" ),
			new HBoxLayoutData( new Margins( 5, 0, 0, 10 ) ) );

		formPanel.add( this.horarioFimTF,
			new HBoxLayoutData( new Margins( 0, 20, 0, 5 ) ) );

		this.adicionarHorarioBT = new Button( "Adicionar",
			AbstractImagePrototype.create( Resources.DEFAULTS.save16() ) );

		formPanel.add( this.adicionarHorarioBT,
			new HBoxLayoutData( new Margins( 0, 0, 0, 10 ) ) );

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton( this.adicionarHorarioBT );

		this.contentPanel = new ContentPanel();
		this.contentPanel.setBodyStyle( "background-color: #CED9E7;" );

		VBoxLayout layout = new VBoxLayout();
		layout.setVBoxLayoutAlign( VBoxLayoutAlign.STRETCH );

		this.contentPanel.setLayout( layout );
		this.contentPanel.setHeaderVisible( false );
		this.contentPanel.setBodyBorder( false );
		this.contentPanel.add( formPanel,
			new VBoxLayoutData( new Margins( 0, 0, 5, 0 ) ) );

		VBoxLayoutData flex = new VBoxLayoutData( new Margins( 0 ) );
		flex.setFlex( 1 );
		this.contentPanel.add( this.gridPanel, flex );

		this.simpleModal.setContent( this.contentPanel );
	}
	

	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}

	@Override
	public Long getEntidadeId()
	{
		return this.entidadeId;
	}
	
	@Override
	public String getEntidadeTipo()
	{
		return this.entidadeTipo;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< DisponibilidadeDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public ListStore< DisponibilidadeDTO > getStore()
	{
		return this.gridPanel.getStore();
	}

	@Override
	public TextField< String > getHorarioInicioTF()
	{
		return this.horarioInicioTF;
	}
	
	@Override
	public TextField< String > getHorarioFimTF()
	{
		return this.horarioFimTF;
	}

	@Override
	public Button getAdicionarHorarioBT()
	{
		return this.adicionarHorarioBT;
	}
	
	@Override
	public DisponibilidadesGrid<DisponibilidadeDTO> getGrid(){
		return this.gridPanel;
	}
}
