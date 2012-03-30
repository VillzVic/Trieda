package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.Scroll;
import com.extjs.gxt.ui.client.util.Margins;
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
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ParametrosPresenter;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox;
import com.gapso.web.trieda.shared.util.view.FuncaoObjetivoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ParametrosView
	extends MyComposite
	implements ParametrosPresenter.Display
{
	private Button submitBt;
	private ContentPanel form;
	private GTabItem tabItem;
	private ParametroDTO parametroDTO;

	private Radio taticoRadio;
	private Radio operacionalRadio;
	private Radio otimizarPorBlocoCurricular;
	private Radio otimizarPorAluno;
	private TurnoComboBox turnoComboBox;
	private CampusComboBox campusComboBox;

	private CheckBox cargaHorariaAlunoCheckBox;
	private CargaHorariaComboBox cargaHorariaAlunoComboBox;
	private CheckBox alunoDePeriodoMesmaSalaCheckBox;
	private CheckBox alunoEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoAlunoCheckBox;

	private CheckBox cargaHorariaProfessorCheckBox;
	private CargaHorariaComboBox cargaHorariaProfessorComboBox;
	private CheckBox professorEmMuitosCampiCheckBox;
	private CheckBox minimizarDeslocamentoProfessorCheckBox;
	private NumberField minimizarDeslocamentoProfessorNumberField;
	private CheckBox minimizarGapProfessorCheckBox;
	private CheckBox evitarReducaoCargaHorariaProfessorCheckBox;
	private NumberField evitarReducaoCargaHorariaProfessorNumberField;
	private CheckBox evitarUltimoEPrimeiroHorarioProfessorCheckBox;
	private CheckBox preferenciaDeProfessoresCheckBox;
	private CheckBox avaliacaoDesempenhoProfessorCheckBox;

	private FuncaoObjetivoComboBox funcaoObjetivoCheckBox;
	private CheckBox considerarEquivalenciaCheckBox;
	private CheckBox nivelDificuldadeDisciplinaCheckBox;
	private CheckBox compatibilidadeDisciplinasMesmoDiaCheckBox;
	private CheckBox regrasGenericasDivisaoCreditoCheckBox;
	private CheckBox regrasEspecificasDivisaoCreditoCheckBox;
	private CheckBox maximizarNotaAvaliacaoCorpoDocenteCheckBox;
	private CheckBox minimizarCustoDocenteCursosCheckBox;
	private CheckBox minAlunosParaAbrirTurmaCheckBox;
	private NumberField minAlunosParaAbrirTurmaValueNumberField;
	private CheckBox compartilharDisciplinasCampiCheckBox;
	private CheckBox percentuaisMinimosMestresCheckBox;
	private CheckBox percentuaisMinimosDoutoresCheckBox;
	private CheckBox areaTitulacaoProfessoresECursosCheckBox;
	private CheckBox limitarMaximoDisciplinaProfessorCheckBox;

	private Button maximizarNotaAvaliacaoCorpoDocenteButton; 
	private Button minimizarCustoDocenteCursosButton; 
	private Button compartilharDisciplinasCampiButton; 

	public ParametrosView( ParametroDTO parametroDTO )
	{
		this.parametroDTO = parametroDTO;

		initUI();
	}

	private void initUI()
	{
		this.form = new FormPanel();
		this.form.setHeading( "Master Data » Parâmetros de Planejamento" );

		createForm();
		createTabItem();
		initComponent( this.tabItem );
	}

	private void createTabItem()
	{
		this.tabItem = new GTabItem( "Parâmetros de Planejamento",
			Resources.DEFAULTS.parametroPlanejamento16() );

		this.tabItem.setContent( this.form, new Margins( 5, 5, 0, 5 ) );
	}

	private void createForm()
	{
		FormData formData = new FormData("100%");

		this.form.setScrollMode( Scroll.AUTO );
		this.form.setButtonAlign( HorizontalAlignment.RIGHT );

		FormLayout formLayout = null;
		RowLayout rowLayout = null;
		Label label = null;

		formLayout = new FormLayout();
		formLayout.setLabelWidth( 120 );
		this.form.setLayout( formLayout );

		RadioGroup group = new RadioGroup();  
		group.setFieldLabel( "Modo de Otimização" );

		this.taticoRadio = new Radio();  
		this.taticoRadio.setBoxLabel( "Tático" );
		this.taticoRadio.setValue( this.parametroDTO.isTatico()
			|| !parametroDTO.isOperacional() );
		group.add( this.taticoRadio );

		this.operacionalRadio = new Radio();  
		this.operacionalRadio.setBoxLabel( "Operacional" );
		this.operacionalRadio.setValue( this.parametroDTO.isOperacional() );
		group.add( this.operacionalRadio );

		this.form.add( group );
		
		RadioGroup grupoOtimizarPor = new RadioGroup();  
		grupoOtimizarPor.setFieldLabel( "Otimizar Por" );
		
		boolean otimizarPor = (this.parametroDTO.getOtimizarPor() == null) ? true : (this.parametroDTO.getOtimizarPor() == ParametroDTO.OTIMIZAR_POR_ALUNO);

		this.otimizarPorAluno = new Radio();
		this.otimizarPorAluno.setBoxLabel("Aluno");
		this.otimizarPorAluno.setValue(otimizarPor);
		grupoOtimizarPor.add(this.otimizarPorAluno);
		
		this.otimizarPorBlocoCurricular = new Radio();
		this.otimizarPorBlocoCurricular.setBoxLabel("Bloco Curricular");
		this.otimizarPorBlocoCurricular.setValue(!otimizarPor);
		grupoOtimizarPor.add(this.otimizarPorBlocoCurricular);

		this.form.add( grupoOtimizarPor );

		this.funcaoObjetivoCheckBox = new FuncaoObjetivoComboBox();
		this.funcaoObjetivoCheckBox.setValue( this.parametroDTO.getFuncaoObjetivo() );
		this.funcaoObjetivoCheckBox.setFieldLabel( "Função Objetivo" );
		this.form.add( this.funcaoObjetivoCheckBox );

		this.campusComboBox = new CampusComboBox();
		this.campusComboBox.setFieldLabel( "Campus" );
		this.form.add( this.campusComboBox );

		turnoComboBox = new TurnoComboBox( campusComboBox );
		turnoComboBox.setFieldLabel( "Turno" );
		form.add( turnoComboBox );

		FieldSet alunoFS = new FieldSet();
		alunoFS.setHeading( "Preferências do Aluno" );
		alunoFS.setCollapsible( true );
		alunoFS.setLayout( new ColumnLayout() );

		formLayout = new FormLayout();
		formLayout.setExtraStyle( "borderBottom" );
		LayoutContainer alunoLeft = new LayoutContainer( formLayout );
		rowLayout = new RowLayout();
		rowLayout.setExtraStyle( "x-form-item borderBottom" );
		LayoutContainer alunoRight = new LayoutContainer( rowLayout );

		this.cargaHorariaAlunoCheckBox = createCheckBox(
			"Distribuição da carga horária semanal do aluno",
			this.parametroDTO.getCargaHorariaAluno() );

		this.alunoDePeriodoMesmaSalaCheckBox = createCheckBox(
			"Manter alunos do mesmo curso-período na mesma sala",
			this.parametroDTO.getAlunoDePeriodoMesmaSala() );

		this.alunoEmMuitosCampiCheckBox = createCheckBox(
			"Permitir que o aluno estude em mais de um Campus",
			this.parametroDTO.getAlunoEmMuitosCampi() );

		this.alunoEmMuitosCampiCheckBox.disable();

		this.minimizarDeslocamentoAlunoCheckBox = createCheckBox(
			"Minimizar Deslocamento de Alunos entre Campi",
			this.parametroDTO.getMinimizarDeslocamentoAluno() );

		this.minimizarDeslocamentoAlunoCheckBox.disable();

		alunoLeft.add( this.cargaHorariaAlunoCheckBox, formData );
		alunoLeft.add( this.alunoDePeriodoMesmaSalaCheckBox, formData );
		alunoLeft.add( this.alunoEmMuitosCampiCheckBox, formData );
		alunoLeft.add( this.minimizarDeslocamentoAlunoCheckBox, formData );

		this.cargaHorariaAlunoComboBox = createComboBox(
			this.parametroDTO.getCargaHorariaAlunoSel() );

		alunoRight.add( this.cargaHorariaAlunoComboBox );

		for ( int i = 0; i < 3; i++ )
		{
			label = new Label();
			label.setHeight( 22 );
			alunoRight.add( label );
		}

		alunoFS.add( alunoLeft, new ColumnData( 0.5 ) );
		alunoFS.add( alunoRight, new ColumnData( 0.4 ) );

		FieldSet professorFS = new FieldSet();
		professorFS.setHeading( "Preferências do Professor" );
		professorFS.setCollapsible( true );
		professorFS.setLayout( new ColumnLayout() );

		formLayout = new FormLayout();
		formLayout.setExtraStyle( "borderBottom" );
		LayoutContainer professorLeft = new LayoutContainer( formLayout );
		rowLayout = new RowLayout();
		rowLayout.setExtraStyle( "x-form-item borderBottom" );
		LayoutContainer professorRight = new LayoutContainer( rowLayout );

		this.cargaHorariaProfessorCheckBox = createCheckBox(
			"Distribuição da carga horária semanal do professor",
			this.parametroDTO.getCargaHorariaProfessor() );

		this.professorEmMuitosCampiCheckBox = createCheckBox(
			"Permitir que o professor ministre aulas em mais de um Campus",
			this.parametroDTO.getProfessorEmMuitosCampi() );

		this.professorEmMuitosCampiCheckBox.disable();

		this.minimizarDeslocamentoProfessorCheckBox = createCheckBox(
			"Minimizar Deslocamentos de Professores entre Campi",
			this.parametroDTO.getMinimizarDeslocamentoProfessor() );

		this.minimizarDeslocamentoProfessorCheckBox.disable();

		this.minimizarGapProfessorCheckBox = createCheckBox(
			"Minimizar Gaps nos Horários dos Professores",
			this.parametroDTO.getMinimizarGapProfessor() );

		this.evitarReducaoCargaHorariaProfessorCheckBox = createCheckBox(
			"Evitar Redução de Carga Horária de Professor",
			this.parametroDTO.getEvitarReducaoCargaHorariaProfessor() );

		this.evitarUltimoEPrimeiroHorarioProfessorCheckBox = createCheckBox(
			"Evitar alocação de professores no último " +
			"horário do dia e primeiro do dia seguinte",
			this.parametroDTO.getEvitarUltimoEPrimeiroHorarioProfessor() );

		this.preferenciaDeProfessoresCheckBox = createCheckBox(
			"Considerar preferência de professores por disciplinas",
			this.parametroDTO.getPreferenciaDeProfessores() );

		this.avaliacaoDesempenhoProfessorCheckBox = createCheckBox(
			"Considerar avaliação de desempenho de professores",
			this.parametroDTO.getAvaliacaoDesempenhoProfessor() );

		professorLeft.add( this.cargaHorariaProfessorCheckBox, formData );
		professorLeft.add( this.professorEmMuitosCampiCheckBox, formData );
		professorLeft.add( this.minimizarDeslocamentoProfessorCheckBox, formData );
		professorLeft.add( this.minimizarGapProfessorCheckBox, formData );
		professorLeft.add( this.evitarReducaoCargaHorariaProfessorCheckBox, formData );
		professorLeft.add( this.evitarUltimoEPrimeiroHorarioProfessorCheckBox, formData );
		professorLeft.add( this.preferenciaDeProfessoresCheckBox, formData );
		professorLeft.add( this.avaliacaoDesempenhoProfessorCheckBox, formData );

		this.cargaHorariaProfessorComboBox = createComboBox(
			this.parametroDTO.getCargaHorariaProfessorSel() );
		professorRight.add( this.cargaHorariaProfessorComboBox );
		label = new Label();
		label.setHeight( 22 );
		professorRight.add( label );

		this.minimizarDeslocamentoProfessorNumberField = new NumberField();
		this.minimizarDeslocamentoProfessorNumberField.setEmptyText(
			"Configurar Máx de deslocamento" );
		this.minimizarDeslocamentoProfessorNumberField.setValue(
			this.parametroDTO.getMinimizarDeslocamentoProfessorValue() );
		this.minimizarDeslocamentoProfessorNumberField.disable();

		professorRight.add( this.minimizarDeslocamentoProfessorNumberField );
		label = new Label();
		label.setHeight( 22 );
		professorRight.add( label );

		this.evitarReducaoCargaHorariaProfessorNumberField = new NumberField();
		this.evitarReducaoCargaHorariaProfessorNumberField.setEmptyText(
			"Configurar % de tolerância" );
		this.evitarReducaoCargaHorariaProfessorNumberField.setValue(
			this.parametroDTO.getEvitarReducaoCargaHorariaProfessorValue() );
		professorRight.add( this.evitarReducaoCargaHorariaProfessorNumberField );
		
		for ( int i = 0; i < 3; i++ )
		{
			label = new Label();
			label.setHeight( 22 );
			professorRight.add( label );
		}

		professorFS.add( professorLeft, new ColumnData( 0.5 ) );
		professorFS.add( professorRight, new ColumnData( 0.4 ) );

		FieldSet instituicaoFS = new FieldSet();
		instituicaoFS.setHeading( "Preferências da Instituição" );
		instituicaoFS.setCollapsible( true );
		instituicaoFS.setLayout( new ColumnLayout() );

		formLayout = new FormLayout();
		formLayout.setExtraStyle( "borderBottom" );
		LayoutContainer instituicaoLeft
			= new LayoutContainer( formLayout );
		rowLayout = new RowLayout();
		rowLayout.setExtraStyle( "x-form-item borderBottom" );
		LayoutContainer instituicaoRight
			= new LayoutContainer( rowLayout );

		this.considerarEquivalenciaCheckBox = createCheckBox(
			"Considerar Equivalências entre Disciplinas",
			this.parametroDTO.getConsiderarEquivalencia() );

		this.minAlunosParaAbrirTurmaCheckBox = createCheckBox(
			"Número mínimo de alunos para abertura de turma",
			this.parametroDTO.getMinAlunosParaAbrirTurma() );

		this.nivelDificuldadeDisciplinaCheckBox = createCheckBox(
			"Considerar nível de dificuldade de disciplinas",
			this.parametroDTO.getNivelDificuldadeDisciplina() );

		this.nivelDificuldadeDisciplinaCheckBox.disable();

		this.compatibilidadeDisciplinasMesmoDiaCheckBox = createCheckBox(
			"Considerar compatibilidade de disciplinas no mesmo dia",
			this.parametroDTO.getCompatibilidadeDisciplinasMesmoDia() );

		this.regrasGenericasDivisaoCreditoCheckBox = createCheckBox(
			"Considerar regras genéricas de divisão de créditos",
			this.parametroDTO.getRegrasGenericasDivisaoCredito() );

		this.regrasEspecificasDivisaoCreditoCheckBox = createCheckBox(
			"Considerar regras específicas de divisão de créditos",
			this.parametroDTO.getRegrasEspecificasDivisaoCredito() );

		this.maximizarNotaAvaliacaoCorpoDocenteCheckBox = createCheckBox(
			"Maximizar nota da avaliação do corpo docente de cursos específicos",
			this.parametroDTO.getMaximizarNotaAvaliacaoCorpoDocente() );

		this.minimizarCustoDocenteCursosCheckBox = createCheckBox(
			"Minimizar custo docente de cursos específicos",
			this.parametroDTO.getMinimizarCustoDocenteCursos() );

		this.compartilharDisciplinasCampiCheckBox = createCheckBox(
			"Permitir compartilhamento de disciplinas entre cursos",
			this.parametroDTO.getCompartilharDisciplinasCampi() );

		this.percentuaisMinimosMestresCheckBox = createCheckBox(
			"Considerar percentuais mínimos de mestres",
			this.parametroDTO.getPercentuaisMinimosMestres() );

		this.percentuaisMinimosDoutoresCheckBox = createCheckBox(
			"Considerar percentuais mínimos de doutores",
			this.parametroDTO.getPercentuaisMinimosDoutores() );

		this.areaTitulacaoProfessoresECursosCheckBox = createCheckBox(
			"Considerar áreas de titulação dos professores e cursos",
			this.parametroDTO.getAreaTitulacaoProfessoresECursos() );

		this.limitarMaximoDisciplinaProfessorCheckBox = createCheckBox(
			"Limitar máximo de disciplinas que um professor pode ministrar por curso",
			this.parametroDTO.getLimitarMaximoDisciplinaProfessor() );
		
		instituicaoLeft.add( this.considerarEquivalenciaCheckBox, formData );
		instituicaoLeft.add( this.minAlunosParaAbrirTurmaCheckBox, formData );
		instituicaoLeft.add( this.nivelDificuldadeDisciplinaCheckBox, formData );
		instituicaoLeft.add( this.compatibilidadeDisciplinasMesmoDiaCheckBox, formData );
		instituicaoLeft.add( this.regrasGenericasDivisaoCreditoCheckBox, formData );
		instituicaoLeft.add( this.regrasEspecificasDivisaoCreditoCheckBox, formData );
		instituicaoLeft.add( this.maximizarNotaAvaliacaoCorpoDocenteCheckBox, formData );
		instituicaoLeft.add( this.minimizarCustoDocenteCursosCheckBox, formData );
		instituicaoLeft.add( this.compartilharDisciplinasCampiCheckBox, formData );
		instituicaoLeft.add( this.percentuaisMinimosMestresCheckBox, formData );
		instituicaoLeft.add( this.percentuaisMinimosDoutoresCheckBox, formData );
		instituicaoLeft.add( this.areaTitulacaoProfessoresECursosCheckBox, formData );
		instituicaoLeft.add( this.limitarMaximoDisciplinaProfessorCheckBox, formData );

		this.minAlunosParaAbrirTurmaValueNumberField = new NumberField();
		this.minAlunosParaAbrirTurmaValueNumberField.setEmptyText( "Quantidade mínimo" );
		this.minAlunosParaAbrirTurmaValueNumberField.setValue(
			this.parametroDTO.getMinAlunosParaAbrirTurmaValue() );

		label = new Label();
		label.setHeight( 22 );
		instituicaoRight.add( label );
		instituicaoRight.add( this.minAlunosParaAbrirTurmaValueNumberField );

		Button dificuldadeButton = createButton(
			"Configurar níveis de dificuldade" );
		dificuldadeButton.disable();

		instituicaoRight.add( dificuldadeButton );
		instituicaoRight.add( createButton( "Configurar compatibilidades" ) );
		instituicaoRight.add( createButton( "Configurar regras genéricas" ) );

		label = new Label();
		label.setHeight( 22 );
		instituicaoRight.add( label );

		this.maximizarNotaAvaliacaoCorpoDocenteButton = createButton( "Configurar cursos" );
		this.minimizarCustoDocenteCursosButton = createButton( "Configurar cursos" );
		this.compartilharDisciplinasCampiButton = createButton( "Cursos que não compartilham" );

		instituicaoRight.add( this.maximizarNotaAvaliacaoCorpoDocenteButton );
		instituicaoRight.add( this.minimizarCustoDocenteCursosButton );
		instituicaoRight.add( this.compartilharDisciplinasCampiButton );

		for ( int i = 0; i < 4; i++ )
		{
			label = new Label();
			label.setHeight( 22 );
			instituicaoRight.add( label );
		}

		instituicaoFS.add( instituicaoLeft, new ColumnData( 0.5 ) );
		instituicaoFS.add( instituicaoRight, new ColumnData( 0.4 ) );

		this.form.add( alunoFS );
		this.form.add( professorFS );
		this.form.add( instituicaoFS );

		this.submitBt = new Button( "Gerar Grade",
			AbstractImagePrototype.create( Resources.DEFAULTS.gerarGrade16() ) );
		this.form.addButton( this.submitBt );
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
	public TurnoComboBox getTurnoComboBox()
	{
		return this.turnoComboBox;
	}

	@Override
	public CampusComboBox getCampusComboBox()
	{
		return this.campusComboBox;
	}

	@Override
	public CheckBox getConsiderarEquivalenciaCheckBox()
	{
		return this.considerarEquivalenciaCheckBox;
	}
}
