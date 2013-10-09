package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.DisciplinaPreRequisitoFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.PeriodoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class DisciplinaPreRequisitoFormView
	extends MyComposite
	implements DisciplinaPreRequisitoFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CurriculoComboBox curriculoCB;
	private PeriodoComboBox periodoCB;
	private DisciplinaComboBox disciplinaCB;
	private DisciplinaAutoCompleteBox disciplinaPreRequisitoCB;
	private CenarioDTO cenarioDTO;

	public DisciplinaPreRequisitoFormView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI() {
		String title = "Inserção de Disciplina Pré-Requisito";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.disciplina16());
		simpleModal.setHeight(210);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		curriculoCB = new CurriculoComboBox(cenarioDTO);
		curriculoCB.setName(DisciplinaDTO.PROPERTY_USA_DOMINGO);
		curriculoCB.setFieldLabel("Matriz Curricular");
		curriculoCB.setAllowBlank(false);
		formPanel.add(curriculoCB, formData);
		
		periodoCB = new PeriodoComboBox(curriculoCB, this);
		periodoCB.setAllowBlank(false);
		formPanel.add(periodoCB, formData);
		
		disciplinaCB = new DisciplinaComboBox(cenarioDTO, curriculoCB, periodoCB);
		disciplinaCB.setAllowBlank(false);
		formPanel.add(disciplinaCB, formData);
		
		disciplinaPreRequisitoCB = new DisciplinaAutoCompleteBox( cenarioDTO );
		disciplinaPreRequisitoCB.setFieldLabel("Disciplina Pré-Requisito");
		disciplinaPreRequisitoCB.setAllowBlank(false);
		formPanel.add(disciplinaPreRequisitoCB, formData);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(curriculoCB);
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
	public CurriculoComboBox getCurriculoComboBox() {
		return curriculoCB;
	}
	
	@Override
	public PeriodoComboBox getPeriodoComboBox() {
		return periodoCB;
	}
	
	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}
	
	@Override
	public DisciplinaAutoCompleteBox getDisciplinaPreRequisitoComboBox() {
		return disciplinaPreRequisitoCB;
	}
}