package com.gapso.web.trieda.shared.mvp.view;

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
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ProfessorCpfComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorNomeComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;


public class RelatorioVisaoProfessorView extends RelatorioVisaoView	implements RelatorioVisaoProfessorPresenter.Display{
	private ProfessorVirtualComboBox professorVirtualCB;
	private ProfessorNomeComboBox professorTF;
	private ProfessorCpfComboBox cpfTF;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor;
	private RelatorioVisaoProfessorFiltro filtro;
	private Button proxProfessorBt;
	private Button antProfessorBt;
	private Button proxCpfBt;
	private Button antCpfBt;
	private Button proxProfessorVirtualBt;
	private Button antProfessorVirtualBt;

	public RelatorioVisaoProfessorView(CenarioDTO cenarioDTO, UsuarioDTO usuario, boolean isVisaoProfessor){
		this.usuario = usuario;
		this.isVisaoProfessor = isVisaoProfessor;
		this.cenarioDTO = cenarioDTO;
		
		this.setFiltro(new RelatorioVisaoProfessorFiltro());
		this.initUI();
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}
	
	@Override
	protected String getHeadingPanel(){
		return cenarioDTO.getNome() + " » Grade Horária Visão Professor";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor16());
	}
	
	@Override
	protected GradeHorariaProfessorGrid createGradeHorariaVisao(){
		return new GradeHorariaProfessorGrid(cenarioDTO, isVisaoProfessor);
	}

	protected void createFilter(){
		this.professorTF = new ProfessorNomeComboBox(cenarioDTO);
		this.professorTF.setEmptyText("Digite o nome do professor");
		this.professorTF.setName("professor");
		this.professorTF.setFieldLabel("Professor");
		this.professorTF.setWidth(250);
		filtro.addProfessorNomeValueListener(professorTF);
		proxProfessorBt = new Button();
		proxProfessorBt.disable();
		antProfessorBt = new Button();
		antProfessorBt.setStyleAttribute("margin-left", "10px");
		antProfessorBt.disable();
		proxProfessorBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antProfessorBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		
		this.cpfTF = new ProfessorCpfComboBox(cenarioDTO);
		this.cpfTF.setEmptyText("Digite o cpf do professor");
		this.cpfTF.setName("cpf");
		this.cpfTF.setFieldLabel("CPF");
		this.cpfTF.setWidth(250);
		filtro.addProfessorCpfValueListener(cpfTF);
		proxCpfBt = new Button();
		proxCpfBt.disable();
		antCpfBt = new Button();
		antCpfBt.setStyleAttribute("margin-left", "10px");
		antCpfBt.disable();
		proxCpfBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antCpfBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		
		this.professorVirtualCB = new ProfessorVirtualComboBox(cenarioDTO);
		filtro.addProfessorVirtualValueListener(this.professorVirtualCB);
		this.professorVirtualCB.setWidth(210);
		proxProfessorVirtualBt = new Button();
		proxProfessorVirtualBt.disable();
		antProfessorVirtualBt = new Button();
		antProfessorVirtualBt.setStyleAttribute("margin-left", "10px");
		antProfessorVirtualBt.disable();
		proxProfessorVirtualBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antProfessorVirtualBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		
		panel.setHeaderVisible(true);
		panel.setHeadingHtml("Filtro");

		final LayoutContainer main = new LayoutContainer(new ColumnLayout());
		LayoutContainer left = new LayoutContainer(new ColumnLayout());
		LayoutContainer right = new LayoutContainer(new ColumnLayout());
		LayoutContainer center = new LayoutContainer(new ColumnLayout());
		LayoutContainer submit = new LayoutContainer(new FormLayout());
		LayoutContainer export = new LayoutContainer(new FormLayout());
		
		LabelField labelProfessor = new LabelField();
		labelProfessor.setValue("Professor:");
		labelProfessor.setStyleAttribute("margin-top", "3px");
		LabelField labelCpf = new LabelField();
		labelCpf.setValue("Cpf:");
		labelCpf.setStyleAttribute("margin-top", "3px");
		LabelField labelProfessorVirtual = new LabelField();
		labelProfessorVirtual.setValue("Professor Virtual:");
		labelProfessorVirtual.setStyleAttribute("margin-top", "3px");
		
		left.add(labelProfessor);
		left.add(antProfessorBt, formData);
		left.add(professorTF, formData);
		left.add(proxProfessorBt, formData);
		center.add(labelCpf);
		center.add(antCpfBt, formData);
		center.add(cpfTF, formData);
		center.add(proxCpfBt, formData);
		right.add(labelProfessorVirtual);
		right.add(antProfessorVirtualBt, formData);
		right.add(professorVirtualCB, formData);
		right.add(proxProfessorVirtualBt, formData);
		
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
		main.add(center, new ColumnData(0.29));
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
	public GradeHorariaProfessorGrid getGrid(){
		return (GradeHorariaProfessorGrid) this.grid;
	}
	
	@Override
	public ProfessorNomeComboBox getProfessorTextField(){
		return this.professorTF;
	}
	
	@Override
	public ProfessorCpfComboBox getCpfTextField(){
		return this.cpfTF;
	}

	@Override
	public RelatorioVisaoProfessorFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoProfessorFiltro) filtro;
	}

	@Override
	public ProfessorVirtualComboBox getProfessorVirtualComboBox(){
		return this.professorVirtualCB;
	}
	
	@Override
	public Button getProxProfessorButton()
	{
		return proxProfessorBt;
	}
	
	@Override
	public Button getAntProfessorButton()
	{
		return antProfessorBt;
	}
	
	@Override
	public Button getProxCpfButton()
	{
		return proxCpfBt;
	}
	
	@Override
	public Button getAntCpfButton()
	{
		return antCpfBt;
	}
	
	@Override
	public Button getProxProfessorVirtualButton()
	{
		return proxProfessorVirtualBt;
	}
	
	@Override
	public Button getAntProfessorVirtualButton()
	{
		return antProfessorVirtualBt;
	}

	@Override
	public void disableView() {
		getProfessorTextField().disable();
		getCpfTextField().disable();
		getProfessorVirtualComboBox().disable();
		getExportXlsExcelButton().disable();
		getExportXlsxExcelButton().disable();
		getSubmitBuscaButton().disable();
		getExportExcelButton().disable();
		
	}

	@Override
	public void checkRegistered() {
		if(!cenarioDTO.hasProfessores()){
			disableView();
		}
	}

}
