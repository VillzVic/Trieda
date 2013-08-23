package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CampusComboBox
	extends ComboBox< CampusDTO >
{
	private CurriculoComboBox curriculoComboBox;
	private boolean campusTodos;
	private CenarioDTO cenario;

	public CampusComboBox(CenarioDTO cenarioDTO)
	{
		this( cenarioDTO, null, false );
	}

	public CampusComboBox( CenarioDTO cenarioDTO, CurriculoComboBox curriculoCB, boolean adicionaCampusTodos )
	{
		this.curriculoComboBox = curriculoCB;
		this.campusTodos = adicionaCampusTodos;
		this.cenario = cenarioDTO;

		RpcProxy< ListLoadResult< CampusDTO > > proxy =
			new RpcProxy< ListLoadResult< CampusDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< CampusDTO > > callback )
			{
				if ( curriculoComboBox != null )
				{
					Services.campi().getListByCurriculo( cenario,
						curriculoComboBox.getValue(), callback );
				}
				else
				{
					if( campusTodos )
					{
						//Lista com todos os campi mais um campus com nome TODOS
						Services.campi().getListAllCampiTodos( cenario, callback );
					}
					else
					{
						Services.campi().getListAll( cenario, callback );
					}
				}
			}
		};

		
		setStore( new ListStore< CampusDTO >(
			new BaseListLoader< BaseListLoadResult< CampusDTO > >( proxy ) ) );

		if ( curriculoComboBox != null )
		{
			setEnabled( false );
			addListeners();
		}

		setDisplayField( CampusDTO.PROPERTY_CODIGO );
		setFieldLabel( "Campus" );
		setEmptyText( "Selecione o campus" );
		setSimpleTemplate( "{" + CampusDTO.PROPERTY_NOME +
			"} ({" + CampusDTO.PROPERTY_CODIGO + "})" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		curriculoComboBox.addSelectionChangedListener(
			new SelectionChangedListener< CurriculoDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< CurriculoDTO > se )
			{
				final CurriculoDTO curriculoDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue( null );
				setEnabled( curriculoDTO != null );

				if( curriculoDTO != null )
				{
					getStore().getLoader().load();
				}
			}
		});
	}
}
