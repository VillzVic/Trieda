package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.view.AlocacaoManualView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosDemandaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualDisciplinaFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		SimpleModal getSimpleModal();
		ListStore< ResumoMatriculaDTO > getStore();
		Grid< ResumoMatriculaDTO > getGrid();
		TextField< String > getCodigoBuscaTextField();
		CampusComboBox getCampusBuscaComboBox();
		CursoComboBox getCursoBuscaComboBox();
		Button getSalvarButton();
	}
	
	private Display display;
	private CenarioDTO cenarioDTO;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private GTab gTab;
	private GTabItem gTabItem;
	
	public AlocacaoManualDisciplinaFormPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display, GTab gTab, GTabItem gTabItem )
	{
		this.display = display;
		this.cenarioDTO = cenarioDTO;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.gTab = gTab;
		this.gTabItem = gTabItem;
		load();
		setListeners();

	}
	
	private void setListeners() {
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.demandas().getDemandaDTO(cenarioDTO, display.getGrid().getSelectionModel().getSelectedItem(),  new AbstractAsyncCallbackWithDefaultOnFailure<QuintetoDTO<CampusDTO, List<DemandaDTO>, DisciplinaDTO, Integer, Integer>>(display.getI18nMessages().falhaOperacao(),display) {
					@Override
					public void onSuccess(QuintetoDTO<CampusDTO, List<DemandaDTO>, DisciplinaDTO, Integer, Integer> result) {
						Presenter presenter = new AlocacaoManualPresenter( instituicaoEnsinoDTO, cenarioDTO,
							new AlocacaoManualView( cenarioDTO, result, display.getGrid().getSelectionModel().getSelectedItem() ) );

						if (gTabItem != null)
						{
							gTab.remove(gTabItem);
							presenter.go( gTab );
							
						}
						else
						{
							presenter.go( gTab );
						}
						display.getSimpleModal().hide();
					}
				});
			}
		});
	}

	private void load()
	{
		String codigo = display.getCodigoBuscaTextField().getValue();
		
		CampusDTO campusDTO
			= display.getCampusBuscaComboBox().getValue();
		
		CursoDTO cursoDTO
			= display.getCursoBuscaComboBox().getValue();
		
		display.getGrid().mask( display.getI18nMessages().loading(), "loading" );
		AlunosDemandaServiceAsync service = Services.alunosDemanda();
		service.getResumoAtendimentosDisciplinaList( cenarioDTO, codigo, campusDTO,
				cursoDTO, new AbstractAsyncCallbackWithDefaultOnFailure<ListLoadResult<ResumoMatriculaDTO>>(display.getI18nMessages().falhaOperacao(),display) {
			@Override
			public void onSuccess(ListLoadResult<ResumoMatriculaDTO> result) {
				display.getStore().add(result.getData());
				display.getGrid().getView().refresh(false);
				display.getGrid().unmask();
			}
		});
		
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
