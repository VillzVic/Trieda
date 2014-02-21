package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.GradeHorariaProfessorView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public abstract class ProfessoresListPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		TextField< String > getCpfBuscaTextField();
		TipoContratoComboBox getTipoContratoBuscaComboBox();
		TitulacaoComboBox getTitulacaoBuscaComboBox();
		AreaTitulacaoComboBox getAreaTitulacaoBuscaComboBox();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		Button getGradeHorariaButton();
		SimpleGrid< ProfessorDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< ProfessorDTO > > proxy );
		RelatorioProfessorFiltro getProfessorFiltro();
	}

	protected InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	protected CenarioDTO cenario;
	protected Long campusDTO;
	protected Display display;
	private GTab gTab;

	public ProfessoresListPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Long campusDTO, Display display )
	{
		this.instituicaoEnsinoDTO  = instituicaoEnsinoDTO;
		this.campusDTO = campusDTO;
		this.cenario = cenarioDTO;
		this.display = display;

		configureProxy();
		setListeners();
	}

	protected abstract void configureProxy();

	protected void setListeners()
	{
		display.getGradeHorariaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final ProfessorDTO professorDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Services.professores().getHorariosDisponiveis( professorDTO,
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
						Presenter presenter = new GradeHorariaProfessorFormPresenter(
							new GradeHorariaProfessorView( cenario, professorDTO ) );

						presenter.go( null );
					}
				});
			}
		});

		display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getCpfBuscaTextField().setValue( null );
				display.getTipoContratoBuscaComboBox().setValue( null );
				display.getTitulacaoBuscaComboBox().setValue( null );
				display.getAreaTitulacaoBuscaComboBox().setValue( null );
				display.getGrid().updateList();
			}
		});

		display.getSubmitBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getGrid().updateList();
			}
		});
	}

	@Override
	public void go( Widget widget )
	{
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
