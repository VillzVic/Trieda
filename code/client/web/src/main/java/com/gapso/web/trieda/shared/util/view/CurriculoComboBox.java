package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CurriculoComboBox
	extends ComboBox< CurriculoDTO >
{
	private CursoComboBox cursoComboBox;
	private CenarioDTO cenarioDTO;

	public CurriculoComboBox( CenarioDTO cenario )
	{
		this( cenario, null );
	}

	public CurriculoComboBox( CenarioDTO cenario, CursoComboBox cursoCB )
	{
		this.cursoComboBox = cursoCB;
		this.cenarioDTO = cenario;

		RpcProxy< ListLoadResult< CurriculoDTO > > proxy =
			new RpcProxy< ListLoadResult< CurriculoDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< CurriculoDTO > > callback )
			{
				if ( cursoComboBox != null )
				{
					Services.curriculos().getListByCurso(
						cursoComboBox.getValue(), callback );
				}
				else
				{
					Services.curriculos().getListAll( cenarioDTO, callback );
				}
			}
		};

		setStore( new ListStore< CurriculoDTO >(
			new BaseListLoader< BaseListLoadResult< CurriculoDTO > >( proxy ) ) );

		if ( cursoComboBox != null )
		{
			setEnabled( false );
			addListeners();
		}

		setDisplayField( CurriculoDTO.PROPERTY_DISPLAY_TEXT );
		setFieldLabel( "Matriz Curricular" );
		setEmptyText( "Selecione a matriz curricular" );
		setSimpleTemplate( "{" + CurriculoDTO.PROPERTY_DISPLAY_TEXT + "}" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		cursoComboBox.addSelectionChangedListener(
			new SelectionChangedListener< CursoDTO >()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< CursoDTO > se )
			{
				final CursoDTO cursoDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue( null );
				setEnabled( cursoDTO != null );

				if ( cursoDTO != null )
				{
					getStore().getLoader().load();
				}
			}
		});
	}
}
