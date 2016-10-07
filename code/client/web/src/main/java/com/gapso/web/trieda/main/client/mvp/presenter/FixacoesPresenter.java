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
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.view.FixacaoFormView;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;
import com.googlecode.future.FutureResult;
import com.googlecode.future.FutureSynchronizer;

public class FixacoesPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		Button getNewButton();
		Button getEditButton();
		Button getRemoveButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		TextField< String > getCodigoBuscaTextField();
		Button getSubmitBuscaButton();
		Button getResetBuscaButton();
		SimpleGrid< FixacaoDTO > getGrid();
		Component getComponent();
		void setProxy( RpcProxy< PagingLoadResult< FixacaoDTO > > proxy );
	}

	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenario;
	private Display display; 

	public FixacoesPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenario, Display display )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenario = cenario;
		this.display = display;

		configureProxy();
		setListeners();
	}

	private void configureProxy() {
		RpcProxy<PagingLoadResult<FixacaoDTO>> proxy = new RpcProxy<PagingLoadResult<FixacaoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<FixacaoDTO>> callback) {
				String codigo = display.getCodigoBuscaTextField().getValue();
				Services.fixacoes().getBuscaList(codigo, (PagingLoadConfig)loadConfig, callback);
			}
		};
		display.setProxy(proxy);
	}
	
	private void setListeners() {
		display.getNewButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected( ButtonEvent ce )
			{
				Presenter presenter = new FixacaoFormPresenter( instituicaoEnsinoDTO,
					cenario, new FixacaoFormView( cenario, new FixacaoDTO(), null, null, null, null, null, null, false ), display.getGrid() );

				presenter.go( null );
			}
		});
		display.getEditButton().addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				final FixacaoDTO fixacaoDTO = display.getGrid().getGrid().getSelectionModel().getSelectedItem();
				
				final FutureResult<ProfessorDTO> futureProfessorDTO = new FutureResult<ProfessorDTO>();
				final FutureResult<DisciplinaDTO> futureDisciplinaDTO = new FutureResult<DisciplinaDTO>();
			
				final FutureResult<CampusDTO> futureCampusDTO = new FutureResult<CampusDTO>();
				final FutureResult<UnidadeDTO> futureUnidadeDTO = new FutureResult<UnidadeDTO>();
				final FutureResult<SalaDTO> futureSalaDTO = new FutureResult<SalaDTO>();
				final FutureResult<List<HorarioDisponivelCenarioDTO>> futureHorariosDTO = new FutureResult<List<HorarioDisponivelCenarioDTO>>();
				
				Services.professores().getProfessor(fixacaoDTO.getProfessorId(), futureProfessorDTO);
				Services.disciplinas().getDisciplina(fixacaoDTO.getDisciplinaId(), futureDisciplinaDTO);
				
				Services.campi().getCampus(fixacaoDTO.getCampusId(), futureCampusDTO);
				Services.unidades().getUnidade(fixacaoDTO.getUnidadeId(), futureUnidadeDTO);
				Services.salas().getSala(fixacaoDTO.getSalaId(), futureSalaDTO);
				Services.fixacoes().getHorariosSelecionados(fixacaoDTO, futureHorariosDTO);
				
				FutureSynchronizer synch = new FutureSynchronizer(futureProfessorDTO, futureDisciplinaDTO, futureCampusDTO, futureUnidadeDTO, futureSalaDTO, futureHorariosDTO);
				
				synch.addCallback(new AsyncCallback<Boolean>() {
					@Override
					public void onFailure(Throwable caught) {
						caught.printStackTrace();
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Boolean result) {
						ProfessorDTO professorDTO = futureProfessorDTO.result();
						DisciplinaDTO disciplinaDTO = futureDisciplinaDTO.result();
						
						CampusDTO campusDTO = futureCampusDTO.result();
						UnidadeDTO unidadeDTO = futureUnidadeDTO.result();
						SalaDTO salaDTO = futureSalaDTO.result();
						List<HorarioDisponivelCenarioDTO> horariosDTOList = futureHorariosDTO.result();
						
						Presenter presenter = new FixacaoFormPresenter( instituicaoEnsinoDTO,
							cenario, new FixacaoFormView( cenario, fixacaoDTO, professorDTO, disciplinaDTO,
								campusDTO, unidadeDTO, salaDTO, horariosDTOList, false ), display.getGrid() );

						presenter.go( null );
					}
				});
				
			}
		});
		display.getRemoveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<FixacaoDTO> list = display.getGrid().getGrid().getSelectionModel().getSelectedItems();
				Services.fixacoes().remove(list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().updateList();
						Info.display("Removido", "Item removido com sucesso!");
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
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.FIXACOES, instituicaoEnsinoDTO, cenario );

				ImportExcelFormView importExcelFormView = new ImportExcelFormView( parametros, display.getGrid() );

				importExcelFormView.show();
			}
		});

		this.display.getExportXlsExcelButton().addSelectionListener(
			new SelectionListener< MenuEvent >()
		{
			@Override
			public void componentSelected( MenuEvent ce )
			{
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.FIXACOES, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros, display.getI18nConstants(), display.getI18nMessages() );

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
				
				ExcelParametros parametros = new ExcelParametros(ExcelInformationType.FIXACOES, instituicaoEnsinoDTO, cenario, fileExtension );

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(parametros, display.getI18nConstants(), display.getI18nMessages() );

				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});

		display.getResetBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getCodigoBuscaTextField().setValue(null);
				display.getGrid().updateList();
			}
		});
		display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().updateList();
			}
		});
	}
	
	@Override
	public void go(Widget widget) {
		GTab tab = (GTab)widget;
		tab.add((GTabItem)display.getComponent());
	}

}
