package com.gapso.web.trieda.professor.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisponibilidadesProfessorFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		SimpleModal getSimpleModal();
		ProfessorDTO getProfessorDTO();
		void setProxy(RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>> proxy);
		ListStore<HorarioDisponivelCenarioDTO> getStore();
	}

	@SuppressWarnings("unused")
	private CenarioDTO cenario;

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private SemanaLetivaDTO semanaLetiva;

	public DisponibilidadesProfessorFormPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, SemanaLetivaDTO semanaLetiva, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.semanaLetiva = semanaLetiva;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>> proxy = new RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>>() {
			@Override
			protected void load(Object loadConfig, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback) {
				Services.semanasLetiva().getHorariosDisponiveisCenario(semanaLetiva, callback);
			}
		};
		display.setProxy(proxy);
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getStore().commitChanges();
				List<HorarioDisponivelCenarioDTO> hdcDTOList = display.getStore().getModels();
				
				Services.professores().saveHorariosDisponiveis(getDTO(), semanaLetiva, hdcDTOList, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
					}
					@Override
					public void onSuccess(Void result) {
						Info.display("Atualizado", "Horários atualizados com sucesso!");
						display.getSimpleModal().hide();
					}
					
				});
			}
		});
	}

	private ProfessorDTO getDTO()
	{
		ProfessorDTO dto = display.getProfessorDTO();
		dto.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		return dto;
	}

	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}
}
