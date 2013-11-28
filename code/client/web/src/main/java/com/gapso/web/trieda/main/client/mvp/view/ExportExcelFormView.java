package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.shared.excel.PlanilhasExportExcel;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ExportExcelModal;
import com.gapso.web.trieda.main.client.mvp.presenter.ExportExcelFormPresenter;

public class ExportExcelFormView
	extends MyComposite
	implements ExportExcelFormPresenter.Display
{
	private ExportExcelModal exportExcelModal;
	private FormPanel formPanel;
	private CheckBox campiExportExcelCB;
	private CheckBox unidadesExportExcelCB;
	private CheckBox salasExportExcelCB;
	private CheckBox disponibilidadesSalasExportExcelCB;
	private CheckBox cursosExportExcelCB;
	private CheckBox areasTitulacaoExportExcelCB;
	private CheckBox cursoAreaTitulacaoExportExcelCB;
	private CheckBox disciplinasExportExcelCB;
	private CheckBox disciplinasSalasExportExcelCB;
	private CheckBox equivalenciasExportExcelCB;
	private CheckBox campiTrabalhoExportExcelCB;
	private CheckBox curriculosExportExcelCB;
	private CheckBox ofertasDemandasExportExcelCB;
	private CheckBox alunosExportExcelCB;
	private CheckBox DemandasPorAlunoExportExcelCB;
	private CheckBox professoresExportExcelCB;
	private CheckBox disponibilidadesProfessoresExportExcelCB;
	private CheckBox habilitacaoProfessoresExportExcelCB;
	private CheckBox resumoCursoExportExcelCB;
	private CheckBox resumoDisciplinaExportExcelCB;
	private CheckBox atendimentosMatriculaExportExcelCB;
	private CheckBox atendimentosDisciplinaExportExcelCB;
	private CheckBox atendimentosFaixaDemandaExportExcelCB;
	private CheckBox atendimentosCargaHorariaExportExcelCB;
	private CheckBox relatorioVisaoSalaExportExcelCB;
	private CheckBox relatorioVisaoProfessorExportExcelCB;
	private CheckBox relatorioVisaoCursoExportExcelCB;
	private CheckBox relatorioVisaoAlunoExportExcelCB;
	private CheckBox atendimentosPorAlunoExportExcelCB;
	private CheckBox percentMestresDoutoresExportExcelCB;
	private CheckBox aulasExportExcelCB;
	private CheckBox atendimentosCargaHorariaCB;
	private CheckBox disicplinasPreRequisitosCB;
	private CheckBox disicplinasCoRequisitosCB;
	private CheckBox alunosDisciplinasCursadasCB;
	private CheckBox atendimentosFaixaCreditoCB;
	private CheckBox atendimentosFaixaDisciplinaCB;
	private CheckBox professoresQuantidadeJanelasCB;
	private CheckBox professoresDisciplinasHabilitadasCB;
	private CheckBox professoresDisciplinasLecionadasCB;
	private CheckBox professoresTitulacoesCB;
	private CheckBox professoresAreasConhecimentoCB;
	private CheckBox ambientesFaixaOcupacaoCB;
	private CheckBox ambientesFaixaUtilizacaoCB;
	private TextField<String> nomeArquivoTF;
	
	public ExportExcelFormView()
	{
		initUI();
	}
	
	private void initUI() 
	{
		String title = "Exportação Excel";
		exportExcelModal = new ExportExcelModal(title, Resources.DEFAULTS.exportar16());
		exportExcelModal.setHeight(600);
		exportExcelModal.setWidth(500);
		createForm();
		exportExcelModal.setContent(formPanel);
	}
	
	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setFrame(true);
		formPanel.setHeadingHtml("Escolha quais planilhas deseja exportar");
		formPanel.setSize(600, -1);
		formPanel.setButtonAlign(HorizontalAlignment.CENTER);
		formPanel.setScrollMode(Style.Scroll.AUTOY);
		
		LayoutContainer main = new LayoutContainer();
	    main.setLayout(new FormLayout());
	    
	    //Geral
	    LayoutContainer leftGeral = new LayoutContainer();
	    leftGeral.setLayout(new FormLayout());
	    
	    LayoutContainer rightGeral = new LayoutContainer();
	    rightGeral.setLayout(new FormLayout());
	    
		FieldSet geralFS = new FieldSet();
		geralFS.setWidth(440);
		geralFS.setCollapsible(true);
		geralFS.setHeadingHtml("&nbsp;");
		geralFS.setLayout(new ColumnLayout());
		
		campiExportExcelCB = new CheckBox();
		campiExportExcelCB.setName(PlanilhasExportExcel.CAMPI);
		campiExportExcelCB.setValue(false);
		campiExportExcelCB.setFieldLabel("Campi");
		leftGeral.add(campiExportExcelCB, formData);
		
		unidadesExportExcelCB = new CheckBox();
		unidadesExportExcelCB.setName(PlanilhasExportExcel.UNIDADES);
		unidadesExportExcelCB.setValue(false);
		unidadesExportExcelCB.setFieldLabel("Unidades");
		rightGeral.add(unidadesExportExcelCB, formData);
		
		salasExportExcelCB = new CheckBox();
		salasExportExcelCB.setName(PlanilhasExportExcel.SALAS);
		salasExportExcelCB.setValue(false);
		salasExportExcelCB.setFieldLabel("Salas");
		leftGeral.add(salasExportExcelCB, formData);
		
		disponibilidadesSalasExportExcelCB = new CheckBox();
		disponibilidadesSalasExportExcelCB.setName(PlanilhasExportExcel.DISPONIBILIDADES_SALAS);
		disponibilidadesSalasExportExcelCB.setValue(false);
		disponibilidadesSalasExportExcelCB.setFieldLabel("Disponibilidades das Salas");
		rightGeral.add(disponibilidadesSalasExportExcelCB, formData);
		
		cursosExportExcelCB = new CheckBox();
		cursosExportExcelCB.setName(PlanilhasExportExcel.CURSOS);
		cursosExportExcelCB.setValue(false);
		cursosExportExcelCB.setFieldLabel("Cursos");
		leftGeral.add(cursosExportExcelCB, formData);
		
		areasTitulacaoExportExcelCB = new CheckBox();
		areasTitulacaoExportExcelCB.setName(PlanilhasExportExcel.AREAS_TITULACAO);
		areasTitulacaoExportExcelCB.setValue(false);
		areasTitulacaoExportExcelCB.setFieldLabel("Áreas de Conhecimento");
		rightGeral.add(areasTitulacaoExportExcelCB, formData);
		
		cursoAreaTitulacaoExportExcelCB = new CheckBox();
		cursoAreaTitulacaoExportExcelCB.setName(PlanilhasExportExcel.CURSO_AREAS_TITULACAO);
		cursoAreaTitulacaoExportExcelCB.setValue(false);
		cursoAreaTitulacaoExportExcelCB.setFieldLabel("Curso - Áreas de Conhecimento");
		leftGeral.add(cursoAreaTitulacaoExportExcelCB, formData);
		
		curriculosExportExcelCB = new CheckBox();
		curriculosExportExcelCB.setName(PlanilhasExportExcel.CURRICULOS);
		curriculosExportExcelCB.setValue(false);
		curriculosExportExcelCB.setFieldLabel("Matrizes Curriculares");
		rightGeral.add(curriculosExportExcelCB, formData);
		
		disciplinasExportExcelCB = new CheckBox();
		disciplinasExportExcelCB.setName(PlanilhasExportExcel.DISCIPLINAS);
		disciplinasExportExcelCB.setValue(false);
		disciplinasExportExcelCB.setFieldLabel("Disciplinas");
		leftGeral.add(disciplinasExportExcelCB, formData);
		
		disciplinasSalasExportExcelCB = new CheckBox();
		disciplinasSalasExportExcelCB.setName(PlanilhasExportExcel.DISCIPLINAS_SALAS);
		disciplinasSalasExportExcelCB.setValue(false);
		disciplinasSalasExportExcelCB.setFieldLabel("Disciplinas-Salas");
		rightGeral.add(disciplinasSalasExportExcelCB, formData);
		
		equivalenciasExportExcelCB = new CheckBox();
		equivalenciasExportExcelCB.setName(PlanilhasExportExcel.EQUIVALENCIAS);
		equivalenciasExportExcelCB.setValue(false);
		equivalenciasExportExcelCB.setFieldLabel("Equivalências");
		leftGeral.add(equivalenciasExportExcelCB, formData);
		
		alunosExportExcelCB = new CheckBox();
		alunosExportExcelCB.setName(PlanilhasExportExcel.ALUNOS);
		alunosExportExcelCB.setValue(false);
		alunosExportExcelCB.setFieldLabel("Alunos");
		rightGeral.add(alunosExportExcelCB, formData);
		
		ofertasDemandasExportExcelCB = new CheckBox();
		ofertasDemandasExportExcelCB.setName(PlanilhasExportExcel.DEMANDAS);
		ofertasDemandasExportExcelCB.setValue(false);
		ofertasDemandasExportExcelCB.setFieldLabel("Ofertas e Demandas");
		leftGeral.add(ofertasDemandasExportExcelCB, formData);
		
		DemandasPorAlunoExportExcelCB = new CheckBox();
		DemandasPorAlunoExportExcelCB.setName(PlanilhasExportExcel.DEMANDAS_POR_ALUNO);
		DemandasPorAlunoExportExcelCB.setValue(false);
		DemandasPorAlunoExportExcelCB.setFieldLabel("Demandas por Aluno");
		rightGeral.add(DemandasPorAlunoExportExcelCB, formData);
		
		aulasExportExcelCB = new CheckBox();
		aulasExportExcelCB.setName(PlanilhasExportExcel.AULAS);
		aulasExportExcelCB.setValue(false);
		aulasExportExcelCB.setFieldLabel("Aulas");
		leftGeral.add(aulasExportExcelCB, formData);
		
		professoresExportExcelCB = new CheckBox();
		professoresExportExcelCB.setName(PlanilhasExportExcel.PROFESSORES);
		professoresExportExcelCB.setValue(false);
		professoresExportExcelCB.setFieldLabel("Professores");
		rightGeral.add(professoresExportExcelCB, formData);
		
		disponibilidadesProfessoresExportExcelCB = new CheckBox();
		disponibilidadesProfessoresExportExcelCB.setName(PlanilhasExportExcel.DISPONIBILIDADES_PROFESSORES);
		disponibilidadesProfessoresExportExcelCB.setValue(false);
		disponibilidadesProfessoresExportExcelCB.setFieldLabel("Disponibilidades dos Professores");
		leftGeral.add(disponibilidadesProfessoresExportExcelCB, formData);
		
		campiTrabalhoExportExcelCB = new CheckBox();
		campiTrabalhoExportExcelCB.setName(PlanilhasExportExcel.CAMPI_TRABALHO);
		campiTrabalhoExportExcelCB.setValue(false);
		campiTrabalhoExportExcelCB.setFieldLabel("Campi de Trabalho");
		rightGeral.add(campiTrabalhoExportExcelCB, formData);
		
	    habilitacaoProfessoresExportExcelCB = new CheckBox();
	    habilitacaoProfessoresExportExcelCB.setName(PlanilhasExportExcel.HABILITACAO_PROFESSORES);
	    habilitacaoProfessoresExportExcelCB.setValue(false);
	    habilitacaoProfessoresExportExcelCB.setFieldLabel("Habilitação dos Professores");
	    leftGeral.add(habilitacaoProfessoresExportExcelCB, formData);
		
		geralFS.add(leftGeral, new ColumnData(.5));
		geralFS.add(rightGeral, new ColumnData(.5));
		main.add(geralFS, new FormData());
		
		//Professores
	    LayoutContainer leftProfessores = new LayoutContainer();
	    leftProfessores.setLayout(new FormLayout());
	    
	    LayoutContainer rightProfessores = new LayoutContainer();
	    rightProfessores.setLayout(new FormLayout());
	    
		FieldSet professoresFS = new FieldSet();
		professoresFS.setWidth(440);
		professoresFS.setHeadingHtml("&nbsp;");
		professoresFS.setCollapsible(true);
		professoresFS.setLayout(new ColumnLayout());
		
		percentMestresDoutoresExportExcelCB = new CheckBox();
		percentMestresDoutoresExportExcelCB.setName(PlanilhasExportExcel.PERCENT_MESTRES_DOUTORES);
		percentMestresDoutoresExportExcelCB.setValue(false);
		percentMestresDoutoresExportExcelCB.setFieldLabel("Porcentagem de Mestres e Doutores");
		leftProfessores.add(percentMestresDoutoresExportExcelCB, formData);
		
		professoresDisciplinasHabilitadasCB = new CheckBox();
		professoresDisciplinasHabilitadasCB.setName(PlanilhasExportExcel.PROFESSORES_DISCIPLINAS_HABILITADAS);
		professoresDisciplinasHabilitadasCB.setValue(false);
		professoresDisciplinasHabilitadasCB.setFieldLabel("Professores por Quantidade de Disciplinas Habilitadas");
		rightProfessores.add(professoresDisciplinasHabilitadasCB, formData);
		
		professoresDisciplinasLecionadasCB = new CheckBox();
		professoresDisciplinasLecionadasCB.setName(PlanilhasExportExcel.PROFESSORES_DISCIPLINAS_LECIONADAS);
		professoresDisciplinasLecionadasCB.setValue(false);
		professoresDisciplinasLecionadasCB.setFieldLabel("Professores por Quantidade de Disciplinas Lecionadas");
		leftProfessores.add(professoresDisciplinasLecionadasCB, formData);
		
		professoresTitulacoesCB = new CheckBox();
		professoresTitulacoesCB.setName(PlanilhasExportExcel.PROFESSORES_TITULACOES);
		professoresTitulacoesCB.setValue(false);
		professoresTitulacoesCB.setFieldLabel("Professores por Titulações");
		rightProfessores.add(professoresTitulacoesCB, formData);
		
		professoresAreasConhecimentoCB = new CheckBox();
		professoresAreasConhecimentoCB.setName(PlanilhasExportExcel.PROFESSORES_AREAS_CONHECIMENTO);
		professoresAreasConhecimentoCB.setValue(false);
		professoresAreasConhecimentoCB.setFieldLabel("Professores por Areas de Conhecimento");
		leftProfessores.add(professoresAreasConhecimentoCB, formData);
		
		professoresQuantidadeJanelasCB = new CheckBox();
		professoresQuantidadeJanelasCB.setName(PlanilhasExportExcel.PROFESSORES_QUANTIDADE_JANELAS);
		professoresQuantidadeJanelasCB.setValue(false);
		professoresQuantidadeJanelasCB.setFieldLabel("Professores por Quantidade de Janelas");
		rightProfessores.add(professoresQuantidadeJanelasCB, formData);
		
		professoresFS.add(leftProfessores, new ColumnData(.5));
		professoresFS.add(rightProfessores, new ColumnData(.5));
		main.add(professoresFS, new FormData());
		
		//Resumo
	    LayoutContainer leftResumos = new LayoutContainer();
	    leftResumos.setLayout(new FormLayout());
	    
	    LayoutContainer rightResumos = new LayoutContainer();
	    rightResumos.setLayout(new FormLayout());
	    
		FieldSet resumoFS = new FieldSet();
		resumoFS.setWidth(440);
		resumoFS.setHeadingHtml("&nbsp;");
		resumoFS.setCollapsible(true);
		resumoFS.setLayout(new ColumnLayout());
		
		resumoCursoExportExcelCB = new CheckBox();
		resumoCursoExportExcelCB.setName(PlanilhasExportExcel.RESUMO_CURSO);
		resumoCursoExportExcelCB.setValue(false);
		resumoCursoExportExcelCB.setFieldLabel("Resumo por Curso");
		leftResumos.add(resumoCursoExportExcelCB, formData);
		
		resumoDisciplinaExportExcelCB = new CheckBox();
		resumoDisciplinaExportExcelCB.setName(PlanilhasExportExcel.RESUMO_DISCIPLINA);
		resumoDisciplinaExportExcelCB.setValue(false);
		resumoDisciplinaExportExcelCB.setFieldLabel("Resumo por Disciplina");
		rightResumos.add(resumoDisciplinaExportExcelCB, formData);
		
		resumoFS.add(leftResumos, new ColumnData(.5));
		resumoFS.add(rightResumos, new ColumnData(.5));
		main.add(resumoFS, new FormData());
		
		//Atendimentos
	    LayoutContainer leftAtendimentos = new LayoutContainer();
	    leftAtendimentos.setLayout(new FormLayout());
	    
	    LayoutContainer rightAtendimentos = new LayoutContainer();
	    rightAtendimentos.setLayout(new FormLayout());
	    
		FieldSet atendimentosFS = new FieldSet();
		atendimentosFS.setWidth(440);
		atendimentosFS.setHeadingHtml("&nbsp;");
		atendimentosFS.setCollapsible(true);
		atendimentosFS.setLayout(new ColumnLayout());
		
		atendimentosCargaHorariaExportExcelCB = new CheckBox();
		atendimentosCargaHorariaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_CARGA_HORARIA);
		atendimentosCargaHorariaExportExcelCB.setValue(false);
		atendimentosCargaHorariaExportExcelCB.setFieldLabel("Atendimentos Por Carga Horária");
		leftAtendimentos.add(atendimentosCargaHorariaExportExcelCB, formData);
		
		atendimentosMatriculaExportExcelCB = new CheckBox();
		atendimentosMatriculaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_MATRICULA);
		atendimentosMatriculaExportExcelCB.setValue(false);
		atendimentosMatriculaExportExcelCB.setFieldLabel("Atendimentos por Matrícula");
		rightAtendimentos.add(atendimentosMatriculaExportExcelCB, formData);
		
		atendimentosDisciplinaExportExcelCB = new CheckBox();
		atendimentosDisciplinaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_DISCIPLINA);
		atendimentosDisciplinaExportExcelCB.setValue(false);
		atendimentosDisciplinaExportExcelCB.setFieldLabel("Atendimentos por Disciplina");
		leftAtendimentos.add(atendimentosDisciplinaExportExcelCB, formData);
		
		atendimentosFaixaDemandaExportExcelCB = new CheckBox();
		atendimentosFaixaDemandaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DEMANDA);
		atendimentosFaixaDemandaExportExcelCB.setValue(false);
		atendimentosFaixaDemandaExportExcelCB.setFieldLabel("Atendimentos Faixa de Demanda");
		rightAtendimentos.add(atendimentosFaixaDemandaExportExcelCB, formData);
		
		atendimentosFaixaCreditoCB = new CheckBox();
		atendimentosFaixaCreditoCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_CREDITO);
		atendimentosFaixaCreditoCB.setValue(false);
		atendimentosFaixaCreditoCB.setFieldLabel("Atendimentos por Faixa de Crédito");
		leftAtendimentos.add(atendimentosFaixaCreditoCB, formData);
		
		atendimentosFaixaDisciplinaCB = new CheckBox();
		atendimentosFaixaDisciplinaCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DISCIPLINA);
		atendimentosFaixaDisciplinaCB.setValue(false);
		atendimentosFaixaDisciplinaCB.setFieldLabel("Atendimentos por Faixa de Disciplina");
		rightAtendimentos.add(atendimentosFaixaDisciplinaCB, formData);
		
		atendimentosPorAlunoExportExcelCB = new CheckBox();
		atendimentosPorAlunoExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_POR_ALUNO);
		atendimentosPorAlunoExportExcelCB.setValue(false);
		atendimentosPorAlunoExportExcelCB.setFieldLabel("Atendimentos por Aluno");
		leftAtendimentos.add(atendimentosPorAlunoExportExcelCB, formData);

		atendimentosFS.add(leftAtendimentos, new ColumnData(.5));
		atendimentosFS.add(rightAtendimentos, new ColumnData(.5));
		main.add(atendimentosFS, new FormData());
		
		//Ambientes
	    LayoutContainer leftAmbientes = new LayoutContainer();
	    leftAmbientes.setLayout(new FormLayout());
	    
	    LayoutContainer rightAmbientes = new LayoutContainer();
	    rightAmbientes.setLayout(new FormLayout());
	    
		FieldSet ambientesFS = new FieldSet();
		ambientesFS.setWidth(440);
		ambientesFS.setHeadingHtml("&nbsp;");
		ambientesFS.setCollapsible(true);
		ambientesFS.setLayout(new ColumnLayout());
		
		ambientesFaixaOcupacaoCB = new CheckBox();
		ambientesFaixaOcupacaoCB.setName(PlanilhasExportExcel.AMBIENTES_FAIXA_OCUPACAO);
		ambientesFaixaOcupacaoCB.setValue(false);
		ambientesFaixaOcupacaoCB.setFieldLabel("Ambientes por Faixa de Ocupação de Horários");
		leftAmbientes.add(ambientesFaixaOcupacaoCB, formData);
		
		ambientesFaixaUtilizacaoCB = new CheckBox();
		ambientesFaixaUtilizacaoCB.setName(PlanilhasExportExcel.AMBIENTES_FAIXA_UTILIZACAO);
		ambientesFaixaUtilizacaoCB.setValue(false);
		ambientesFaixaUtilizacaoCB.setFieldLabel("Ambientes por Faixa de Utilização da Capacidade");
		rightAmbientes.add(ambientesFaixaUtilizacaoCB, formData);
		
		ambientesFS.add(leftAmbientes, new ColumnData(.5));
		ambientesFS.add(rightAmbientes, new ColumnData(.5));
		main.add(ambientesFS, new FormData());
		
		//GradeHoraria
	    LayoutContainer leftGradeHoraria = new LayoutContainer();
	    leftGradeHoraria.setLayout(new FormLayout());
	    
	    LayoutContainer rightGradeHoraria = new LayoutContainer();
	    rightGradeHoraria.setLayout(new FormLayout());
	    
		FieldSet gradeHorariaFS = new FieldSet();
		gradeHorariaFS.setWidth(440);
		gradeHorariaFS.setHeadingHtml("&nbsp;");
		gradeHorariaFS.setCollapsible(true);
		gradeHorariaFS.setLayout(new ColumnLayout());
		
		relatorioVisaoSalaExportExcelCB = new CheckBox();
		relatorioVisaoSalaExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_SALA);
		relatorioVisaoSalaExportExcelCB.setValue(false);
		relatorioVisaoSalaExportExcelCB.setFieldLabel("Grade Horária Visão Sala");
		leftGradeHoraria.add(relatorioVisaoSalaExportExcelCB, formData);
		
		relatorioVisaoProfessorExportExcelCB = new CheckBox();
		relatorioVisaoProfessorExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_PROFESSOR);
		relatorioVisaoProfessorExportExcelCB.setValue(false);
		relatorioVisaoProfessorExportExcelCB.setFieldLabel("Grade Horária Visão Professor");
		rightGradeHoraria.add(relatorioVisaoProfessorExportExcelCB, formData);
		
		relatorioVisaoCursoExportExcelCB = new CheckBox();
		relatorioVisaoCursoExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_CURSO);
		relatorioVisaoCursoExportExcelCB.setValue(false);
		relatorioVisaoCursoExportExcelCB.setFieldLabel("Grade Horária Visão Curso");
		leftGradeHoraria.add(relatorioVisaoCursoExportExcelCB, formData);
		
		relatorioVisaoAlunoExportExcelCB = new CheckBox();
		relatorioVisaoAlunoExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_ALUNO);
		relatorioVisaoAlunoExportExcelCB.setValue(false);
		relatorioVisaoAlunoExportExcelCB.setFieldLabel("Grade Horária Visão Aluno");
		rightGradeHoraria.add(relatorioVisaoAlunoExportExcelCB, formData);
		
		gradeHorariaFS.add(leftGradeHoraria, new ColumnData(.5));
		gradeHorariaFS.add(rightGradeHoraria, new ColumnData(.5));
		main.add(gradeHorariaFS, new FormData());
		
		//Disciplinas
	    LayoutContainer leftDisciplinas = new LayoutContainer();
	    leftDisciplinas.setLayout(new FormLayout());
	    
	    LayoutContainer rightDisciplinas = new LayoutContainer();
	    rightDisciplinas.setLayout(new FormLayout());
	    
		FieldSet disciplinasFS = new FieldSet();
		disciplinasFS.setWidth(440);
		disciplinasFS.setHeadingHtml("&nbsp;");
		disciplinasFS.setCollapsible(true);
		disciplinasFS.setLayout(new ColumnLayout());
		
		disicplinasPreRequisitosCB = new CheckBox();
		disicplinasPreRequisitosCB.setName(PlanilhasExportExcel.DISCIPLINAS_PRE_REQUISITOS);
		disicplinasPreRequisitosCB.setValue(false);
		disicplinasPreRequisitosCB.setFieldLabel("Disciplinas Pré-Requisitos");
		leftDisciplinas.add(disicplinasPreRequisitosCB, formData);
		
		disicplinasCoRequisitosCB = new CheckBox();
		disicplinasCoRequisitosCB.setName(PlanilhasExportExcel.DISCIPLINAS_CO_REQUISITOS);
		disicplinasCoRequisitosCB.setValue(false);
		disicplinasCoRequisitosCB.setFieldLabel("Disciplinas Co-Requisitos");
		rightDisciplinas.add(disicplinasCoRequisitosCB, formData);
		
		alunosDisciplinasCursadasCB = new CheckBox();
		alunosDisciplinasCursadasCB.setName(PlanilhasExportExcel.ALUNOS_DISCIPLINAS_CURSADAS);
		alunosDisciplinasCursadasCB.setValue(false);
		alunosDisciplinasCursadasCB.setFieldLabel("Disciplinas Cursadas por Alunos");
		leftDisciplinas.add(alunosDisciplinasCursadasCB, formData);
		
		disciplinasFS.add(leftDisciplinas, new ColumnData(.5));
		disciplinasFS.add(rightDisciplinas, new ColumnData(.5));
		main.add(disciplinasFS, new FormData());
		
		nomeArquivoTF = new TextField<String>();
		nomeArquivoTF.setEmptyText("digite um nome para a planilha a ser exportada");
		nomeArquivoTF.setFieldLabel("Nome do Arquivo");
		nomeArquivoTF.setName("nomeArquivo");
		formPanel.add(nomeArquivoTF, new FormData("100%"));
		
	  
	    formPanel.add(main, new FormData("100%"));  

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(exportExcelModal.getExportarXlsBt());
		binding.addButton(exportExcelModal.getExportarXlsxBt());

		exportExcelModal.setFocusWidget(aulasExportExcelCB);
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public FormPanel getFormPanel() {
		return this.formPanel;
	}

	@Override
	public Button getExportarXlsButton() {
		return exportExcelModal.getExportarXlsBt();
	}
	
	@Override
	public Button getExportarXlsxButton() {
		return exportExcelModal.getExportarXlsxBt();
	}

	@Override
	public ExportExcelModal getExportExcelModal() {
		return exportExcelModal;
	}

	@Override
	public CheckBox getCampiExportExcelCheckBox() {
		return campiExportExcelCB;
	}

	@Override
	public CheckBox getUnidadesExportExcelCheckBox() {
		return unidadesExportExcelCB;
	}

	@Override
	public CheckBox getSalasExportExcelCheckBox() {
		return salasExportExcelCB;
	}
	
	@Override
	public CheckBox getDisponibilidadesSalasExportExcelCheckBox() {
		return disponibilidadesSalasExportExcelCB;
	}

	@Override
	public CheckBox getCursosExportExcelCheckBox() {
		return cursosExportExcelCB;
	}

	@Override
	public CheckBox getAreasTitulacaoExportExcelCheckBox() {
		return areasTitulacaoExportExcelCB;
	}

	@Override
	public CheckBox getCursoAreaTitulacaoExportExcelCheckBox() {
		return cursosExportExcelCB;
	}

	@Override
	public CheckBox getDisciplinasExportExcelCheckBox() {
		return disciplinasExportExcelCB;
	}

	@Override
	public CheckBox getDisciplinasSalasExportExcelCheckBox() {
		return disciplinasSalasExportExcelCB;
	}

	@Override
	public CheckBox getEquivalenciasExportExcelCheckBox() {
		return equivalenciasExportExcelCB;
	}

	@Override
	public CheckBox getCampiTrabalhoExportExcelCheckBox() {
		return campiTrabalhoExportExcelCB;
	}

	@Override
	public CheckBox getCurriculosExportExcelCheckBox() {
		return curriculosExportExcelCB;
	}

	@Override
	public CheckBox getOfertasDemandasExportExcelCheckBox() {
		return ofertasDemandasExportExcelCB;
	}

	@Override
	public CheckBox getAlunosExportExcelCheckBox() {
		return alunosExportExcelCB;
	}

	@Override
	public CheckBox getDemandasPorAlunoExportExcelCheckBox() {
		return DemandasPorAlunoExportExcelCB;
	}

	@Override
	public CheckBox getProfessoresExportExcelCheckBox() {
		return professoresExportExcelCB;
	}

	@Override
	public CheckBox getDisponibilidadesProfessoresExportExcelCheckBox() {
		return disponibilidadesProfessoresExportExcelCB;
	}

	@Override
	public CheckBox getHabilitacaoProfessoresExportExcelCheckBox() {
		return habilitacaoProfessoresExportExcelCB;
	}

	@Override
	public CheckBox getResumoCursoExportExcelCheckBox() {
		return resumoCursoExportExcelCB;
	}

	@Override
	public CheckBox getResumoDisciplinaExportExcelCheckBox() {
		return resumoDisciplinaExportExcelCB;
	}

	@Override
	public CheckBox getAtendimentosMatriculaExportExcelCheckBox() {
		return atendimentosMatriculaExportExcelCB;
	}

	@Override
	public CheckBox getAtendimentosDisciplinaExportExcelCheckBox() {
		return atendimentosDisciplinaExportExcelCB;
	}

	@Override
	public CheckBox getAtendimentosFaixaDemandaExportExcelCheckBox() {
		return atendimentosFaixaDemandaExportExcelCB;
	}
	
	@Override
	public CheckBox getAtendimentosCargaHorariaExportExcelCheckBox() {
		return atendimentosCargaHorariaExportExcelCB;
	}

	@Override
	public CheckBox getRelatorioVisaoSalaExportExcelCheckBox() {
		return relatorioVisaoSalaExportExcelCB;
	}

	@Override
	public CheckBox getRelatorioVisaoProfessorExportExcelCheckBox() {
		return relatorioVisaoProfessorExportExcelCB;
	}

	@Override
	public CheckBox getRelatorioVisaoAlunoExportExcelCheckBox() {
		return relatorioVisaoAlunoExportExcelCB;
	}

	@Override
	public CheckBox getRelatorioVisaoCursoExportExcelCheckBox() {
		return relatorioVisaoCursoExportExcelCB;
	}

	@Override
	public CheckBox getAtendimentosPorAlunoExportExcelCheckBox() {
		return atendimentosPorAlunoExportExcelCB;
	}

	@Override
	public CheckBox getAulasExportExcelCheckBox() {
		return aulasExportExcelCB;
	}
	
	@Override
	public CheckBox getPercentMestresDoutores() {
		return percentMestresDoutoresExportExcelCB;
	}
	
	@Override
	public TextField<String> getNomeArquivoTextField() {
		return nomeArquivoTF;
	}

	@Override
	public CheckBox getAtendimentosCargaHoraria() {
		return atendimentosCargaHorariaCB;
	}

	@Override
	public CheckBox getDisicplinasPreRequisitos() {
		return disicplinasPreRequisitosCB;
	}

	@Override
	public CheckBox getDisicplinasCoRequisitos() {
		return disicplinasCoRequisitosCB;
	}

	@Override
	public CheckBox getAlunosDisciplinasCursadas() {
		return alunosDisciplinasCursadasCB;
	}

	@Override
	public CheckBox getAtendimentosFaixaCredito() {
		return atendimentosFaixaCreditoCB;
	}

	@Override
	public CheckBox getAtendimentosFaixaDisciplina() {
		return atendimentosFaixaDisciplinaCB;
	}

	@Override
	public CheckBox getProfessoresQuantidadeJanelas() {
		return professoresQuantidadeJanelasCB;
	}

	@Override
	public CheckBox getProfessoresDisciplinasHabilitadas() {
		return professoresDisciplinasHabilitadasCB;
	}

	@Override
	public CheckBox getProfessoresDisciplinasLecionadas() {
		return professoresDisciplinasLecionadasCB;
	}

	@Override
	public CheckBox getProfessoresTitulacoes() {
		return professoresTitulacoesCB;
	}

	@Override
	public CheckBox getProfessoresAreasConhecimento() {
		return professoresAreasConhecimentoCB;
	}

	@Override
	public CheckBox getAmbientesFaixaOcupacao() {
		return ambientesFaixaOcupacaoCB;
	}

	@Override
	public CheckBox getAmbientesFaixaUtilizacao() {
		return ambientesFaixaUtilizacaoCB;
	}

}
