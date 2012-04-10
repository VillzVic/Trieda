package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.main.client.command.ICommand;
import com.gapso.web.trieda.main.client.command.util.CommandFactory;
import com.gapso.web.trieda.main.client.command.util.CommandSelectionListener;
import com.gapso.web.trieda.main.client.mvp.view.gateways.ICarregarSolucaoViewGateway;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class CarregarSolucaoView extends MyComposite implements ICarregarSolucaoViewGateway {
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private NumberField roundNF;
	private CenarioDTO cenarioDTO;

	public CarregarSolucaoView(CenarioDTO cenarioDTO) {
		this.cenarioDTO = cenarioDTO;
		initUI();
		initActions();
	}

	private void initUI() {
		this.simpleModal = new SimpleModal("Carregar Solução",Resources.DEFAULTS.trieda16());

		createForm();
		this.simpleModal.setWidth(320);
		this.simpleModal.setHeight(115);
		this.simpleModal.setContent(formPanel);
	}

	private void createForm() {
        this.formPanel = new FormPanel();
        this.formPanel.setLabelWidth(50);
        this.formPanel.setFrame(true);
        this.formPanel.setHeaderVisible(false);
        this.formPanel.setButtonAlign(HorizontalAlignment.CENTER);
        
        this.roundNF = new NumberField();
		this.roundNF.setName("round");
		this.roundNF.setFieldLabel("Round");
		this.roundNF.setAllowBlank(false);

        this.formPanel.add(this.roundNF);
		this.simpleModal.setFocusWidget(this.roundNF);

		FormButtonBinding binding = new FormButtonBinding(this.formPanel);
		binding.addButton(this.simpleModal.getSalvarBt());
	}

	private void initActions() {
		ICommand cmd = CommandFactory.createCarregarSolucaoCommand(cenarioDTO,this,this); 
		this.simpleModal.getSalvarBt().addSelectionListener(CommandSelectionListener.<ButtonEvent>create(cmd));
	}
	
	public void show() {
		this.simpleModal.show();
	}

	@Override
	public Long getRound() {
		Number round = roundNF.getValue();
		return (round != null) ? round.longValue() : null;
	}
}