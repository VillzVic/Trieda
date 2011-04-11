package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.presenter.CompartilharCursosPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;

public class CompartilharCursosView extends MyComposite implements CompartilharCursosPresenter.Display {

	private SimpleModal simpleModal;
	
	private ContentPanel panel;
	
	private CursoComboBox curso1CB;
	private CursoComboBox curso2CB;
	private Grid<CursoDescompartilhaDTO> grid;
	private Button adicionarBt;
	
	private ParametroDTO parametro;
	private List<CursoDescompartilhaDTO> cursos;
	
	public CompartilharCursosView(ParametroDTO parametro, List<CursoDescompartilhaDTO> cursos) {
		this.parametro = parametro;
		this.cursos = cursos;
		initUI();
		// TODO
		// initComponent(simpleModal);
	}

	private void initUI() {
		simpleModal = new SimpleModal("Fechar", null, "Cursos não compartilham", Resources.DEFAULTS.curso16());
		simpleModal.setWidth(600);
		simpleModal.setHeight(400);
		createForm();
		simpleModal.setContent(panel);
	}
	
	private void createForm() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		formPanel.setAutoHeight(true);
		
		curso1CB = new CursoComboBox();
		formPanel.add(curso1CB, formData);
		curso2CB = new CursoComboBox();
		formPanel.add(curso2CB, formData);
		
		adicionarBt = new Button("Adicionar");
		formPanel.addButton(adicionarBt);
		
		panel.setTopComponent(formPanel);
		
		ListStore<CursoDescompartilhaDTO> store = new ListStore<CursoDescompartilhaDTO>();
		store.add(cursos);
		grid = new Grid<CursoDescompartilhaDTO>(store, new ColumnModel(getColumnList()));
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		panel.setBodyBorder(false);
		panel.add(grid, bld);
	}
	
	private List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		ColumnConfig cc = new ColumnConfig(CursoDescompartilhaDTO.PROPERTY_CURSO1_DISPLAY, "Curso", 200); 
		cc.setAlignment(HorizontalAlignment.RIGHT);
		list.add(cc);
		cc = new ColumnConfig("blank", "", 100);
		cc.setAlignment(HorizontalAlignment.CENTER);
		cc.setRenderer(new GridCellRenderer<CursoDescompartilhaDTO>() {
			@Override
			public Object render(CursoDescompartilhaDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore<CursoDescompartilhaDTO> store,
					Grid<CursoDescompartilhaDTO> grid) {
				if(colIndex == 1) return "Não compartilha";
				return null;
			}
		});
		list.add(cc);
		list.add(new ColumnConfig(CursoDescompartilhaDTO.PROPERTY_CURSO2_DISPLAY, "Curso", 200));
		return list;
	}
	
	@Override
	public CursoComboBox getCurso1ComboBox() {
		return curso1CB;
	}
	
	@Override
	public CursoComboBox getCurso2ComboBox() {
		return curso2CB;
	}

	@Override
	public Grid<CursoDescompartilhaDTO> getGrid() {
		return grid;
	}

	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public Button getFecharBT() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public Button getAdicionarBT() {
		return adicionarBt;
	}
	
	@Override
	public ParametroDTO getParametro() {
		return parametro;
	}
}
