package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.presenter.CurriculoDisciplinaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;

public class CurriculoDisciplinaFormView extends MyComposite implements CurriculoDisciplinaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private DisciplinaComboBox disciplinaCB;
	private NumberField periodoTF;
	private CurriculoDisciplinaDTO curriculoDisciplinaDTO;
	private CurriculoDTO curriculoDTO;

	public CurriculoDisciplinaFormView(CurriculoDisciplinaDTO curriculoDisciplinaDTO, CurriculoDTO curriculoDTO) {
		this.curriculoDisciplinaDTO = curriculoDisciplinaDTO;
		this.curriculoDTO = curriculoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = "Inserção de Disciplina no Curriculo";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.disciplina16());
		simpleModal.setHeight(400);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		TextField<String> curriculoTF = new TextField<String>();
		curriculoTF.setValue(curriculoDTO.getCodigo());
		curriculoTF.setFieldLabel("Matriz Curriculo");
		curriculoTF.setReadOnly(true);
		formPanel.add(curriculoTF, formData);
		
		TextField<String> cursoTF = new TextField<String>();
		cursoTF.setValue(curriculoDTO.getCursoString());
		cursoTF.setFieldLabel("Curso");
		cursoTF.setReadOnly(true);
		formPanel.add(cursoTF, formData);
		
		disciplinaCB = new DisciplinaComboBox();
		disciplinaCB.setName("disciplina");
		disciplinaCB.setFieldLabel("Disciplina");
		disciplinaCB.setAllowBlank(false);
		disciplinaCB.setEmptyText("Preencha a disciplina");
		formPanel.add(disciplinaCB, formData);
		
		periodoTF = new NumberField();
		periodoTF.setName(CurriculoDisciplinaDTO.PROPERTY_PERIODO);
		periodoTF.setValue(curriculoDisciplinaDTO.getPeriodo());
		periodoTF.setFieldLabel("Período");
		periodoTF.setAllowBlank(false);
		periodoTF.setAllowDecimals(false);
		periodoTF.setMaxValue(99);
		periodoTF.setEmptyText("Preencha o período");
		formPanel.add(periodoTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(curriculoTF);
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}

	@Override
	public NumberField getPeriodoTextField() {
		return periodoTF;
	}

	@Override
	public CurriculoDTO getCurriculoDTO() {
		return curriculoDTO;
	}

	@Override
	public CurriculoDisciplinaDTO getCurriculoDisciplinaDTO() {
		return curriculoDisciplinaDTO;
	}

}
