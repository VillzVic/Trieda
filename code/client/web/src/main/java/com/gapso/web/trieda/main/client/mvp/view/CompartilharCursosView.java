package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.ColumnData;
import com.extjs.gxt.ui.client.widget.layout.ColumnLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.CompartilharCursosPresenter;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class CompartilharCursosView extends MyComposite implements
		CompartilharCursosPresenter.Display {

	private SimpleModal simpleModal;

	private ContentPanel panel;

	private CursoComboBox curso1CB;
	private CursoComboBox curso2CB;
	private Grid<CursoDescompartilhaDTO> grid;
	private Button adicionarBt;
	private Button removerBt;

	private ParametroDTO parametro;
	private List<CursoDescompartilhaDTO> cursos;

	public CompartilharCursosView(ParametroDTO parametro,
			List<CursoDescompartilhaDTO> cursos) {
		this.parametro = parametro;
		this.cursos = cursos;
		initUI();
	}

	private void initUI() {
		simpleModal = new SimpleModal("Fechar", null,
				"Cursos não compartilham", Resources.DEFAULTS.curso16());
		simpleModal.setWidth(600);
		simpleModal.setHeight(400);
		createForm();
		simpleModal.setContent(panel);
	}

	private void createForm() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);

		ContentPanel formPanel = new ContentPanel(new ColumnLayout());
		// formPanel.setHeaderVisible(false);
		formPanel.setHeading("Cadastre os cursos que não compartilham");
		// formPanel.setBodyBorder(false);
		formPanel.setBodyStyle("background-color: transparent; padding: 10px;");

		FormData formData = new FormData("100%");

		LayoutContainer left = new LayoutContainer();
		LayoutContainer center = new LayoutContainer();
		LayoutContainer right = new LayoutContainer();

		FormLayout layoutLeft = new FormLayout();
		layoutLeft.setLabelAlign(LabelAlign.TOP);
		FormLayout layoutRight = new FormLayout();
		layoutRight.setLabelAlign(LabelAlign.TOP);

		left.setLayout(layoutLeft);
		right.setLayout(layoutRight);

		left.setStyleAttribute("text-align", "center");
		curso1CB = new CursoComboBox();
		curso1CB.setLabelSeparator("");
		left.add(curso1CB, formData);

		center.setStyleAttribute("line-height", "42px");
		center.setStyleAttribute("vertical-align", "middle");
		center.setStyleAttribute("text-align", "center");
		center.add(new Label("Não compartilha"));

		right.setStyleAttribute("text-align", "center");
		curso2CB = new CursoComboBox();
		curso2CB.setLabelSeparator("");
		right.add(curso2CB, formData);

		formPanel.add(left, new ColumnData(.4));
		formPanel.add(center, new ColumnData(.2));
		formPanel.add(right, new ColumnData(.4));

		adicionarBt = new Button("Adicionar");
		removerBt = new Button("Remover");
		formPanel.addButton(adicionarBt);
		formPanel.addButton(removerBt);

		panel.setTopComponent(formPanel);

		ListStore<CursoDescompartilhaDTO> store = new ListStore<CursoDescompartilhaDTO>();
		store.add(cursos);
		grid = new Grid<CursoDescompartilhaDTO>(store, new ColumnModel(
				getColumnList()));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		panel.setBodyBorder(false);
		panel.add(grid, bld);
	}

	private List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		ColumnConfig cc = new ColumnConfig(
				CursoDescompartilhaDTO.PROPERTY_CURSO1_DISPLAY, "Curso", 200);
		cc.setAlignment(HorizontalAlignment.RIGHT);
		list.add(cc);
		cc = new ColumnConfig("blank", "", 100);
		cc.setAlignment(HorizontalAlignment.CENTER);
		cc.setRenderer(new GridCellRenderer<CursoDescompartilhaDTO>() {
			@Override
			public Object render(CursoDescompartilhaDTO model, String property,
					com.extjs.gxt.ui.client.widget.grid.ColumnData config,
					int rowIndex, int colIndex,
					ListStore<CursoDescompartilhaDTO> store,
					Grid<CursoDescompartilhaDTO> grid) {
				if (colIndex == 1)
					return "Não compartilha";
				return null;
			}
		});
		list.add(cc);
		list.add(new ColumnConfig(
				CursoDescompartilhaDTO.PROPERTY_CURSO2_DISPLAY, "Curso", 200));
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
	public Button getRemoverBT() {
		return removerBt;
	}

	@Override
	public ParametroDTO getParametro() {
		return parametro;
	}

	@Override
	public List<CursoDescompartilhaDTO> getCursos() {
		return cursos;
	}
}
