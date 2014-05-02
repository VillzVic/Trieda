package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.CurriculoDisciplinaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class CurriculoDisciplinaFormView
	extends MyComposite
	implements CurriculoDisciplinaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private DisciplinaAutoCompleteBox disciplinaCB;
	private NumberField periodoTF;
	private NumberField maturidadeTF;
	private CurriculoDisciplinaDTO curriculoDisciplinaDTO;
	private CurriculoDTO curriculoDTO;
	private CenarioDTO cenarioDTO;

	public CurriculoDisciplinaFormView( CenarioDTO cenarioDTO,
		CurriculoDisciplinaDTO curriculoDisciplinaDTO,
		CurriculoDTO curriculoDTO )
	{
		this.curriculoDisciplinaDTO = curriculoDisciplinaDTO;
		this.curriculoDTO = curriculoDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
	}

	private void initUI()
	{
		String title = "Inserção de Disciplina na Matriz Curricular";
		simpleModal = new SimpleModal( title,
			Resources.DEFAULTS.disciplina16() );
		simpleModal.setHeight( 230 );
		simpleModal.setWidth( 430 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		formPanel = new FormPanel();
		formPanel.setHeaderVisible( false );

		TextField< String > curriculoTF = new TextField< String >();
		curriculoTF.setValue( curriculoDTO.getCodigo() );
		curriculoTF.setFieldLabel( "Matriz Curricular" );
		curriculoTF.setReadOnly( true );
		formPanel.add( curriculoTF, formData );

		TextField< String > cursoTF = new TextField< String >();
		cursoTF.setValue( curriculoDTO.getCursoString() );
		cursoTF.setFieldLabel( "Curso" );
		cursoTF.setReadOnly( true );
		formPanel.add( cursoTF, formData );

		disciplinaCB = new DisciplinaAutoCompleteBox( cenarioDTO );
		disciplinaCB.setName( "disciplina" );
		disciplinaCB.setFieldLabel( "Disciplina" );
		disciplinaCB.setAllowBlank( false );
		disciplinaCB.setEmptyText( "Preencha a disciplina" );
		formPanel.add( disciplinaCB, formData );

		periodoTF = new NumberField();
		periodoTF.setName( CurriculoDisciplinaDTO.PROPERTY_PERIODO );
		periodoTF.setValue( curriculoDisciplinaDTO.getPeriodo() );
		periodoTF.setFieldLabel( "Período" );
		periodoTF.setAllowBlank( false );
		periodoTF.setAllowDecimals( false );
		periodoTF.setMaxValue( 99 );
		periodoTF.setEmptyText( "Preencha o período" );
		formPanel.add( periodoTF, formData );
		
		maturidadeTF = new NumberField();
		maturidadeTF.setName( CurriculoDisciplinaDTO.PROPERTY_MATURIDADE );
		maturidadeTF.setValue( curriculoDisciplinaDTO.getMaturidade() );
		maturidadeTF.setFieldLabel( "Maturidade" );
		maturidadeTF.setAllowBlank( true );
		maturidadeTF.setAllowDecimals( false );
		maturidadeTF.setMaxValue( 999 );
		maturidadeTF.setEmptyText( "Preencha a maturidade" );
		maturidadeTF.setToolTip("É o número mínimo de horas que o aluno deve ter cursado para poder cursar a disciplina em questão.");
		formPanel.add( maturidadeTF, formData );

		FormButtonBinding binding = new FormButtonBinding( formPanel );
		binding.addButton( simpleModal.getSalvarBt() );

		simpleModal.setFocusWidget( curriculoTF );
	}

	public boolean isValid()
	{
		return formPanel.isValid();
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
	public DisciplinaAutoCompleteBox getDisciplinaComboBox()
	{
		return disciplinaCB;
	}

	@Override
	public NumberField getPeriodoTextField()
	{
		return periodoTF;
	}
	
	@Override
	public NumberField getMaturidadeTextField()
	{
		return maturidadeTF;
	}


	@Override
	public CurriculoDTO getCurriculoDTO()
	{
		return curriculoDTO;
	}

	@Override
	public CurriculoDisciplinaDTO getCurriculoDisciplinaDTO()
	{
		return curriculoDisciplinaDTO;
	}
}
