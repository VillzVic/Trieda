package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.DisciplinaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DificuldadeComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoDisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class DisciplinaFormView
	extends MyComposite
	implements DisciplinaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private NumberField creditosTeoricoTF;
	private NumberField creditosPraticoTF;
	private CheckBox laboratorioCB;
	private CheckBox aulasContinuasCB;
	private CheckBox professorUnicoCB;
	private CheckBox usaSabadoCB;
	private CheckBox usaDomingoCB;
	private TipoDisciplinaComboBox tipoDisciplinaCB;
	private DificuldadeComboBox dificuldadeCB;
	private NumberField maxAlunosTeoricoTF;
	private NumberField maxAlunosPraticoTF;
	private NumberField cargaHorariaTF;
	private DisciplinaDTO disciplinaDTO;
	private TipoDisciplinaDTO tipoDisciplinaDTO;
	private CenarioDTO cenarioDTO;

	public DisciplinaFormView( CenarioDTO cenarioDTO )
	{
		this( new DisciplinaDTO(), null, cenarioDTO );
	}

	public DisciplinaFormView( DisciplinaDTO disciplinaDTO,
		TipoDisciplinaDTO tipoDisciplinaDTO, CenarioDTO cenarioDTO )
	{
		this.disciplinaDTO = disciplinaDTO;
		this.tipoDisciplinaDTO = tipoDisciplinaDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI() {
		String title = (disciplinaDTO.getId() == null) ? "Inserção de Disciplina"
				: "Edição de Disciplina";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.disciplina16());
		simpleModal.setHeight(430);
		simpleModal.setWidth(400);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setLabelWidth(165);
		formPanel.setHeaderVisible(false);

		codigoTF = new UniqueTextField( cenarioDTO, UniqueDomain.DISCIPLINA );
		codigoTF.setName(DisciplinaDTO.PROPERTY_CODIGO);
		codigoTF.setValue(disciplinaDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);

		nomeTF = new TextField<String>();
		nomeTF.setName(DisciplinaDTO.PROPERTY_NOME);
		nomeTF.setValue(disciplinaDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);

		creditosTeoricoTF = new NumberField();
		creditosTeoricoTF.setName(DisciplinaDTO.PROPERTY_CREDITOS_TEORICO);
		creditosTeoricoTF.setValue(disciplinaDTO.getCreditosTeorico());
		creditosTeoricoTF.setFieldLabel("Créditos Teóricos");
		creditosTeoricoTF.setAllowBlank(false);
		creditosTeoricoTF.setAllowDecimals(false);
		creditosTeoricoTF.setMaxValue(99);
		creditosTeoricoTF
				.setEmptyText("Preencha a quantidade de créditos teóricos");
		formPanel.add(creditosTeoricoTF, formData);

		creditosPraticoTF = new NumberField();
		creditosPraticoTF.setName(DisciplinaDTO.PROPERTY_CREDITOS_PRATICO);
		creditosPraticoTF.setValue(disciplinaDTO.getCreditosPratico());
		creditosPraticoTF.setFieldLabel("Créditos Práticos");
		creditosPraticoTF.setAllowBlank(false);
		creditosPraticoTF.setAllowDecimals(false);
		creditosPraticoTF.setMaxValue(99);
		creditosPraticoTF
				.setEmptyText("Preencha a quantidade de créditos práticos");
		formPanel.add(creditosPraticoTF, formData);

		laboratorioCB = new CheckBox();
		laboratorioCB.setName(DisciplinaDTO.PROPERTY_LABORATORIO);
		laboratorioCB.setValue(disciplinaDTO.getLaboratorio());
		laboratorioCB.setFieldLabel("Usa Laboratório");
		laboratorioCB.setToolTip("Se estiver marcada, o Trieda buscará associações desta disciplina à laboratórios para realizar a alocação. Caso não haja nenhum laboratório associado a esta disciplina o Trieda acusa um erro de cadastro e não permitirá que a otimização ocorra até que o erro seja resolvido.");
		formPanel.add(laboratorioCB, formData);

		tipoDisciplinaCB = new TipoDisciplinaComboBox(cenarioDTO);
		tipoDisciplinaCB.setName("tipoDisciplina");
		tipoDisciplinaCB.setFieldLabel("Tipo de Disciplina");
		tipoDisciplinaCB.setAllowBlank(false);
		tipoDisciplinaCB.setValue(tipoDisciplinaDTO);
		tipoDisciplinaCB.setEmptyText("Selecione o tipo de disciplina");
		tipoDisciplinaCB.setToolTip("As disciplinas Presenciais serão alocadas pelo Trieda em salas e horários. As disciplinas Online não serão alocadas em salas nem horários pelo Trieda.");
		formPanel.add(tipoDisciplinaCB, formData);

		dificuldadeCB = new DificuldadeComboBox();
		dificuldadeCB.setName("dificuldade");
		dificuldadeCB.setFieldLabel("Nível de Dificuldade");
		dificuldadeCB.setAllowBlank(false);
		dificuldadeCB.setValue(disciplinaDTO.getDificuldade());
		dificuldadeCB.setEmptyText("Selecione o nível de dificuldade");
		formPanel.add(dificuldadeCB, formData);

		maxAlunosTeoricoTF = new NumberField();
		maxAlunosTeoricoTF.setName(DisciplinaDTO.PROPERTY_MAX_ALUNOS_TEORICO);
		maxAlunosTeoricoTF.setValue(disciplinaDTO.getMaxAlunosTeorico());
		maxAlunosTeoricoTF.setFieldLabel("Máx. Alunos - Teórico");
		maxAlunosTeoricoTF.setAllowBlank(false);
		maxAlunosTeoricoTF.setAllowDecimals(false);
		maxAlunosTeoricoTF.setMaxValue(999);
		maxAlunosTeoricoTF
				.setEmptyText("Preencha o número máximo de alunos teóricos");
		maxAlunosTeoricoTF.setToolTip("(OPCIONAL) Restrição pedagógica do número máximo de alunos em sala de aula para uma disciplina específica. Quanto omitido, o TRIEDA assume a capacidade da sala alocada.");
		formPanel.add(maxAlunosTeoricoTF, formData);

		maxAlunosPraticoTF = new NumberField();
		maxAlunosPraticoTF.setName(DisciplinaDTO.PROPERTY_MAX_ALUNOS_PRATICO);
		maxAlunosPraticoTF.setValue(disciplinaDTO.getMaxAlunosPratico());
		maxAlunosPraticoTF.setFieldLabel("Máx. Alunos - Prático");
		maxAlunosPraticoTF.setAllowBlank(false);
		maxAlunosPraticoTF.setAllowDecimals(false);
		maxAlunosPraticoTF.setMaxValue(999);
		maxAlunosPraticoTF
				.setEmptyText("Preencha o número máximo de alunos práticos");
		maxAlunosPraticoTF.setToolTip("(OPCIONAL) Restrição pedagógica do número máximo de alunos em laboratório para uma disciplina específica. Quanto omitido, o TRIEDA assume a capacidade do laboratório alocado.");
		formPanel.add(maxAlunosPraticoTF, formData);
		
		cargaHorariaTF = new NumberField();
		cargaHorariaTF.setName(DisciplinaDTO.PROPERTY_CARGA_HORARIA);
		cargaHorariaTF.setValue(disciplinaDTO.getCargaHoraria());
		cargaHorariaTF.setFieldLabel("Carga Horária");
		cargaHorariaTF.setAllowBlank(true);
		cargaHorariaTF.setAllowDecimals(false);
		cargaHorariaTF.setMaxValue(999);
		cargaHorariaTF
				.setEmptyText("Preencha a carga horária");
		cargaHorariaTF.hide();
		formPanel.add(cargaHorariaTF, formData);
		
		aulasContinuasCB = new CheckBox();
		aulasContinuasCB.setName(DisciplinaDTO.PROPERTY_AULAS_CONTINUAS);
		aulasContinuasCB.setValue(disciplinaDTO.getAulasContinuas());
		aulasContinuasCB.setFieldLabel("Aulas Contínuas?");
		aulasContinuasCB.setToolTip("(Apenas quando a disciplina tem créditos teóricos e práticos.) Determina se as aulas teóricas e práticas devem ser lecionadas em sequência.");
		formPanel.add(aulasContinuasCB, formData);
		
		professorUnicoCB = new CheckBox();
		professorUnicoCB.setName(DisciplinaDTO.PROPERTY_PROFESSOR_UNICO);
		professorUnicoCB.setValue(disciplinaDTO.getProfessorUnico());
		professorUnicoCB.setFieldLabel("Professor Único?");
		professorUnicoCB.setToolTip("(Apenas quando a disciplina tem créditos teóricos e práticos.) Determina se o professor deve ser o mesmo para as aulas teóricas e práticas.");
		formPanel.add(professorUnicoCB, formData);
		
		usaSabadoCB = new CheckBox();
		usaSabadoCB.setName(DisciplinaDTO.PROPERTY_USA_SABADO);
		usaSabadoCB.setValue(disciplinaDTO.getUsaSabado());
		usaSabadoCB.setFieldLabel("Pode ser alocada no sábado");
		usaSabadoCB.setToolTip("Determina se a disciplina deverá herdar os horários das semanas letivas também no sábado.");
		formPanel.add(usaSabadoCB, formData);

		usaDomingoCB = new CheckBox();
		usaDomingoCB.setName(DisciplinaDTO.PROPERTY_USA_DOMINGO);
		usaDomingoCB.setValue(disciplinaDTO.getUsaDomingo());
		usaDomingoCB.setFieldLabel("Pode ser alocada no domingo");
		usaDomingoCB.setToolTip("Determina se a disciplina deverá herdar os horários das semanas letivas também no domingo.");
		formPanel.add(usaDomingoCB, formData);

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
	public DisciplinaDTO getDisciplinaDTO() {
		return disciplinaDTO;
	}

	@Override
	public TipoDisciplinaComboBox getTipoDisciplinaComboBox() {
		return tipoDisciplinaCB;
	}

	@Override
	public NumberField getCreditosTeoricoTextField() {
		return creditosTeoricoTF;
	}

	@Override
	public NumberField getCreditosPraticoTextField() {
		return creditosPraticoTF;
	}

	@Override
	public CheckBox getLaboratorioCheckBox() {
		return laboratorioCB;
	}

	@Override
	public DificuldadeComboBox getDificuldadeComboBox() {
		return dificuldadeCB;
	}

	@Override
	public NumberField getMaxAlunosTeoricoTextField() {
		return maxAlunosTeoricoTF;
	}

	@Override
	public NumberField getMaxAlunosPraticoTextField() {
		return maxAlunosPraticoTF;
	}
	
	@Override
	public NumberField getCargaHorariaTextField() {
		return cargaHorariaTF;
	}
	
	@Override
	public CheckBox getUsaSabadoCheckBox() {
		return usaSabadoCB;
	}
	
	@Override
	public CheckBox getUsaDomingoCheckBox() {
		return usaDomingoCB;
	}

	@Override
	public CheckBox getAulasContinuasCheckBox() {
		return aulasContinuasCB;
	}

	@Override
	public CheckBox getProfessorUnicoCheckBox() {
		return professorUnicoCB;
	}
}
