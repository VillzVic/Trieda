package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.main.client.util.view.CursoComboBox;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.ui.Widget;

public class CompartilharCursosPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		CursoComboBox getCurso1ComboBox();
		CursoComboBox getCurso2ComboBox();
		Grid<CursoDescompartilhaDTO> getGrid();
		ParametroDTO getParametro();
		Button getFecharBT();
		Button getAdicionarBT();
		Button getRemoverBT();
		List<CursoDescompartilhaDTO> getCursos();
		Component getComponent();
		
		SimpleModal getSimpleModal();
	}
	private Display display;
	
	public CompartilharCursosPresenter(Display display) {
		this.display = display;
		setListeners();
	}
	
	private void setListeners() {
		display.getAdicionarBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				CursoDescompartilhaDTO cd = new CursoDescompartilhaDTO();
				cd.setParametroId(display.getParametro().getId());
				CursoDTO curso1 = display.getCurso1ComboBox().getValue();
				cd.setCurso1Id(curso1.getId());
				cd.setCurso1Display(curso1.getDisplayText());
				CursoDTO curso2 = display.getCurso2ComboBox().getValue();
				cd.setCurso2Id(curso2.getId());
				cd.setCurso2Display(curso2.getDisplayText());
				display.getGrid().getStore().add(cd);
				display.getCursos().add(cd);
			}
		});
		display.getRemoverBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<CursoDescompartilhaDTO> cds = display.getGrid().getSelectionModel().getSelectedItems();
				for(CursoDescompartilhaDTO cd : cds) {
					display.getGrid().getStore().remove(cd);
					display.getCursos().remove(cd);
				}
			}
		});
		display.getFecharBT().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getSimpleModal().hide();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
