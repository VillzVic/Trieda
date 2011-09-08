package com.gapso.web.trieda.professor.client.mvp.presenter;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.professor.client.mvp.view.DisponibilidadesProfessorView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessoresPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessoresDisciplinaPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.mvp.view.CampusProfessoresView;
import com.gapso.web.trieda.shared.mvp.view.ProfessoresDisciplinaView;
import com.gapso.web.trieda.shared.mvp.view.RelatorioVisaoProfessorView;
import com.gapso.web.trieda.shared.util.view.GTab;
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
	private Display toolBar;
	private GTab gTab;

	public ToolBarPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO masterData, UsuarioDTO usuario, Display toolBar )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.masterData = masterData;
		this.usuario = usuario;
		this.toolBar = toolBar;

		addListeners();
	}

	private void addListeners()
	{
		toolBar.getProfessoresCampusListprofessoresBt().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new CampusProfessoresPresenter(
					instituicaoEnsinoDTO, masterData, usuario,
					new CampusProfessoresView( usuario ), true );

				presenter.go( gTab );
			}
		});

		toolBar.getProfessoresDisciplinaListProfessoresButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new ProfessoresDisciplinaPresenter(
					instituicaoEnsinoDTO, masterData, usuario,
					new ProfessoresDisciplinaView( usuario ), true );

				presenter.go( gTab );
			}
		});

		toolBar.getDisponibilidadeProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new DisponibilidadesProfessorPresenter( instituicaoEnsinoDTO,
					masterData, usuario, new DisponibilidadesProfessorView( usuario ), true );

				presenter.go( gTab );
			}
		});

		toolBar.getRelatorioVisaoProfessorButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new RelatorioVisaoProfessorPresenter(
					instituicaoEnsinoDTO, masterData, usuario,
					new RelatorioVisaoProfessorView( usuario, true ), true );

				presenter.go( gTab );
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		AppPresenter.Display container = (AppPresenter.Display) widget;
		this.gTab = container.getGTab();
		container.getPanel().setTopComponent( toolBar.getComponent() );
	}
}
