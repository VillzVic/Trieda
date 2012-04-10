package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.IconButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class ErrorsWarningsInputSolverPresenter extends AbstractRequisicaoOtimizacaoPresenter {
	
	public interface Display extends ITriedaI18nGateway {
		ContentPanel getMessagesWarningPanel();
		ContentPanel getMessagesErrorPanel();
		SimpleModal getSimpleModal();
		Button getSubmitButton();
		Button getCancelButton();
		ToolButton getCloseButton();
	}

	private CenarioDTO cenarioDTO;
	private ParametroDTO parametroDTO;
	private List<String> warnings;
	private List<String> errors;
	private Display display;
	private Button submitButtonParametros;

	public ErrorsWarningsInputSolverPresenter(CenarioDTO cenarioDTO, ParametroDTO parametroDTO, List<String> errors, List<String> warnings, Display display, Button submitButtonParametros) {
		super(display);
		this.cenarioDTO = cenarioDTO;
		this.parametroDTO = parametroDTO;
		this.display = display;
		this.errors = errors;
		this.warnings = warnings;
		this.submitButtonParametros = submitButtonParametros;

		initUI();
		setListeners();
	}

	private void initUI() {
		for (String msg : this.warnings) {
			this.display.getMessagesWarningPanel().addText("• " + msg);
		}

		for (String msg : this.errors) {
			this.display.getMessagesErrorPanel().addText("• " + msg);
		}

		this.display.getMessagesWarningPanel().setVisible(!this.warnings.isEmpty());
		this.display.getMessagesErrorPanel().setVisible(!this.errors.isEmpty());
		this.display.getSubmitButton().setEnabled(this.errors.isEmpty());
	}

	private void setListeners() {
		this.display.getSubmitButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				desabilitaBotaoParametros();
				enviaRequisicaoDeOtimizacao(parametroDTO,cenarioDTO);
			}
		});

		this.display.getCancelButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				habilitarBotaoParametros();
			}
		});

		if (this.display.getCloseButton() != null) {
			this.display.getCloseButton().addSelectionListener(new SelectionListener<IconButtonEvent>() {
				@Override
				public void componentSelected(IconButtonEvent ce) {
					habilitarBotaoParametros();
				}
			});
		}
	}
	
	@Override
	protected void enviaRequisicaoOtimizacaoOnFailure() {
		habilitarBotaoParametros();
	}

	@Override
	protected void enviaRequisicaoOtimizacaoOnSuccess() {
		display.getSimpleModal().hide();
	}

	@Override
	protected void otimizacaoFinalizada() {
		habilitarBotaoParametros();
	}

	private void desabilitaBotaoParametros() {
		this.submitButtonParametros.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.ajax16()));
		this.submitButtonParametros.disable();
	}

	private void habilitarBotaoParametros() {
		this.submitButtonParametros.enable();
		this.submitButtonParametros.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.gerarGrade16()));
	}

	@Override
	public void go(Widget widget) {
		this.display.getSimpleModal().show();
	}
}