package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.main.client.mvp.presenter.AmbientesFaixaOcupacaoCapacidadePresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.AmbientesFaixaUtilizacaoHorariosPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioSalasPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.FaixaCapacidadeSalaComboBox;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioSalaFiltro;
import com.gapso.web.trieda.shared.util.view.RelatorioView;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioSalasView  extends RelatorioView
	implements RelatorioSalasPresenter.Display
{
	
	RelatorioSalaFiltro filtro;
	TurnoComboBox turnoCB;
	FaixaCapacidadeSalaComboBox faixaCapacidadeSalaCB;
	
	public RelatorioSalasView(InstituicaoEnsinoDTO instituicaoEnsinoDTO, CenarioDTO cenarioDTO, GTab gTab) {
		super(instituicaoEnsinoDTO, cenarioDTO, gTab);
	}
	
	@Override
	protected String getHeadingPanel() {
		return "Master Data » Relatório de Ambientes";
	}
	
	@Override
	protected GTabItem createGTabItem() {
		return new GTabItem("Relatório de Ambientes", Resources.DEFAULTS.sala16());
	}
	
	@Override
	protected void setFilter() {
		this.filtro = new RelatorioSalaFiltro();
		
		turnoCB = new TurnoComboBox(cenarioDTO);
		faixaCapacidadeSalaCB = new FaixaCapacidadeSalaComboBox(cenarioDTO);
	}
	
	@Override
	protected List<Field<?>> getFiltersList() {
		List<Field<?>> listFiltro = new ArrayList<Field<?>>();
		listFiltro.add(turnoCB);
		listFiltro.add(faixaCapacidadeSalaCB);
		
		return listFiltro;
	}
	
	@Override
	public RelatorioSalaFiltro getSalaFiltro()
	{
		return filtro;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return turnoCB;
	}
	
	@Override
	public FaixaCapacidadeSalaComboBox getFaixaCapacidadeSalaComboBox()
	{
		return faixaCapacidadeSalaCB;
	}

	@Override
	protected void addListener(Button bt, RelatorioDTO model) {
		switch (model.getButtonIndex())
		{
		case 0:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new AmbientesFaixaUtilizacaoHorariosPresenter( instituicaoEnsinoDTO,
							cenarioDTO, new AmbientesFaixaUtilizacaoHorariosView( cenarioDTO ) );

					presenter.go( gTab );
				}
			});
			break;
		case 1:
			bt.addSelectionListener(new SelectionListener<ButtonEvent>(){
				@Override
				public void componentSelected(ButtonEvent ce){
					Presenter presenter = new AmbientesFaixaOcupacaoCapacidadePresenter( instituicaoEnsinoDTO,
							cenarioDTO, new AmbientesFaixaOcupacaoCapacidadeView( cenarioDTO ) );

					presenter.go( gTab );
				}
			});
			break;
		}
	}
}