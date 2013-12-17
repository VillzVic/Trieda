package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SemanaLetivaComboBox
	extends ComboBox< SemanaLetivaDTO >
{
	private CenarioDTO cenarioDTO;
	
	public SemanaLetivaComboBox( CenarioDTO cenario )
	{
		this.cenarioDTO = cenario;
		RpcProxy< ListLoadResult< SemanaLetivaDTO > > proxy
			= new RpcProxy< ListLoadResult< SemanaLetivaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< SemanaLetivaDTO > > callback )
			{
				Services.semanasLetiva().getList( cenarioDTO, callback );
			}
		};

		setStore( new ListStore< SemanaLetivaDTO >(
			new BaseListLoader< BaseListLoadResult< SemanaLetivaDTO > >( proxy ) ) );

		setDisplayField( SemanaLetivaDTO.PROPERTY_CODIGO );
		setFieldLabel( "Semana Letiva" );
		setEmptyText( "Selecione a semana letiva" );
		setSimpleTemplate( "{" + SemanaLetivaDTO.PROPERTY_CODIGO +
				"} ({" + SemanaLetivaDTO.PROPERTY_TEMPO + "}min)" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
}
