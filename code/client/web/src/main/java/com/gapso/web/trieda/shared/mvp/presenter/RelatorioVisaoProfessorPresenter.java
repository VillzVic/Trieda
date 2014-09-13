package com.gapso.web.trieda.shared.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.services.ProfessoresServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.ProfessorCpfComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorNomeComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class RelatorioVisaoProfessorPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		ProfessorVirtualComboBox getProfessorVirtualComboBox();
		GradeHorariaProfessorGrid getGrid();
		ProfessorCpfComboBox getCpfTextField();
		ProfessorNomeComboBox getProfessorTextField();
		Button getProxProfessorButton();
		Button getAntProfessorButton();
		Button getProxCpfButton();
		Button getAntCpfButton();
		Button getProxProfessorVirtualButton();
		Button getAntProfessorVirtualButton();
	}

	private boolean isVisaoProfessor;

	public RelatorioVisaoProfessorPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenario, UsuarioDTO usuario,
		Display display, boolean isVisaoProfessor)
	{
		super(instituicaoEnsinoDTO, cenario, display);
		this.isVisaoProfessor = isVisaoProfessor;
	}

	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) this.getDisplay();
		
		if (display.getProfessorVirtualComboBox() != null) {
			display.getProfessorVirtualComboBox().addSelectionChangedListener(new SelectionChangedListener<ProfessorVirtualDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<ProfessorVirtualDTO> se){
					display.getProfessorTextField().setValue(null);
					display.getCpfTextField().setValue(null);
					if (display.getProfessorVirtualComboBox().getValue() == null)
					{
						display.getProxProfessorVirtualButton().disable();
						display.getAntProfessorVirtualButton().disable();
					}
				}
			});
		}
		
		if (display.getProfessorTextField() != null) {
			display.getProfessorTextField().addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se){
					if (display.getProfessorTextField().getValue() != null){
						display.getCpfTextField().setValue(null);
					}
					else
					{
						display.getProxProfessorButton().disable();
						display.getAntProfessorButton().disable();
					}
				}
			});
		}
		
		if (display.getCpfTextField() != null) {
			display.getCpfTextField().addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se){
					if (display.getCpfTextField().getValue() != null){
						display.getProfessorTextField().setValue(null);
					}
					else
					{
						display.getProxCpfButton().disable();
						display.getAntCpfButton().disable();
					}
				}
			});
		}
		
		this.display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				final ProfessorDTO professorDTO = display.getProfessorTextField().getValue();
				if ( professorDTO == null )
				{
					display.getProxProfessorButton().disable();
					display.getAntProfessorButton().disable();
				}
				else
				{
					display.getProxProfessorButton().enable();
					display.getAntProfessorButton().enable();
				}
				
				final ProfessorDTO professorCpfDTO = display.getCpfTextField().getValue();
				if ( professorCpfDTO == null )
				{
					display.getProxCpfButton().disable();
					display.getAntCpfButton().disable();
				}
				else
				{
					display.getProxCpfButton().enable();
					display.getAntCpfButton().enable();
				}
				
				final ProfessorVirtualDTO professorVirtualDTO = display.getProfessorVirtualComboBox().getValue();
				if ( professorVirtualDTO == null )
				{
					display.getProxProfessorVirtualButton().disable();
					display.getAntProfessorVirtualButton().disable();
				}
				else
				{
					display.getProxProfessorVirtualButton().enable();
					display.getAntProfessorVirtualButton().enable();
				}
			}
		});
		
		display.getProxProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getProfessorTextField().getValue() != null)
				{
					ProfessoresServiceAsync service = Services.professores();

					service.getProxProfessor(cenarioDTO, display.getProfessorTextField().getValue(), "nome", new AsyncCallback< ProfessorDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo Professor", null );
							
						}
						@Override
						public void onSuccess(ProfessorDTO result) {
							display.getProfessorTextField().getStore().add(result);
							display.getProfessorTextField().setValue(result);
							if (display.getProfessorTextField().getValue() != null)
							{
								display.getProxProfessorButton().enable();
								display.getAntProfessorButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxProfessorButton().disable();
								display.getAntProfessorButton().disable();
							}
						}
					});
				}
			}
		});
		
		display.getAntProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getProfessorTextField().getValue() != null)
				{
					ProfessoresServiceAsync service = Services.professores();

					service.getAntProfessor(cenarioDTO, display.getProfessorTextField().getValue(), "nome", new AsyncCallback< ProfessorDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo professor", null );
							
						}
						@Override
						public void onSuccess(ProfessorDTO result) {
							display.getProfessorTextField().getStore().add(result);
							display.getProfessorTextField().setValue(result);
							if (display.getProfessorTextField().getValue() != null)
							{
								display.getProxProfessorButton().enable();
								display.getAntProfessorButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxProfessorButton().disable();
								display.getAntProfessorButton().disable();
							}
						}
					});
				}
			}
		});
		
		display.getProxCpfButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getCpfTextField().getValue() != null)
				{
					ProfessoresServiceAsync service = Services.professores();

					service.getProxProfessor(cenarioDTO, display.getCpfTextField().getValue(), "cpf", new AsyncCallback< ProfessorDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo Professor", null );
							
						}
						@Override
						public void onSuccess(ProfessorDTO result) {
							display.getCpfTextField().getStore().add(result);
							display.getCpfTextField().setValue(result);
							if (display.getCpfTextField().getValue() != null)
							{
								display.getProxCpfButton().enable();
								display.getAntCpfButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxCpfButton().disable();
								display.getAntCpfButton().disable();
							}
						}
					});
				}
			}
		});
		
		display.getAntCpfButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getCpfTextField().getValue() != null)
				{
					ProfessoresServiceAsync service = Services.professores();

					service.getAntProfessor(cenarioDTO, display.getCpfTextField().getValue(), "cpf", new AsyncCallback< ProfessorDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo professor", null );
							
						}
						@Override
						public void onSuccess(ProfessorDTO result) {
							display.getCpfTextField().getStore().add(result);
							display.getCpfTextField().setValue(result);
							if (display.getCpfTextField().getValue() != null)
							{
								display.getProxCpfButton().enable();
								display.getAntCpfButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxCpfButton().disable();
								display.getAntCpfButton().disable();
							}
						}
					});
				}
			}
		});
		
		display.getProxProfessorVirtualButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getProfessorVirtualComboBox().getValue() != null)
				{
					ProfessoresServiceAsync service = Services.professores();

					service.getProxProfessorVirtual(cenarioDTO, display.getProfessorVirtualComboBox().getValue(), "id", new AsyncCallback< ProfessorVirtualDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo Professor", null );
							
						}
						@Override
						public void onSuccess(ProfessorVirtualDTO result) {
							display.getProfessorVirtualComboBox().getStore().add(result);
							display.getProfessorVirtualComboBox().setValue(result);
							if (display.getProfessorVirtualComboBox().getValue() != null)
							{
								display.getProxProfessorVirtualButton().enable();
								display.getAntProfessorVirtualButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxProfessorVirtualButton().disable();
								display.getAntProfessorVirtualButton().disable();
							}
						}
					});
				}
			}
		});
		
		display.getAntProfessorVirtualButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				if (display.getProfessorVirtualComboBox().getValue() != null)
				{
					ProfessoresServiceAsync service = Services.professores();

					service.getAntProfessorVirtual(cenarioDTO, display.getProfessorVirtualComboBox().getValue(), "id", new AsyncCallback< ProfessorVirtualDTO >()
					{
						@Override
						public void onFailure(Throwable caught) {
							MessageBox.alert( "ERRO!", "Não foi possível obter o próximo professor", null );
							
						}
						@Override
						public void onSuccess(ProfessorVirtualDTO result) {
							display.getProfessorVirtualComboBox().getStore().add(result);
							display.getProfessorVirtualComboBox().setValue(result);
							if (display.getProfessorVirtualComboBox().getValue() != null)
							{
								display.getProxProfessorVirtualButton().enable();
								display.getAntProfessorVirtualButton().enable();
								display.getSubmitBuscaButton().el().click();
								
							}
							else
							{
								display.getProxProfessorVirtualButton().disable();
								display.getAntProfessorVirtualButton().disable();
							}
						}
					});
				}
			}
		});
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}

}
