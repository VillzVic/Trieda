package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class InstituicaoEnsinoComboBox
	extends ComboBox< InstituicaoEnsinoDTO >
{

	public InstituicaoEnsinoComboBox()
	{
		RpcProxy< ListLoadResult< InstituicaoEnsinoDTO > > proxy
			= new RpcProxy< ListLoadResult< InstituicaoEnsinoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< InstituicaoEnsinoDTO > > callback )
			{
				Services.instituicoesEnsino().getList( callback );
			}
		};

		setStore( new ListStore< InstituicaoEnsinoDTO >(
			new BaseListLoader< BaseListLoadResult< InstituicaoEnsinoDTO > >( proxy ) ) );

		setFieldLabel( "Instituição de Ensino" );
		setEmptyText( "Selecione a instituição de ensino" );
		setDisplayField( InstituicaoEnsinoDTO.PROPERTY_DISPLAY_TEXT );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
}
