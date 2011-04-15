package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class CurriculoDisciplinaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		DisciplinaComboBox getDisciplinaComboBox();
		NumberField getPeriodoTextField();
		CurriculoDTO getCurriculoDTO();
		CurriculoDisciplinaDTO getCurriculoDisciplinaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private Grid<CurriculoDisciplinaDTO> grid;
	private Display display;
	
	public CurriculoDisciplinaFormPresenter(Display display, Grid<CurriculoDisciplinaDTO> grid) {
		this.grid = grid;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final CurriculosServiceAsync service = Services.curriculos();
					service.saveDisciplina(display.getCurriculoDTO(), getDTO(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							grid.getStore().getLoader().load();
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", "Verifique os campos digitados", null);
				}
			}
		});
	}
	
	private boolean isValid() {
		return display.isValid();
	}
	
	private CurriculoDisciplinaDTO getDTO() {
		CurriculoDisciplinaDTO curriculoDisciplinaDTO = display.getCurriculoDisciplinaDTO();
		curriculoDisciplinaDTO.setDisciplinaId(display.getDisciplinaComboBox().getValue().getId());
		curriculoDisciplinaDTO.setDisciplinaString(display.getDisciplinaComboBox().getValue().getCodigo());
		curriculoDisciplinaDTO.setPeriodo(display.getPeriodoTextField().getValue().intValue());
		Integer crTeorico = display.getDisciplinaComboBox().getValue().getCreditosTeorico();
		Integer crPratico = display.getDisciplinaComboBox().getValue().getCreditosPratico();
		curriculoDisciplinaDTO.setCreditosTeorico(crTeorico);
		curriculoDisciplinaDTO.setCreditosPratico(crPratico);
		curriculoDisciplinaDTO.setCreditosTotal(crTeorico + crPratico);
		return curriculoDisciplinaDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
