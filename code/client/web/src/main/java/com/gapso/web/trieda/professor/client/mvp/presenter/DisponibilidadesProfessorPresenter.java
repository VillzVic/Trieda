package com.gapso.web.trieda.professor.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.professor.client.mvp.view.DisponibilidadesProfessorFormView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisponibilidadesProfessorPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getDiasDeAulaButton();
		SimpleGrid< SemanaLetivaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< SemanaLetivaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display; 
	private CenarioDTO cenario;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor;

	public DisponibilidadesProfessorPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenario,
		UsuarioDTO usuario, Display display, boolean isVisaoProfessor )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		this.cenario = cenario;
		this.usuario = usuario;
		this.isVisaoProfessor = isVisaoProfessor;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final SemanasLetivaServiceAsync service = Services.semanasLetiva();

		RpcProxy< PagingLoadResult< SemanaLetivaDTO > > proxy
			= new RpcProxy< PagingLoadResult< SemanaLetivaDTO > >()
		{
			@Override
			public void load( Object loadConfig, AsyncCallback< PagingLoadResult< SemanaLetivaDTO > > callback )
			{
				service.getBuscaList( "", "", (PagingLoadConfig) loadConfig, callback );
			}
		};

		display.setProxy( proxy );
	}
	
	private void setListeners()
	{
		display.getDiasDeAulaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final SemanaLetivaDTO semanaLetivaDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				final ProfessorDTO professorDTO = new ProfessorDTO();
				professorDTO.setId( usuario.getProfessorId() );

				Services.professores().getHorariosDisponiveis( professorDTO, semanaLetivaDTO,
					new AbstractAsyncCallbackWithDefaultOnFailure< List< HorarioDisponivelCenarioDTO > >( display )
				{
					@Override
					public void onSuccess( List< HorarioDisponivelCenarioDTO > result )
					{
						Presenter presenter = new DisponibilidadesProfessorFormPresenter(
							instituicaoEnsinoDTO, cenario, semanaLetivaDTO,
							new DisponibilidadesProfessorFormView( professorDTO, result ) );

						presenter.go( null );
					}
				});
			}
		});
	}

	public boolean isVisaoProfessor()
	{
		return isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) display.getComponent() );
	}
}
