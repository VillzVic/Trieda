package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.extjs.gxt.ui.client.widget.layout.BoxLayout.BoxLayoutPack;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout.HBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ParametrosPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.SelecionarCampiPresenter.ParametrosViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosView extends MyComposite implements ParametrosPresenter.Display, ParametrosViewGateway {
	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	private ParametroDTO parametroDTO;

	private Radio taticoRadio;
	private Radio operacionalRadio;
	private Radio otimizarPorBlocoCurricular;
	private Radio otimizarPorAluno;
	private Label campiLabel;
	private Button selecionarCampiButton;
	private Label turnosLabel;
	private Button selecionarTurnosButton;

	private CheckBox cargaHorariaAlunoCheckBox;
	private CargaHorariaComboBox cargaHorariaAlunoComboBox;
	private CheckBox alunoDePeriodoMesmaSalaCheckBox;
	private CheckBox alunoEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoAlunoCheckBox;
	private CheckBox priorizarCalourosCheckBox;
	private CheckBox considerarPrioridadePorAlunosCheckBox;

	private CheckBox cargaHorariaProfessorCheckBox;
	private CargaHorariaComboBox cargaHorariaProfessorComboBox;
	private CheckBox professorEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoProfessorCheckBox;
	private NumberField minimizarDeslocamentoProfessorNumberField;
	private CheckBox minimizarGapProfessorCheckBox;
	private CheckBox proibirGapProfessorCheckBox;
	private CheckBox evitarReducaoCargaHorariaProfessorCheckBox;
	private NumberField evitarReducaoCargaHorariaProfessorNumberField;
	private NumberField evitarUltimoEPrimeiroHorarioProfessorNumberField;
	private CheckBox evitarUltimoEPrimeiroHorarioProfessorCheckBox;
	private CheckBox preferenciaDeProfessoresCheckBox;
	private CheckBox avaliacaoDesempenhoProfessorCheckBox;

	private FuncaoObjetivoComboBox funcaoObjetivoCheckBox;
	private CheckBox considerarEquivalenciaCheckBox;
	private CheckBox proibeCiclosEmEquivalenciaCheckBox;
	private CheckBox consideraTransitividadeEmEquivalenciaCheckBox;
	private CheckBox proibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox;
	private CheckBox nivelDificuldadeDisciplinaCheckBox;
	private CheckBox compatibilidadeDisciplinasMesmoDiaCheckBox;
	private CheckBox regrasGenericasDivisaoCreditoCheckBox;
	private CheckBox regrasEspecificasDivisaoCreditoCheckBox;
	private CheckBox maximizarNotaAvaliacaoCorpoDocenteCheckBox;
	private CheckBox minimizarCustoDocenteCursosCheckBox;
	private CheckBox minAlunosParaAbrirTurmaCheckBox;
	private NumberField minAlunosParaAbrirTurmaValueNumberField;
	private CheckBox violarMinAlunosAbrirTurmaParaFormandosCheckBox;
	private CheckBox compartilharDisciplinasCampiCheckBox;
	private CheckBox percentuaisMinimosMestresCheckBox;
	private CheckBox percentuaisMinimosDoutoresCheckBox;
	private CheckBox areaTitulacaoProfessoresECursosCheckBox;
	private CheckBox limitarMaximoDisciplinaProfessorCheckBox;
	private CheckBox considerarDemandasPrioridade2CheckBox;
	private CheckBox considerarCapacidadeMaximaDasSalasCheckBox;
	private CheckBox percentuaisMinimosTempoParcialIntegralCheckBox;
	private CheckBox percentuaisMinimosTempoIntegralCheckBox;
	private CheckBox considerarCoRequisitosCheckBox;

	private Button maximizarNotaAvaliacaoCorpoDocenteButton; 
	private Button minimizarCustoDocenteCursosButton; 
	private Button compartilharDisciplinasCampiButton; 
	
	private CenarioDTO cenarioDTO;

	public ParametrosView(CenarioDTO cenarioDTO, ParametroDTO parametroDTO) {
		this.cenarioDTO = cenarioDTO;
		this.parametroDTO = parametroDTO;
		initUI();
	}

	private void initUI() {
		createForm();
		createTabItem();
		initComponent(this.tabItem);
	}

	private void createTabItem() {
		this.tabItem = new GTabItem("Parâmetros de Planejamento",Resources.DEFAULTS.parametroPlanejamento16());
		this.tabItem.setContent(this.form,new Margins(5,5,0,5));
	}

	private void createForm() {
		this.form = new FormPanel();
		this.form.setHeadingHtml(cenarioDTO.getNome() + " » Parâmetros de Planejamento");
		this.form.setScrollMode(Scroll.AUTO);
		this.form.setButtonAlign(HorizontalAlignment.RIGHT);
		FormLayout formLayout = new FormLayout();
		formLayout.setLabelWidth(120);
		this.form.setLayout(formLayout);

		criaParametrosGerais();

		this.form.add(criaParametrosPreferenciasAlunos());
		this.form.add(criaParametrosPreferenciasProfessores());
		this.form.add(criaParametrosPreferenciasInstituicaoEnsino());

		this.submitBt = new Button("Gerar Grade",AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
		this.form.addButton(this.submitBt);
	}

	private FieldSet criaParametrosPreferenciasInstituicaoEnsino() {
		// coluna 1
		this.considerarEquivalenciaCheckBox = createCheckBox("Considerar equivalências entre disciplinas",this.parametroDTO.getConsiderarEquivalencia());
		this.minAlunosParaAbrirTurmaCheckBox = createCheckBox("Número mínimo de alunos para abertura de turma",this.parametroDTO.getMinAlunosParaAbrirTurma());
		this.violarMinAlunosAbrirTurmaParaFormandosCheckBox = createCheckBox("Desconsiderar mínimo de alunos para abertura de turma no caso de formandos",this.parametroDTO.getViolarMinAlunosAbrirTurmaParaFormandos());
		this.nivelDificuldadeDisciplinaCheckBox = createCheckBox("Considerar nível de dificuldade de disciplinas",this.parametroDTO.getNivelDificuldadeDisciplina());
		this.nivelDificuldadeDisciplinaCheckBox.hide();
		this.compatibilidadeDisciplinasMesmoDiaCheckBox = createCheckBox("Considerar compatibilidade de disciplinas no mesmo dia",this.parametroDTO.getCompatibilidadeDisciplinasMesmoDia());
		this.compatibilidadeDisciplinasMesmoDiaCheckBox.hide();
		this.regrasGenericasDivisaoCreditoCheckBox = createCheckBox("Considerar regras genéricas de divisão de créditos",this.parametroDTO.getRegrasGenericasDivisaoCredito());
		this.regrasEspecificasDivisaoCreditoCheckBox = createCheckBox("Considerar regras específicas de divisão de créditos",this.parametroDTO.getRegrasEspecificasDivisaoCredito());
		this.maximizarNotaAvaliacaoCorpoDocenteCheckBox = createCheckBox("Maximizar nota da avaliação do corpo docente de cursos específicos",this.parametroDTO.getMaximizarNotaAvaliacaoCorpoDocente());
		this.maximizarNotaAvaliacaoCorpoDocenteCheckBox.disable();
		this.minimizarCustoDocenteCursosCheckBox = createCheckBox("Minimizar custo docente de cursos específicos",this.parametroDTO.getMinimizarCustoDocenteCursos());
		this.minimizarCustoDocenteCursosCheckBox.hide();
		this.compartilharDisciplinasCampiCheckBox = createCheckBox("Permitir compartilhamento de disciplinas entre cursos",this.parametroDTO.getCompartilharDisciplinasCampi());
		this.percentuaisMinimosMestresCheckBox = createCheckBox("Considerar percentuais mínimos de mestres + doutores",this.parametroDTO.getPercentuaisMinimosMestres());
		this.percentuaisMinimosDoutoresCheckBox = createCheckBox("Considerar percentuais mínimos de doutores",this.parametroDTO.getPercentuaisMinimosDoutores());
		this.areaTitulacaoProfessoresECursosCheckBox = createCheckBox("Considerar áreas de titulação dos professores e cursos",this.parametroDTO.getAreaTitulacaoProfessoresECursos());
		this.areaTitulacaoProfessoresECursosCheckBox.hide();
		this.limitarMaximoDisciplinaProfessorCheckBox = createCheckBox("Limitar máximo de disciplinas que um professor pode ministrar por curso",this.parametroDTO.getLimitarMaximoDisciplinaProfessor());
		this.considerarDemandasPrioridade2CheckBox = createCheckBox("Considerar demandas de prioridade 2 para preencher não atendimentos de prioridade 1",this.parametroDTO.getConsiderarDemandasDePrioridade2());
		this.considerarCapacidadeMaximaDasSalasCheckBox= createCheckBox("Considerar capacidade máxima das salas",this.parametroDTO.getConsiderarCapacidadeMaxSalas());;
		this.percentuaisMinimosTempoParcialIntegralCheckBox = createCheckBox("Considerar percentuais mínimos de tempo parcial + integral",this.parametroDTO.getPercentuaisMinimosParcialIntegral());
		this.percentuaisMinimosTempoIntegralCheckBox = createCheckBox("Considerar percentuais mínimos de tempo integral",this.parametroDTO.getPercentuaisMinimosIntegral());
		this.considerarCoRequisitosCheckBox = createCheckBox("Considerar co-requisitos",this.parametroDTO.getConsiderarCoRequisitos());
		
		// coluna 2
		this.minAlunosParaAbrirTurmaValueNumberField = new NumberField();
		this.minAlunosParaAbrirTurmaValueNumberField.setEmptyText("Quantidade mínimo");
		this.minAlunosParaAbrirTurmaValueNumberField.setValue(this.parametroDTO.getMinAlunosParaAbrirTurmaValue());
		Button dificuldadeButton = createButton("Configurar níveis de dificuldade");
		dificuldadeButton.disable();
		dificuldadeButton.hide();
		this.maximizarNotaAvaliacaoCorpoDocenteButton = createButton("Configurar cursos");
		this.maximizarNotaAvaliacaoCorpoDocenteButton.disable();
		this.minimizarCustoDocenteCursosButton = createButton("Configurar cursos");
		this.compartilharDisciplinasCampiButton = createButton("Cursos que não compartilham");
		
		// outras colunas
		this.proibeCiclosEmEquivalenciaCheckBox = createCheckBox("Proibir ciclos em equivalências",this.parametroDTO.getProibirCiclosEmEquivalencia());
		this.proibeCiclosEmEquivalenciaCheckBox.setEnabled(this.considerarEquivalenciaCheckBox.getValue());
		this.consideraTransitividadeEmEquivalenciaCheckBox = createCheckBox("Considerar transitividade em equivalências",this.parametroDTO.getConsiderarTransitividadeEmEquivalencia());
		this.consideraTransitividadeEmEquivalenciaCheckBox.setEnabled(this.considerarEquivalenciaCheckBox.getValue());
		this.proibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox = createCheckBox("Proibir troca por disciplinas online ou sem créditos em equivalências",this.parametroDTO.getProibirTrocaPorDiscOnlineOuCredZeradosEmEquivalencia());
		this.proibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox.setEnabled(this.considerarEquivalenciaCheckBox.getValue());

		LayoutContainer equivalenciasContainer = new LayoutContainer(new ColumnLayout());
		equivalenciasContainer.add(this.considerarEquivalenciaCheckBox);
		equivalenciasContainer.add(this.proibeCiclosEmEquivalenciaCheckBox);
		equivalenciasContainer.add(this.consideraTransitividadeEmEquivalenciaCheckBox);
		equivalenciasContainer.add(this.proibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox);
		
		LayoutContainer minAlunosAberturaTurmaContainer = new LayoutContainer(new ColumnLayout());
		minAlunosAberturaTurmaContainer.add(this.minAlunosParaAbrirTurmaCheckBox);
		minAlunosAberturaTurmaContainer.add(this.minAlunosParaAbrirTurmaValueNumberField);
		
		LayoutContainer nivelDificuldadeDiscContainer = new LayoutContainer(new ColumnLayout());
		nivelDificuldadeDiscContainer.add(this.nivelDificuldadeDisciplinaCheckBox);
		nivelDificuldadeDiscContainer.add(dificuldadeButton);
		
		
		LayoutContainer compatibilidadeDiscContainer = new LayoutContainer(new ColumnLayout());
		compatibilidadeDiscContainer.add(this.compatibilidadeDisciplinasMesmoDiaCheckBox);
		Button configurarCompatibilidades = createButton("Configurar compatibilidades");
		configurarCompatibilidades.hide();
		compatibilidadeDiscContainer.add(configurarCompatibilidades);
		
		LayoutContainer regrasGenDivCredContainer = new LayoutContainer(new ColumnLayout());
		regrasGenDivCredContainer.add(this.regrasGenericasDivisaoCreditoCheckBox);
		regrasGenDivCredContainer.add(createButton("Configurar regras genéricas"));
		
		LayoutContainer notaAvaliacaoProfessoresContainer = new LayoutContainer(new ColumnLayout());
		notaAvaliacaoProfessoresContainer.add(this.maximizarNotaAvaliacaoCorpoDocenteCheckBox);
		notaAvaliacaoProfessoresContainer.add(this.maximizarNotaAvaliacaoCorpoDocenteButton);
		
		LayoutContainer minCustoProfessoresCursoEspecificoContainer = new LayoutContainer(new ColumnLayout());
		minCustoProfessoresCursoEspecificoContainer.add(this.minimizarCustoDocenteCursosCheckBox);
		minCustoProfessoresCursoEspecificoContainer.add(this.minimizarCustoDocenteCursosButton);
		minCustoProfessoresCursoEspecificoContainer.hide();
		
		LayoutContainer compartilharDiscEntreCursosContainer = new LayoutContainer(new ColumnLayout());
		compartilharDiscEntreCursosContainer.add(this.compartilharDisciplinasCampiCheckBox);
		compartilharDiscEntreCursosContainer.add(this.compartilharDisciplinasCampiButton);
		
		FieldSet instituicaoFS = new FieldSet();
		instituicaoFS.setHeadingHtml("Preferências da Instituição");
		instituicaoFS.setCollapsible(true);
		instituicaoFS.setLayout(new RowLayout());
		instituicaoFS.add(equivalenciasContainer);
		instituicaoFS.add(minAlunosAberturaTurmaContainer);
		instituicaoFS.add(this.violarMinAlunosAbrirTurmaParaFormandosCheckBox);
		instituicaoFS.add(nivelDificuldadeDiscContainer);
		instituicaoFS.add(compatibilidadeDiscContainer);
		instituicaoFS.add(regrasGenDivCredContainer);
		instituicaoFS.add(this.regrasEspecificasDivisaoCreditoCheckBox);
		instituicaoFS.add(notaAvaliacaoProfessoresContainer);
		instituicaoFS.add(minCustoProfessoresCursoEspecificoContainer);
		instituicaoFS.add(compartilharDiscEntreCursosContainer);
		instituicaoFS.add(this.percentuaisMinimosMestresCheckBox);
		instituicaoFS.add(this.percentuaisMinimosDoutoresCheckBox);
		instituicaoFS.add(this.areaTitulacaoProfessoresECursosCheckBox);
		instituicaoFS.add(this.limitarMaximoDisciplinaProfessorCheckBox);
		instituicaoFS.add(this.considerarDemandasPrioridade2CheckBox);
		instituicaoFS.add(this.considerarCapacidadeMaximaDasSalasCheckBox);
		instituicaoFS.add(this.percentuaisMinimosTempoParcialIntegralCheckBox);
		instituicaoFS.add(this.percentuaisMinimosTempoIntegralCheckBox);
		instituicaoFS.add(this.considerarCoRequisitosCheckBox);
		
		return instituicaoFS;
	}

	private FieldSet criaParametrosPreferenciasProfessores() {
		this.cargaHorariaProfessorCheckBox = createCheckBox("Distribuição da carga horária semanal do professor",this.parametroDTO.getCargaHorariaProfessor());
		this.cargaHorariaProfessorCheckBox.disable();
		this.professorEmMuitosCampiCheckBox = createCheckBox("Permitir que o professor ministre aulas em mais de um campus",this.parametroDTO.getProfessorEmMuitosCampi());
		this.minimizarDeslocamentoProfessorCheckBox = createCheckBox("Minimizar deslocamentos de professores entre campi",this.parametroDTO.getMinimizarDeslocamentoProfessor());
		this.minimizarGapProfessorCheckBox = createCheckBox("Minimizar gaps nos horários dos professores",this.parametroDTO.getMinimizarGapProfessor());
		this.proibirGapProfessorCheckBox = createCheckBox("Proibir gaps nos horários dos professores",this.parametroDTO.getProibirGapProfessor());
		this.evitarReducaoCargaHorariaProfessorCheckBox = createCheckBox("Evitar redução de carga horária dos professores",this.parametroDTO.getEvitarReducaoCargaHorariaProfessor());
		this.evitarReducaoCargaHorariaProfessorCheckBox.disable();
		this.evitarUltimoEPrimeiroHorarioProfessorCheckBox = createCheckBox("Considerar interjornada mínima (em horas)",this.parametroDTO.getEvitarUltimoEPrimeiroHorarioProfessor());
		this.preferenciaDeProfessoresCheckBox = createCheckBox("Considerar preferência de professores por disciplinas",this.parametroDTO.getPreferenciaDeProfessores());
		this.preferenciaDeProfessoresCheckBox.disable();
		this.avaliacaoDesempenhoProfessorCheckBox = createCheckBox("Considerar avaliação de desempenho de professores",this.parametroDTO.getAvaliacaoDesempenhoProfessor());
		this.avaliacaoDesempenhoProfessorCheckBox.disable();
		
		this.cargaHorariaProfessorComboBox = createComboBox(this.parametroDTO.getCargaHorariaProfessorSel());
		this.cargaHorariaProfessorComboBox.disable();
		this.minimizarDeslocamentoProfessorNumberField = new NumberField();
		this.minimizarDeslocamentoProfessorNumberField.setEmptyText("Configurar Máx de deslocamento");
		this.minimizarDeslocamentoProfessorNumberField.setValue(this.parametroDTO.getMinimizarDeslocamentoProfessorValue());
		this.minimizarDeslocamentoProfessorNumberField.disable();
		this.evitarReducaoCargaHorariaProfessorNumberField = new NumberField();
		this.evitarReducaoCargaHorariaProfessorNumberField.setEmptyText("Configurar % de tolerância");
		this.evitarReducaoCargaHorariaProfessorNumberField.setValue(this.parametroDTO.getEvitarReducaoCargaHorariaProfessorValue());
		this.evitarReducaoCargaHorariaProfessorNumberField.disable();
		this.evitarUltimoEPrimeiroHorarioProfessorNumberField = new NumberField();
		this.evitarUltimoEPrimeiroHorarioProfessorNumberField.setEmptyText("Valor da interjornada (horas)");
		this.evitarUltimoEPrimeiroHorarioProfessorNumberField.setValue(this.parametroDTO.getEvitarUltimoEPrimeiroHorarioProfessorValue());
		
		LayoutContainer cargaHorariaSemanalProfessorContainer = new LayoutContainer(new ColumnLayout());
		cargaHorariaSemanalProfessorContainer.add(this.cargaHorariaProfessorCheckBox);
		cargaHorariaSemanalProfessorContainer.add(this.cargaHorariaProfessorComboBox);
		
		LayoutContainer minDeslocamentoProfessorContainer = new LayoutContainer(new ColumnLayout());
		minDeslocamentoProfessorContainer.add(this.minimizarDeslocamentoProfessorCheckBox);
		minDeslocamentoProfessorContainer.add(this.minimizarDeslocamentoProfessorNumberField);
		
		LayoutContainer evitarReducaoCHProfessorContainer = new LayoutContainer(new ColumnLayout());
		evitarReducaoCHProfessorContainer.add(this.evitarReducaoCargaHorariaProfessorCheckBox);
		evitarReducaoCHProfessorContainer.add(this.evitarReducaoCargaHorariaProfessorNumberField);
		
		LayoutContainer evitarUltimoEPrimeiroHorarioProfessorContainer = new LayoutContainer(new ColumnLayout());
		evitarUltimoEPrimeiroHorarioProfessorContainer.add(this.evitarUltimoEPrimeiroHorarioProfessorCheckBox);
		evitarUltimoEPrimeiroHorarioProfessorContainer.add(this.evitarUltimoEPrimeiroHorarioProfessorNumberField);

		FieldSet professorFS = new FieldSet();
		professorFS.setHeadingHtml("Preferências do Professor");
		professorFS.setCollapsible(true);
		professorFS.setLayout(new RowLayout());
		professorFS.add(cargaHorariaSemanalProfessorContainer);
		professorFS.add(this.professorEmMuitosCampiCheckBox);
		professorFS.add(minDeslocamentoProfessorContainer);
		LayoutContainer gapsContainer = new LayoutContainer(new ColumnLayout());
		gapsContainer.add(this.minimizarGapProfessorCheckBox);
		gapsContainer.add(this.proibirGapProfessorCheckBox);
		professorFS.add(gapsContainer);
		professorFS.add(evitarReducaoCHProfessorContainer);
		professorFS.add(evitarUltimoEPrimeiroHorarioProfessorContainer);
		professorFS.add(this.preferenciaDeProfessoresCheckBox);
		professorFS.add(this.avaliacaoDesempenhoProfessorCheckBox);
		
		return professorFS;
	}

	private FieldSet criaParametrosPreferenciasAlunos() {		
		this.cargaHorariaAlunoCheckBox = createCheckBox("Distribuição da carga horária semanal do aluno",this.parametroDTO.getCargaHorariaAluno());
		this.cargaHorariaAlunoCheckBox.disable();
		this.alunoDePeriodoMesmaSalaCheckBox = createCheckBox("Manter alunos do mesmo curso-período na mesma sala",this.parametroDTO.getAlunoDePeriodoMesmaSala());
		this.alunoEmMuitosCampiCheckBox = createCheckBox("Permitir que o aluno estude em mais de um campus",this.parametroDTO.getAlunoEmMuitosCampi());
		this.alunoEmMuitosCampiCheckBox.disable();
		this.minimizarDeslocamentoAlunoCheckBox = createCheckBox("Minimizar Deslocamento de Alunos entre campi",this.parametroDTO.getMinimizarDeslocamentoAluno());
		this.minimizarDeslocamentoAlunoCheckBox.disable();
		this.cargaHorariaAlunoComboBox = createComboBox(this.parametroDTO.getCargaHorariaAlunoSel());
		this.cargaHorariaAlunoComboBox.disable();
		this.priorizarCalourosCheckBox = createCheckBox("Priorizar calouros", this.parametroDTO.getPriorizarCalouros());
		this.considerarPrioridadePorAlunosCheckBox = createCheckBox("Considerar prioridade por aluno", this.parametroDTO.getConsiderarPrioridadePorAlunos());
		
		LayoutContainer cargaHorariaSemanalAlunoContainer = new LayoutContainer(new ColumnLayout());
		cargaHorariaSemanalAlunoContainer.add(this.cargaHorariaAlunoCheckBox);
		cargaHorariaSemanalAlunoContainer.add(this.cargaHorariaAlunoComboBox);

		FieldSet preferenciasAlunosFS = new FieldSet();
		preferenciasAlunosFS.setHeadingHtml("Preferências do Aluno");
		preferenciasAlunosFS.setCollapsible(true);
		preferenciasAlunosFS.setLayout(new RowLayout());
		preferenciasAlunosFS.add(cargaHorariaSemanalAlunoContainer);
		preferenciasAlunosFS.add(this.alunoDePeriodoMesmaSalaCheckBox);
		preferenciasAlunosFS.add(this.alunoEmMuitosCampiCheckBox);
		preferenciasAlunosFS.add(this.minimizarDeslocamentoAlunoCheckBox);
		preferenciasAlunosFS.add(this.priorizarCalourosCheckBox);
		preferenciasAlunosFS.add(this.considerarPrioridadePorAlunosCheckBox);
		
		return preferenciasAlunosFS;
	}

	private void criaParametrosGerais() {
	
		// MODO DE OTIMIZAÇÃO
		RadioGroup modoDeOtimizacaoRadioGroup = new RadioGroup();  
		modoDeOtimizacaoRadioGroup.setFieldLabel("Modo de Otimização");
		this.taticoRadio = new Radio();  
		this.taticoRadio.setBoxLabel("Tático");
		modoDeOtimizacaoRadioGroup.add(this.taticoRadio);
		this.operacionalRadio = new Radio();  
		this.operacionalRadio.setBoxLabel("Operacional");
		modoDeOtimizacaoRadioGroup.add(this.operacionalRadio);
		this.form.add(modoDeOtimizacaoRadioGroup);
		
		// OTIMIZAR POR
		RadioGroup otimizarPorRadioGroup = new RadioGroup();  
		otimizarPorRadioGroup.setFieldLabel("Otimizar Por");
		this.otimizarPorAluno = new Radio();
		this.otimizarPorAluno.setBoxLabel("Aluno");
		otimizarPorRadioGroup.add(this.otimizarPorAluno);
		this.otimizarPorBlocoCurricular = new Radio();
		this.otimizarPorBlocoCurricular.setBoxLabel("Bloco Curricular");
		otimizarPorRadioGroup.add(this.otimizarPorBlocoCurricular);
		otimizarPorRadioGroup.hide();
		this.form.add(otimizarPorRadioGroup);

		// FUNÇÃO OBJETIVO
		this.funcaoObjetivoCheckBox = new FuncaoObjetivoComboBox();
		this.funcaoObjetivoCheckBox.setFieldLabel("Função Objetivo");
		this.funcaoObjetivoCheckBox.hide();
		this.form.add(this.funcaoObjetivoCheckBox);
		
		// CAMPI SELECIONADOS
		this.campiLabel = new Label("Nenhum campus selecionado");
		this.selecionarCampiButton = new Button("Selecionar Campi");
		LayoutContainer campiSelecionadosContainer = new LayoutContainer();  
        HBoxLayout campiSelecionadosLayout = new HBoxLayout();  
        campiSelecionadosLayout.setPadding(new Padding(5));  
        campiSelecionadosLayout.setHBoxLayoutAlign(HBoxLayoutAlign.MIDDLE);  
        campiSelecionadosLayout.setPack(BoxLayoutPack.START);  
        campiSelecionadosContainer.setLayout(campiSelecionadosLayout);  
        HBoxLayoutData layoutData = new HBoxLayoutData(new Margins(0,5,0,0));  
        campiSelecionadosContainer.add(this.campiLabel,layoutData);  
        campiSelecionadosContainer.add(this.selecionarCampiButton,layoutData);  
		this.form.add(campiSelecionadosContainer);
		
		// TURNOS SELECIONADOS
		this.turnosLabel = new Label("Nenhum turno selecionado");
		this.selecionarTurnosButton = new Button("Selecionar Turnos");
		LayoutContainer turnosSelecionadosContainer = new LayoutContainer();  
        HBoxLayout turnosSelecionadosLayout = new HBoxLayout();  
        turnosSelecionadosLayout.setPadding(new Padding(5));  
        turnosSelecionadosLayout.setHBoxLayoutAlign(HBoxLayoutAlign.MIDDLE);  
        turnosSelecionadosLayout.setPack(BoxLayoutPack.START);  
        turnosSelecionadosContainer.setLayout(turnosSelecionadosLayout);  
        turnosSelecionadosContainer.add(this.turnosLabel,layoutData);  
        turnosSelecionadosContainer.add(this.selecionarTurnosButton,layoutData);  
		this.form.add(turnosSelecionadosContainer);
		
		// INICIALIZA PARAMETROS
		this.taticoRadio.setValue(this.parametroDTO.isTatico() || !parametroDTO.isOperacional());
		this.operacionalRadio.setValue(this.parametroDTO.isOperacional());
		boolean otimizarPor = (this.parametroDTO.getOtimizarPor() == null) ? true : (this.parametroDTO.getOtimizarPor().equals(ParametroDTO.OTIMIZAR_POR_ALUNO));
		this.otimizarPorAluno.setValue(otimizarPor);
		this.otimizarPorBlocoCurricular.setValue(!otimizarPor);
		this.funcaoObjetivoCheckBox.setValue(this.parametroDTO.getFuncaoObjetivo());
		updateQuantidadeCampiSelecionados(this.parametroDTO.getCampi().size());
		updateQuantidadeTurnosSelecionados(this.parametroDTO.getTurnos().size());
	}

	private CheckBox createCheckBox(
		String label, boolean value )
	{
		CheckBox cb = new CheckBox();

		cb.setHideLabel( true );
		cb.setBoxLabel( label );
		cb.setValue( value );

		return cb;
	}
	
	private CargaHorariaComboBox createComboBox( String value )
	{
		CargaHorariaComboBox cb = new CargaHorariaComboBox();

		cb.setValue( value );
		cb.setWidth( 200 );

		return cb;
	}
	
	private Button createButton( String text )
	{
		Button bt = new Button( text );

		return bt;
	}
	
	@Override
	public ParametroDTO getParametroDTO()
	{
		return this.parametroDTO;
	}
	
	@Override
	public CheckBox getCargaHorariaAlunoCheckBox()
	{
		return this.cargaHorariaAlunoCheckBox;
	}

	@Override
	public CargaHorariaComboBox getCargaHorariaAlunoComboBox()
	{
		return this.cargaHorariaAlunoComboBox;
	}
	
	@Override
	public CheckBox getPriorizarCalourosComboBox()
	{
		return this.priorizarCalourosCheckBox;
	}
	
	
	@Override
	public CheckBox getConsiderarPrioridadePorAlunosCheckBox()
	{
		return this.considerarPrioridadePorAlunosCheckBox;
	}

	@Override
	public CheckBox getAlunoDePeriodoMesmaSalaCheckBox()
	{
		return this.alunoDePeriodoMesmaSalaCheckBox;
	}

	@Override
	public CheckBox getAlunoEmMuitosCampiCheckBox()
	{
		return this.alunoEmMuitosCampiCheckBox;
	}

	@Override
	public CheckBox getMinimizarDeslocamentoAlunoCheckBox()
	{
		return this.minimizarDeslocamentoAlunoCheckBox;
	}

	@Override
	public CheckBox getCargaHorariaProfessorCheckBox()
	{
		return this.cargaHorariaProfessorCheckBox;
	}

	@Override
	public CargaHorariaComboBox getCargaHorariaProfessorComboBox()
	{
		return this.cargaHorariaProfessorComboBox;
	}

	@Override
	public CheckBox getProfessorEmMuitosCampiCheckBox()
	{
		return this.professorEmMuitosCampiCheckBox;
	}

	@Override
	public CheckBox getMinimizarDeslocamentoProfessorCheckBox()
	{
		return this.minimizarDeslocamentoProfessorCheckBox;
	}

	@Override
	public NumberField getMinimizarDeslocamentoProfessorNumberField()
	{
		return this.minimizarDeslocamentoProfessorNumberField;
	}

	@Override
	public CheckBox getMinimizarGapProfessorCheckBox()
	{
		return this.minimizarGapProfessorCheckBox;
	}
	
	@Override
	public CheckBox getProibirGapProfessorCheckBox()
	{
		return this.proibirGapProfessorCheckBox;
	}

	@Override
	public CheckBox getEvitarReducaoCargaHorariaProfessorCheckBox()
	{
		return this.evitarReducaoCargaHorariaProfessorCheckBox;
	}

	@Override
	public NumberField getEvitarReducaoCargaHorariaProfessorNumberField()
	{
		return this.evitarReducaoCargaHorariaProfessorNumberField;
	}

	@Override
	public CheckBox getEvitarUltimoEPrimeiroHorarioProfessorCheckBox()
	{
		return this.evitarUltimoEPrimeiroHorarioProfessorCheckBox;
	}
	
	@Override
	public NumberField getEvitarUltimoEPrimeiroHorarioProfessorNumberField()
	{
		return this.evitarUltimoEPrimeiroHorarioProfessorNumberField;
	}

	@Override
	public CheckBox getPreferenciaDeProfessoresCheckBox()
	{
		return this.preferenciaDeProfessoresCheckBox;
	}

	@Override
	public CheckBox getAvaliacaoDesempenhoProfessorCheckBox()
	{
		return this.avaliacaoDesempenhoProfessorCheckBox;
	}

	@Override
	public CheckBox getPercentuaisMinimosTempoParcialIntegralCheckBox()
	{
		return this.percentuaisMinimosTempoParcialIntegralCheckBox;
	}
	
	@Override
	public CheckBox getPercentuaisMinimosTempoIntegralCheckBox()
	{
		return this.percentuaisMinimosTempoIntegralCheckBox;
	}
	
	@Override
	public CheckBox getConsiderarCoRequisitosCheckBox() {
		return considerarCoRequisitosCheckBox;
	}
	
	@Override
	public CheckBox getNivelDificuldadeDisciplinaCheckBox()
	{
		return this.nivelDificuldadeDisciplinaCheckBox;
	}

	@Override
	public CheckBox getCompatibilidadeDisciplinasMesmoDiaCheckBox()
	{
		return this.compatibilidadeDisciplinasMesmoDiaCheckBox;
	}

	@Override
	public CheckBox getRegrasGenericasDivisaoCreditoCheckBox()
	{
		return this.regrasGenericasDivisaoCreditoCheckBox;
	}

	@Override
	public CheckBox getRegrasEspecificasDivisaoCreditoCheckBox()
	{
		return this.regrasEspecificasDivisaoCreditoCheckBox;
	}

	@Override
	public CheckBox getMaximizarNotaAvaliacaoCorpoDocenteCheckBox()
	{
		return this.maximizarNotaAvaliacaoCorpoDocenteCheckBox;
	}

	@Override
	public CheckBox getMinimizarCustoDocenteCursosCheckBox()
	{
		return this.minimizarCustoDocenteCursosCheckBox;
	}

	@Override
	public CheckBox getMinAlunosParaAbrirTurmaCheckBox()
	{
		return this.minAlunosParaAbrirTurmaCheckBox;
	}

	@Override
	public NumberField getMinAlunosParaAbrirTurmaValueNumberField()
	{
		return this.minAlunosParaAbrirTurmaValueNumberField;
	}
	
	@Override
	public CheckBox getViolarMinAlunosAbrirTurmaParaFormandosCheckBoxCheckBox() {
		return this.violarMinAlunosAbrirTurmaParaFormandosCheckBox;
	}

	@Override
	public CheckBox getCompartilharDisciplinasCampiCheckBox()
	{
		return this.compartilharDisciplinasCampiCheckBox;
	}

	@Override
	public CheckBox getPercentuaisMinimosMestresCheckBox()
	{
		return this.percentuaisMinimosMestresCheckBox;
	}

	@Override
	public CheckBox getPercentuaisMinimosDoutoresCheckBox()
	{
		return this.percentuaisMinimosDoutoresCheckBox;
	}

	@Override
	public CheckBox getAreaTitulacaoProfessoresECursosCheckBox()
	{
		return this.areaTitulacaoProfessoresECursosCheckBox;
	}

	@Override
	public CheckBox getLimitarMaximoDisciplinaProfessorCheckBox()
	{
		return this.limitarMaximoDisciplinaProfessorCheckBox;
	}
	
	@Override
	public CheckBox getConsiderarDemandasPrioridade2CheckBox() {
		return this.considerarDemandasPrioridade2CheckBox;
	}
	
	@Override
	public CheckBox getConsiderarCapacidadeMaximaDasSalasCheckBox() {
		return this.considerarCapacidadeMaximaDasSalasCheckBox;
	}

	@Override
	public Button getSubmitButton()
	{
		return this.submitBt;
	}

	@Override
	public Radio getTaticoRadio()
	{
		return this.taticoRadio;
	}

	@Override
	public Radio getOperacionalRadio()
	{
		return this.operacionalRadio;
	}
	
	@Override
	public Radio getOtimizarPorAlunoRadio(){
		return this.otimizarPorAluno;
	}
	
	@Override
	public Radio getOtimizarPorBlocoRadio(){
		return this.otimizarPorBlocoCurricular;
	}

	@Override
	public Button getMaximizarNotaAvaliacaoCorpoDocenteButton()
	{
		return this.maximizarNotaAvaliacaoCorpoDocenteButton;
	}

	@Override
	public Button getMinimizarCustoDocenteCursosButton()
	{
		return this.minimizarCustoDocenteCursosButton;
	}

	@Override
	public Button getCompartilharDisciplinasCampiButton()
	{
		return this.compartilharDisciplinasCampiButton;
	}

	@Override
	public FuncaoObjetivoComboBox getFuncaoObjetivoComboBox()
	{
		return this.funcaoObjetivoCheckBox;
	}

	@Override
	public CheckBox getConsiderarEquivalenciaCheckBox()
	{
		return this.considerarEquivalenciaCheckBox;
	}
	
	@Override
	public CheckBox getProibeCiclosEmEquivalenciaCheckBox() {
		return this.proibeCiclosEmEquivalenciaCheckBox;
	}
	
	@Override
	public CheckBox getConsideraTransitividadeEmEquivalenciaCheckBox() {
		return this.consideraTransitividadeEmEquivalenciaCheckBox;
	}
	
	@Override
	public CheckBox getProibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox() {
		return this.proibeTrocaPorDisciplinasOnlineOuSemCreditosEmEquivalenciaCheckBox;
	}

	@Override
	public Label getCampiLabel() {
		return this.campiLabel;
	}

	@Override
	public Button getSelecionarCampiButton() {
		return this.selecionarCampiButton;
	}
	
	@Override
	public Label getTurnosLabel() {
		return this.turnosLabel;
	}

	@Override
	public Button getSelecionarTurnosButton() {
		return this.selecionarTurnosButton;
	}

	@Override
	public void updateQuantidadeCampiSelecionados(int qtdCampiSelecionados) { 
		if (qtdCampiSelecionados <= 0) {
			this.campiLabel.setHtml("Nenhum campus selecionado");
		} else if (qtdCampiSelecionados == 1) {
			this.campiLabel.setHtml(qtdCampiSelecionados + " campus selecionado");
		} else {
			this.campiLabel.setHtml(qtdCampiSelecionados + " campi selecionados");
		}
	}
	
	@Override
	public void updateQuantidadeTurnosSelecionados(int qtdTurnosSelecionados) {
		if (qtdTurnosSelecionados <= 0) {
			this.turnosLabel.setHtml("Nenhum turno selecionado");
		} else if (qtdTurnosSelecionados == 1) {
			this.turnosLabel.setHtml(qtdTurnosSelecionados + " turno selecionado");
		} else {
			this.turnosLabel.setHtml(qtdTurnosSelecionados + " turnos selecionados");
		}
	}
}
