package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class CompartilharCursosPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway {
		CursoComboBox getCurso1ComboBox();
		CursoComboBox getCurso2ComboBox();
		Grid<CursoDescompartilhaDTO> getGrid();
		ParametroDTO getParametro();
		Button getFecharBT();
		Button getAdicionarBT();
		Button getRemoverBT();
		List< CursoDescompartilhaDTO > getCursos();
		Component getComponent();
		SimpleModal getSimpleModal();
	}

	private Display display;

	public CompartilharCursosPresenter( Display display )
	{
		this.display = display;

		setListeners();
	}
	
	private void setListeners()
	{
		this.display.getAdicionarBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				CursoDescompartilhaDTO dto = new CursoDescompartilhaDTO();
				dto.setInstituicaoEnsinoId(display.getParametro().getInstituicaoEnsinoId());
				dto.setParametroId(display.getParametro().getId());
				CursoDTO curso1 = display.getCurso1ComboBox().getValue();
				dto.setCurso1Id(curso1.getId());
				dto.setCurso1Display(curso1.getDisplayText());
				CursoDTO curso2 = display.getCurso2ComboBox().getValue();
				dto.setCurso2Id(curso2.getId());
				dto.setCurso2Display(curso2.getDisplayText());
				
				final CursoDescompartilhaDTO finalDTO = dto; 
				String errorMsg = display.getI18nMessages().erroAoSalvar("Proibição de Compartilhamento entre Cursos");
				Services.cursos().insereBDProibicaoCompartilhamentoCursos(dto,new AbstractAsyncCallbackWithDefaultOnFailure<Void>(errorMsg,display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().getStore().add(finalDTO);
						display.getCursos().add(finalDTO);
					}
				});
			}
		});

		this.display.getRemoverBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				final List<CursoDescompartilhaDTO> dtos = display.getGrid().getSelectionModel().getSelectedItems();
				
				String errorMsg = display.getI18nMessages().erroAoRemover("Proibição de Compartilhamento entre Cursos");
				Services.cursos().removeBDProibicaoCompartilhamentoCursos(dtos,new AbstractAsyncCallbackWithDefaultOnFailure<Void>(errorMsg,display) {
					@Override
					public void onSuccess(Void result) {
						for(CursoDescompartilhaDTO cd : dtos) {
							display.getGrid().getStore().remove(cd);
							display.getCursos().remove(cd);
						}
					}
				});
			}
		});

		this.display.getFecharBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getSimpleModal().hide();
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
