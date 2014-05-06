package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.services.CurriculosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaCursoGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class RelatorioVisaoCursoPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		CampusComboBox getCampusComboBox();
		CursoComboBox getCursoComboBox();
		CurriculoComboBox getCurriculoComboBox();
		TurnoComboBox getTurnoComboBox();
		SimpleComboBox<Integer> getPeriodoComboBox();
		GradeHorariaCursoGrid getGrid();
	}

	public RelatorioVisaoCursoPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, cenario, display);
	}

	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) this.getDisplay();
		
		display.getCurriculoComboBox().addSelectionChangedListener(new SelectionChangedListener<CurriculoDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CurriculoDTO> se){
				final CurriculoDTO curriculoDTO = se.getSelectedItem();

				display.getPeriodoComboBox().getStore().removeAll();
				display.getPeriodoComboBox().setValue(null);
				display.getPeriodoComboBox().setEnabled(curriculoDTO != null);

				if(curriculoDTO != null){
					CurriculosServiceAsync service = Services.curriculos();

					service.getPeriodos(curriculoDTO, cenarioDTO, new AsyncCallback<List<Integer>>(){
						@Override
						public void onFailure(Throwable caught){
							MessageBox.alert( "Erro", "Erro no servidor ao pegar os períodos da Matriz Curricular", null);
						}

						@Override
						public void onSuccess(List<Integer> result){
							display.getPeriodoComboBox().add(result);
						}
					});
				}
			}
		});
	}

}
