package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.AssociarDisciplinasPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class AssociarDisciplinasView extends MyComposite implements
	AssociarDisciplinasPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CheckBox salaCheckBox;
	private CheckBox labCheckBox;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private TipoSalaDTO tipoSalaDTO;
	private CenarioDTO cenarioDTO;

	public AssociarDisciplinasView( CenarioDTO cenarioDTO )
	{
		this( new SalaDTO(), null, null, null, cenarioDTO );
		this.cenarioDTO = cenarioDTO;
	}

	public AssociarDisciplinasView( SalaDTO salaDTO, CampusDTO campusDTO,
		UnidadeDTO unidadeDTO, TipoSalaDTO tipoSalaDTO, CenarioDTO cenarioDTO )
	{
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;
		this.tipoSalaDTO = tipoSalaDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI()
	{

		simpleModal = new SimpleModal( "Associar", Resources.DEFAULTS.sala16() );
		simpleModal.setHeight( 150 );
		simpleModal.setWidth( 250 );

		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		
		salaCheckBox = new CheckBox();
		salaCheckBox.setFieldLabel("Salas");
		labCheckBox = new CheckBox();
		labCheckBox.setFieldLabel("Laboratórios");
		
		formPanel = new FormPanel(){
			@Override
			public boolean isValid(boolean b) {
				if(!getSalaCheckBox().getValue() && !getLaboratorioCheckBox().getValue()){
					return false;
				} 
				return super.isValid(b);
			}
		};
		formPanel.setHeaderVisible( false );
		
		
		formPanel.add( salaCheckBox, formData );
		
		
		formPanel.add( labCheckBox, formData );
		

		FormButtonBinding binding = new FormButtonBinding( formPanel );
		
		simpleModal.getSalvarBt().disable();
		binding.addButton( simpleModal.getSalvarBt() );

	}

	@Override
	public Button getSalvarButton()
	{
		return simpleModal.getSalvarBt();
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public CheckBox getSalaCheckBox() {
		return salaCheckBox;
	}

	@Override
	public CheckBox getLaboratorioCheckBox() {
		return labCheckBox;
	}
}
