package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.DificuldadeComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoDisciplinaComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinaFormPresenter
	implements Presenter
{
	public interface Display
	{
		Button getSalvarButton();
		TextField< String > getNomeTextField();
		TextField< String > getCodigoTextField();
		NumberField getCreditosTeoricoTextField();
		NumberField getCreditosPraticoTextField();
		NumberField getCargaHorariaTextField();
		CheckBox getLaboratorioCheckBox();
		CheckBox getAulasContinuasCheckBox();
		CheckBox getProfessorUnicoCheckBox();
		CheckBox getUsaSabadoCheckBox();
		CheckBox getUsaDomingoCheckBox();
		TipoDisciplinaComboBox getTipoDisciplinaComboBox();
		DificuldadeComboBox getDificuldadeComboBox();
		NumberField getMaxAlunosTeoricoTextField();
		NumberField getMaxAlunosPraticoTextField();
		DisciplinaDTO getDisciplinaDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private SimpleGrid< DisciplinaDTO > gridPanel;
	private Display display;
	private String errorMessage;

	public DisciplinaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this( instituicaoEnsinoDTO, cenario, display, null );
	}

	public DisciplinaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display,
		SimpleGrid< DisciplinaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.gridPanel = gridPanel;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					display.getSalvarButton().mask();
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
							display.getSalvarButton().unmask();
						}
					});
				} else {
					MessageBox.alert("ERRO!", errorMessage, null);
					display.getSalvarButton().unmask();
				}
			}
		});
	}
	
	private boolean isValid() {
		if (display.isValid()) {
			if (display.getCreditosTeoricoTextField().getValue().intValue() > 0) {
				if (display.getMaxAlunosTeoricoTextField().getValue().intValue() <= 0) {
					errorMessage = "Max Alunos - Teórico deve ter um valor maior que 0";
					return false;
				}
			}
			if (display.getCreditosPraticoTextField().getValue().intValue() > 0) {
				if (display.getMaxAlunosPraticoTextField().getValue().intValue() <= 0) {
					errorMessage = "Max Alunos - Prático deve ter um valor maior que 0";
					return false;
				}
			}
			if (display.getAulasContinuasCheckBox().getValue().booleanValue()) {
				if (display.getCreditosTeoricoTextField().getValue().intValue() <= 0 ||
						display.getCreditosPraticoTextField().getValue().intValue() <= 0 ||
						!display.getLaboratorioCheckBox().getValue().booleanValue() ||
						(display.getCreditosTeoricoTextField().getValue().intValue() +
						display.getCreditosPraticoTextField().getValue().intValue() > 4)) {
					errorMessage = "Aulas Continuas? Selecionado - A disciplina deve conter creditos praticos e teoricos (soma 2, 3 ou 4)" +
							" e usar laboratório";
					return false;
				}
			}
			return true;
		}
		else
		{
			errorMessage = "Verifique os campos digitados";
			return false;
		}
	}
	
	private DisciplinaDTO getDTO()
	{
		DisciplinaDTO disciplinaDTO = display.getDisciplinaDTO();

		disciplinaDTO.setId( display.getDisciplinaDTO().getId() );
		disciplinaDTO.setVersion( display.getDisciplinaDTO().getVersion() );

		disciplinaDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
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
		disciplinaDTO.setCargaHoraria(display.getCargaHorariaTextField().getValue() != null ?
				display.getCargaHorariaTextField().getValue().intValue() : null);
		disciplinaDTO.setUsaSabado(display.getUsaSabadoCheckBox().getValue());
		disciplinaDTO.setUsaDomingo(display.getUsaDomingoCheckBox().getValue());
		disciplinaDTO.setAulasContinuas(display.getAulasContinuasCheckBox().getValue());
		disciplinaDTO.setProfessorUnico(display.getProfessorUnicoCheckBox().getValue());

		return disciplinaDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
