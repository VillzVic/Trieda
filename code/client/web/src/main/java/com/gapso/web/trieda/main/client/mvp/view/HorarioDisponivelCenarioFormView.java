package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioDisponivelCenarioFormPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.SemanaLetivaGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.main.client.util.view.TextFieldMask;
import com.gapso.web.trieda.main.client.util.view.TurnoComboBox;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class HorarioDisponivelCenarioFormView extends MyComposite implements HorarioDisponivelCenarioFormPresenter.Display {

	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDTO semanaLetivaDTO;
	private SemanaLetivaGrid<HorarioDisponivelCenarioDTO> gridPanel;
	private TurnoComboBox turnoCB;
	private TextField<String> horarioInicioTF;
	private Button adicionarHorarioBT;
	private Button removerHorarioBT;
	
	public HorarioDisponivelCenarioFormView(SemanaLetivaDTO semanaLetivaDTO) {
		this.semanaLetivaDTO = semanaLetivaDTO;
		initUI();
		createGrid();
		// TODO
//		initComponent(simpleModal);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Dias e Horários de Aula", Resources.DEFAULTS.semanaLetiva16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(600);
	}

	private void createGrid() {
		gridPanel = new SemanaLetivaGrid<HorarioDisponivelCenarioDTO>();
		gridPanel.setBodyStyle("background-color: #FFFFFF;");
		
		FormPanel formPanel = new FormPanel();
		formPanel.setLayout(new HBoxLayout());
		formPanel.setHeaderVisible(true);
		formPanel.setHeading("Adicionar/Remover Horário de Aula");
		formPanel.setButtonAlign(HorizontalAlignment.RIGHT);
		
		turnoCB = new TurnoComboBox();
		turnoCB.setName("turno");
		turnoCB.setFieldLabel("Turno");
		turnoCB.setAllowBlank(false);
		turnoCB.setEmptyText("Selecione o turno");
		
		horarioInicioTF = new TextFieldMask("99:99");
		horarioInicioTF.setRegex("([0-1][0-9]|2[0-4]):([0-5][0-9])$");
		horarioInicioTF.setName("horarioInicio");
		horarioInicioTF.setEmptyText("Horário de início de aula");
		
		formPanel.add(new Label("Turno:"), new HBoxLayoutData(new Margins(5,0,0,0)));
		formPanel.add(turnoCB, new HBoxLayoutData(new Margins(0,0,0,10)));
		formPanel.add(new Label("Horário:"), new HBoxLayoutData(new Margins(5,0,0,10)));
		formPanel.add(horarioInicioTF, new HBoxLayoutData(new Margins(0,0,0,5)));
		adicionarHorarioBT = new Button("Adicionar", AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		removerHorarioBT = new Button("Remover", AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()));
		formPanel.add(adicionarHorarioBT, new HBoxLayoutData(new Margins(0,0,0,10)));
		formPanel.add(removerHorarioBT, new HBoxLayoutData(new Margins(0,0,0,10)));
		
		contentPanel = new ContentPanel();
		contentPanel.setBodyStyle("background-color: #CED9E7;");
		VBoxLayout layout = new VBoxLayout();
		layout.setVBoxLayoutAlign(VBoxLayoutAlign.STRETCH);
		contentPanel.setLayout(layout);
		contentPanel.setHeaderVisible(false);
		contentPanel.setBodyBorder(false);
		contentPanel.add(formPanel, new VBoxLayoutData(new Margins(0, 0, 5, 0)));
		VBoxLayoutData flex = new VBoxLayoutData(new Margins(0));
		flex.setFlex(1);
		contentPanel.add(gridPanel, flex);
	    	    
		simpleModal.setContent(contentPanel);
	}

	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public SemanaLetivaDTO getSemanaLetivaDTO() {
		return semanaLetivaDTO;
	}

	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public ListStore<HorarioDisponivelCenarioDTO> getStore() {
		return gridPanel.getStore();
	}

	@Override
	public TurnoComboBox getTurnoCB() {
		return turnoCB;
	}

	@Override
	public TextField<String> getHorarioInicioTF() {
		return horarioInicioTF;
	}

	@Override
	public Button getAdicionarHorarioBT() {
		return adicionarHorarioBT;
	}

	@Override
	public Button getRemoverHorarioBT() {
		return removerHorarioBT;
	}

}
