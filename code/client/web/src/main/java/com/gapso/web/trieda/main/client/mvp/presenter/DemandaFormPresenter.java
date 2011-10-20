package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DemandasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DemandaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		CampusComboBox getCampusComboBox();
		CursoComboBox getCursoComboBox();
		CurriculoComboBox getCurriculoComboBox();
		TurnoComboBox getTurnoComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		NumberField getDemandaTextField();
		DemandaDTO getDemandaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< DemandaDTO > gridPanel;
	private Display display;

	public DemandaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, SimpleGrid< DemandaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final DemandasServiceAsync service = Services.demandas();
					service.save(getDTO(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							gridPanel.updateList();
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
	
	private DemandaDTO getDTO()
	{
		DemandaDTO demandaDTO = display.getDemandaDTO();

		demandaDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		demandaDTO.setCampusId(display.getCampusComboBox().getSelection().get(0).getId());
		demandaDTO.setCampusString(display.getCampusComboBox().getSelection().get(0).getCodigo());
		demandaDTO.setCursoId(display.getCursoComboBox().getSelection().get(0).getId());
		demandaDTO.setCursoString(display.getCursoComboBox().getSelection().get(0).getCodigo());
		demandaDTO.setCurriculoId(display.getCurriculoComboBox().getSelection().get(0).getId());
		demandaDTO.setCurriculoString(display.getCurriculoComboBox().getSelection().get(0).getCodigo());
		demandaDTO.setTurnoId(display.getTurnoComboBox().getSelection().get(0).getId());
		demandaDTO.setTurnoString(display.getTurnoComboBox().getSelection().get(0).getNome());
		demandaDTO.setDisciplinaId(display.getDisciplinaComboBox().getSelection().get(0).getId());
		demandaDTO.setDisciplinaString(display.getDisciplinaComboBox().getSelection().get(0).getCodigo());
		demandaDTO.setDemanda(display.getDemandaTextField().getValue().intValue());
		
		return demandaDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
