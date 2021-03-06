package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AlunosFormPresenter
	implements Presenter
{
	public interface Display
	{
		UniqueTextField getMatriculaTextField();
		TextField< String > getNomeTextField();
		CheckBox getFormandoCheckBox();
		CheckBox getVirtualCheckBox();
		NumberField getPeriodoNumberField();
		NumberField getPrioridadeNumberField();
		Button getSalvarButton();
		AlunoDTO getAlunoDTO();
		boolean isValid();
		SimpleModal getSimpleModal();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private SimpleGrid< AlunoDTO > gridPanel;
	private Display display;
	private CenarioDTO cenarioDTO;

	public AlunosFormPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display,
		SimpleGrid< AlunoDTO > gridPanel )
	{
		this.cenarioDTO = cenarioDTO;
		this.gridPanel = gridPanel;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		this.setListeners();
	}

	public AlunosFormPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this( instituicaoEnsinoDTO, cenarioDTO, display, null );
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
					final AlunosServiceAsync service = Services.alunos();

					service.saveAluno( getDTO(), new AsyncCallback< Void >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi possível salvar os dados do aluno", null );
						}

						@Override
						public void onSuccess( Void result )
						{
							display.getSimpleModal().hide();

							if ( gridPanel != null )
							{
								gridPanel.updateList();
							}

							Info.display( "Salvo", "Item salvo com sucesso!" );
						}
					});
				}
				else
				{
					MessageBox.alert( "ERRO!", "Verifique os campos digitados", null );
				}
			}
		});
	}

	private boolean isValid()
	{
		return this.display.isValid();
	}
	
	private AlunoDTO getDTO()
	{
		AlunoDTO alunoDTO = this.display.getAlunoDTO();

		alunoDTO.setInstituicaoEnsinoId( this.instituicaoEnsinoDTO.getId() );
		alunoDTO.setNome( this.display.getNomeTextField().getValue() );
		alunoDTO.setMatricula( this.display.getMatriculaTextField().getValue() );
		alunoDTO.setFormando( this.display.getFormandoCheckBox().getValue() );
		alunoDTO.setVirtual( this.display.getVirtualCheckBox().getValue() );
		alunoDTO.setCriadoTrieda(false);
		alunoDTO.setPeriodo( this.display.getPeriodoNumberField().getValue() != null ? 
				this.display.getPeriodoNumberField().getValue().intValue() : null );
		alunoDTO.setPrioridade( this.display.getPrioridadeNumberField().getValue() != null ? 
				this.display.getPrioridadeNumberField().getValue().intValue() : null );
		alunoDTO.setCenarioId( this.cenarioDTO.getId() );

		return alunoDTO;
	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
