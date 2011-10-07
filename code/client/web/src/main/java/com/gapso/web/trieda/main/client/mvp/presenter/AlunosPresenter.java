package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.view.AlunosFormView;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AlunosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class AlunosPresenter
	implements Presenter
{
	public interface Display
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		TextField< String > getNomeBuscaTextField();
		TextField< String > getMatriculaBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< AlunoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< AlunoDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private Display display;
	private CenarioDTO cenarioDTO;

	public AlunosPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.cenarioDTO = cenarioDTO;
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final AlunosServiceAsync service = Services.alunos();

		RpcProxy< PagingLoadResult< AlunoDTO > > proxy =
			new RpcProxy< PagingLoadResult< AlunoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< AlunoDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				String matricula = display.getMatriculaBuscaTextField().getValue();

				service.getAlunosList( nome, matricula, callback );
			}
		};

		this.display.setProxy( proxy );
	}

	private void setListeners()
	{
		this.display.getNewButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new AlunosFormPresenter( instituicaoEnsinoDTO,
					cenarioDTO, new AlunosFormView( cenarioDTO, new AlunoDTO() ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				AlunoDTO alunoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new AlunosFormPresenter( instituicaoEnsinoDTO, cenarioDTO,
					new AlunosFormView( cenarioDTO, alunoDTO ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				List< AlunoDTO > list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				final AlunosServiceAsync service = Services.alunos();

				service.removeAlunos( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover o(s) aluno(s)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						Info.display( "Removido", "Aluno(s) removido(s) com sucesso!" );
					}
				});
			}
		});

		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getNomeBuscaTextField().setValue( null );
				display.getMatriculaBuscaTextField().setValue( null );
				display.getGrid().updateList();
			}
		});

		this.display.getSubmitBuscaButton().addSelectionListener(
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
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
