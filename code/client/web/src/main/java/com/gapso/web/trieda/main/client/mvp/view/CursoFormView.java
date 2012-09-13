package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.CursoFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoCursoComboBox;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class CursoFormView
	extends MyComposite
	implements CursoFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > nomeTF;
	private TextField< String > codigoTF;
	private TipoCursoComboBox tipoCursoCB;
	private NumberField numMinDoutoresTF;
	private NumberField numMinMestresTF;
	private NumberField minTempoIntegralParcialTF;
	private NumberField minTempoIntegralTF;
	private NumberField maxDisciplinasPeloProfessorTF;
	private CheckBox admMaisDeUmDisciplinaCB;
	private CursoDTO cursoDTO;
	private TipoCursoDTO tipoCursoDTO;
	private CenarioDTO cenarioDTO;

	public CursoFormView( CenarioDTO cenarioDTO )
	{
		this( new CursoDTO(), null, cenarioDTO );
	}

	public CursoFormView( CursoDTO cursoDTO,
		TipoCursoDTO tipoCursoDTO, CenarioDTO cenarioDTO )
	{
		this.cursoDTO = cursoDTO;
		this.tipoCursoDTO = tipoCursoDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI() {
		String title = (cursoDTO.getId() == null) ? "Inserção de Curso"
				: "Edição de Curso";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.curso16());
		simpleModal.setWidth(550);
		simpleModal.setHeight(320);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setLabelWidth(210);
		//formPanel.setFieldWidth(fieldWidth);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);

		codigoTF = new UniqueTextField( cenarioDTO, UniqueDomain.CURSO );
		codigoTF.setName(CursoDTO.PROPERTY_CODIGO);
		codigoTF.setValue(cursoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);

		nomeTF = new TextField<String>();
		nomeTF.setName(CursoDTO.PROPERTY_NOME);
		nomeTF.setValue(cursoDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);

		tipoCursoCB = new TipoCursoComboBox();
		tipoCursoCB.setName("tipoCurso");
		tipoCursoCB.setFieldLabel("Tipo");
		tipoCursoCB.setAllowBlank(false);
		tipoCursoCB.setValue(tipoCursoDTO);
		tipoCursoCB.setEmptyText("Selecione um tipo de curso");
		formPanel.add(tipoCursoCB, formData);

		numMinMestresTF = new NumberField();
		numMinMestresTF.setName(CursoDTO.PROPERTY_NUM_MIN_MESTRES);
		numMinMestresTF.setValue(cursoDTO.getNumMinMestres());
		numMinMestresTF.setFieldLabel("% Min. Mestres + Doutores");
		numMinMestresTF.setAllowBlank(false);
		numMinMestresTF.setAllowDecimals(false);
		numMinMestresTF.setMaxValue(100);
		numMinMestresTF
				.setEmptyText("Preencha a porcentagem mínima de mestres");
		formPanel.add(numMinMestresTF, formData);

		numMinDoutoresTF = new NumberField();
		numMinDoutoresTF.setName(CursoDTO.PROPERTY_NUM_MIN_DOUTORES);
		numMinDoutoresTF.setValue(cursoDTO.getNumMinDoutores());
		numMinDoutoresTF.setFieldLabel("% Min. Doutores");
		numMinDoutoresTF.setAllowBlank(false);
		numMinDoutoresTF.setAllowDecimals(false);
		numMinDoutoresTF.setMaxValue(100);
		numMinDoutoresTF
				.setEmptyText("Preencha a porcentagem mínima de doutores");
		formPanel.add(numMinDoutoresTF, formData);

		minTempoIntegralParcialTF = new NumberField();
		minTempoIntegralParcialTF
				.setName(CursoDTO.PROPERTY_MIN_TEMPO_INTEGRAL_PARCIAL);
		minTempoIntegralParcialTF.setValue(cursoDTO
				.getMinTempoIntegralParcial());
		minTempoIntegralParcialTF.setFieldLabel("% Min. Tempo Integral + Parcial");
		minTempoIntegralParcialTF.setAllowBlank(false);
		minTempoIntegralParcialTF.setAllowDecimals(false);
		minTempoIntegralParcialTF.setMaxValue(100);
		formPanel.add(minTempoIntegralParcialTF, formData);

		minTempoIntegralTF = new NumberField();
		minTempoIntegralTF.setName(CursoDTO.PROPERTY_MIN_TEMPO_INTEGRAL);
		minTempoIntegralTF.setValue(cursoDTO.getMinTempoIntegral());
		minTempoIntegralTF.setFieldLabel("% Min Tempo Integral");
		minTempoIntegralTF.setAllowBlank(false);
		minTempoIntegralTF.setAllowDecimals(false);
		minTempoIntegralTF.setMaxValue(100);
		formPanel.add(minTempoIntegralTF, formData);

		maxDisciplinasPeloProfessorTF = new NumberField();
		maxDisciplinasPeloProfessorTF
				.setName(CursoDTO.PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR);
		maxDisciplinasPeloProfessorTF.setValue(cursoDTO
				.getMaxDisciplinasPeloProfessor());
		maxDisciplinasPeloProfessorTF.setFieldLabel("Max. Disciplinas por Professor");
		maxDisciplinasPeloProfessorTF.setAllowBlank(false);
		maxDisciplinasPeloProfessorTF.setAllowDecimals(false);
		maxDisciplinasPeloProfessorTF.setMaxValue(99);
		maxDisciplinasPeloProfessorTF
				.setEmptyText("Número máximo de disciplina por professor");
		formPanel.add(maxDisciplinasPeloProfessorTF, formData);

		admMaisDeUmDisciplinaCB = new CheckBox();
		admMaisDeUmDisciplinaCB
				.setName(CursoDTO.PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA);
		admMaisDeUmDisciplinaCB.setValue(cursoDTO.getAdmMaisDeUmDisciplina());
		admMaisDeUmDisciplinaCB
				.setFieldLabel("Permite mais de uma Disc. por Prof.?");
		formPanel.add(admMaisDeUmDisciplinaCB, formData);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(codigoTF);
	}

	public boolean isValid() {
		return formPanel.isValid();
	}

	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public TextField<String> getNomeTextField() {
		return nomeTF;
	}

	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}

	@Override
	public CursoDTO getCursoDTO() {
		return cursoDTO;
	}

	@Override
	public TipoCursoComboBox getTipoCursoComboBox() {
		return tipoCursoCB;
	}

	@Override
	public NumberField getNumMinDoutoresTextField() {
		return numMinDoutoresTF;
	}

	@Override
	public NumberField getNumMinMestresTextField() {
		return numMinMestresTF;
	}

	@Override
	public NumberField getMaxDisciplinasPeloProfessorTextField() {
		return maxDisciplinasPeloProfessorTF;
	}

	@Override
	public CheckBox getAdmMaisDeUmDisciplinaCheckBox() {
		return admMaisDeUmDisciplinaCB;
	}

	@Override
	public NumberField getMinTempoIntegralParcialTextField() {
		return minTempoIntegralParcialTF;
	}

	@Override
	public NumberField getMinTempoIntegralTextField() {
		return minTempoIntegralTF;
	}

}
