package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.DificuldadeComboBox;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoDisciplinaComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		TextField<String> getNomeTextField();
		TextField<String> getCodigoTextField();
		NumberField getCreditosTeoricoTextField();
		NumberField getCreditosPraticoTextField();
		CheckBox getLaboratorioCheckBox();
		TipoDisciplinaComboBox getTipoDisciplinaComboBox();
		DificuldadeComboBox getDificuldadeComboBox();
		NumberField getMaxAlunosTeoricoTextField();
		NumberField getMaxAlunosPraticoTextField();
		DisciplinaDTO getDisciplinaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}
	private CenarioDTO cenario;
	private SimpleGrid<DisciplinaDTO> gridPanel;
	private Display display;
	
	public DisciplinaFormPresenter(CenarioDTO cenario, Display display) {
		this(cenario, display, null);
	}
	public DisciplinaFormPresenter(CenarioDTO cenario, Display display, SimpleGrid<DisciplinaDTO> gridPanel) {
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final DisciplinasServiceAsync service = Services.disciplinas();
					service.save(getDTO(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
						@Override
						public void onSuccess(Void result) {
							display.getSimpleModal().hide();
							if(gridPanel != null) {
								gridPanel.updateList();
							}
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
	
	private DisciplinaDTO getDTO() {
		DisciplinaDTO disciplinaDTO = display.getDisciplinaDTO();
		disciplinaDTO.setCenarioId(cenario.getId());
		disciplinaDTO.setNome(display.getNomeTextField().getValue());
		disciplinaDTO.setCodigo(display.getCodigoTextField().getValue());
		disciplinaDTO.setCreditosTeorico(display.getCreditosTeoricoTextField().getValue().intValue());
		disciplinaDTO.setCreditosPratico(display.getCreditosPraticoTextField().getValue().intValue());
		disciplinaDTO.setLaboratorio(display.getLaboratorioCheckBox().getValue());
		disciplinaDTO.setTipoId(display.getTipoDisciplinaComboBox().getSelection().get(0).getId());
		disciplinaDTO.setTipoString(display.getTipoDisciplinaComboBox().getSelection().get(0).getNome());
		disciplinaDTO.setDificuldade(display.getDificuldadeComboBox().getValue().getValue().name());
		disciplinaDTO.setMaxAlunosTeorico(display.getMaxAlunosTeoricoTextField().getValue().intValue());
		disciplinaDTO.setMaxAlunosPratico(display.getMaxAlunosPraticoTextField().getValue().intValue());
		return disciplinaDTO;
	}
	
	@Override
	public void go(Widget widget) {
		display.getSimpleModal().show();
	}

}
