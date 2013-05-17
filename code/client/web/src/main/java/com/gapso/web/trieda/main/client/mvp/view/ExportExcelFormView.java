package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
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
	private CheckBox relatorioVisaoSalaExportExcelCB;
	private CheckBox relatorioVisaoProfessorExportExcelCB;
	private CheckBox relatorioVisaoCursoExportExcelCB;
	private CheckBox relatorioVisaoAlunoExportExcelCB;
	private CheckBox atendimentosPorAlunoExportExcelCB;
	private CheckBox percentMestresDoutoresExportExcelCB;
	private CheckBox aulasExportExcelCB;
	private TextField<String> nomeArquivoTF;
	
	public ExportExcelFormView()
	{
		initUI();
	}
	
	private void initUI() 
	{
		String title = "Exportação Excel";
		exportExcelModal = new ExportExcelModal(title, Resources.DEFAULTS.exportar16());
		exportExcelModal.setHeight(660);
		exportExcelModal.setWidth(500);
		createForm();
		exportExcelModal.setContent(formPanel);
	}
	
	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setFrame(true);
		formPanel.setHeading("Escolha quais planilhas deseja exportar");
		formPanel.setSize(600, -1);
		formPanel.setButtonAlign(HorizontalAlignment.CENTER);
		
		LayoutContainer main = new LayoutContainer();
	    main.setLayout(new ColumnLayout());
	    
	    LayoutContainer left = new LayoutContainer();
	    FormLayout layout = new FormLayout();
	    left.setLayout(layout);
		
		campiExportExcelCB = new CheckBox();
		campiExportExcelCB.setName(PlanilhasExportExcel.CAMPI);
		campiExportExcelCB.setValue(false);
		campiExportExcelCB.setFieldLabel("Campi");
		left.add(campiExportExcelCB, formData);
		
		unidadesExportExcelCB = new CheckBox();
		unidadesExportExcelCB.setName(PlanilhasExportExcel.UNIDADES);
		unidadesExportExcelCB.setValue(false);
		unidadesExportExcelCB.setFieldLabel("Unidades");
		left.add(unidadesExportExcelCB, formData);
		
		salasExportExcelCB = new CheckBox();
		salasExportExcelCB.setName(PlanilhasExportExcel.SALAS);
		salasExportExcelCB.setValue(false);
		salasExportExcelCB.setFieldLabel("Salas");
		left.add(salasExportExcelCB, formData);
		
		cursosExportExcelCB = new CheckBox();
		cursosExportExcelCB.setName(PlanilhasExportExcel.CURSOS);
		cursosExportExcelCB.setValue(false);
		cursosExportExcelCB.setFieldLabel("Cursos");
		left.add(cursosExportExcelCB, formData);
		
		areasTitulacaoExportExcelCB = new CheckBox();
		areasTitulacaoExportExcelCB.setName(PlanilhasExportExcel.AREAS_TITULACAO);
		areasTitulacaoExportExcelCB.setValue(false);
		areasTitulacaoExportExcelCB.setFieldLabel("Areas de Titulação");
		left.add(areasTitulacaoExportExcelCB, formData);
		
		cursoAreaTitulacaoExportExcelCB = new CheckBox();
		cursoAreaTitulacaoExportExcelCB.setName(PlanilhasExportExcel.CURSO_AREAS_TITULACAO);
		cursoAreaTitulacaoExportExcelCB.setValue(false);
		cursoAreaTitulacaoExportExcelCB.setFieldLabel("Curso - Areas de Titulação");
		left.add(cursoAreaTitulacaoExportExcelCB, formData);
		
		disciplinasExportExcelCB = new CheckBox();
		disciplinasExportExcelCB.setName(PlanilhasExportExcel.DISCIPLINAS);
		disciplinasExportExcelCB.setValue(false);
		disciplinasExportExcelCB.setFieldLabel("Disciplinas");
		left.add(disciplinasExportExcelCB, formData);
		
		disciplinasSalasExportExcelCB = new CheckBox();
		disciplinasSalasExportExcelCB.setName(PlanilhasExportExcel.DISCIPLINAS_SALAS);
		disciplinasSalasExportExcelCB.setValue(false);
		disciplinasSalasExportExcelCB.setFieldLabel("Disciplinas-Salas");
		left.add(disciplinasSalasExportExcelCB, formData);
		
		equivalenciasExportExcelCB = new CheckBox();
		equivalenciasExportExcelCB.setName(PlanilhasExportExcel.EQUIVALENCIAS);
		equivalenciasExportExcelCB.setValue(false);
		equivalenciasExportExcelCB.setFieldLabel("Equivalencias");
		left.add(equivalenciasExportExcelCB, formData);
		
		campiTrabalhoExportExcelCB = new CheckBox();
		campiTrabalhoExportExcelCB.setName(PlanilhasExportExcel.CAMPI_TRABALHO);
		campiTrabalhoExportExcelCB.setValue(false);
		campiTrabalhoExportExcelCB.setFieldLabel("Campi de Trabalho");
		left.add(campiTrabalhoExportExcelCB, formData);
		
		curriculosExportExcelCB = new CheckBox();
		curriculosExportExcelCB.setName(PlanilhasExportExcel.CURRICULOS);
		curriculosExportExcelCB.setValue(false);
		curriculosExportExcelCB.setFieldLabel("Curriculos");
		left.add(curriculosExportExcelCB, formData);
		
		
		ofertasDemandasExportExcelCB = new CheckBox();
		ofertasDemandasExportExcelCB.setName(PlanilhasExportExcel.DEMANDAS);
		ofertasDemandasExportExcelCB.setValue(false);
		ofertasDemandasExportExcelCB.setFieldLabel("Demandas");
		left.add(ofertasDemandasExportExcelCB, formData);
		
		alunosExportExcelCB = new CheckBox();
		alunosExportExcelCB.setName(PlanilhasExportExcel.ALUNOS);
		alunosExportExcelCB.setValue(false);
		alunosExportExcelCB.setFieldLabel("Alunos");
		left.add(alunosExportExcelCB, formData);
		
		DemandasPorAlunoExportExcelCB = new CheckBox();
		DemandasPorAlunoExportExcelCB.setName(PlanilhasExportExcel.DEMANDAS_POR_ALUNO);
		DemandasPorAlunoExportExcelCB.setValue(false);
		DemandasPorAlunoExportExcelCB.setFieldLabel("Demandas por Aluno");
		left.add(DemandasPorAlunoExportExcelCB, formData);
		
		professoresExportExcelCB = new CheckBox();
		professoresExportExcelCB.setName(PlanilhasExportExcel.PROFESSORES);
		professoresExportExcelCB.setValue(false);
		professoresExportExcelCB.setFieldLabel("Professores");
		left.add(professoresExportExcelCB, formData);
		
		disponibilidadesProfessoresExportExcelCB = new CheckBox();
		disponibilidadesProfessoresExportExcelCB.setName(PlanilhasExportExcel.DISPONIBILIDADES_PROFESSORES);
		disponibilidadesProfessoresExportExcelCB.setValue(false);
		disponibilidadesProfessoresExportExcelCB.setFieldLabel("Disponibilidades Professores");
		left.add(disponibilidadesProfessoresExportExcelCB, formData);
		
	    LayoutContainer right = new LayoutContainer();
	    right.setStyleAttribute("paddingLeft", "10px");
	    layout = new FormLayout();
	    right.setLayout(layout);
		
	    habilitacaoProfessoresExportExcelCB = new CheckBox();
	    habilitacaoProfessoresExportExcelCB.setName(PlanilhasExportExcel.HABILITACAO_PROFESSORES);
	    habilitacaoProfessoresExportExcelCB.setValue(false);
	    habilitacaoProfessoresExportExcelCB.setFieldLabel("Habilitação dos Professores");
		right.add(habilitacaoProfessoresExportExcelCB, formData);
		
		resumoCursoExportExcelCB = new CheckBox();
		resumoCursoExportExcelCB.setName(PlanilhasExportExcel.RESUMO_CURSO);
		resumoCursoExportExcelCB.setValue(false);
		resumoCursoExportExcelCB.setFieldLabel("Resumo por Curso");
		right.add(resumoCursoExportExcelCB, formData);
		
		resumoDisciplinaExportExcelCB = new CheckBox();
		resumoDisciplinaExportExcelCB.setName(PlanilhasExportExcel.RESUMO_DISCIPLINA);
		resumoDisciplinaExportExcelCB.setValue(false);
		resumoDisciplinaExportExcelCB.setFieldLabel("Resumo por Disciplina");
		right.add(resumoDisciplinaExportExcelCB, formData);
		
		atendimentosMatriculaExportExcelCB = new CheckBox();
		atendimentosMatriculaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_MATRICULA);
		atendimentosMatriculaExportExcelCB.setValue(false);
		atendimentosMatriculaExportExcelCB.setFieldLabel("Atendimentos por Matricula");
		right.add(atendimentosMatriculaExportExcelCB, formData);
		
		atendimentosDisciplinaExportExcelCB = new CheckBox();
		atendimentosDisciplinaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_DISCIPLINA);
		atendimentosDisciplinaExportExcelCB.setValue(false);
		atendimentosDisciplinaExportExcelCB.setFieldLabel("Atendimentos por Disciplina");
		right.add(atendimentosDisciplinaExportExcelCB, formData);
		
		atendimentosFaixaDemandaExportExcelCB = new CheckBox();
		atendimentosFaixaDemandaExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DEMANDA);
		atendimentosFaixaDemandaExportExcelCB.setValue(false);
		atendimentosFaixaDemandaExportExcelCB.setFieldLabel("Atendimentos Faixa de Demanda");
		right.add(atendimentosFaixaDemandaExportExcelCB, formData);
		
		relatorioVisaoSalaExportExcelCB = new CheckBox();
		relatorioVisaoSalaExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_SALA);
		relatorioVisaoSalaExportExcelCB.setValue(false);
		relatorioVisaoSalaExportExcelCB.setFieldLabel("Relatório Visão Sala");
		right.add(relatorioVisaoSalaExportExcelCB, formData);
		
		relatorioVisaoProfessorExportExcelCB = new CheckBox();
		relatorioVisaoProfessorExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_PROFESSOR);
		relatorioVisaoProfessorExportExcelCB.setValue(false);
		relatorioVisaoProfessorExportExcelCB.setFieldLabel("Relatório Visão Professor");
		right.add(relatorioVisaoProfessorExportExcelCB, formData);
		
		relatorioVisaoCursoExportExcelCB = new CheckBox();
		relatorioVisaoCursoExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_CURSO);
		relatorioVisaoCursoExportExcelCB.setValue(false);
		relatorioVisaoCursoExportExcelCB.setFieldLabel("Relatório Visão Curso");
		right.add(relatorioVisaoCursoExportExcelCB, formData);
		
		relatorioVisaoAlunoExportExcelCB = new CheckBox();
		relatorioVisaoAlunoExportExcelCB.setName(PlanilhasExportExcel.RELATORIO_VISAO_ALUNO);
		relatorioVisaoAlunoExportExcelCB.setValue(false);
		relatorioVisaoAlunoExportExcelCB.setFieldLabel("Relatório Visão Aluno");
		right.add(relatorioVisaoAlunoExportExcelCB, formData);
		
		atendimentosPorAlunoExportExcelCB = new CheckBox();
		atendimentosPorAlunoExportExcelCB.setName(PlanilhasExportExcel.ATENDIMENTOS_POR_ALUNO);
		atendimentosPorAlunoExportExcelCB.setValue(false);
		atendimentosPorAlunoExportExcelCB.setFieldLabel("Atendimentos por Sala");
		right.add(atendimentosPorAlunoExportExcelCB, formData);
		
		percentMestresDoutoresExportExcelCB = new CheckBox();
		percentMestresDoutoresExportExcelCB.setName(PlanilhasExportExcel.PERCENT_MESTRES_DOUTORES);
		percentMestresDoutoresExportExcelCB.setValue(false);
		percentMestresDoutoresExportExcelCB.setFieldLabel("Porcentagem de Mestres e Doutores");
		right.add(percentMestresDoutoresExportExcelCB, formData);
		
		aulasExportExcelCB = new CheckBox();
		aulasExportExcelCB.setName(PlanilhasExportExcel.AULAS);
		aulasExportExcelCB.setValue(false);
		aulasExportExcelCB.setFieldLabel("Aulas");
		right.add(aulasExportExcelCB, formData);
		
		nomeArquivoTF = new TextField<String>();
		nomeArquivoTF.setEmptyText("digite um nome para a planilha a ser exportada");
		nomeArquivoTF.setFieldLabel("Nome do Arquivo");
		nomeArquivoTF.setName("nomeArquivo");
		formPanel.add(nomeArquivoTF, new FormData("100%"));
		
		main.add(left, new ColumnData(.5));
	    main.add(right, new ColumnData(.5));
	  
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

}
