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
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioDisponivelCenarioFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TextFieldMask;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class HorarioDisponivelCenarioFormView
	extends MyComposite
	implements HorarioDisponivelCenarioFormPresenter.Display
{
	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDTO semanaLetivaDTO;
	private SemanaLetivaGrid< HorarioDisponivelCenarioDTO > gridPanel;
	private TurnoComboBox turnoCB;
	private TextField< String > horarioInicioTF;
	private Button adicionarHorarioBT;
	private Button removerHorarioBT;
	private CenarioDTO cenarioDTO;

	public HorarioDisponivelCenarioFormView(
		CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO )
	{
		this.semanaLetivaDTO = semanaLetivaDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
		createGrid();
	}

	private void initUI()
	{
		String title = getI18nConstants().diasHorariosAula();
		if (this.semanaLetivaDTO != null) {
			title += " (" + this.semanaLetivaDTO.getCodigo() + " - " + this.semanaLetivaDTO.getDescricao() + ")";
		}
		this.simpleModal = new SimpleModal(title,Resources.DEFAULTS.semanaLetiva16());

		this.simpleModal.setHeight( 580 );
		this.simpleModal.setWidth( 620 );
	}

	private void createGrid()
	{
		this.gridPanel = new SemanaLetivaGrid< HorarioDisponivelCenarioDTO >();
		this.gridPanel.setBodyStyle( "background-color: #FFFFFF;" );

		FormPanel formPanel = new FormPanel();
		formPanel.setLayout( new HBoxLayout() );
		formPanel.setHeaderVisible( true );
		formPanel.setHeading( "Adicionar/Remover Horário de Aula" );
		formPanel.setButtonAlign( HorizontalAlignment.RIGHT );

		this.turnoCB = new TurnoComboBox( cenarioDTO );
		this.turnoCB.setName( "turno" );
		this.turnoCB.setFieldLabel( "Turno" );
		this.turnoCB.setAllowBlank( false );
		this.turnoCB.setEmptyText( "Selecione o turno" );

		this.horarioInicioTF = new TextFieldMask( "99:99" );
		this.horarioInicioTF.setRegex( "([0-1][0-9]|2[0-4]):([0-5][0-9])$" );
		this.horarioInicioTF.setName( "horarioInicio" );
		this.horarioInicioTF.setAllowBlank( false );
		this.horarioInicioTF.setEmptyText( "Horário de início de aula" );

		formPanel.add( new Label( "Turno:" ),
			new HBoxLayoutData( new Margins( 5, 0, 0, 0 ) ) );

		formPanel.add( this.turnoCB,
			new HBoxLayoutData( new Margins( 0, 0, 0, 10 ) ) );

		formPanel.add( new Label( "Horário:" ),
			new HBoxLayoutData( new Margins( 5, 0, 0, 10 ) ) );

		formPanel.add( this.horarioInicioTF,
			new HBoxLayoutData( new Margins( 0, 0, 0, 5 ) ) );

		this.adicionarHorarioBT = new Button( "Adicionar",
			AbstractImagePrototype.create( Resources.DEFAULTS.save16() ) );

		this.removerHorarioBT = new Button( "Remover",
			AbstractImagePrototype.create( Resources.DEFAULTS.cancel16() ) );

		formPanel.add( this.adicionarHorarioBT,
			new HBoxLayoutData( new Margins( 0, 0, 0, 10 ) ) );

		formPanel.add( this.removerHorarioBT,
			new HBoxLayoutData( new Margins( 0, 0, 0, 10 ) ) );
			
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton( this.adicionarHorarioBT );
		binding.addButton( this.removerHorarioBT );

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
	public SemanaLetivaDTO getSemanaLetivaDTO()
	{
		return this.semanaLetivaDTO;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public ListStore< HorarioDisponivelCenarioDTO > getStore()
	{
		return this.gridPanel.getStore();
	}

	@Override
	public TurnoComboBox getTurnoCB()
	{
		return this.turnoCB;
	}

	@Override
	public TextField< String > getHorarioInicioTF()
	{
		return this.horarioInicioTF;
	}

	@Override
	public Button getAdicionarHorarioBT()
	{
		return this.adicionarHorarioBT;
	}

	@Override
	public Button getRemoverHorarioBT()
	{
		return this.removerHorarioBT;
	}
	
	@Override
	public SemanaLetivaGrid<HorarioDisponivelCenarioDTO> getGrid(){
		return this.gridPanel;
	}	
}
