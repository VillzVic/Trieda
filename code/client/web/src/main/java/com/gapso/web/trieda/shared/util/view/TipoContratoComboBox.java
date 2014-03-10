package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoContratoComboBox
	extends ComboBox< TipoContratoDTO >
{
	CenarioDTO cenarioDTO;
	
	public TipoContratoComboBox( CenarioDTO cenario )
	{
		cenarioDTO = cenario;
		RpcProxy< ListLoadResult< TipoContratoDTO > > proxy =
			new RpcProxy< ListLoadResult< TipoContratoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< TipoContratoDTO > > callback )
			{
				Services.professores().getTiposContratoAll( cenarioDTO, callback );
			}
		};

		setStore( new ListStore< TipoContratoDTO >(
			new BaseListLoader< BaseListLoadResult< TipoContratoDTO > >( proxy ) ) );

		setFieldLabel( "Tipo de Contrato" );
		setDisplayField( TipoContratoDTO.PROPERTY_NOME );
		setEmptyText( "Selecione tipo de contrato" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
	
    @Override
    public void onLoad(StoreEvent<TipoContratoDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Tipos de Contrato cadastrados", null);
        }
    }
}
