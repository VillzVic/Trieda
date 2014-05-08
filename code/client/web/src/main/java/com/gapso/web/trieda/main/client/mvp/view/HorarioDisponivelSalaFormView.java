package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioDisponivelSalaFormPresenter;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class HorarioDisponivelSalaFormView
	extends MyComposite
	implements HorarioDisponivelSalaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDoCenarioGrid< HorarioDisponivelCenarioDTO > gridPanel;
	private List< HorarioDisponivelCenarioDTO > horariosDisponiveis;
	private SalaDTO salaDTO;

	public HorarioDisponivelSalaFormView( SalaDTO salaDTO,
		List< HorarioDisponivelCenarioDTO > horariosDisponiveis )
	{
		this.salaDTO = salaDTO;
		this.horariosDisponiveis = horariosDisponiveis;

		initUI();
		createGrid();
	}

	private void initUI()
	{
		this.simpleModal = new SimpleModal( "Disponibilidade da Sala (" +
			this.salaDTO.getCodigo() + ")", Resources.DEFAULTS.sala16() );

		this.simpleModal.setHeight( 500 );
		this.simpleModal.setWidth( 750 );
	}

	private void createGrid()
	{
		this.contentPanel = new ContentPanel( new FitLayout() );

		this.contentPanel.setHeaderVisible( false );
		this.contentPanel.setBodyBorder( false );
		this.gridPanel = new SemanaLetivaDoCenarioGrid< HorarioDisponivelCenarioDTO >(
			this.horariosDisponiveis, HorarioDisponivelCenarioDTO.PROPERTY_ID, this );

		this.contentPanel.add( this.gridPanel );
		this.simpleModal.setContent( this.contentPanel );
	}

	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}

	@Override
	public SalaDTO getSalaDTO()
	{
		return this.salaDTO;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public void setProxy(
		RpcProxy< PagingLoadResult< HorarioDisponivelCenarioDTO > > proxy )
	{
		this.gridPanel.setProxy( proxy );
	}

	@Override
	public ListStore< HorarioDisponivelCenarioDTO > getStore()
	{
		return this.gridPanel.getStore();
	}
}
