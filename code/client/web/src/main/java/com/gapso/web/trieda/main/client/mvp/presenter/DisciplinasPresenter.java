
package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.DisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.DisponibilidadesFormView;
import com.gapso.web.trieda.main.client.mvp.view.DivisaoCreditoDisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.GruposSalasAssociarDisciplinaView;
import com.gapso.web.trieda.main.client.mvp.view.HorarioDisponivelDisciplinaFormView;
import com.gapso.web.trieda.main.client.mvp.view.SalasAssociarDisciplinaView;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean;
import com.gapso.web.trieda.shared.util.view.DificuldadeComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.OperadorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.TipoDisciplinaComboBox;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class DisciplinasPresenter	
implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Button getDivisaoCreditoButton();
		Button getDisponibilidadeButton();
		TextField< String > getNomeBuscaTextField();
		TextField< String > getCodigoBuscaTextField();
		TipoDisciplinaComboBox getTipoDisciplinaBuscaComboBox();
		NumberField getCreditosTeoricosField();
		OperadorComboBox getOperadorCreditosTeoricos();
		NumberField getCreditosPraticosField();
		OperadorComboBox getOperadorCreditosPraticos();
		ComboBoxBoolean getExigeLaboratorio();
		DificuldadeComboBox getNivelDificuldade();
		NumberField getMaxAlunosTeoricosField();
		OperadorComboBox getOperadorMaxAlunosTeoricos();
		NumberField getMaxAlunosPraticosField();
		OperadorComboBox getOperadorMaxAlunosPraticos();
		ComboBoxBoolean getAulasContinuas();
		ComboBoxBoolean getProfessorUnico();
		ComboBoxBoolean getUsaSabado();
		ComboBoxBoolean getUsaDomingo();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		Button getAssociarSalasButton();
		Button getAssociarGruposSalasButton();
		SimpleGrid< DisciplinaDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< DisciplinaDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display;
	private GTab gTab;

	public DisciplinasPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy()
	{
		final DisciplinasServiceAsync service = Services.disciplinas();

		RpcProxy< PagingLoadResult< DisciplinaDTO > > proxy =
			new RpcProxy< PagingLoadResult< DisciplinaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< DisciplinaDTO > > callback )
			{
				String nome = display.getNomeBuscaTextField().getValue();
				String codigo = display.getCodigoBuscaTextField().getValue();
				String operadorCreditosTeorico = (display.getOperadorCreditosTeoricos().getValue()==null)?null: display.getOperadorCreditosTeoricos().getValue().getValue().getOperadorSQL();
				Integer creditosTeorico = display.getCreditosTeoricosField().getValue() == null?null:display.getCreditosTeoricosField().getValue().intValue();
				String operadorCreditosPratico = (display.getOperadorCreditosPraticos().getValue()==null)?null: display.getOperadorCreditosPraticos().getValue().getValue().getOperadorSQL();
				Integer creditosPratico = display.getCreditosPraticosField().getValue() == null?null:display.getCreditosPraticosField().getValue().intValue();
				Boolean exigeLaboratorio = (display.getExigeLaboratorio().getValue()==null)?null:display.getExigeLaboratorio().getValue().getValue().getValue();

				String operadorMaxAlunosTeorico = (display.getOperadorMaxAlunosTeoricos().getValue()==null)?null: display.getOperadorMaxAlunosTeoricos().getValue().getValue().getOperadorSQL();
				Integer maxAlunosTeorico = display.getMaxAlunosTeoricosField().getValue() == null?null:display.getMaxAlunosTeoricosField().getValue().intValue();
				String operadorMaxAlunosPratico = (display.getOperadorMaxAlunosPraticos().getValue()==null)?null: display.getOperadorMaxAlunosPraticos().getValue().getValue().getOperadorSQL();
				Integer maxAlunosPratico = display.getMaxAlunosPraticosField().getValue() == null?null:display.getMaxAlunosPraticosField().getValue().intValue();
				Boolean aulasContinuas = (display.getAulasContinuas().getValue()==null)?null:display.getAulasContinuas().getValue().getValue().getValue();
				Boolean professorUnico = (display.getProfessorUnico().getValue()==null)?null:display.getProfessorUnico().getValue().getValue().getValue();
				Boolean usaSabado = (display.getUsaSabado().getValue()==null)?null:display.getUsaSabado().getValue().getValue().getValue();
				Boolean usaDomingo = (display.getUsaDomingo().getValue()==null)?null:display.getUsaDomingo().getValue().getValue().getValue();

				
				String dificuldade =  (display.getNivelDificuldade().getValue()==null)?null:display.getNivelDificuldade().getValue().getValue().name();
				

				TipoDisciplinaDTO tipoDisciplinaDTO
					= display.getTipoDisciplinaBuscaComboBox().getValue();

				service.getBuscaList( cenario, nome, codigo, tipoDisciplinaDTO, 
						operadorCreditosTeorico, creditosTeorico, operadorCreditosPratico, creditosPratico,
						exigeLaboratorio, operadorMaxAlunosTeorico, maxAlunosTeorico, operadorMaxAlunosPratico, maxAlunosPratico,
						aulasContinuas, professorUnico, usaSabado, usaDomingo, dificuldade,
					(PagingLoadConfig) loadConfig, callback );
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
				Presenter presenter = new DisciplinaFormPresenter( instituicaoEnsinoDTO, cenario,
					new DisciplinaFormView( new DisciplinaDTO(), null, cenario ), display.getGrid() );

				presenter.go( null );
			}
		});

		this.display.getEditButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DisciplinaDTO disciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				final DisciplinasServiceAsync service = Services.disciplinas();

				service.getTipoDisciplina( disciplinaDTO.getTipoId(), new AsyncCallback< TipoDisciplinaDTO >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						caught.printStackTrace();
					}

					@Override
					public void onSuccess( TipoDisciplinaDTO tipoDisciplinaDTO )
					{
						Presenter presenter = new DisciplinaFormPresenter( instituicaoEnsinoDTO, cenario,
							new DisciplinaFormView( disciplinaDTO, tipoDisciplinaDTO, cenario ), display.getGrid() );

						presenter.go( null );
					}
				});
			}
		});

		this.display.getRemoveButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				List< DisciplinaDTO > list
					= display.getGrid().getGrid().getSelectionModel().getSelectedItems();

				final DisciplinasServiceAsync service = Services.disciplinas();

				service.remove( list, new AsyncCallback< Void >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!", "Não foi possível remover a(s) disciplina(s)", null );
					}

					@Override
					public void onSuccess( Void result )
					{
						display.getGrid().updateList();
						Info.display( "Removido", "Item(s) removido(s) com sucesso!" );
					}
				});
			}
		});

		this.display.getImportExcelButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DISCIPLINAS, instituicaoEnsinoDTO, cenario );

				ImportExcelFormView importExcelFormView
					= new ImportExcelFormView( parametros, display.getGrid() );

				importExcelFormView.show();
				display.getGrid().updateList();
			}
		});

		this.display.getExportXlsExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					ExcelInformationType.DISCIPLINAS, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
		
		this.display.getExportXlsxExcelButton().addSelectionListener(
				new SelectionListener< MenuEvent >()
			{
				@Override
				public void componentSelected( MenuEvent ce )
				{
					String fileExtension = "xlsx";
					
					ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.DISCIPLINAS, instituicaoEnsinoDTO, cenario, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});

		this.display.getDisponibilidadeButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DisciplinaDTO disciplinaDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				Presenter presenter = new DisponibilidadesFormPresenter(
					instituicaoEnsinoDTO, cenario, disciplinaDTO.getId(), 
					new DisponibilidadesFormView( cenario, disciplinaDTO.getId(), DisponibilidadeDTO.DISCIPLINA, disciplinaDTO.getCodigo() ) );

				presenter.go( null );

/*				Services.disciplinas().getHorariosDisponiveis( disciplinaDTO,
					new AsyncCallback< List< HorarioDisponivelCenarioDTO > >()
				{
					@Override
					public void onFailure( Throwable caught )
					{
						MessageBox.alert( "ERRO!",
							"Não foi posssível exibir as tela de disponiblidade", null );
					}

					@Override
					public void onSuccess( List< HorarioDisponivelCenarioDTO > result )
					{
						Presenter presenter = new HorarioDisponivelDisciplinaFormPresenter(
							instituicaoEnsinoDTO, cenario, new HorarioDisponivelDisciplinaFormView( disciplinaDTO, result ) );

						presenter.go( null );
					}
				});*/
			}
		});

		this.display.getDivisaoCreditoButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DisciplinaDTO disciplinaDTO
					= display.getGrid().getGrid().getSelectionModel().getSelectedItem();

				Services.disciplinas().getDivisaoCredito( disciplinaDTO,
					new AbstractAsyncCallbackWithDefaultOnFailure< DivisaoCreditoDTO >( display )
				{
					@Override
					public void onSuccess( DivisaoCreditoDTO result )
					{
						Presenter presenter = new DivisaoCreditoDisciplinaFormPresenter(
							instituicaoEnsinoDTO, cenario,
							new DivisaoCreditoDisciplinaFormView( result, disciplinaDTO ) );

						presenter.go( null );
					}
				});
			}
		});
		
		this.display.getAssociarSalasButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				final DisciplinaDTO disciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				Presenter presenter = new SalasAssociarDisciplinaPresenter(
						instituicaoEnsinoDTO, cenario,
						new SalasAssociarDisciplinaView( cenario, disciplinaDTO ) );
					presenter.go( gTab );
			}
		});

		this.display.getAssociarGruposSalasButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					final DisciplinaDTO disciplinaDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
					Presenter presenter = new GruposSalasAssociarDisciplinaPresenter(
							instituicaoEnsinoDTO, cenario,
							new GruposSalasAssociarDisciplinaView( cenario, disciplinaDTO ) );
						presenter.go( gTab );
				}
			});
		
		this.display.getResetBuscaButton().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				display.getNomeBuscaTextField().setValue( null );
				display.getCodigoBuscaTextField().setValue( null );
				display.getTipoDisciplinaBuscaComboBox().setValue( null );
				display.getCreditosTeoricosField().setValue( null );
				display.getOperadorCreditosTeoricos().setValue( null );
				display.getCreditosPraticosField().setValue( null );
				display.getOperadorCreditosPraticos().setValue( null );
				display.getExigeLaboratorio().setValue( null );
				display.getNivelDificuldade().clearSelections();
				display.getMaxAlunosTeoricosField().setValue( null );
				display.getOperadorMaxAlunosTeoricos().setValue( null );
				display.getMaxAlunosPraticosField().setValue( null );
				display.getOperadorMaxAlunosPraticos().setValue( null );
				display.getAulasContinuas().setValue( null);
				display.getProfessorUnico().setValue( null );
				display.getUsaSabado().setValue( null );
				display.getUsaDomingo().setValue( null );

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
		this.gTab = (GTab) widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}
}
