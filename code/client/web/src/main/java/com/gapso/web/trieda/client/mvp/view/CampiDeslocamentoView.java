package com.gapso.web.trieda.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoCampusDTO;
import com.gapso.web.trieda.client.mvp.presenter.CampiDeslocamentoPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.client.util.view.DeslocamentoToolBar;
import com.gapso.web.trieda.client.util.view.EstadoComboBox;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;

public class CampiDeslocamentoView extends MyComposite implements CampiDeslocamentoPresenter.Display {

	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private TextField<String> codigoBuscaTextField;
	private EstadoComboBox estadoBuscaComboBox;
	private TextField<String> municipioBuscaTextField;
	private TextField<String> bairroBuscaTextField;
	private DeslocamentoGrid<DeslocamentoCampusDTO> deslocamentoGrid;
	private DeslocamentoToolBar deslocamentoToolBar;
	private List<DeslocamentoCampusDTO> deslocamentoCampusDTOList;
	
	private ContentPanel panel;
	private GTabItem tabItem;

	public CampiDeslocamentoView(List<DeslocamentoCampusDTO> deslocamentoCampusDTOList) {
		this.deslocamentoCampusDTOList = deslocamentoCampusDTOList;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Deslocamentos de Campi");
		createFilter();
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Deslocamentos de Campi", Resources.DEFAULTS.deslocamento16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		deslocamentoToolBar = new DeslocamentoToolBar();
		panel.setTopComponent(deslocamentoToolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    deslocamentoGrid = new DeslocamentoGrid<DeslocamentoCampusDTO>(deslocamentoCampusDTOList);
	    deslocamentoGrid.setContainsCusto(true);
	    panel.add(deslocamentoGrid, bld);
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(0, 0, 0, 5));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter(true);
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel("Nome");
		codigoBuscaTextField = new TextField<String>();
		codigoBuscaTextField.setFieldLabel("Código");
		estadoBuscaComboBox = new EstadoComboBox();
		estadoBuscaComboBox.setFieldLabel("Estado");
		municipioBuscaTextField = new TextField<String>();
		municipioBuscaTextField.setFieldLabel("Município");
		bairroBuscaTextField = new TextField<String>();
		bairroBuscaTextField.setFieldLabel("Bairro");
		filter.addField(nomeBuscaTextField);
		filter.addField(codigoBuscaTextField); 
//		TODO filter.addField(estadoBuscaComboBox); 
		filter.addField(municipioBuscaTextField); 
		filter.addField(bairroBuscaTextField); 
		
		panel.add(filter, bld);
	}

	@Override
	public Button getSaveButton() {
		return deslocamentoToolBar.getSaveButton();
	}

	@Override
	public Button getCancelButton() {
		return deslocamentoToolBar.getCancelButton();
	}

	@Override
	public Button getSimetricaButton() {
		return deslocamentoToolBar.getSimetriaButton();
	}
	
	@Override
	public Button getImportExcelButton() {
		return deslocamentoToolBar.getImportExcelButton();
	}
	
	@Override
	public Button getExportExcelButton() {
		return deslocamentoToolBar.getExportExcelButton();
	}

	@Override
	public DeslocamentoGrid<DeslocamentoCampusDTO> getGrid() {
		return deslocamentoGrid;
	}

	@Override
	public TextField<String> getCodigoBuscaTextField() {
		return codigoBuscaTextField;
	}

	@Override
	public TextField<String> getNomeBuscaTextField() {
		return nomeBuscaTextField;
	}

	@Override
	public EstadoComboBox getEstadoBuscaComboBox() {
		return estadoBuscaComboBox;
	}

	@Override
	public TextField<String> getMunicipioBuscaTextField() {
		return municipioBuscaTextField;
	}

	@Override
	public TextField<String> getBairroBuscaTextField() {
		return bairroBuscaTextField;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

}
