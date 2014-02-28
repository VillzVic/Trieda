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
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoCursoComboBox
	extends ComboBox< TipoCursoDTO >
{	
	CenarioDTO cenarioDTO;
	
	public TipoCursoComboBox( CenarioDTO cenario )
	{
		this.cenarioDTO = cenario;
		RpcProxy< ListLoadResult< TipoCursoDTO > > proxy =
			new RpcProxy< ListLoadResult< TipoCursoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< TipoCursoDTO > > callback )
			{
				Services.tiposCursos().getList( cenarioDTO, callback );
			}
		};

		setStore( new ListStore< TipoCursoDTO >(
			new BaseListLoader< BaseListLoadResult< TipoCursoDTO > >( proxy ) ) );

		setFieldLabel( "Tipo de Curso" );
		setDisplayField( TipoCursoDTO.PROPERTY_CODIGO );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
	
    @Override
    public void onLoad(StoreEvent<TipoCursoDTO> se) {
        super.onLoad(se);
        System.out.println(getStore().getModels().size());
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Tipos de Curso cadastrados", null);
        }
    }
}
