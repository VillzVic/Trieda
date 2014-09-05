package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoAlunoPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaAlunoGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.AlunosMatriculaComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoAlunoView extends RelatorioVisaoView implements RelatorioVisaoAlunoPresenter.Display{
	private AlunosComboBox alunoTF;
	private AlunosMatriculaComboBox matriculaTF;
	private RelatorioVisaoAlunoFiltro filtro;
	private Button proxAlunoBt;
	private Button antAlunoBt;
	private Button proxMatriculaBt;
	private Button antMatriculaBt;

	public RelatorioVisaoAlunoView(CenarioDTO cenarioDTO){
		super(cenarioDTO, new RelatorioVisaoAlunoFiltro());
	}
	
	@Override
	protected String getHeadingPanel(){
		return cenarioDTO.getNome() + " » Grade Horária Visão Aluno";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Aluno", Resources.DEFAULTS.saidaProfessor16());
	}
	
	@Override
	protected GradeHorariaAlunoGrid createGradeHorariaVisao(){
		return new GradeHorariaAlunoGrid(cenarioDTO);
	}

	@Override
	protected void createFilter(){
		this.alunoTF = new AlunosComboBox(cenarioDTO);
		this.alunoTF.setEmptyText("Digite o nome do aluno");
		this.alunoTF.setName("aluno");
		this.alunoTF.setFieldLabel("Aluno");
		this.alunoTF.setWidth(250);
		filtro.addAlunoNomeValueListener(alunoTF);
		proxAlunoBt = new Button();
		proxAlunoBt.disable();
		antAlunoBt = new Button();
		antAlunoBt.setStyleAttribute("margin-left", "10px");
		antAlunoBt.disable();
		proxAlunoBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antAlunoBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		
		
		this.matriculaTF = new AlunosMatriculaComboBox(cenarioDTO);
		this.matriculaTF.setEmptyText("Digite o número da matricula");
		this.matriculaTF.setName("matricula");
		this.matriculaTF.setFieldLabel("Matricula");
		this.matriculaTF.setWidth(250);
		filtro.addAlunoMatriculaValueListener(matriculaTF);
		proxMatriculaBt = new Button();
		proxMatriculaBt.disable();
		antMatriculaBt = new Button();
		antMatriculaBt.setStyleAttribute("margin-left", "10px");
		antMatriculaBt.disable();
		proxMatriculaBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antMatriculaBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		
		panel.setHeaderVisible(true);
		panel.setHeadingHtml("Filtro");
		
		final LayoutContainer main = new LayoutContainer(new ColumnLayout());
		LayoutContainer left = new LayoutContainer(new ColumnLayout());
		LayoutContainer right = new LayoutContainer(new ColumnLayout());
		LayoutContainer submit = new LayoutContainer(new FormLayout());
		LayoutContainer export = new LayoutContainer(new FormLayout());
		
		LabelField labelAluno = new LabelField();
		labelAluno.setValue("Aluno:");
		labelAluno.setStyleAttribute("margin-top", "3px");
		LabelField labelMatricula = new LabelField();
		labelMatricula.setValue("Matricula:");
		labelMatricula.setStyleAttribute("margin-top", "3px");
		
		left.add(labelAluno);
		left.add(antAlunoBt, formData);
		left.add(alunoTF, formData);
		left.add(proxAlunoBt, formData);
		right.add(labelMatricula);
		right.add(antMatriculaBt, formData);
		right.add(matriculaTF, formData);
		right.add(proxMatriculaBt, formData);
		
		this.submitBt = new Button("Filtrar", AbstractImagePrototype.create(Resources.DEFAULTS.filter16()));
		submit.add(submitBt, new HBoxLayoutData(new Margins(0, 0, 0, 5)));
		
		this.exportExcelBt = new Button("ExportarExcel", AbstractImagePrototype.create(Resources.DEFAULTS.exportar16()));
		this.exportExcelBt.setArrowAlign(ButtonArrowAlign.RIGHT);
		Menu menu = new Menu();
		menu.add(new MenuItem("Exportar como xls"));
		menu.add(new MenuItem("Exportar como xlsx"));
		this.exportExcelBt.setMenu(menu);
		export.add(this.exportExcelBt, new HBoxLayoutData(new Margins(0, 0, 0, 5)));
		
		main.add(left, new ColumnData(0.29));
		main.add(right, new ColumnData(0.28));
		main.add(submit, new ColumnData(0.05));
		main.add(export, new ColumnData(0.09));

		panel.add(main, new FormData("100%"));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);

		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
//		panel.setAutoHeight(true);
		
		bld.setSize(48 + 26);

		this.panel.add(panel, bld);
	}

	@Override
	public GradeHorariaAlunoGrid getGrid(){
		return (GradeHorariaAlunoGrid) this.grid;
	}

	@Override
	public RelatorioVisaoAlunoFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoAlunoFiltro) filtro;
	}
	
	@Override
	public AlunosComboBox getAlunoTextField(){
		return this.alunoTF;
	}
	
	@Override
	public AlunosMatriculaComboBox getAlunoMatriculaTextField(){
		return this.matriculaTF;
	}
	
	@Override
	public Button getProxAlunoButton()
	{
		return proxAlunoBt;
	}
	
	@Override
	public Button getAntAlunoButton()
	{
		return antAlunoBt;
	}

	@Override
	public Button getProxMatriculaButton()
	{
		return proxMatriculaBt;
	}
	
	@Override
	public Button getAntMatriculaButton()
	{
		return antMatriculaBt;
	}
	
	@Override
	public void disableView() {
		getAlunoMatriculaTextField().disable();
		getAlunoTextField().disable();
		getExportXlsExcelButton().disable();
		getExportXlsxExcelButton().disable();
		getSubmitBuscaButton().disable();
		getExportExcelButton().disable();
	}
	
	@Override
	public void checkRegistered() {
		if(!cenarioDTO.hasAlunos()){
			disableView();
		}
	}

	
}
