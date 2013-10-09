package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaRequisitoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.PeriodoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinaPreRequisitoFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		boolean isValid();
		SimpleModal getSimpleModal();
		CurriculoComboBox getCurriculoComboBox();
		PeriodoComboBox getPeriodoComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		DisciplinaAutoCompleteBox getDisciplinaPreRequisitoComboBox();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private SimpleGrid< DisciplinaRequisitoDTO > gridPanel;
	private Display display;
	private String errorMessage;

	public DisciplinaPreRequisitoFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this( instituicaoEnsinoDTO, cenario, display, null );
	}

	public DisciplinaPreRequisitoFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< DisciplinaRequisitoDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CurriculosServiceAsync service = Services.curriculos();
					service.saveDisciplinaPreRequisito(cenario, getDTO(), display.getDisciplinaPreRequisitoComboBox().getValue(),
							new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							if(gridPanel != null) {
								gridPanel.updateList();
							}
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", errorMessage, null);
				}
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}
	
	private DisciplinaRequisitoDTO getDTO()
	{
		DisciplinaRequisitoDTO disciplinaRequisitoDTO = new DisciplinaRequisitoDTO();

		disciplinaRequisitoDTO.setCurriculoId(display.getCurriculoComboBox().getValue().getId());
		disciplinaRequisitoDTO.setCurriculoString(display.getCurriculoComboBox().getValue().getCodigo());
		disciplinaRequisitoDTO.setPeriodo(display.getPeriodoComboBox().getSimpleValue());
		disciplinaRequisitoDTO.setDisciplinaId(display.getDisciplinaComboBox().getValue().getId());
		disciplinaRequisitoDTO.setDisciplinaString(display.getDisciplinaComboBox().getValue().getCodigo());
		disciplinaRequisitoDTO.setDisciplinaRequisitoId(display.getDisciplinaPreRequisitoComboBox().getValue().getId());
		disciplinaRequisitoDTO.setDisciplinaRequisitoString(display.getDisciplinaPreRequisitoComboBox().getValue().getCodigo());

		return disciplinaRequisitoDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
