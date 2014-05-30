package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.BaseEvent;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.Radio;
import com.extjs.gxt.ui.client.widget.form.RadioGroup;
import com.gapso.web.trieda.main.client.mvp.view.DemandasPorAlunoView;
import com.gapso.web.trieda.main.client.mvp.view.DemandasView;
import com.gapso.web.trieda.main.client.mvp.view.OfertasView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DemandasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ParametrosGeracaoDemandaPresenter 
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway {
		Component getComponent();
		Button getSubmitButton();
		Button getOfertasCursosButton();
		Button getOfertasDemandasButton();
		Button getDemandasAlunosButton();
		TurnoComboBox getTurnoComboBox();
		CampusComboBox getCampusComboBox();	
		CheckBox getUsarDemandasPrioridade2CheckBox();
		NumberField getDistanciaMaxEmPeriodosParaPrioridade2NumberField();
		CheckBox getConsiderarPreRequisitosCheckBox();
		CheckBox getConsiderarCoRequisitosCheckBox();
		CheckBox getConsiderarMaturidadeCheckBox();
		Radio getCreditoPeriodoRadio();
		Radio getCreditoManualRadio();
		CheckBox getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox();
		NumberField getFatorAumentoDeMaxCreditosNumberField();
		NumberField getCreditoManualNumberField();
		RadioGroup getMetodoGeracaoRadioGroup();
		Radio getEvolucaoRadio();
		Radio getCriacaoAutomaticaRadio();
		Radio getCriacaoDiretaRadio();
		void selecionaCriacaoAutomatica();
		void selecionaCriacaoDireta();
		void selecionaEvolucaoAlunos();
	}
	
	private Display display; 
	private CenarioDTO cenarioDTO;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private GTab gTab;

	public ParametrosGeracaoDemandaPresenter(CenarioDTO cenarioDTO, InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display) {
		this.cenarioDTO = cenarioDTO;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		setListeners();
	}
	
	private void setListeners() {
		this.display.getSubmitButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final DemandasServiceAsync service = Services.demandas();

				MessageBox.confirm("Gerar Demanda?","Deseja gerar a demanda?",new Listener<MessageBoxEvent>() {
					@Override
					public void handleEvent(MessageBoxEvent be) {
						if (be.getButtonClicked().getHtml().equalsIgnoreCase("yes") ||
								be.getButtonClicked().getHtml().equalsIgnoreCase("sim")) {
							try {
						    	new AcompanhamentoPanelPresenter("chaveGeracaoDemanda", new AcompanhamentoPanelView());
								service.calculaPrioridadesParaDisciplinasNaoCursadasPorAluno(cenarioDTO, getDTO(), new AsyncCallback<Void>() {
									@Override
									public void onSuccess(Void result) {
										MessageBox.info("Gerar Demanda", "Demanda gerado com sucesso", null);
									}

									@Override
									public void onFailure(Throwable caught) {

									}
								});
							} catch(Exception e) {
								MessageBox.alert("ERRO!","Não foi possível gerar a demanda.",null);
							}
						} else {
						}
					}
				});
			}
		});
		
		this.display.getUsarDemandasPrioridade2CheckBox().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				boolean value = ((CheckBox)be.getSource()).getValue();
				display.getDistanciaMaxEmPeriodosParaPrioridade2NumberField().setEnabled(value);
			}
		});
		
		this.display.getCreditoManualRadio().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				boolean value = ((CheckBox)be.getSource()).getValue();
				display.getCreditoManualNumberField().setEnabled(value);
			}
		});
		
		this.display.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				boolean value = ((CheckBox)be.getSource()).getValue();
				display.getFatorAumentoDeMaxCreditosNumberField().setEnabled(value);
			}
		});
		
		this.display.getMetodoGeracaoRadioGroup().addListener(Events.Change,new Listener<BaseEvent>() {
			@Override
			public void handleEvent(BaseEvent be) {
				if (display.getEvolucaoRadio().getValue())
				{
					display.selecionaEvolucaoAlunos();
				}
				else if (display.getCriacaoAutomaticaRadio().getValue())
				{
					display.selecionaCriacaoAutomatica();
				}
				else if (display.getCriacaoDiretaRadio().getValue())
				{
					display.selecionaCriacaoDireta();
				}
			}
		});
		
		this.display.getOfertasCursosButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new OfertasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new OfertasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.display.getOfertasDemandasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DemandasPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DemandasView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
		
		this.display.getDemandasAlunosButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DemandasPorAlunoPresenter(
					instituicaoEnsinoDTO, cenarioDTO, new DemandasPorAlunoView( cenarioDTO ) );

				presenter.go( gTab );
			}
		});
	}
	
	public ParametroGeracaoDemandaDTO getDTO() {
		ParametroGeracaoDemandaDTO dto = new ParametroGeracaoDemandaDTO();
		
		dto.setInstituicaoEnsinoId(this.instituicaoEnsinoDTO.getId());
		dto.setCampusId(display.getCampusComboBox().getValue().getId());
		dto.setTurnoId(display.getTurnoComboBox().getValue().getId());
		dto.setUsarDemandasDePrioridade2(display.getUsarDemandasPrioridade2CheckBox().getValue());
		dto.setDistanciaMaxEmPeriodosParaPrioridade2(display.getDistanciaMaxEmPeriodosParaPrioridade2NumberField().getValue().intValue());
		dto.setConsiderarPreRequisitos(display.getConsiderarPreRequisitosCheckBox().getValue());
		dto.setConsiderarCoRequisitos(display.getConsiderarCoRequisitosCheckBox().getValue());
		dto.setMaxCreditosPeriodo(display.getCreditoPeriodoRadio().getValue());
		dto.setMaxCreditosManual(display.getCreditoManualRadio().getValue() ? display.getCreditoManualNumberField().getValue().intValue() : 0);
		dto.setAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas(display.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadasCheckBox().getValue());
		dto.setFatorDeAumentoDeMaxCreditos(display.getFatorAumentoDeMaxCreditosNumberField().getValue().intValue());
		
		return dto;
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab) widget;
		gTab.add((GTabItem)this.display.getComponent());
	}
	
}
