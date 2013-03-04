package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.Map;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public abstract class RelatorioVisaoPresenter implements Presenter{
	public interface Display extends ITriedaI18nGateway{
		GradeHorariaVisao getGrid();
		Component getComponent();
		Button getSubmitBuscaButton();
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		RelatorioVisaoFiltro getFiltro();
	}

	protected InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	protected Display display;

	public RelatorioVisaoPresenter(InstituicaoEnsinoDTO instituicaoEnsinoDTO, Display display){
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.display = display;
		
		this.setListeners();
	}

	protected void setListeners(){
		this.display.getSubmitBuscaButton().addSelectionListener(new SelectionListener<ButtonEvent>(){
			@Override
			public void componentSelected(ButtonEvent ce){
				display.getGrid().requestAtendimentos();
			}
		});

		this.display.getExportXlsExcelButton().addSelectionListener(new SelectionListener<MenuEvent>(){
			@Override
			public void componentSelected(MenuEvent ce){
				String fileExtension = "xls";
				
				ExcelParametros parametros = new ExcelParametros(
					display.getFiltro().getExcelType(), instituicaoEnsinoDTO, fileExtension
				);

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages()
				);
				
				Map<String, String> mapStringIds = display.getFiltro().getMapStringIds();

				for(String id : mapStringIds.keySet()) e.addParameter(id, mapStringIds.get(id));

				e.submit();
			}
		});

		this.display.getExportXlsxExcelButton().addSelectionListener(new SelectionListener<MenuEvent>(){
			@Override
			public void componentSelected(MenuEvent ce){
				String fileExtension = "xlsx";
				
				ExcelParametros parametros = new ExcelParametros(
					display.getFiltro().getExcelType(), instituicaoEnsinoDTO, fileExtension
				);

				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages()
				);
				
				Map<String, String> mapStringIds = display.getFiltro().getMapStringIds();

				for(String id : mapStringIds.keySet()) e.addParameter(id, mapStringIds.get(id));

				e.submit();
			}
		});
	}
	
	@Override
	public void go(Widget widget){
		GTab tab = (GTab) widget;
		tab.add((GTabItem) display.getComponent());
	}
	
	public Display getDisplay(){
		return this.display;
	}
	
	public InstituicaoEnsinoDTO getInstituicaoEnsinoDTO(){
		return this.instituicaoEnsinoDTO;
	}
	
}
