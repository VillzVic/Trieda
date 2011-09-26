package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessorComboBox
	extends ComboBox< ProfessorDTO >
{
	private CampusComboBox campusComboBox;
	private Boolean readOnly;

	public ProfessorComboBox( boolean readOnly )
	{
		this( null, readOnly );
	}

	public ProfessorComboBox()
	{
		this( null, false );
	}

	public ProfessorComboBox( CampusComboBox campusCB )
	{
		this( campusCB, false );
	}

	public ProfessorComboBox(
		CampusComboBox campusCB, boolean readOnly )
	{
		this.campusComboBox = campusCB;
		this.readOnly = readOnly;
		this.setReadOnly( readOnly );

		if ( !this.readOnly )
		{
			RpcProxy< ListLoadResult< ProfessorDTO > > proxy =
				new RpcProxy< ListLoadResult< ProfessorDTO > >()
			{
				@Override
				public void load( Object loadConfig,
					AsyncCallback< ListLoadResult< ProfessorDTO > > callback )
				{
					if ( campusComboBox != null
						&& campusComboBox.getValue() != null )
					{
						Services.professores().getProfessoresEmCampus(
							campusComboBox.getValue(), callback );
					}
					else
					{
						Services.professores().getList( callback );
					}
				}
			};

			setStore( new ListStore< ProfessorDTO >(
				new BaseListLoader< BaseListLoadResult< ProfessorDTO > >( proxy ) ) );
		}
		else
		{
			setStore( new ListStore< ProfessorDTO >() );
		}

		setDisplayField( ProfessorDTO.PROPERTY_NOME );
		setFieldLabel( "Professor" );
		setEmptyText( "Selecione o professor" );
		setSimpleTemplate( "{" + ProfessorDTO.PROPERTY_NOME + "} ({" +
			ProfessorDTO.PROPERTY_CPF + "})" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
		setUseQueryCache( false );
	}
}
