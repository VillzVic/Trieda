package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.DivisaoCreditoDisciplinaFormPresenter;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class DivisaoCreditoDisciplinaFormView
	extends MyComposite
	implements DivisaoCreditoDisciplinaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private NumberField dia1NF;
	private NumberField dia2NF;
	private NumberField dia3NF;
	private NumberField dia4NF;
	private NumberField dia5NF;
	private NumberField dia6NF;
	private NumberField dia7NF;
	private DivisaoCreditoDTO divisaoCreditoDTO;
	private Button clearDivisaoCreditoBt;

	private DisciplinaDTO disciplinaDTO;

	public DivisaoCreditoDisciplinaFormView(
		DivisaoCreditoDTO divisaoCreditoDTO,
		DisciplinaDTO disciplinaDTO )
	{
		this.divisaoCreditoDTO = divisaoCreditoDTO;
		this.disciplinaDTO = disciplinaDTO;

		this.initUI();
	}

	private void initUI()
	{
		String title = "Divisão de Créditos da disciplina \""
			+ this.disciplinaDTO.getNome() + "\"";

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.divisaoDeCreditos16() );

		this.simpleModal.setHeight( 270 );
		this.createForm();
		this.simpleModal.setContent( this.formPanel );

		this.clearDivisaoCreditoBt = new Button( "Apagar",
			AbstractImagePrototype.create( Resources.DEFAULTS.cancel16() ) );
		this.simpleModal.addButton( this.clearDivisaoCreditoBt );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );

		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		this.dia1NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA1,
			"Dia 1", this.divisaoCreditoDTO.getDia1() );

		this.dia2NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA2,
			"Dia 2", this.divisaoCreditoDTO.getDia2() );

		this.dia3NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA3,
			"Dia 3", this.divisaoCreditoDTO.getDia3() );

		this.dia4NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA4,
			"Dia 4", this.divisaoCreditoDTO.getDia4() );

		this.dia5NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA5,
			"Dia 5", this.divisaoCreditoDTO.getDia5() );

		this.dia6NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA6,
			"Dia 6", this.divisaoCreditoDTO.getDia6() );

		this.dia7NF = createNumberField( DivisaoCreditoDTO.PROPERTY_DIA7,
			"Dia 7", this.divisaoCreditoDTO.getDia7() );

		this.formPanel.add( this.dia1NF, formData );
		this.formPanel.add( this.dia2NF, formData );
		this.formPanel.add( this.dia3NF, formData );
		this.formPanel.add( this.dia4NF, formData );
		this.formPanel.add( this.dia5NF, formData );
		this.formPanel.add( this.dia6NF, formData );
		this.formPanel.add( this.dia7NF, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.dia1NF );
	}

	private NumberField createNumberField(
		String name, String label, Integer value )
	{
		NumberField nf = new NumberField();

		nf.setName( name );
		nf.setValue( value );
		nf.setFieldLabel( label );
		nf.setAllowDecimals( false );
		nf.setAllowNegative( false );
		nf.setMinValue( 0 );
		nf.setMaxValue( 99 );
		nf.setMaxLength( 99 );
		nf.setEmptyText( "Somente números inteiros" );

		return nf;
	}

	public boolean isValid()
	{
		int totalCreditos = 0;

		totalCreditos += ( this.dia1NF.getValue() == null ? 0 : this.dia1NF.getValue().intValue() );
		totalCreditos += ( this.dia2NF.getValue() == null ? 0 : this.dia2NF.getValue().intValue() );
		totalCreditos += ( this.dia3NF.getValue() == null ? 0 : this.dia3NF.getValue().intValue() );
		totalCreditos += ( this.dia4NF.getValue() == null ? 0 : this.dia4NF.getValue().intValue() );
		totalCreditos += ( this.dia5NF.getValue() == null ? 0 : this.dia5NF.getValue().intValue() );
		totalCreditos += ( this.dia6NF.getValue() == null ? 0 : this.dia6NF.getValue().intValue() );
		totalCreditos += ( this.dia7NF.getValue() == null ? 0 : this.dia7NF.getValue().intValue() );

		return ( this.formPanel.isValid()
			&& totalCreditos == this.disciplinaDTO.getTotalCreditos() );
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
	public NumberField getDia1NumberField()
	{
		return this.dia1NF;
	}

	@Override
	public NumberField getDia2NumberField()
	{
		return this.dia2NF;
	}

	@Override
	public NumberField getDia3NumberField()
	{
		return this.dia3NF;
	}

	@Override
	public NumberField getDia4NumberField()
	{
		return this.dia4NF;
	}

	@Override
	public NumberField getDia5NumberField()
	{
		return this.dia5NF;
	}

	@Override
	public NumberField getDia6NumberField()
	{
		return this.dia6NF;
	}

	@Override
	public NumberField getDia7NumberField()
	{
		return this.dia7NF;
	}

	@Override
	public DivisaoCreditoDTO getDivisaoCreditoDTO()
	{
		return this.divisaoCreditoDTO;
	}

	@Override
	public DisciplinaDTO getDisciplinaDTO()
	{
		return this.disciplinaDTO;
	}

	@Override
	public Button getClearButton()
	{
		return this.clearDivisaoCreditoBt;
	}
}
