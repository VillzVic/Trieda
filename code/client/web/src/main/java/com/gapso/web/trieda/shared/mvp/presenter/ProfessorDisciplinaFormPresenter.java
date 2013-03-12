package com.gapso.web.trieda.shared.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.services.ProfessoresDisciplinaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ProfessorDisciplinaFormPresenter implements Presenter {

	public interface Display {
		Button getSalvarButton();
		ProfessorComboBox getProfessorComboBox();
		DisciplinaComboBox getDisciplinaComboBox();
		NumberField getPreferenciaNumberField();
		NumberField getNotaDesempenhoNumberField();
		ProfessorDisciplinaDTO getProfessorDisciplinaDTO();
		boolean isValid();
		
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< ProfessorDisciplinaDTO > gridPanel;
	private Display display;
	private UsuarioDTO usuario;

	public ProfessorDisciplinaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, UsuarioDTO usuario,
		Display display, SimpleGrid< ProfessorDisciplinaDTO > gridPanel )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.usuario = usuario;;
		this.gridPanel = gridPanel;
		this.display = display;
		setListeners();
	}

	private void setListeners() {
		display.getSalvarButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				if(isValid()) {
					final ProfessoresDisciplinaServiceAsync service = Services.professoresDisciplina();
					service.save(getDTO(), new AsyncCallback<Void>() {
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						}
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
	}
	
	private boolean isValid() {
		return display.isValid() && (display.getDisciplinaComboBox().getValue() != null);
	}
	
	private ProfessorDisciplinaDTO getDTO()
	{
		ProfessorDisciplinaDTO unidadeDTO = display.getProfessorDisciplinaDTO();

		if ( usuario.isAdministrador() )
		{
			unidadeDTO.setProfessorId( display.getProfessorComboBox().getValue().getId() );
			unidadeDTO.setProfessorString( display.getProfessorComboBox().getValue().getNome() );
			unidadeDTO.setProfessorCpf( display.getProfessorComboBox().getValue().getCpf() );
			unidadeDTO.setDisciplinaId( display.getDisciplinaComboBox().getValue().getId() );
			unidadeDTO.setDisciplinaString( display.getDisciplinaComboBox().getValue().getCodigo() );
			unidadeDTO.setNotaDesempenho( display.getNotaDesempenhoNumberField().getValue().intValue() );
		}

		unidadeDTO.setInstituicaoEnsinoId( instituicaoEnsinoDTO.getId() );
		unidadeDTO.setPreferencia( display.getPreferenciaNumberField().getValue().intValue() );

		return unidadeDTO;
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
