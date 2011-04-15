package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.CurriculoFormPresenter;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class CurriculoFormView extends MyComposite implements CurriculoFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private CursoComboBox cursoCB;
	private CurriculoDTO curriculoDTO;
	private CursoDTO cursoDTO;
	
	public CurriculoFormView(CurriculoDTO curriculoDTO, CursoDTO cursoDTO) {
		this.curriculoDTO = curriculoDTO;
		this.cursoDTO = cursoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (curriculoDTO.getId() == null)? "Inserção de Matriz Curricular" : "Edição de Matriz Curricular";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.matrizCurricular16());
		simpleModal.setHeight(160);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		codigoTF = new TextField<String>();
		codigoTF.setName(CurriculoDTO.PROPERTY_CODIGO);
		codigoTF.setValue(curriculoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);
		
		cursoCB = new CursoComboBox();
		cursoCB.setName("curso");
		cursoCB.setFieldLabel("Curso");
		cursoCB.setAllowBlank(false);
		cursoCB.setValue(cursoDTO);
		cursoCB.setEmptyText("Selecione o curso");
		formPanel.add(cursoCB, formData);
		
		descricaoTF = new TextField<String>();
		descricaoTF.setName(CurriculoDTO.PROPERTY_DESCRICAO);
		descricaoTF.setValue(curriculoDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setAllowBlank(false);
		descricaoTF.setMinLength(5);
		descricaoTF.setMaxLength(20);
		descricaoTF.setEmptyText("Preencha um comentário");
		formPanel.add(descricaoTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(codigoTF);
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public TextField<String> getDescricaoTextField() {
		return descricaoTF;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}
	
	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoCB;
	}

	@Override
	public CurriculoDTO getCurriculoDTO() {
		return curriculoDTO;
	}
	

}
