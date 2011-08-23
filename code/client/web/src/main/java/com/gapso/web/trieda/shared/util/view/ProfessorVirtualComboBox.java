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
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessorVirtualComboBox
	extends ComboBox< ProfessorVirtualDTO >
{
	private CampusComboBox campusComboBox;

	public ProfessorVirtualComboBox( CampusComboBox campusCB )
	{
		this.campusComboBox = campusCB;
		addListeners();

		RpcProxy< ListLoadResult< ProfessorVirtualDTO > > proxy = new RpcProxy< ListLoadResult< ProfessorVirtualDTO > >()
		{
			@Override
			public void load( Object loadConfig, AsyncCallback< ListLoadResult< ProfessorVirtualDTO > > callback )
			{
				Services.atendimentos().getProfessoresVirtuais( campusComboBox.getValue(), callback );
			}
		};

		setStore( new ListStore< ProfessorVirtualDTO >(
			new BaseListLoader< BaseListLoadResult< ProfessorVirtualDTO > >( proxy ) ) );

		setDisplayField( ProfessorVirtualDTO.PROPERTY_NOME );
		setFieldLabel( "Professor Virtual" );
		setEmptyText( "Selecione o professor virtual" );
		setSimpleTemplate( "{"+ProfessorVirtualDTO.PROPERTY_NOME + "}" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
		setEnabled( campusComboBox.getValue() != null );
		setUseQueryCache( false );
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
