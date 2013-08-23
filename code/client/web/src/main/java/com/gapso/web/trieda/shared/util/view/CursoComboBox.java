package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CursoComboBox
	extends ComboBox< CursoDTO >
{
	private CenarioDTO cenarioDTO;
	
	public CursoComboBox( CenarioDTO cenario )
	{
		this.cenarioDTO = cenario;
		
		RpcProxy< ListLoadResult< CursoDTO > > proxy =
			new RpcProxy< ListLoadResult< CursoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< CursoDTO > > callback )
			{
				Services.cursos().getListAll( cenarioDTO, callback );
			}
		};

		setStore( new ListStore< CursoDTO >(
			new BaseListLoader< BaseListLoadResult< CursoDTO > >( proxy ) ) );

		setDisplayField( CursoDTO.PROPERTY_DISPLAY_TEXT );
		setFieldLabel( "Curso" );
		setEmptyText( "Selecione o curso" );
		setSimpleTemplate( "{" + CursoDTO.PROPERTY_NOME + "} ({" + CursoDTO.PROPERTY_CODIGO + "})" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}
}
