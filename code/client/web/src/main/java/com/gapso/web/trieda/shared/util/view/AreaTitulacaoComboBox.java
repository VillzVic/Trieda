package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class AreaTitulacaoComboBox
	extends ComboBox< AreaTitulacaoDTO >
{
	public AreaTitulacaoComboBox()
	{
		RpcProxy< ListLoadResult< AreaTitulacaoDTO > > proxy =
			new RpcProxy< ListLoadResult< AreaTitulacaoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< AreaTitulacaoDTO > > callback )
			{
				Services.areasTitulacao().getListAll(callback);
			}
		};

		setStore( new ListStore< AreaTitulacaoDTO >(
			new BaseListLoader< BaseListLoadResult< AreaTitulacaoDTO > >( proxy ) ) );

		setFieldLabel( "Área de Conhecimento" );
		setDisplayField( AbstractDTO.PROPERTY_DISPLAY_TEXT );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
}
