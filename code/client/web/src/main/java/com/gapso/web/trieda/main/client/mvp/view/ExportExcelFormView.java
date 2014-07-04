package com.gapso.web.trieda.main.client.mvp.view;


import com.extjs.gxt.ui.client.Style;
import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.CheckBoxGroup;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
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
	private CheckBox ofertasCursosCampiExportExcelCB;
	private CheckBox ofertasDemandasExportExcelCB;
	private CheckBox alunosExportExcelCB;
	private CheckBox demandasPorAlunoExportExcelCB;
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
	private CheckBox ocupacaoAmbientesSemanaCB;
	private CheckBox ambientesFaixaUtilizacaoCB;
	private CheckBox turnosCB;
	private CheckBox tiposCursoCB;
	private CheckBox divisaoCreditosCB;
	private CheckBox resumoCenarioCB;
	private CheckBox resumoCampusCB;
	private CheckBox semanaLetivaCB;
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
		exportExcelModal.setWidth(610);
		createForm();
		exportExcelModal.setContent(formPanel);
	}
	
	private void createForm()
	{
		formPanel = new FormPanel();
		formPanel.setFrame(true);
		formPanel.setHeadingHtml("Escolha quais planilhas deseja exportar");
		formPanel.setSize(600, -1);
		formPanel.setButtonAlign(HorizontalAlignment.LEFT);
		formPanel.setScrollMode(Style.Scroll.AUTOY);
		
		LayoutContainer main = new LayoutContainer();
	    main.setLayout(new FormLayout());
	    
	    //Geral
		FieldSet geralFS = new FieldSet();
		geralFS.setWidth(550);
		geralFS.setCollapsible(true);
		geralFS.setHeadingHtml("Dados de Entrada");
		geralFS.setLayout(new RowLayout());
		
		turnosCB = new CheckBox();
		turnosCB.setName(PlanilhasExportExcel.TURNOS);
		turnosCB.setValue(false);
		turnosCB.setBoxLabel("Turnos");
		
		semanaLetivaCB = new CheckBox();
		semanaLetivaCB.setName(PlanilhasExportExcel.SEMANAS_LETIVA);
		semanaLetivaCB.setValue(false);
		semanaLetivaCB.setBoxLabel("Semanas Letiva");
		
		tiposCursoCB = new CheckBox();
		tiposCursoCB.setName(PlanilhasExportExcel.TIPOS_CURSO);
		tiposCursoCB.setValue(false);
		tiposCursoCB.setBoxLabel("Tipos de Curso");
		
		campiExportExcelCB = new CheckBox();
		campiExportExcelCB.setName(PlanilhasExportExcel.CAMPI);
		campiExportExcelCB.setValue(false);
		campiExportExcelCB.setBoxLabel("Campi");
		
		unidadesExportExcelCB = new CheckBox();
		unidadesExportExcelCB.setName(PlanilhasExportExcel.UNIDADES);
		unidadesExportExcelCB.setValue(false);
		unidadesExportExcelCB.setBoxLabel("Unidades");
		
		salasExportExcelCB = new CheckBox();
		salasExportExcelCB.setName(PlanilhasExportExcel.SALAS);
		salasExportExcelCB.setValue(false);
		salasExportExcelCB.setBoxLabel("Ambientes");
		
		disponibilidadesSalasExportExcelCB = new CheckBox();
		disponibilidadesSalasExportExcelCB.setName(PlanilhasExportExcel.DISPONIBILIDADES_SALAS);
		disponibilidadesSalasExportExcelCB.setValue(false);
		disponibilidadesSalasExportExcelCB.setBoxLabel("Disponibilidades dos Ambientes");
		
		cursosExportExcelCB = new CheckBox();
		cursosExportExcelCB.setName(PlanilhasExportExcel.CURSOS);
		cursosExportExcelCB.setValue(false);
		cursosExportExcelCB.setBoxLabel("Cursos");
		
		areasTitulacaoExportExcelCB = new CheckBox();
		areasTitulacaoExportExcelCB.setName(PlanilhasExportExcel.AREAS_TITULACAO);
		areasTitulacaoExportExcelCB.setValue(false);
		areasTitulacaoExportExcelCB.setBoxLabel("Áreas de Conhecimento");
		
		cursoAreaTitulacaoExportExcelCB = new CheckBox();
		cursoAreaTitulacaoExportExcelCB.setName(PlanilhasExportExcel.CURSO_AREAS_TITULACAO);
		cursoAreaTitulacaoExportExcelCB.setValue(false);
		cursoAreaTitulacaoExportExcelCB.setBoxLabel("Curso - Áreas de Conhecimento");
		
		curriculosExportExcelCB = new CheckBox();
		curriculosExportExcelCB.setName(PlanilhasExportExcel.CURRICULOS);
		curriculosExportExcelCB.setValue(false);
		curriculosExportExcelCB.setBoxLabel("Matrizes Curriculares");
		
		disciplinasExportExcelCB = new CheckBox();
		disciplinasExportExcelCB.setName(PlanilhasExportExcel.DISCIPLINAS);
		disciplinasExportExcelCB.setValue(false);
		disciplinasExportExcelCB.setBoxLabel("Disciplinas");
		
		disciplinasSalasExportExcelCB = new CheckBox();
		disciplinasSalasExportExcelCB.setName(PlanilhasExportExcel.DISCIPLINAS_SALAS);
		disciplinasSalasExportExcelCB.setValue(false);
		disciplinasSalasExportExcelCB.setBoxLabel("Disciplinas-Ambientes");
		
		equivalenciasExportExcelCB = new CheckBox();
		equivalenciasExportExcelCB.setName(PlanilhasExportExcel.EQUIVALENCIAS);
		equivalenciasExportExcelCB.setValue(false);
		equivalenciasExportExcelCB.setBoxLabel("Equivalências");
		
		alunosExportExcelCB = new CheckBox();
		alunosExportExcelCB.setName(PlanilhasExportExcel.ALUNOS);
		alunosExportExcelCB.setValue(false);
		alunosExportExcelCB.setBoxLabel("Alunos");
		
		ofertasCursosCampiExportExcelCB = new CheckBox();
		ofertasCursosCampiExportExcelCB.setName(PlanilhasExportExcel.OFERTA_CURSOS_CAMPI);
		ofertasCursosCampiExportExcelCB.setValue(false);
		ofertasCursosCampiExportExcelCB.setBoxLabel("Ofertas de Cursos em Campi");
		
		ofertasDemandasExportExcelCB = new CheckBox();
		ofertasDemandasExportExcelCB.setName(PlanilhasExportExcel.DEMANDAS);
		ofertasDemandasExportExcelCB.setValue(false);
		ofertasDemandasExportExcelCB.setBoxLabel("Ofertas e Demandas");
		
		demandasPorAlunoExportExcelCB = new CheckBox();
		demandasPorAlunoExportExcelCB.setName(PlanilhasExportExcel.DEMANDAS_POR_ALUNO);
		demandasPorAlunoExportExcelCB.setValue(false);
		demandasPorAlunoExportExcelCB.setBoxLabel("Demandas por Aluno");
		
		aulasExportExcelCB = new CheckBox();
		aulasExportExcelCB.setName(PlanilhasExportExcel.AULAS);
		aulasExportExcelCB.setValue(false);
		aulasExportExcelCB.setBoxLabel("Aulas");
		
		professoresExportExcelCB = new CheckBox();
		professoresExportExcelCB.setName(PlanilhasExportExcel.PROFESSORES);
		professoresExportExcelCB.setValue(false);
		professoresExportExcelCB.setBoxLabel("Professores");
		
		disponibilidadesProfessoresExportExcelCB = new CheckBox();
		disponibilidadesProfessoresExportExcelCB.setName(PlanilhasExportExcel.DISPONIBILIDADES_PROFESSORES);
		disponibilidadesProfessoresExportExcelCB.setValue(false);
		disponibilidadesProfessoresExportExcelCB.setBoxLabel("Disponibilidades dos Professores");
		
		campiTrabalhoExportExcelCB = new CheckBox();
		campiTrabalhoExportExcelCB.setName(PlanilhasExportExcel.CAMPI_TRABALHO);
		campiTrabalhoExportExcelCB.setValue(false);
		campiTrabalhoExportExcelCB.setBoxLabel("Campi de Trabalho");
		
	    habilitacaoProfessoresExportExcelCB = new CheckBox();
	    habilitacaoProfessoresExportExcelCB.setName(PlanilhasExportExcel.HABILITACAO_PROFESSORES);
	    habilitacaoProfessoresExportExcelCB.setValue(false);
	    habilitacaoProfessoresExportExcelCB.setBoxLabel("Habilitação dos Professores");
	    
	    divisaoCreditosCB = new CheckBox();
	    divisaoCreditosCB.setName(PlanilhasExportExcel.DIVISAO_CREDITOS);
	    divisaoCreditosCB.setValue(false);
	    divisaoCreditosCB.setBoxLabel("Regras de Divisão de Créditos");
	    
		disicplinasPreRequisitosCB = new CheckBox();
		disicplinasPreRequisitosCB.setName(PlanilhasExportExcel.DISCIPLINAS_PRE_REQUISITOS);
		disicplinasPreRequisitosCB.setValue(false);
		disicplinasPreRequisitosCB.setBoxLabel("Disciplinas Pré-Requisitos");
		
		disicplinasCoRequisitosCB = new CheckBox();
		disicplinasCoRequisitosCB.setName(PlanilhasExportExcel.DISCIPLINAS_CO_REQUISITOS);
		disicplinasCoRequisitosCB.setValue(false);
		disicplinasCoRequisitosCB.setBoxLabel("Disciplinas Co-Requisitos");
		
		alunosDisciplinasCursadasCB = new CheckBox();
		alunosDisciplinasCursadasCB.setName(PlanilhasExportExcel.ALUNOS_DISCIPLINAS_CURSADAS);
		alunosDisciplinasCursadasCB.setValue(false);
		alunosDisciplinasCursadasCB.setBoxLabel("Disciplinas Cursadas por Alunos");
	    
		geralFS.add(createCheckBoxGroup(semanaLetivaCB, equivalenciasExportExcelCB));
		geralFS.add(createCheckBoxGroup(turnosCB, alunosExportExcelCB));
		geralFS.add(createCheckBoxGroup(tiposCursoCB, ofertasCursosCampiExportExcelCB));
		geralFS.add(createCheckBoxGroup(divisaoCreditosCB, ofertasDemandasExportExcelCB));
		geralFS.add(createCheckBoxGroup(campiExportExcelCB, demandasPorAlunoExportExcelCB));
		geralFS.add(createCheckBoxGroup(unidadesExportExcelCB, professoresExportExcelCB));
		geralFS.add(createCheckBoxGroup(salasExportExcelCB, disponibilidadesProfessoresExportExcelCB));
		geralFS.add(createCheckBoxGroup(disponibilidadesSalasExportExcelCB, campiTrabalhoExportExcelCB));
		geralFS.add(createCheckBoxGroup(cursosExportExcelCB, habilitacaoProfessoresExportExcelCB));
		geralFS.add(createCheckBoxGroup(areasTitulacaoExportExcelCB, disciplinasExportExcelCB));
		geralFS.add(createCheckBoxGroup(cursoAreaTitulacaoExportExcelCB, aulasExportExcelCB));
		geralFS.add(createCheckBoxGroup(curriculosExportExcelCB, disicplinasPreRequisitosCB));
		geralFS.add(createCheckBoxGroup(disicplinasCoRequisitosCB, alunosDisciplinasCursadasCB));
		geralFS.add(createCheckBoxGroup(disciplinasSalasExportExcelCB, null));
		
		main.add(geralFS, new FormData());

		//Professores
		FieldSet professoresFS = new FieldSet();
		professoresFS.setWidth(550);
		professoresFS.setHeadingHtml("Perfil de Docentes");
		professoresFS.setCollapsible(true);
		professoresFS.setLayout(new RowLayout());
		
		percentMestresDoutoresExportExcelCB = new CheckBox();
		percentMestresDoutoresExportExcelCB.setName(PlanilhasExportExcel.PERCENT_MESTRES_DOUTORES);
		percentMestresDoutoresExportExcelCB.setValue(false);
		percentMestresDoutoresExportExcelCB.setBoxLabel("Porcentagem de Mestres e Doutores");
		
		professoresTitulacoesCB = new CheckBox();
		professoresTitulacoesCB.setName(PlanilhasExportExcel.PROFESSORES_TITULACOES);
		professoresTitulacoesCB.setValue(false);
		professoresTitulacoesCB.setBoxLabel("Professores por Titulações");
		
		professoresDisciplinasLecionadasCB = new CheckBox();
		professoresDisciplinasLecionadasCB.setName(PlanilhasExportExcel.PROFESSORES_DISCIPLINAS_LECIONADAS);
		professoresDisciplinasLecionadasCB.setValue(false);
		professoresDisciplinasLecionadasCB.setBoxLabel("Professores por Qtde de Disciplinas Lecionadas");
		
		professoresDisciplinasHabilitadasCB = new CheckBox();
		professoresDisciplinasHabilitadasCB.setName(PlanilhasExportExcel.PROFESSORES_DISCIPLINAS_HABILITADAS);
		professoresDisciplinasHabilitadasCB.setValue(false);
		professoresDisciplinasHabilitadasCB.setBoxLabel("Professores por Qtde de Disciplinas \nHabilitadas");
		professoresDisciplinasHabilitadasCB.setStyleAttribute("height", "40");
		professoresDisciplinasHabilitadasCB.addStyleName("boxLabel");
		
		professoresAreasConhecimentoCB = new CheckBox();
		professoresAreasConhecimentoCB.setName(PlanilhasExportExcel.PROFESSORES_AREAS_CONHECIMENTO);
		professoresAreasConhecimentoCB.setValue(false);
		professoresAreasConhecimentoCB.setBoxLabel("Professores por Áreas de Conhecimento");
		
		professoresQuantidadeJanelasCB = new CheckBox();
		professoresQuantidadeJanelasCB.setName(PlanilhasExportExcel.PROFESSORES_QUANTIDADE_JANELAS);
		professoresQuantidadeJanelasCB.setValue(false);
		professoresQuantidadeJanelasCB.setBoxLabel("Professores por Qtde de Janelas");
		
		professoresFS.add(createCheckBoxGroup(percentMestresDoutoresExportExcelCB, professoresDisciplinasLecionadasCB));
		professoresFS.add(createCheckBoxGroup(professoresQuantidadeJanelasCB, professoresTitulacoesCB));
		professoresFS.add(createCheckBoxGroup(professoresDisciplinasHabilitadasCB, professoresAreasConhecimentoCB));
		main.add(professoresFS, new FormData());
		
		//Resumo
		FieldSet resumoFS = new FieldSet();
		resumoFS.setWidth(550);
		resumoFS.setHeadingHtml("Resumos");
		resumoFS.setCollapsible(true);
		resumoFS.setLayout(new RowLayout());
		
		resumoCursoExportExcelCB = new CheckBox();
		resumoCursoExportExcelCB.setName(PlanilhasExportExcel.RESUMO_CURSO);
		resumoCursoExportExcelCB.setValue(false);
		resumoCursoExportExcelCB.setBoxLabel("Resumo por Curso");
		
		resumoDisciplinaExportExcelCB = new CheckBox();
		resumoDisciplinaExportExcelCB.setName(PlanilhasExportExcel.RESUMO_DISCIPLINA);
		resumoDisciplinaExportExcelCB.setValue(false);
		resumoDisciplinaExportExcelCB.setBoxLabel("Resumo por Disciplina");
		
		resumoCenarioCB = new CheckBox();
		resumoCenarioCB.setName(PlanilhasExportExcel.RESUMO_CENARIO);
		resumoCenarioCB.setValue(false);
		resumoCenarioCB.setBoxLabel("Resumo do Cenário");
		
		resumoCampusCB = new CheckBox();
		resumoCampusCB.setName(PlanilhasExportExcel.RESUMO_CAMPUS);
		resumoCampusCB.setValue(false);
		resumoCampusCB.setBoxLabel("Resumo por Campus");
		
		resumoFS.add(createCheckBoxGroup(resumoCenarioCB, resumoCursoExportExcelCB));
		resumoFS.add(createCheckBoxGroup(resumoCampusCB, resumoDisciplinaExportExcelCB));
		main.add(resumoFS, new FormData());
		
		//Atendimentos
		FieldSet atendimentosFS = new FieldSet();
		atendimentosFS.setWidth(550);
		atendimentosFS.setHeadingHtml("Relatórios de Atendimentos");
		atendimentosFS.setCollapsible(true);
		atendimentosFS.setLayout(new RowLayout());
		
		atendimentosCargaHorariaExportExcelCB = new CheckBox();
		atendimentosCargaHorariaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_CARGA_HORARIA);
		atendimentosCargaHorariaExportExcelCB.setValue(false);
		atendimentosCargaHorariaExportExcelCB.setBoxLabel("Atendimentos Por Carga Horária");
		
		atendimentosMatriculaExportExcelCB = new CheckBox();
		atendimentosMatriculaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_MATRICULA);
		atendimentosMatriculaExportExcelCB.setValue(false);
		atendimentosMatriculaExportExcelCB.setBoxLabel("Atendimentos por Matrícula");
		
		atendimentosDisciplinaExportExcelCB = new CheckBox();
		atendimentosDisciplinaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_DISCIPLINA);
		atendimentosDisciplinaExportExcelCB.setValue(false);
		atendimentosDisciplinaExportExcelCB.setBoxLabel("Atendimentos por Disciplina");
		
		atendimentosFaixaDemandaExportExcelCB = new CheckBox();
		atendimentosFaixaDemandaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DEMANDA);
		atendimentosFaixaDemandaExportExcelCB.setValue(false);
		atendimentosFaixaDemandaExportExcelCB.setBoxLabel("Atendimentos Faixa de Demanda");
		
		atendimentosFaixaCreditoCB = new CheckBox();
		atendimentosFaixaCreditoCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_CREDITO);
		atendimentosFaixaCreditoCB.setValue(false);
		atendimentosFaixaCreditoCB.setBoxLabel("Atendimentos por Faixa de Crédito");
		
		atendimentosFaixaDisciplinaCB = new CheckBox();
		atendimentosFaixaDisciplinaCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DISCIPLINA);
		atendimentosFaixaDisciplinaCB.setValue(false);
		atendimentosFaixaDisciplinaCB.setBoxLabel("Atendimentos por Faixa de Disciplina");
		
		atendimentosPorAlunoExportExcelCB = new CheckBox();
		atendimentosPorAlunoExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_POR_ALUNO);
		atendimentosPorAlunoExportExcelCB.setValue(false);
		atendimentosPorAlunoExportExcelCB.setBoxLabel("Atendimentos por Aluno");

		atendimentosFS.add(createCheckBoxGroup(atendimentosDisciplinaExportExcelCB, atendimentosFaixaDemandaExportExcelCB));
		atendimentosFS.add(createCheckBoxGroup(atendimentosMatriculaExportExcelCB, atendimentosFaixaCreditoCB));
		atendimentosFS.add(createCheckBoxGroup(atendimentosCargaHorariaExportExcelCB, atendimentosFaixaDisciplinaCB));
		atendimentosFS.add(createCheckBoxGroup(atendimentosPorAlunoExportExcelCB, null));
		main.add(atendimentosFS, new FormData());
		
		//Ambientes
		FieldSet ambientesFS = new FieldSet();
		ambientesFS.setWidth(550);
		ambientesFS.setHeadingHtml("Ambientes");
		ambientesFS.setCollapsible(true);
		ambientesFS.setLayout(new RowLayout());
		
		ambientesFaixaUtilizacaoCB = new CheckBox();
		ambientesFaixaUtilizacaoCB.setName(PlanilhasExportExcel.AMBIENTES_FAIXA_UTILIZACAO);
		ambientesFaixaUtilizacaoCB.setValue(false);
		ambientesFaixaUtilizacaoCB.setBoxLabel("Ambientes por Faixa de Utilização dos Horários");
		
		ambientesFaixaOcupacaoCB = new CheckBox();
		ambientesFaixaOcupacaoCB.setName(PlanilhasExportExcel.AMBIENTES_FAIXA_OCUPACAO);
		ambientesFaixaOcupacaoCB.setValue(false);
		ambientesFaixaOcupacaoCB.setBoxLabel("Ambientes por Faixa de Ocupação da Capacidade");
		ambientesFaixaOcupacaoCB.setStyleAttribute("height", "40");
		ambientesFaixaOcupacaoCB.addStyleName("boxLabel");
		
		ocupacaoAmbientesSemanaCB = new CheckBox();
		ocupacaoAmbientesSemanaCB.setName(PlanilhasExportExcel.AMBIENTES_OCUPACAO_SEMANA);
		ocupacaoAmbientesSemanaCB.setValue(false);
		ocupacaoAmbientesSemanaCB.setBoxLabel("Ocupação dos Ambientes por Dia da Semana");
		ocupacaoAmbientesSemanaCB.setStyleAttribute("height", "40");
		ocupacaoAmbientesSemanaCB.addStyleName("boxLabel");
		
		ambientesFS.add(createCheckBoxGroup(ambientesFaixaOcupacaoCB, ambientesFaixaUtilizacaoCB));
		ambientesFS.add(createCheckBoxGroup(ocupacaoAmbientesSemanaCB, null));
		main.add(ambientesFS, new FormData());
		
		//GradeHoraria
		FieldSet gradeHorariaFS = new FieldSet();
		gradeHorariaFS.setWidth(550);
		gradeHorariaFS.setHeadingHtml("Grades Horárias");
		gradeHorariaFS.setCollapsible(true);
		gradeHorariaFS.setLayout(new RowLayout());
		
		relatorioVisaoSalaExportExcelCB = new CheckBox();
		relatorioVisaoSalaExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_SALA);
		relatorioVisaoSalaExportExcelCB.setValue(false);
		relatorioVisaoSalaExportExcelCB.setBoxLabel("Grade Horária Visão Ambiente");
		
		relatorioVisaoProfessorExportExcelCB = new CheckBox();
		relatorioVisaoProfessorExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_PROFESSOR);
		relatorioVisaoProfessorExportExcelCB.setValue(false);
		relatorioVisaoProfessorExportExcelCB.setBoxLabel("Grade Horária Visão Professor");
		
		relatorioVisaoCursoExportExcelCB = new CheckBox();
		relatorioVisaoCursoExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_CURSO);
		relatorioVisaoCursoExportExcelCB.setValue(false);
		relatorioVisaoCursoExportExcelCB.setBoxLabel("Grade Horária Visão Curso");
		
		relatorioVisaoAlunoExportExcelCB = new CheckBox();
		relatorioVisaoAlunoExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_ALUNO);
		relatorioVisaoAlunoExportExcelCB.setValue(false);
		relatorioVisaoAlunoExportExcelCB.setBoxLabel("Grade Horária Visão Aluno");
		
		gradeHorariaFS.add(createCheckBoxGroup(relatorioVisaoSalaExportExcelCB, relatorioVisaoCursoExportExcelCB));
		gradeHorariaFS.add(createCheckBoxGroup(relatorioVisaoProfessorExportExcelCB, relatorioVisaoAlunoExportExcelCB));
		main.add(gradeHorariaFS, new FormData());
		
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
	
	private CheckBoxGroup createCheckBoxGroup(CheckBox c1, CheckBox c2) {
		CheckBoxGroup checkBoxGroup = new CheckBoxGroup();
		c1.setWidth(250);
		checkBoxGroup.add(c1);
		if (c2 != null)
			checkBoxGroup.add(c2);
		
		return checkBoxGroup;
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
	public CheckBox getOfertasCursosCampiExportExcelCheckBox() {
		return ofertasCursosCampiExportExcelCB;
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
		return demandasPorAlunoExportExcelCB;
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
	public CheckBox getOcupacaoAmbientesSemana() {
		return ocupacaoAmbientesSemanaCB;
	}

	@Override
	public CheckBox getAmbientesFaixaUtilizacao() {
		return ambientesFaixaUtilizacaoCB;
	}
	
	@Override
	public CheckBox getTurnos() {
		return turnosCB;
	}

	@Override
	public CheckBox getTiposCurso() {
		return tiposCursoCB;
	}
	
	@Override
	public CheckBox getDivisaoCreditos() {
		return divisaoCreditosCB;
	}
	
	@Override
	public CheckBox getResumoCenario() {
		return resumoCenarioCB;
	}
	
	@Override
	public CheckBox getResumoCampus() {
		return resumoCampusCB;
	}
	
	@Override
	public CheckBox getSemanasLetiva() {
		return semanaLetivaCB;
	}

}
