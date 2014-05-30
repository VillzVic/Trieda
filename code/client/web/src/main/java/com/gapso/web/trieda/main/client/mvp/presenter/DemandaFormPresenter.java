package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.EditorGrid;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DemandasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
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
		DisciplinaAutoCompleteBox getDisciplinaComboBox();
		NumberField getDemandaTextField();
		SimpleComboBox<Integer> getPeriodoComboBox();
		DemandaDTO getDemandaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
		ListStore< DisciplinaDemandaDTO > getStore();
		EditorGrid<DisciplinaDemandaDTO> getGrid();
		Button getPreencheTudoButton();
		NumberField getPreencheTudoNumberField();
		Button getMarcarEquivalenciasForcadasButton();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< DemandaDTO > gridPanel;
	private Display display;
	private String errorMsg = "Verifique os campos digitados";

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
					service.save(getDTO(), display.getGrid().getStore().getModels(), display.getPeriodoComboBox().getValue().getValue(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display.getI18nMessages().erroAoSalvar(display.getI18nConstants().demanda()),display) {
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							gridPanel.updateList();
							Info.display("Salvo", "Item salvo com sucesso!");
						}
					});
				} else {
					MessageBox.alert("ERRO!", errorMsg, null);
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
						List<Integer> listPeriodos = new ArrayList<Integer>(result.getPeriodosList());
						display.getPeriodoComboBox().removeAll();
						display.getPeriodoComboBox().add(listPeriodos);
						display.getPeriodoComboBox().enable();
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
		
		display.getPeriodoComboBox().addSelectionChangedListener(new SelectionChangedListener<SimpleComboValue<Integer>>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<SimpleComboValue<Integer>> se) {
				display.getGrid().mask( display.getI18nMessages().loading() );
				Services.ofertas().getDisciplinas(getDTO(), display.getOfertaComboBox().getValue(), se.getSelectedItem().getValue(), new AbstractAsyncCallbackWithDefaultOnFailure<ListLoadResult<DisciplinaDemandaDTO>>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(ListLoadResult<DisciplinaDemandaDTO> result) {
						display.getStore().removeAll();
						display.getStore().add( result.getData() );
						display.getGrid().unmask();
					}
				});
			}
		});
		
		display.getPreencheTudoButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if (display.getPreencheTudoNumberField().getValue() != null )
				{
					for (DisciplinaDemandaDTO model : display.getGrid().getStore().getModels())
					{
						display.getGrid().getStore().getRecord(model).set(DisciplinaDemandaDTO.PROPERTY_NOVA_DEMANDA, display.getPreencheTudoNumberField().getValue().intValue());
					}
				}
			}
		});
		
		display.getMarcarEquivalenciasForcadasButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
		@Override
		public void componentSelected(ButtonEvent ce) {
			for (DisciplinaDemandaDTO model : display.getGrid().getStore().getModels())
			{
				display.getGrid().getStore().getRecord(model).set(DisciplinaDemandaDTO.PROPERTY_EXIGE_EQUIVALENCIA_FORCADA, true);
			}
		}
	});
	}
	
	private boolean isValid() {
		if (display.isValid())
		{
			if (display.getGrid().getStore().getModifiedRecords().isEmpty())
			{
				errorMsg = "Nenhuma demanda foi alterada";
				return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	
	private DemandaDTO getDTO(){
		DemandaDTO demandaDTO = display.getDemandaDTO();
		demandaDTO.setInstituicaoEnsinoId(instituicaoEnsinoDTO.getId());
		demandaDTO.setOfertaId(display.getOfertaComboBox().getValue().getId());
		
		return demandaDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
