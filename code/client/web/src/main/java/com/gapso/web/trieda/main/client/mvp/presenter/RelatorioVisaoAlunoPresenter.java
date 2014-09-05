package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.services.AlunosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoPresenter;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.AlunosMatriculaComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class RelatorioVisaoAlunoPresenter extends RelatorioVisaoPresenter{
	public interface Display extends RelatorioVisaoPresenter.Display{
		RelatorioVisaoAlunoFiltro getFiltro();
		AlunosComboBox getAlunoTextField();
		AlunosMatriculaComboBox getAlunoMatriculaTextField();
		Button getProxAlunoButton();
		Button getAntAlunoButton();
		Button getProxMatriculaButton();
		Button getAntMatriculaButton();
	}
	
	public RelatorioVisaoAlunoPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display)
	{
		super(instituicaoEnsinoDTO, cenario, display);
	}
	
	protected void setListeners(){
		super.setListeners();
		
		final Display display = (Display) this.getDisplay();
		
		if (display.getAlunoTextField() != null) {
			display.getAlunoTextField().addSelectionChangedListener(new SelectionChangedListener<AlunoDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<AlunoDTO> se){
					if (display.getAlunoTextField().getValue() != null){
						display.getAlunoMatriculaTextField().setValue(null);
					}
					else
					{
						display.getProxAlunoButton().disable();
						display.getAntAlunoButton().disable();
					}
				}
			});
		}
		
		if (display.getAlunoMatriculaTextField() != null) {
			display.getAlunoMatriculaTextField().addSelectionChangedListener(new SelectionChangedListener<AlunoDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<AlunoDTO> se){
					if (display.getAlunoMatriculaTextField().getValue() != null){
						display.getAlunoTextField().setValue(null);
					}
					else
					{
						display.getProxMatriculaButton().disable();
						display.getAntMatriculaButton().disable();
					}
				}
			});
		}
		
		this.display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				final AlunoDTO alunoDTO = display.getAlunoTextField().getValue();
				if ( alunoDTO == null )
				{
					display.getProxAlunoButton().disable();
					display.getAntAlunoButton().disable();
				}
				else
				{
					display.getProxAlunoButton().enable();
					display.getAntAlunoButton().enable();
				}
				
				final AlunoDTO alunoMatriculaDTO = display.getAlunoMatriculaTextField().getValue();
				if ( alunoMatriculaDTO == null )
				{
					display.getProxMatriculaButton().disable();
					display.getAntMatriculaButton().disable();
				}
				else
				{
					display.getProxMatriculaButton().enable();
					display.getAntMatriculaButton().enable();
				}
			}
		});
		
		display.getProxAlunoButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (display.getAlunoTextField().getValue() != null)
					{
						AlunosServiceAsync service = Services.alunos();

						service.getProxAluno(cenarioDTO, display.getAlunoTextField().getValue(), "nome", new AsyncCallback< AlunoDTO >()
						{
							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!", "Não foi possível obter o próximo aluno", null );
								
							}
							@Override
							public void onSuccess(AlunoDTO result) {
								display.getAlunoTextField().setValue(result);
								if (display.getAlunoTextField().getValue() != null)
								{
									display.getProxAlunoButton().enable();
									display.getAntAlunoButton().enable();
									display.getSubmitBuscaButton().el().click();
									
								}
								else
								{
									display.getProxAlunoButton().disable();
									display.getAntAlunoButton().disable();
								}
							}
						});
					}
				}
			});
			
			display.getAntAlunoButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (display.getAlunoTextField().getValue() != null)
					{
						AlunosServiceAsync service = Services.alunos();

						service.getAntAluno(cenarioDTO, display.getAlunoTextField().getValue(), "nome", new AsyncCallback< AlunoDTO >()
						{
							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!", "Não foi possível obter o próximo aluno", null );
								
							}
							@Override
							public void onSuccess(AlunoDTO result) {
								display.getAlunoTextField().setValue(result);
								if (display.getAlunoTextField().getValue() != null)
								{
									display.getProxAlunoButton().enable();
									display.getAntAlunoButton().enable();
									display.getSubmitBuscaButton().el().click();
									
								}
								else
								{
									display.getProxAlunoButton().disable();
									display.getAntAlunoButton().disable();
								}
							}
						});
					}
				}
			});
			
			display.getProxMatriculaButton().addSelectionListener(
					new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (display.getAlunoMatriculaTextField().getValue() != null)
					{
						AlunosServiceAsync service = Services.alunos();

						service.getProxAluno(cenarioDTO, display.getAlunoMatriculaTextField().getValue(), "matricula", new AsyncCallback< AlunoDTO >()
						{
							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!", "Não foi possível obter o próximo aluno", null );
								
							}
							@Override
							public void onSuccess(AlunoDTO result) {
								display.getAlunoMatriculaTextField().setValue(result);
								if (display.getAlunoMatriculaTextField().getValue() != null)
								{
									display.getProxMatriculaButton().enable();
									display.getAntMatriculaButton().enable();
									display.getSubmitBuscaButton().el().click();
									
								}
								else
								{
									display.getProxMatriculaButton().disable();
									display.getAntMatriculaButton().disable();
								}
							}
						});
					}
				}
			});
				
			display.getAntMatriculaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					if (display.getAlunoMatriculaTextField().getValue() != null)
					{
						AlunosServiceAsync service = Services.alunos();

						service.getAntAluno(cenarioDTO, display.getAlunoMatriculaTextField().getValue(), "matricula", new AsyncCallback< AlunoDTO >()
						{
							@Override
							public void onFailure(Throwable caught) {
								MessageBox.alert( "ERRO!", "Não foi possível obter o próximo aluno", null );
								
							}
							@Override
							public void onSuccess(AlunoDTO result) {
								display.getAlunoMatriculaTextField().setValue(result);
								if (display.getAlunoMatriculaTextField().getValue() != null)
								{
									display.getProxMatriculaButton().enable();
									display.getAntMatriculaButton().enable();
									display.getSubmitBuscaButton().el().click();
									
								}
								else
								{
									display.getProxMatriculaButton().disable();
									display.getAntMatriculaButton().disable();
								}
							}
						});
					}
				}
			});
	}
	
	
}
