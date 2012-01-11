package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DemandasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.OfertaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class DemandaFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		OfertaComboBox getOfertaComboBox();
		TextField<CampusDTO> getCampusTextField();
		TextField<CursoDTO> getCursoTextField();
		TextField<CurriculoDTO> getCurriculoTextField();
		TextField<TurnoDTO> getTurnoTextField();
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
					service.save(getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display.getI18nMessages().erroAoSalvar(display.getI18nConstants().demanda()),display) {
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
		
		display.getOfertaComboBox().addSelectionChangedListener(new SelectionChangedListener<OfertaDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<OfertaDTO> se) {
				final OfertaDTO ofertaDTO = se.getSelectedItem();
				Services.campi().getCampus(ofertaDTO.getCampusId(),new AbstractAsyncCallbackWithDefaultOnFailure<CampusDTO>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(CampusDTO result) {
						display.getCampusTextField().setValue(result);
					}
				});
				Services.curriculos().getCurriculo(ofertaDTO.getMatrizCurricularId(),new AbstractAsyncCallbackWithDefaultOnFailure<CurriculoDTO>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(CurriculoDTO result) {
						display.getCurriculoTextField().setValue(result);
						Services.cursos().getCurso(result.getCursoId(),new AbstractAsyncCallbackWithDefaultOnFailure<CursoDTO>(display.getI18nMessages().falhaOperacao(),display) {
							@Override
							public void onSuccess(CursoDTO result) {
								display.getCursoTextField().setValue(result);
							}
						});
					}
				});
				Services.turnos().getTurno(ofertaDTO.getTurnoId(),new AbstractAsyncCallbackWithDefaultOnFailure<TurnoDTO>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(TurnoDTO result) {
						display.getTurnoTextField().setValue(result);
					}
				});
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
		demandaDTO.setCampusId(display.getCampusTextField().getValue().getId());
		demandaDTO.setCampusString(display.getCampusTextField().getValue().getCodigo());
		demandaDTO.setCursoId(display.getCursoTextField().getValue().getId());
		demandaDTO.setCursoString(display.getCursoTextField().getValue().getCodigo());
		demandaDTO.setCurriculoId(display.getCurriculoTextField().getValue().getId());
		demandaDTO.setCurriculoString(display.getCurriculoTextField().getValue().getCodigo());
		demandaDTO.setTurnoId(display.getTurnoTextField().getValue().getId());
		demandaDTO.setTurnoString(display.getTurnoTextField().getValue().getNome());
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
