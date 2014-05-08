package com.gapso.web.trieda.shared.util.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.ListModelPropertyEditor;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.util.Operador;

public class OperadorComboBox
	extends SimpleComboBox< Operador >
{
	public OperadorComboBox()
	{
		List< Operador > enumList = Arrays.asList( Operador.values() );
		add( enumList );
		setEmptyText("Selecione");
		setSimpleValue( null );
		setLazyRender( false );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );

		setPropertyEditor( new ListModelPropertyEditor< SimpleComboValue< Operador > >()
		{
			@Override
			public String getStringValue( SimpleComboValue< Operador > value )
			{
				return value.getValue().getNome();
			}
		});

		setDisplayField( "value" );
	}

	
}
