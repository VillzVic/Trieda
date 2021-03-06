package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.Info;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.services.UnidadesServiceAsync;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ImportExcelFormView;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Widget;

public class UnidadesDeslocamentoPresenter
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
		CampusComboBox getCampusComboBox();
		DeslocamentoGrid< DeslocamentoUnidadeDTO > getGrid();
		Component getComponent();
		CampusDTO getCampusDTO();
	}

	private Display display; 
	private GTab gTab;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;

	public UnidadesDeslocamentoPresenter( InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display )
	{
		this.display = display;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		//configureProxy();
		setListeners();
	}

//	private void configureProxy() {
//		UnidadesServiceAsync service = Services.unidades();
//		CampusDTO campusDTO = display.getCampusDTO();
//		
//		final FutureResult<List<DeslocamentoUnidadeDTO>> futureDeslocamentoUnidadeDTOList = new FutureResult<List<DeslocamentoUnidadeDTO>>();
//		service.getDeslocamento(campusDTO, futureDeslocamentoUnidadeDTOList);
//		
//		FutureSynchronizer synch = new FutureSynchronizer(futureDeslocamentoUnidadeDTOList);
//		synch.addCallback(new AsyncCallback<Boolean>() {
//			@Override
//			public void onFailure(Throwable caught) {
//				MessageBox.alert("ERRO!", "Deu falha na conexão", null);
//			}
//			@Override
//			public void onSuccess(Boolean result) {
//				display.getGrid().updateList(futureDeslocamentoUnidadeDTOList.result());	
//			}
//		});
//	}
	
	private void setListeners() {
		display.getSaveButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				List<DeslocamentoUnidadeDTO> list = display.getGrid().getStore().getModels();
				UnidadesServiceAsync service = Services.unidades();
				service.saveDeslocamento(display.getCampusDTO(), list, new AsyncCallback<Void>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
					}
					@Override
					public void onSuccess(Void result) {
						display.getGrid().getStore().commitChanges();
						Info.display("Salvo", "Deslocamentos atualizado com sucesso");
					}
				});
			}
		});
		display.getCancelButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().getStore().rejectChanges();
				Info.display("Cancelado", "Alterações canceladas com sucesso");
			}
		});
		display.getSimetricaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce) {
				display.getGrid().igualarOrigemDestino();
			}
		});
		display.getCampusComboBox().addSelectionChangedListener(new SelectionChangedListener<CampusDTO>() {
			@Override
			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
				display.getGrid().mask(display.getI18nMessages().loading(), "loading");
				CampusDTO campusDTO = se.getSelectedItem();
				UnidadesServiceAsync service = Services.unidades();
				service.getDeslocamento(campusDTO, new AsyncCallback<List<DeslocamentoUnidadeDTO>>() {
					@Override
					public void onFailure(Throwable caught) {
						MessageBox.alert("ERRO!", "Deu falha na conexão", null);
						display.getGrid().unmask();
					}

					@Override
					public void onSuccess(List<DeslocamentoUnidadeDTO> result) {
						display.getGrid().updateList(result);
						display.getGrid().unmask();
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
							ExcelInformationType.UNIDADES_DESLOCAMENTO, instituicaoEnsinoDTO, cenarioDTO );

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
						ExcelInformationType.UNIDADES_DESLOCAMENTO, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

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
						ExcelInformationType.UNIDADES_DESLOCAMENTO, instituicaoEnsinoDTO, cenarioDTO, fileExtension );

					ExportExcelFormSubmit e = new ExportExcelFormSubmit(
						parametros, display.getI18nConstants(), display.getI18nMessages() );

					e.submit();
					new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
				}
			});
	}
	
	@Override
	public void go(Widget widget) {
		gTab = (GTab)widget;
		gTab.add((GTabItem)display.getComponent());
	}

}
