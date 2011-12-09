package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.Loader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class AlunosComboBox
	extends ComboBox< AlunoDTO >
{
	private ListStore< AlunoDTO > store;

	public AlunosComboBox()
	{
		RpcProxy< PagingLoadResult< AlunoDTO > > proxy =
			new RpcProxy< PagingLoadResult< AlunoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< PagingLoadResult< AlunoDTO > > callback )
			{
				Services.alunos().getAlunosList( null, null, callback );
			}
		};

		ListLoader< BaseListLoadResult< AlunoDTO > > load
			= new BaseListLoader< BaseListLoadResult< AlunoDTO > >( proxy );

		load.addListener( Loader.BeforeLoad, new Listener< LoadEvent >()
		{
			public void handleEvent( LoadEvent be )
			{
				be.< ModelData > getConfig().set( "offset", 0 );
				be.< ModelData > getConfig().set( "limit", 10 );
			}
		});

		this.store = new ListStore< AlunoDTO >( load );

		setFieldLabel( "Aluno" );
		setDisplayField( AlunoDTO.PROPERTY_ALUNO_NOME );
		setSimpleTemplate( "{" + AlunoDTO.PROPERTY_ALUNO_NOME + "} ({" + AlunoDTO.PROPERTY_ALUNO_MATRICULA + "})" ); //setTemplate( getTemplateCB() );
		setStore( this.store );
		setHideTrigger( true );  
		setTriggerAction( TriggerAction.QUERY );
		setMinChars( 1 );
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({matricula})</div>',
			'</tpl>'
		].join("");
	}-*/;
}
