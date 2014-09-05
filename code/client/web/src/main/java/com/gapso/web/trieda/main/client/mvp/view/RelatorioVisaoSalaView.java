package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.ButtonArrowAlign;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.LabelField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.menu.Menu;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoSalaPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SalaAutoCompleteBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class RelatorioVisaoSalaView	extends RelatorioVisaoView	implements RelatorioVisaoSalaPresenter.Display{
	private SalaAutoCompleteBox salaTF;
	private RelatorioVisaoSalaFiltro filtro;
	private Button proxAmbienteBt;
	private Button antAmbienteBt;

	public RelatorioVisaoSalaView(CenarioDTO cenarioDTO){
		super(cenarioDTO, new RelatorioVisaoSalaFiltro());
	}

	@Override
	protected String getHeadingPanel(){
		return cenarioDTO.getNome() + " » Grade Horária Visão Ambiente";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Ambiente", Resources.DEFAULTS.saidaSala16());
	}
	
	@Override
	protected GradeHorariaSalaGrid createGradeHorariaVisao(){
		return new GradeHorariaSalaGrid(cenarioDTO);
	}
	
	@Override
	protected void createFilter(){
		this.salaTF = new SalaAutoCompleteBox(cenarioDTO);
		this.salaTF.setEmptyText("Digite o código do ambiente");
		this.salaTF.setName("salaTF");
		this.salaTF.setFieldLabel("Código do Ambiente");
		this.salaTF.setWidth(200);
		filtro.addSalaValueListener(salaTF);
		proxAmbienteBt = new Button();
		proxAmbienteBt.disable();
		antAmbienteBt = new Button();
		antAmbienteBt.setStyleAttribute("margin-left", "10px");
		antAmbienteBt.disable();
		proxAmbienteBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.prox16()));
		antAmbienteBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ant16()));
		
		FormData formData = new FormData("100%");
		FormPanel panel = new FormPanel();
		
		panel.setHeaderVisible(true);
		panel.setHeadingHtml("Filtro");

		final LayoutContainer main = new LayoutContainer(new ColumnLayout());
		FormLayout layout = new FormLayout(LabelAlign.LEFT);
		layout.setHideLabels(true);
		LayoutContainer left = new LayoutContainer(new ColumnLayout());
		LayoutContainer submit = new LayoutContainer(new FormLayout());
		LayoutContainer export = new LayoutContainer(new FormLayout());
		
		LabelField labelSala = new LabelField();
		labelSala.setValue("Código do Ambiente:");
		labelSala.setStyleAttribute("margin-top", "3px");
		
		left.add(labelSala);
		left.add(antAmbienteBt, formData);
		left.add(salaTF, formData);
		left.add(proxAmbienteBt, formData);
		
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
		main.add(submit, new ColumnData(0.05));
		main.add(export, new ColumnData(0.09));

		panel.add(main, new FormData("100%"));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);

		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
//		panel.setAutoHeight(true);
		
		bld.setSize(48 +  26);

		this.panel.add(panel, bld);
	}

	@Override
	public GradeHorariaSalaGrid getGrid(){
		return (GradeHorariaSalaGrid) this.grid;
	}
	
	@Override
	public RelatorioVisaoSalaFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoSalaFiltro) filtro;
	}
	
	@Override
	public SalaAutoCompleteBox getSalaTextField(){
		return this.salaTF;
	}
	
	@Override
	public Button getProxAmbienteButton()
	{
		return proxAmbienteBt;
	}
	
	@Override
	public Button getAntAmbienteButton()
	{
		return antAmbienteBt;
	}

	@Override
	public void disableView() {
		getSalaTextField().disable();
		getExportXlsExcelButton().disable();
		getExportXlsxExcelButton().disable();
		getSubmitBuscaButton().disable();
		getExportExcelButton().disable();
	}

	@Override
	public void checkRegistered() {
		if(!cenarioDTO.hasSalas()){
			disableView();
		}
	}

}
