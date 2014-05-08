package com.gapso.web.trieda.shared.util.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.ListModelPropertyEditor;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.util.view.ComboBoxBoolean.Operador;

public class ComboBoxBoolean
	extends SimpleComboBox< Operador >
{
	public ComboBoxBoolean()
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


	public enum Operador
	{
		TRUE( "Sim", true ),
	    FALSE( "Não", false );

		private boolean value;
	    private String nome;

	    Operador( String nome, boolean value )
	    {
	    	this.nome = nome;
	    	this.value = value;
	    }

	    public String getNome()
	    {
	    	return this.nome;
	    }
	    
	    public boolean getValue()
	    {
	    	return this.value;
	    }

	    @Override
	    public String toString()
	    {
	    	return this.getNome();
	    }
	}
}
