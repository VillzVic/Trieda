package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.gapso.web.trieda.main.client.mvp.view.GradeHorariaProfessorView;
import com.gapso.web.trieda.main.client.mvp.view.MotivosUsoProfessorVirtualView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.AtendimentosServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class ProfessoresVirtuaisPresenter
	 implements Presenter
 {
 	public interface Display extends ITriedaI18nGateway
 	{
 		TitulacaoComboBox getTitulacaoBuscaComboBox();
 		Button getSubmitBuscaButton();
 		Button getResetBuscaButton();
 		SimpleGrid< ProfessorVirtualDTO > getGrid();
 		Component getComponent();
 		Button getGradeHorariaButton();
 		Button getMotivoUsoButton();
 		void setProxy( RpcProxy< PagingLoadResult< ProfessorVirtualDTO > > proxy );
 	}

 	private CenarioDTO cenario;
 	private Long campusId;
 	private Display display;
 	private GTab gTab;

 	public ProfessoresVirtuaisPresenter(
 		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
 		CenarioDTO cenarioDTO, Long campusID, Display display )
 	{
 		this.cenario = cenarioDTO;
 		this.campusId = campusID;
 		this.display = display;

 		configureProxy();
 		setListeners();
 	}

 	private void configureProxy()
 	{
 		final AtendimentosServiceAsync service = Services.atendimentos();

 		RpcProxy< PagingLoadResult< ProfessorVirtualDTO > > proxy =
 			new RpcProxy< PagingLoadResult< ProfessorVirtualDTO > >()
 		{
 			@Override
 			public void load( Object loadConfig,
 				AsyncCallback< PagingLoadResult< ProfessorVirtualDTO > > callback)
 			{
 				TitulacaoDTO titulacaoDTO
 					= display.getTitulacaoBuscaComboBox().getValue();

 				if(campusId == null){
 					service.getProfessoresVirtuais( cenario, titulacaoDTO, (PagingLoadConfig) loadConfig, callback );
 				}else{
 					service.getProfessoresVirtuais( cenario, titulacaoDTO, campusId, (PagingLoadConfig) loadConfig, callback );
 				}
 				display.getGrid().getGrid().getView().setEmptyText("Não foram utilizados professores virtuais" +
 						" ou o modo de otimização utilizado não foi o operacional");
 				
 			}
 		};

 		display.setProxy( proxy );
 	}

 	private void setListeners()
 	{
 		display.getResetBuscaButton().addSelectionListener(
 			new SelectionListener< ButtonEvent >()
 		{
 			@Override
 			public void componentSelected( ButtonEvent ce )
 			{
 				display.getTitulacaoBuscaComboBox().setValue( null );
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
 		
		display.getGradeHorariaButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final ProfessorVirtualDTO professorVirtualDTO
						= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

					Presenter presenter = new GradeHorariaProfessorFormPresenter(
						new GradeHorariaProfessorView( cenario, professorVirtualDTO ) );
	
					presenter.go( null );
				}
			});
		
		display.getMotivoUsoButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final ProfessorVirtualDTO professorVirtualDTO
						= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

					Presenter presenter = new MotivosUsoProfessorVirtualPresenter(cenario,
						new MotivosUsoProfessorVirtualView( cenario, professorVirtualDTO ), display.getGrid() );
	
					presenter.go( null );
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