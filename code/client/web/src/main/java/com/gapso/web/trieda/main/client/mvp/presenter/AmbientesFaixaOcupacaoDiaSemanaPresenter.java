package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.charts.client.Chart;
import com.extjs.gxt.charts.client.model.ChartModel;
import com.extjs.gxt.charts.client.model.LineDataProvider;
import com.extjs.gxt.charts.client.model.axis.XAxis;
import com.extjs.gxt.charts.client.model.axis.YAxis;
import com.extjs.gxt.charts.client.model.charts.BarChart;
import com.extjs.gxt.charts.client.model.charts.CylinderBarChart;
import com.extjs.gxt.charts.client.model.charts.LineChart;
import com.extjs.gxt.ui.client.event.MenuEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDoubleDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelPresenter;
import com.gapso.web.trieda.shared.util.view.AcompanhamentoPanelView;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.ExcelParametros;
import com.gapso.web.trieda.shared.util.view.ExportExcelFormSubmit;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class AmbientesFaixaOcupacaoDiaSemanaPresenter
	implements Presenter
{
	public interface Display
		extends ITriedaI18nGateway
	{
		MenuItem getExportXlsExcelButton();
		MenuItem getExportXlsxExcelButton();
		Grid< RelatorioQuantidadeDoubleDTO > getGrid();
		ListStore< RelatorioQuantidadeDoubleDTO > getStore();
		Component getComponent();
		CampusComboBox getCampusComboBox();
		Chart getChart();
	}
	
	private Display display;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	
	public AmbientesFaixaOcupacaoDiaSemanaPresenter(
			InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			CenarioDTO cenarioDTO, Display display )
	{
			this.display = display;
			this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
			this.cenarioDTO = cenarioDTO;
	
			setListeners();
	}
	
	private void setListeners()
	{
		this.display.getCampusComboBox().addSelectionChangedListener(
				new SelectionChangedListener< CampusDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< CampusDTO > se )
			{
				if ( se.getSelectedItem() == null )
				{
					return;
				}
				display.getGrid().mask( display.getI18nMessages().loading() );
				Services.atendimentos().getAmbientesFaixaUtilizacaoPorDiaSemana( cenarioDTO, se.getSelectedItem(),
				new AbstractAsyncCallbackWithDefaultOnFailure< List < RelatorioQuantidadeDoubleDTO > >( display )
				{
					@Override
					public void onSuccess(
						List< RelatorioQuantidadeDoubleDTO > list )
						{
							display.getStore().removeAll();
							display.getStore().add( list );
							display.getGrid().unmask();
						}
				});
			}
		});
		
		this.display.getGrid().getSelectionModel().addSelectionChangedListener(new SelectionChangedListener<RelatorioQuantidadeDoubleDTO>() {
	
			@Override
		    public void selectionChanged(SelectionChangedEvent<RelatorioQuantidadeDoubleDTO> se) {
		        display.getChart().setChartModel(getChartModel(se.getSelectedItem()));
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
					ExcelInformationType.AMBIENTES_FAIXA_OCUPACAO_SEMANA, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
	
				if ( display.getCampusComboBox() == null
						|| display.getCampusComboBox().getValue() == null
						|| display.getCampusComboBox().getValue().getId() == null )
				{
					return;
				}
	
				String campusId = display.getCampusComboBox().getValue().getId().toString();
				e.addParameter( "campusId", campusId );
	
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
					ExcelInformationType.AMBIENTES_FAIXA_OCUPACAO_SEMANA, instituicaoEnsinoDTO, cenarioDTO, fileExtension );
	
				ExportExcelFormSubmit e = new ExportExcelFormSubmit(
					parametros, display.getI18nConstants(), display.getI18nMessages() );
				
				if ( display.getCampusComboBox() == null
						|| display.getCampusComboBox().getValue() == null
						|| display.getCampusComboBox().getValue().getId() == null )
				{
					return;
				}
	
				String campusId = display.getCampusComboBox().getValue().getId().toString();
				e.addParameter( "campusId", campusId );
	
				e.submit();
				new AcompanhamentoPanelPresenter(e.getChaveRegistro(), new AcompanhamentoPanelView());
			}
		});
	}
	
    public ChartModel getChartModel(RelatorioQuantidadeDoubleDTO model) {  
        ChartModel cm = new ChartModel("Ocupação por Semana (" + model.getFaixaCredito() + ")",  
            "font-size: 14px; font-family: Verdana; text-align: center;");  
        cm.setBackgroundColour("#ffffff");  
        XAxis xa = new XAxis();  
        xa.setLabels("Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado");
        xa.setZDepth3D(5);  
        xa.setTickHeight(4);  
        xa.setOffset(true);  
        xa.setColour("#909090");  
        cm.setXAxis(xa);  
        YAxis ya = new YAxis();  
        ya.setSteps(10);  
        ya.setMax(100);
        cm.setYAxis(ya);  
        CylinderBarChart bchart = new CylinderBarChart();  
        bchart.setColour("#440088");  
        bchart.setAlpha(.8f);  
        bchart.setTooltip("#val# " + "%");
        
        LineChart line = new LineChart();  
        line.setText("Média");  
        line.setTooltip("#val# " + "%");
        line.setColour("#FF0000");
        LineDataProvider lineProvider = new LineDataProvider("quantidade");
        ListStore<RelatorioQuantidadeDoubleDTO> store = new ListStore<RelatorioQuantidadeDoubleDTO>();
        store.add(criaMedias(model));
        lineProvider.bind(store);  
        line.setDataProvider(lineProvider);
        cm.addChartConfig(line);  
        
        bchart.addBars(new BarChart.Bar(model.getQuantidade(), "#ff0000"));
        bchart.addBars(new BarChart.Bar(model.getQuantidade2(), "#00aa00"));
        bchart.addBars(new BarChart.Bar(model.getQuantidade3(), "#0000ff"));
        bchart.addBars(new BarChart.Bar(model.getQuantidade4(), "#ff9900"));
        bchart.addBars(new BarChart.Bar(model.getQuantidade5(), "#ff00ff"));
        bchart.addBars(new BarChart.Bar(model.getQuantidade6(), "#aa88ff"));
        cm.addChartConfig(bchart);  
        return cm;
    }
    
    private List<RelatorioQuantidadeDoubleDTO> criaMedias(RelatorioQuantidadeDoubleDTO model) {
		List<RelatorioQuantidadeDoubleDTO> medias = new ArrayList<RelatorioQuantidadeDoubleDTO>();
		
		Double valorMedio = 0.0;
		
		valorMedio += model.getQuantidade();
		valorMedio += model.getQuantidade2();
		valorMedio += model.getQuantidade3();
		valorMedio += model.getQuantidade4();
		valorMedio += model.getQuantidade5();
		valorMedio += model.getQuantidade6();
		
		valorMedio /= 6;
		
		RelatorioQuantidadeDoubleDTO media = new RelatorioQuantidadeDoubleDTO("Media1", 0.0);
		media.setQuantidade(valorMedio);
		medias.add(media);
		RelatorioQuantidadeDoubleDTO media2 = new RelatorioQuantidadeDoubleDTO("Media2", 0.0);
		media2.setQuantidade(valorMedio);
		medias.add(media2);
		RelatorioQuantidadeDoubleDTO media3 = new RelatorioQuantidadeDoubleDTO("Media3", 0.0);
		media3.setQuantidade(valorMedio);
		medias.add(media3);
		RelatorioQuantidadeDoubleDTO media4 = new RelatorioQuantidadeDoubleDTO("Media4", 0.0);
		media4.setQuantidade(valorMedio);
		medias.add(media4);
		RelatorioQuantidadeDoubleDTO media5 = new RelatorioQuantidadeDoubleDTO("Media5", 0.0);
		media5.setQuantidade(valorMedio);
		medias.add(media5);
		RelatorioQuantidadeDoubleDTO media6 = new RelatorioQuantidadeDoubleDTO("Media6", 0.0);
		media6.setQuantidade(valorMedio);
		medias.add(media6);
		
		return medias;
	}
	
	@Override
	public void go(Widget widget)
	{
		GTab tab = (GTab)widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
