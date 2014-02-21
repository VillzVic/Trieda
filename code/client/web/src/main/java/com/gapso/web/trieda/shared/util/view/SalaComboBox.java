package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SalaComboBox
	extends ComboBox< SalaDTO >
{
	private UnidadeComboBox unidadeComboBox;

	public SalaComboBox( UnidadeComboBox unidadeCB )
	{
		this.unidadeComboBox = unidadeCB;
		this.addListeners();

		RpcProxy< ListLoadResult< SalaDTO > > proxy	
			= new RpcProxy< ListLoadResult< SalaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< SalaDTO > > callback )
			{
				Services.salas().getBuscaList( unidadeComboBox.getValue(), callback );
			}
		};

		setStore( new ListStore< SalaDTO >(
			new BaseListLoader< BaseListLoadResult< SalaDTO > >( proxy ) ) );

		setDisplayField( SalaDTO.PROPERTY_DISPLAY_TEXT );
		setFieldLabel( "Ambiente" );
		setEmptyText( "Selecione o ambiente" );
		setSimpleTemplate( "{" + SalaDTO.PROPERTY_CODIGO + "}-{"
			+ SalaDTO.PROPERTY_NUMERO + "}" );
		setEditable( false );
		setEnabled( this.unidadeComboBox.getValue() != null );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		this.unidadeComboBox.addSelectionChangedListener(
			new SelectionChangedListener< UnidadeDTO >()
		{
			@Override
			public void selectionChanged(
				SelectionChangedEvent< UnidadeDTO > se )
			{
				final UnidadeDTO unidadeDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue( null );
				setEnabled( unidadeDTO != null );

				if ( unidadeDTO != null )
				{
					getStore().getLoader().load();
				}
			}
		});
	}

	public void updateList( List< SalaDTO > list )
	{
		getStore().removeAll();
		getStore().add(list);
		setEnabled( !list.isEmpty() );
	}
}
