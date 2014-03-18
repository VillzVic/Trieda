package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.dtos.DicaEliminacaoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.MotivoUsoProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class MotivosUsoProfessorVirtualPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		SimpleModal getSimpleModal();
		ProfessorVirtualDTO getProfessorVirtualDTO();
		ListStore< ConfirmacaoTurmaDTO > getTurmaStore();
		ListStore< MotivoUsoProfessorVirtualDTO > getMotivosStore();
		ListStore< DicaEliminacaoProfessorVirtualDTO > getDicasStore();
		Grid<ConfirmacaoTurmaDTO> getTurmaGrid();
		Grid<MotivoUsoProfessorVirtualDTO> getMotivosGrid();
		Grid<DicaEliminacaoProfessorVirtualDTO> getDicasGrid();
	}

	private CenarioDTO cenarioDTO;
	private SimpleGrid< ProfessorVirtualDTO > gridPanel;
	private Display display;

	public MotivosUsoProfessorVirtualPresenter(
		CenarioDTO cenarioDTO,
		Display display, SimpleGrid< ProfessorVirtualDTO > gridPanel )
	{
		this.cenarioDTO = cenarioDTO;
		this.gridPanel = gridPanel;
		this.display = display;

		load();
		setListeners();
	}
	
	private void load() {
		if (display.getProfessorVirtualDTO().getId() != null )
		{
			AtendimentosServiceAsync service = Services.atendimentos();
			display.getTurmaGrid().mask( display.getI18nMessages().loading(), "loading" );
			service.getTurmasProfessoresVirtuaisList(cenarioDTO, display.getProfessorVirtualDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<ListLoadResult<ConfirmacaoTurmaDTO>>(display.getI18nMessages().falhaOperacao(),display) {
				@Override
				public void onSuccess(ListLoadResult<ConfirmacaoTurmaDTO> result) {
					display.getTurmaStore().add(result.getData());
					display.getTurmaGrid().getView().refresh(false);
					display.getTurmaGrid().unmask();
				}
			});
		}
		
	}

	private void setListeners() {
		
		display.getTurmaGrid().getSelectionModel().addSelectionChangedListener(new SelectionChangedListener<ConfirmacaoTurmaDTO>() {

			@Override
		    public void selectionChanged(SelectionChangedEvent<ConfirmacaoTurmaDTO> se) {
				AtendimentosServiceAsync service = Services.atendimentos();
				ConfirmacaoTurmaDTO turma = display.getTurmaGrid().getSelectionModel().getSelectedItem();
				display.getMotivosGrid().mask( display.getI18nMessages().loading(), "loading" );
				display.getDicasGrid().mask( display.getI18nMessages().loading(), "loading" );
				service.getMotivosUsoProfessorVirtual(cenarioDTO, turma.getDisciplinaId(), turma.getTurma(), (turma.getCreditosTeorico() > 0),
						new AbstractAsyncCallbackWithDefaultOnFailure<ListLoadResult<MotivoUsoProfessorVirtualDTO>>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(ListLoadResult<MotivoUsoProfessorVirtualDTO> result) {
						display.getMotivosStore().add(result.getData());
						display.getMotivosGrid().getView().refresh(false);
						display.getMotivosGrid().unmask();
					}
				});
				
				service.getDicasEliminacaoProfessorVirtual(cenarioDTO, turma.getDisciplinaId(), turma.getTurma(), (turma.getCreditosTeorico() > 0),
						new AbstractAsyncCallbackWithDefaultOnFailure<ListLoadResult<DicaEliminacaoProfessorVirtualDTO>>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(ListLoadResult<DicaEliminacaoProfessorVirtualDTO> result) {
						display.getDicasStore().add(result.getData());
						display.getDicasGrid().getView().refresh(false);
						display.getDicasGrid().unmask();
					}
				});
		    }
		});
	}

	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}
}
