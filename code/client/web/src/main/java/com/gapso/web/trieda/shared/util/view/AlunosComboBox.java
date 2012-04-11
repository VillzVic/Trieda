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
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class AlunosComboBox	extends ComboBox<AlunoDTO>{
	private ListStore<AlunoDTO> store;
	
	public AlunosComboBox(){
		this(null);
	}

	public AlunosComboBox(final CampusComboBox campusCB){
		RpcProxy<PagingLoadResult<AlunoDTO>> proxy = new RpcProxy<PagingLoadResult<AlunoDTO>>(){
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<AlunoDTO>> callback){
				if(campusCB != null)
					Services.alunos().getAlunosListByCampus(campusCB.getValue(), callback);
				else
					Services.alunos().getAlunosList( null, null, callback );
			}
		};

		final ListLoader< BaseListLoadResult< AlunoDTO > > load
			= new BaseListLoader< BaseListLoadResult< AlunoDTO > >( proxy );

		load.addListener( Loader.BeforeLoad, new Listener< LoadEvent >()
		{
			public void handleEvent( LoadEvent be )
			{
				be.< ModelData > getConfig().set( "offset", 0 );
				be.< ModelData > getConfig().set( "limit", 10 );
			}
		});
		
		if(campusCB != null){
			campusCB.addSelectionChangedListener(new SelectionChangedListener<CampusDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<CampusDTO> se){
					load.load();
				}
			});
		}

		this.store = new ListStore< AlunoDTO >( load );

		setFieldLabel( "Aluno" );
		setDisplayField( AlunoDTO.PROPERTY_ALUNO_NOME );
		setSimpleTemplate( "{" + AlunoDTO.PROPERTY_ALUNO_NOME + "} ({" + AlunoDTO.PROPERTY_ALUNO_MATRICULA + "})" ); //setTemplate( getTemplateCB() );
		setStore( this.store );
		setHideTrigger( true );  
		setTriggerAction( TriggerAction.QUERY );
		setMinChars( 1 );
	}

}
