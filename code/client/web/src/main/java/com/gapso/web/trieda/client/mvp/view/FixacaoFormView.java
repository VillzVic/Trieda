package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.presenter.FixacaoFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SalaComboBox;
import com.gapso.web.trieda.client.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;

public class FixacaoFormView extends MyComposite implements FixacaoFormPresenter.Display {

	private SimpleModal simpleModal;
	private LayoutContainer container;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private DisciplinaComboBox disciplinaCB;
	private CampusComboBox campusCB;
	private UnidadeComboBox unidadeCB;
	private SalaComboBox salaCB;
	private SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> grid;
	private boolean selectDefault;
	
	private FixacaoDTO fixacaoDTO;
	private DisciplinaDTO disciplinaDTO;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private SalaDTO salaDTO;
	private List<HorarioDisponivelCenarioDTO> listHorarios;
	
	public FixacaoFormView(FixacaoDTO fixacaoDTO, DisciplinaDTO disciplinaDTO, CampusDTO campusDTO, UnidadeDTO unidadeDTO, SalaDTO salaDTO, List<HorarioDisponivelCenarioDTO> listHorarios, Boolean selectDefault) {
		this.fixacaoDTO = fixacaoDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;
		this.salaDTO = salaDTO;
		this.listHorarios = (listHorarios == null)? new ArrayList<HorarioDisponivelCenarioDTO>() : listHorarios;
		this.selectDefault = selectDefault;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (fixacaoDTO.getId() == null)? "Inserção de Fixação" : "Edição de Fixação";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.fixacao16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(617);
		createForm();
		simpleModal.setContent(container);
	}

	private void createForm() {
		container = new LayoutContainer();
		VBoxLayout layout = new VBoxLayout();  
		layout.setPadding(new Padding(5));  
		layout.setVBoxLayoutAlign(VBoxLayoutAlign.STRETCH);  
		container.setLayout(layout);  
		
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		codigoTF = new TextField<String>();
		codigoTF.setName(FixacaoDTO.PROPERTY_CODIGO);
		codigoTF.setValue(fixacaoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);
		
		descricaoTF = new TextField<String>();
		descricaoTF.setName(FixacaoDTO.PROPERTY_DESCRICAO);
		descricaoTF.setValue(fixacaoDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setAllowBlank(false);
		descricaoTF.setMinLength(1);
		descricaoTF.setMaxLength(50);
		descricaoTF.setEmptyText("Preencha a descrição");
		formPanel.add(descricaoTF, formData);
		
		disciplinaCB = new DisciplinaComboBox();
		disciplinaCB.setValue(disciplinaDTO);
		formPanel.add(disciplinaCB, formData);
		
		campusCB = new CampusComboBox();
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);
		
		unidadeCB = new UnidadeComboBox(campusCB);
		unidadeCB.setValue(unidadeDTO);
		formPanel.add(unidadeCB, formData);
		
		salaCB = new SalaComboBox(unidadeCB);
		salaCB.setValue(salaDTO);
		formPanel.add(salaCB, formData);
		
		grid = new SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO>(listHorarios,HorarioDisponivelCenarioDTO.PROPERTY_ID);
		grid.setSelectDefault(selectDefault);
		
		container.add(formPanel, new VBoxLayoutData(new Margins(0, 0, 5, 0)));
		VBoxLayoutData flex = new VBoxLayoutData(new Margins(0));  
		flex.setFlex(1);  
		container.add(grid, flex);
		
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
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}
	
	@Override
	public FixacaoDTO getFixacaoDTO() {
		return fixacaoDTO;
	}

	@Override
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}
	
	@Override
	public TextField<String> getDescricaoTextField() {
		return descricaoTF;
	}
	
	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}
	
	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public UnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}
	
	@Override
	public SalaComboBox getSalaComboBox() {
		return salaCB;
	}

	@Override
	public SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> getGrid() {
		return grid;
	}

}
