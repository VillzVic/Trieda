package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ParametrosGeracaoDemandaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosGeracaoDemandaView extends MyComposite 
	implements ParametrosGeracaoDemandaPresenter.Display
{
	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	private TurnoComboBox turnoComboBox;
	private CampusComboBox campusComboBox;
	private CheckBox usarDemandasPrioridade2CheckBox;
	private NumberField distanciaMaxEmPeriodosParaPrioridade2NumberField;
	private CheckBox considerarPreRequisitosCheckBox;
	private CheckBox considerarCoRequisitosCheckBox;
	private CheckBox considerarMaturidadeCheckBox;
	private Radio creditoPeriodoRadio;
	private Radio creditoManualRadio;
	private NumberField creditoManualNumberField;
	private CheckBox aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox;
	private NumberField fatorAumentoDeMaxCreditosNumberField;
	private Radio evolucaoRadio;
	private Radio criacaoAutomaticaRadio;
	private Radio criacaoDiretaRadio;
	private RadioGroup metodoGeracaoRadioGroup;
	private FormPanel panel;
	private Button ofertasCursosBt;
	private Button ofertasDemandasBt;
	private Button demandasAlunoBt;
	private LayoutContainer explicacaoMetodo;
	
	private CenarioDTO cenarioDTO;
	private ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO;
	public ParametrosGeracaoDemandaView(CenarioDTO cenarioDTO, ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO) {
		this.cenarioDTO = cenarioDTO;
		this.parametroGeracaoDemandaDTO = parametroGeracaoDemandaDTO;
		initUI();
	}

	private void initUI() {
		createForm();
		createTabItem();
		initComponent(this.tabItem);
	}

	private void createTabItem() {
		this.tabItem = new GTabItem("Parâmetros para Geração de Demanda",Resources.DEFAULTS.parametroPlanejamento16());
		this.tabItem.setContent(this.form,new Margins(5,5,0,5));
	}

	private void createForm() {
		this.form = new FormPanel();
		this.form.setHeadingHtml(cenarioDTO.getNome() + " » Parâmetros para Geração de Demanda");
		this.form.setScrollMode(Scroll.AUTO);
		this.form.setButtonAlign(HorizontalAlignment.RIGHT);
		FormLayout formLayout = new FormLayout();
		formLayout.setLabelWidth(200);
		this.form.setLayout(formLayout);
		
		FieldSet metodoFS = new FieldSet();
		metodoFS.setHeadingHtml("Selecione o método de Geração de Demanda");
		metodoFS.setLayout(new RowLayout());
		metodoFS.setCollapsible(false);
		metodoGeracaoRadioGroup = new RadioGroup();
		metodoGeracaoRadioGroup.setFieldLabel("Metodo de Geração de Demanda");
		this.evolucaoRadio = new Radio();  
		this.evolucaoRadio.setBoxLabel("Evolução dos Alunos na Matriz Curricular");
		this.evolucaoRadio.setValue(true);
		metodoGeracaoRadioGroup.add(this.evolucaoRadio);
		this.criacaoAutomaticaRadio = new Radio();  
		this.criacaoAutomaticaRadio.setBoxLabel("Criação Automática de Alunos e Demandas a partir de Quantidades Informadas");
		metodoGeracaoRadioGroup.add(this.criacaoAutomaticaRadio);
		this.criacaoDiretaRadio = new Radio();
		this.criacaoDiretaRadio.setBoxLabel("Criação Direta de Alunos e Demandas");
		metodoGeracaoRadioGroup.add(this.criacaoDiretaRadio);
		metodoFS.add(metodoGeracaoRadioGroup);
		explicacaoMetodo = new LayoutContainer();
		explicacaoMetodo.setStyleAttribute("margin-top", "17px");
		explicacaoMetodo.addText("As <i>demandas</i> representam o ponto de partida para o <b>Trieda</b> gerar as <i>Grades de Horário</i>" +
				" de cada aluno, professor e ambiente. Uma <i>demanda</i> representa um par (aluno,disciplina) significando que o <b>Trieda</b>" +
				" deve tentar gerar uma solução na qual o <i>aluno</i> assistirá às aulas da <i>disciplina</i>. Dessa forma, o método de geração" +
				" de demanda denominado <b>Evolução dos Alunos na Matriz Curricular</b> irá gerar os pares (aluno,disciplina) com base na evolução" +
				" de cada aluno em sua respectiva matriz curricular. Em outras palavras, a partir das <i>matrizes curriculares</i> e das" +
				" <i>disciplinas já cursadas</i> por cada <i>aluno</i>, o <b>Trieda</b> determinará para cada <i>aluno</i>, de forma automática," +
				" as próximas <i>disciplinas</i> que deveriam ser oferecidas ao <i>aluno</i> em questão. No entanto, esta determinação é controlada" +
				" por pré-requisitos, co-requisitos, máximo de créditos por aluno entre outros. Vale notar que a utilização deste método pressupõe a" +
				" existência dos alunos na base de dados do <b>Trieda</b>.");
		metodoFS.add(explicacaoMetodo);
		form.add(metodoFS);
		
		panel = new FormPanel();
		panel.setBodyBorder(false);
		panel.setHeaderVisible(false);
		panel.setStyleAttribute("margin-top", "10px");
		
		// CAMPI
		campusComboBox = new CampusComboBox(cenarioDTO);
		campusComboBox.setFieldLabel("Campus");
		panel.add(campusComboBox);

		// TURNO
		turnoComboBox = new TurnoComboBox(cenarioDTO);
		turnoComboBox.setFieldLabel("Turno");
		panel.add(turnoComboBox);
		
		panel.add(criaParametros());

		this.ofertasCursosBt = new Button("Ofertas de Cursos em Campi",AbstractImagePrototype.create(Resources.DEFAULTS.ofertaCurso16()));
		ofertasCursosBt.setStyleAttribute("margin-top", "15px");
		ofertasCursosBt.setStyleAttribute("margin-bottom", "45px");
		this.ofertasDemandasBt = new Button("Ofertas e Demandas",AbstractImagePrototype.create(Resources.DEFAULTS.demanda16()));
		ofertasDemandasBt.setStyleAttribute("margin-top", "15px");
		this.demandasAlunoBt =  new Button("Demandas por Aluno",AbstractImagePrototype.create(Resources.DEFAULTS.demandaAluno16()));
		this.demandasAlunoBt.setStyleAttribute("margin-top", "5px");
		this.submitBt = new Button("Gerar Demandas",AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
		panel.addButton(this.submitBt);
		
		this.form.add(panel);
	}
	
	private FieldSet criaParametros() {
		this.usarDemandasPrioridade2CheckBox = createCheckBox("Usar demandas de <u>prioridade 2</u> para maximzar o atendimento", 
				parametroGeracaoDemandaDTO.getUsarDemandasDePrioridade2());
		this.usarDemandasPrioridade2CheckBox.setToolTip("As demandas de prioridade 2 serão ofertadas para o aluno apenas após a alocação das demandas de prioridade 1, caso ainda haja espaço na grade horária do aluno");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField = new NumberField();
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setFieldLabel("Distância máxima (em períodos) do período atual do aluno");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setLabelStyle("width: 330px; padding-left: 30px");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setEmptyText("Distância máxima");
		this.distanciaMaxEmPeriodosParaPrioridade2NumberField.setValue(parametroGeracaoDemandaDTO.getDistanciaMaxEmPeriodosParaPrioridade2());
		this.considerarPreRequisitosCheckBox = createCheckBox("Considerar pré-requisitos", parametroGeracaoDemandaDTO.getConsiderarPreRequisitos());
		this.considerarCoRequisitosCheckBox = createCheckBox("Considerar co-requisitos", parametroGeracaoDemandaDTO.getConsiderarCoRequisitos());
		this.considerarMaturidadeCheckBox = createCheckBox("Considerar maturidade",false);
		this.considerarMaturidadeCheckBox.disable();
		
		RadioGroup maxCreditosRadioGroup = new RadioGroup();
		maxCreditosRadioGroup.setLabelStyle("width: 275px");
		this.creditoPeriodoRadio = new Radio();  
		this.creditoPeriodoRadio.setBoxLabel("Máximo de creditos igual ao total de créditos do periodo do aluno");
		this.creditoPeriodoRadio.setValue(parametroGeracaoDemandaDTO.getMaxCreditosPeriodo());
		maxCreditosRadioGroup.add(this.creditoPeriodoRadio);
		this.creditoManualRadio = new Radio();
		this.creditoManualRadio.setBoxLabel("Máximo de creditos informado manualmente");
		this.creditoManualRadio.setValue(!parametroGeracaoDemandaDTO.getMaxCreditosPeriodo());
		maxCreditosRadioGroup.add(this.creditoManualRadio);
		this.creditoManualNumberField = new NumberField();
		this.creditoManualNumberField.setLabelStyle("width: 200px; padding-left: 30px");
		this.creditoManualNumberField.setEmptyText("Número de créditos");
		this.creditoManualNumberField.setValue(parametroGeracaoDemandaDTO.getMaxCreditosManual());
		
		LayoutContainer maxCreditosContainer = new LayoutContainer(new ColumnLayout());
		maxCreditosContainer.add(maxCreditosRadioGroup);
		maxCreditosContainer.add(creditoManualNumberField);
		
		this.aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox = createCheckBox("Considerar acréscimo de máximo de créditos (em %) se o aluno estiver <u>irregular</u>",
				parametroGeracaoDemandaDTO.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas());
		this.aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox.setToolTip("O aluno apresenta-se em situação irregular quando há em seu histórico uma ou mais matérias de períodos passados ainda não concluídas.");
		this.fatorAumentoDeMaxCreditosNumberField = new NumberField();
		this.fatorAumentoDeMaxCreditosNumberField.setEmptyText("Configurar % de acréscimo");
		this.fatorAumentoDeMaxCreditosNumberField.setValue(parametroGeracaoDemandaDTO.getFatorDeAumentoDeMaxCreditos());
		
		LayoutContainer aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer = new LayoutContainer(new ColumnLayout());
		aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer.add(this.aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox);
		aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer.add(this.fatorAumentoDeMaxCreditosNumberField);

		FieldSet parametrosFS = new FieldSet();
		parametrosFS.setHeadingHtml("Parâmetros");
		parametrosFS.setLayout(new FormLayout());
		parametrosFS.add(usarDemandasPrioridade2CheckBox);
		parametrosFS.add(distanciaMaxEmPeriodosParaPrioridade2NumberField);
		parametrosFS.add(considerarPreRequisitosCheckBox);
		parametrosFS.add(considerarCoRequisitosCheckBox);
		parametrosFS.add(considerarMaturidadeCheckBox);
		parametrosFS.add(maxCreditosContainer);
		parametrosFS.add(aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasContainer);
		
		return parametrosFS;
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

	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return this.turnoComboBox;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return this.campusComboBox;
	}
	
	@Override
	public Button getSubmitButton() {
		return submitBt;
	}
	
	@Override
	public Button getOfertasCursosButton() {
		return ofertasCursosBt;
	}
	
	@Override
	public Button getOfertasDemandasButton() {
		return ofertasDemandasBt;
	}
	
	@Override
	public Button getDemandasAlunosButton() {
		return demandasAlunoBt;
	}

	@Override
	public CheckBox getUsarDemandasPrioridade2CheckBox() {
		return usarDemandasPrioridade2CheckBox;
	}

	@Override
	public NumberField getDistanciaMaxEmPeriodosParaPrioridade2NumberField() {
		return distanciaMaxEmPeriodosParaPrioridade2NumberField;
	}

	@Override
	public CheckBox getConsiderarPreRequisitosCheckBox() {
		return considerarPreRequisitosCheckBox;
	}

	@Override
	public CheckBox getConsiderarCoRequisitosCheckBox() {
		return considerarCoRequisitosCheckBox;
	}

	@Override
	public CheckBox getConsiderarMaturidadeCheckBox() {
		return considerarMaturidadeCheckBox;
	}

	@Override
	public Radio getCreditoPeriodoRadio() {
		return creditoPeriodoRadio;
	}

	@Override
	public Radio getCreditoManualRadio() {
		return creditoManualRadio;
	}
	
	@Override
	public NumberField getCreditoManualNumberField() {
		return creditoManualNumberField;
	}

	@Override
	public CheckBox getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox() {
		return aumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox;
	}

	@Override
	public NumberField getFatorAumentoDeMaxCreditosNumberField() {
		return fatorAumentoDeMaxCreditosNumberField;
	}
	
	@Override
	public RadioGroup getMetodoGeracaoRadioGroup() {
		return metodoGeracaoRadioGroup;
	}
	
	@Override
	public Radio getEvolucaoRadio() {
		return evolucaoRadio;
	}

	@Override
	public Radio getCriacaoAutomaticaRadio() {
		return criacaoAutomaticaRadio;
	}
	
	@Override
	public Radio getCriacaoDiretaRadio() {
		return criacaoDiretaRadio;
	}
	
	@Override
	public void selecionaEvolucaoAlunos() {
		explicacaoMetodo.removeAll();
		explicacaoMetodo.addText("As <i>demandas</i> representam o ponto de partida para o <b>Trieda</b> gerar as <i>Grades de Horário</i>" +
				" de cada aluno, professor e ambiente. Uma <i>demanda</i> representa um par (aluno,disciplina) significando que o <b>Trieda</b>" +
				" deve tentar gerar uma solução na qual o <i>aluno</i> assistirá às aulas da <i>disciplina</i>. Dessa forma, o método de geração" +
				" de demanda denominado <b>Evolução dos Alunos na Matriz Curricular</b> irá gerar os pares (aluno,disciplina) com base na evolução" +
				" de cada aluno em sua respectiva matriz curricular. Em outras palavras, a partir das <i>matrizes curriculares</i> e das" +
				" <i>disciplinas já cursadas</i> por cada <i>aluno</i>, o <b>Trieda</b> determinará para cada <i>aluno</i>, de forma automática," +
				" as próximas <i>disciplinas</i> que deveriam ser oferecidas ao <i>aluno</i> em questão. No entanto, esta determinação é controlada" +
				" por pré-requisitos, co-requisitos, máximo de créditos por aluno entre outros. Vale notar que a utilização deste método pressupõe a" +
				" existência dos alunos na base de dados do <b>Trieda</b>.");
		panel.removeAll();
		panel.add(campusComboBox);
		panel.add(turnoComboBox);
		panel.add(criaParametros());
		panel.getButtonBar().show();
		form.layout();
	}
	
	@Override
	public void selecionaCriacaoAutomatica() {
		explicacaoMetodo.removeAll();
		explicacaoMetodo.addText("As <i>demandas</i> representam o ponto de partida para o <b>Trieda</b> gerar as <i>Grades de Horário</i> de cada aluno," +
				" professor e ambiente. Uma <i>demanda</i> representa um par (aluno,disciplina) significando que o <b>Trieda</b> deve tentar gerar uma" +
				" solução na qual o <i>aluno</i> assistirá às aulas da <i>disciplina</i>. Dessa forma, o método de geração de demanda denominado" +
				" <b>Criação Automática de Alunos e Demandas a partir de Quantidades Informadas</b> irá gerar os pares (aluno,disciplina) com base" +
				" em <i>quantidades de alunos<i> informadas por <i>disciplina</i>. Neste caso, <i>alunos virtuais</i> serão criados automaticamente pelo" +
				" <b>Trieda</b>. Por exemplo, este método poderia ser utilizado para popular o <b>Trieda</b> com a previsão de alunos entrantes num" +
				" determinado semestre letivo. Logo, vale notar que a utilização deste método <b>não</b> pressupõe a existência dos alunos na base de" +
				" dados do <b>Trieda</b>.");
		panel.removeAll();
		panel.getButtonBar().hide();
		panel.addText("Primeiro, cadastrar as ofertas na tela Ofertas de Cursos em Campi. Uma oferta consiste de Turno + Campus + Curso + Matriz Curricular e da receita correspondente.");
		panel.add(ofertasCursosBt);
		panel.addText("Depois, cadastrar a demanda de cada oferta na tela Ofetas e Demandas.");
		panel.add(ofertasDemandasBt);
		form.layout();
	}
	
	@Override
	public void selecionaCriacaoDireta() {
		explicacaoMetodo.removeAll();
		explicacaoMetodo.addText("As <i>demandas</i> representam o ponto de partida para o <b>Trieda</b> gerar as <i>Grades de Horário</i> de cada aluno, professor" +
				" e ambiente. Uma <i>demanda</i> representa um par (aluno,disciplina) significando que o <b>Trieda</b> deve tentar gerar uma solução na qual o" +
				" <i>aluno<i> assistirá às aulas da <i>disciplina</i>. Dessa forma, o método de geração de demanda denominado <b>Criação Direta de Alunos e Demandas</b>" +
				" irá gerar os pares (aluno,disciplina) a partir da importação de uma planilha excel com estes pares. Vale notar que a utilização deste" +
				" método pressupõe a existência dos alunos na base de dados do <b>Trieda</b>.");
		panel.removeAll();
		panel.getButtonBar().hide();
		panel.add(demandasAlunoBt);
		form.layout();
	}
}
