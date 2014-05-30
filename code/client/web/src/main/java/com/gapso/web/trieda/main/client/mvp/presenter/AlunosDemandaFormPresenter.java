package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosDemandaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class AlunosDemandaFormPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getSalvarButton();
		AlunosComboBox getAlunoComboBox();
		DemandaDTO getDemandaDTO();
		AlunoDemandaDTO getAlunoDemandaDTO();
		NumberField getPeriodoNumberField();
		boolean isValid();
		SimpleModal getSimpleModal();
		NumberField getPrioridadeField();
		CheckBox getExigeEquivalenciaForcadaCheckBox();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Grid< AlunoDemandaDTO > grid;
	private Display display;

	public AlunosDemandaFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		Display display, Grid< AlunoDemandaDTO > grid )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.grid = grid;
		this.display = display;

		setListeners();
	}

	private void setListeners()
	{
		this.display.getSalvarButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if ( isValid() )
				{
					final AlunosDemandaServiceAsync service = Services.alunosDemanda();

					service.saveAlunoDemanda( display.getDemandaDTO(),
						getDTO(), new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display.getI18nMessages().erroAoSalvar(display.getI18nConstants().associacaoAlunoDemanda()),display)
					{
						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();
							grid.getStore().getLoader().load();
							Info.display( "Salvo", "Item salvo com sucesso!" );
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!",
						"Verifique os campos digitados", null );
				}
			}
		});
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}

	private AlunoDemandaDTO getDTO()
	{
		AlunoDemandaDTO alunoDemandaDTO = this.display.getAlunoDemandaDTO();

		alunoDemandaDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		alunoDemandaDTO.setIdAluno( this.display.getAlunoComboBox().getValue().getId() );
		alunoDemandaDTO.setAlunoString( this.display.getAlunoComboBox().getValue().getNome() );
		alunoDemandaDTO.setDemandaId( this.display.getDemandaDTO().getId() );
		alunoDemandaDTO.setPeriodo( this.display.getPeriodoNumberField().getValue().intValue() );
		alunoDemandaDTO.setAlunoPrioridade(display.getPrioridadeField().getValue().intValue());
		alunoDemandaDTO.setExigeEquivalenciaForcada(this.display.getExigeEquivalenciaForcadaCheckBox().getValue());

		return alunoDemandaDTO;
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
