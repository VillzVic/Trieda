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
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TurnoComboBox
	extends ComboBox< TurnoDTO >
{
	private CampusComboBox campusComboBox;

	public TurnoComboBox()
	{
		this( null, false );
	}

	public TurnoComboBox( CampusComboBox campusCB )
	{
		this( campusCB, false );
	}

	public TurnoComboBox( CampusComboBox campusCB, final boolean somenteOtimizadoOperacional )
	{
		this.campusComboBox = campusCB;

		RpcProxy< ListLoadResult< TurnoDTO > > proxy = new RpcProxy<ListLoadResult< TurnoDTO > >()
		{
			@Override
			public void load( Object loadConfig, AsyncCallback< ListLoadResult< TurnoDTO > > callback )
			{
				if ( campusComboBox != null )
				{
					Services.turnos().getListByCampus( campusComboBox.getValue(), callback );
				}
				else
				{
					if ( somenteOtimizadoOperacional )
					{
						Services.turnos().getListOtimizedOnly( callback );
					}
					else
					{
						Services.turnos().getList( callback );
					}
				}
			}
		};

		setStore( new ListStore< TurnoDTO >(
			new BaseListLoader< BaseListLoadResult< TurnoDTO > >( proxy ) ) );

		if ( campusComboBox != null )
		{
			setEnabled( false );
			addListeners();
		}

		setDisplayField( TurnoDTO.PROPERTY_NOME );
		setFieldLabel( "Turno" );
		setEmptyText( "Selecione o turno" );
		setSimpleTemplate( "{" + TurnoDTO.PROPERTY_NOME + "} ({" + TurnoDTO.PROPERTY_TEMPO + "}min)" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		campusComboBox.addSelectionChangedListener(
			new SelectionChangedListener< CampusDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< CampusDTO > se )
			{
				final CampusDTO campusDTO = se.getSelectedItem();

				getStore().removeAll();
				setValue( null );
				setEnabled( campusDTO != null );

				if ( campusDTO != null )
				{
					getStore().getLoader().load();
				}
			}
		});
	}
}
