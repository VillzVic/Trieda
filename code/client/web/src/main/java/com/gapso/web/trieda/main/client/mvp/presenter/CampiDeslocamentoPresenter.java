package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.CampiServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.google.gwt.user.client.ui.Widget;

public class CampiDeslocamentoPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		Button getSaveButton();
		Button getCancelButton();
		Button getSimetricaButton();
		Button getImportExcelButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		DeslocamentoGrid< DeslocamentoCampusDTO > getGrid();
		Component getComponent();
	}

	private Display display; 
	private GTab gTab;
	private CenarioDTO cenario;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;

	public CampiDeslocamentoPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenario, Display display )
	{
		this.display = display;
		this.cenario = cenario;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;

		//configureProxy();
		setListeners();
	}

//	private void configureProxy() {
//		CampiServiceAsync service = Services.campi();
//		final FutureResult<List<DeslocamentoCampusDTO>> futureDeslocamentoCampusDTOList = new FutureResult<List<DeslocamentoCampusDTO>>();
//		service.getDeslocamentos(futureDeslocamentoCampusDTOList);
//		
//		FutureSynchronizer synch = new FutureSynchronizer(futureDeslocamentoCampusDTOList);
//		synch.addCallback(new AsyncCallback<Boolean>() {
//			@Override
//			public void onFailure(Throwable caught) {
//				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
//			}
//			@Override
//			public void onSuccess(Boolean result) {
//				display.getGrid().updateList(futureDeslocamentoCampusDTOList.result());	
//			}
//		});
//	}

	private void setListeners()
	{
		this.display.getSaveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DeslocamentoCampusDTO> list = display.getGrid().getStore().getModels();
				CampiServiceAsync service = Services.campi();
				service.saveDeslocamento(cenario, list, new AbstractAsyncCallbackWithDefaultOnFailure<Void>(display) {
					@Override
					public void onSuccess(Void result) {
						display.getGrid().getStore().commitChanges();
						Info.display("Salvo", "Deslocamentos atualizado com sucesso");
					}
				});
			}
		});

		this.display.getCancelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().getStore().rejectChanges();
				Info.display("Cancelado", "Alterações canceladas com sucesso");
			}
		});

		this.display.getSimetricaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().igualarOrigemDestino();
			}
		});
		
		this.display.getImportExcelButton().addSelectionListener(
				new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					ExcelParametros parametros = new ExcelParametros(
							ExcelInformationType.CAMPI_DESLOCAMENTO, instituicaoEnsinoDTO );

					ImportExcelFormView importExcelFormView
						= new ImportExcelFormView( parametros, null );

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
					
					ExcelParametros parametros = new ExcelParametros(
						ExcelInformationType.CAMPI_DESLOCAMENTO, instituicaoEnsinoDTO, fileExtension );

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
						ExcelInformationType.CAMPI_DESLOCAMENTO, instituicaoEnsinoDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});
	}

	@Override
	public void go( Widget widget )
	{
		this.gTab = (GTab)widget;
		this.gTab.add( (GTabItem) this.display.getComponent() );
	}

}
