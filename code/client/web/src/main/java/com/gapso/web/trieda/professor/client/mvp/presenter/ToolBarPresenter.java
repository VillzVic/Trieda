package com.gapso.web.trieda.professor.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessoresPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.HorarioDisponivelProfessorFormPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessoresDisciplinaPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.mvp.view.CampusProfessoresView;
import com.gapso.web.trieda.shared.mvp.view.HorarioDisponivelProfessorFormView;
import com.gapso.web.trieda.shared.mvp.view.ProfessoresDisciplinaView;
import com.gapso.web.trieda.shared.mvp.view.RelatorioVisaoProfessorView;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ToolBarPresenter
	implements Presenter
{
	public interface Display
	{
		Component getComponent();
		Button getProfessoresCampusListprofessoresBt();
		Button getRelatorioVisaoProfessorButton();
		Button getDisponibilidadeProfessorButton();
		Button getProfessoresDisciplinaListProfessoresButton();
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO masterData;
	private UsuarioDTO usuario;
	private ProfessorDTO professor;
	private Display toolBar;
	private GTab gTab;

	public ToolBarPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO masterData, UsuarioDTO usuario, Display toolBar )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.masterData = masterData;
		this.usuario = usuario;
		this.toolBar = toolBar;
		
		professor = new ProfessorDTO();
		professor.setId(usuario.getProfessorId());
		professor.setCpf(usuario.getProfessorCpf());

		addListeners();
	}

	private void addListeners()
	{
		this.toolBar.getProfessoresCampusListprofessoresBt().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CampusProfessoresPresenter(
					instituicaoEnsinoDTO, masterData, usuario,
					new CampusProfessoresView( masterData, usuario ), true );

				presenter.go( gTab );
			}
		});

		this.toolBar.getProfessoresDisciplinaListProfessoresButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ProfessoresDisciplinaPresenter(
					instituicaoEnsinoDTO, masterData, usuario,
					new ProfessoresDisciplinaView( masterData, usuario ), true );

				presenter.go( gTab );
			}
		});

		this.toolBar.getDisponibilidadeProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Services.professores().getHorariosDisponiveis( professor,
						new AsyncCallback< List< HorarioDisponivelCenarioDTO > >()
					{
						@Override
						public void onFailure( Throwable caught )
						{
							MessageBox.alert( "ERRO!", "Não foi posssível exibir as tela de disponiblidade", null );
						}

						@Override
						public void onSuccess(
							List< HorarioDisponivelCenarioDTO > result )
						{
							Presenter presenter = new HorarioDisponivelProfessorFormPresenter(
								instituicaoEnsinoDTO, masterData, 
								new HorarioDisponivelProfessorFormView( professor, result ) );

							presenter.go( null );
						}
					});
			}
		});

		this.toolBar.getRelatorioVisaoProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new RelatorioVisaoProfessorPresenter(
					instituicaoEnsinoDTO, masterData, usuario,
					new RelatorioVisaoProfessorView( masterData, usuario, true ), true );

				presenter.go( gTab );
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		AppPresenter.Display container = (AppPresenter.Display) widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent( this.toolBar.getComponent() );
	}
}
