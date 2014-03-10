package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class DisciplinaComboBox
	extends ComboBox< DisciplinaDTO >
{
	private CurriculoComboBox curriculoComboBox;
	private CenarioDTO cenarioDTO;
	private PeriodoComboBox periodoComboBox;

	public DisciplinaComboBox( CenarioDTO cenario )
	{
		this( cenario, null, null );
	}

	public DisciplinaComboBox( CenarioDTO cenario, CurriculoComboBox curriculoCB, PeriodoComboBox periodoCB )
	{
		this.curriculoComboBox = curriculoCB;
		this.periodoComboBox = periodoCB;
		this.cenarioDTO = cenario;

		RpcProxy< ListLoadResult< DisciplinaDTO > > proxy =
			new RpcProxy< ListLoadResult< DisciplinaDTO > >()
		{
			@Override
			public void load( Object loadConfig,
				AsyncCallback< ListLoadResult< DisciplinaDTO > > callback )
			{
				if ( curriculoComboBox != null && periodoComboBox != null )
				{
					Services.disciplinas().getListByCurriculoIdAndPeriodo( cenarioDTO,
							curriculoComboBox.getValue().getId(), periodoComboBox.getSimpleValue(), callback );
				}
				else
				{
					Services.disciplinas().getList( cenarioDTO, null, callback );
				}
			}
		};

		setStore( new ListStore< DisciplinaDTO >(
			new BaseListLoader< BaseListLoadResult< DisciplinaComboBox > >( proxy ) ) );

		if ( curriculoComboBox != null && periodoComboBox != null)
		{
			setEnabled( false );
			addListeners();
		}

		setDisplayField( DisciplinaDTO.PROPERTY_DISPLAY_TEXT );
		setFieldLabel( "Disciplina" );
		setEmptyText( "Selecione a disciplina" );
		setSimpleTemplate( "{" + DisciplinaDTO.PROPERTY_DISPLAY_TEXT + "}" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
	}

	private void addListeners()
	{
		periodoComboBox.addSelectionChangedListener(
			new SelectionChangedListener<SimpleComboValue<Integer>>()
		{
			@Override
			public void selectionChanged( SelectionChangedEvent< SimpleComboValue<Integer> > se )
			{
				final SimpleComboValue<Integer> value = se.getSelectedItem();
				getStore().removeAll();
				setValue( null );
				setEnabled( value != null );

				if ( value != null )
				{
					getStore().getLoader().load();
				}
			}
		});
	}
	
    @Override
    public void onLoad(StoreEvent<DisciplinaDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Disciplinas cadastradas", null);
        }
    }
}
