package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.form.Validator;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class UniqueTextField
	extends TextField< String >
{
	private CenarioDTO cenarioDTO;
	private UniqueDomain uniqueDomain;
	private boolean checking = false;
	private boolean error = false;
	private final String uniqueInvalidMessage = "Valor já cadastrado para o campo";

	public UniqueTextField( CenarioDTO cenarioDTO,
		UniqueDomain uniqueDomain )
	{
		this.cenarioDTO = cenarioDTO;
		this.uniqueDomain = uniqueDomain;

		addListeners();
	}

	private void addListeners()
	{
		setValidator( new Validator()
		{
			@Override
			public String validate( Field< ? > field, String value )
			{
				return ( error ? uniqueInvalidMessage : null );
			}
		});

		this.addListener( Events.Blur, new Listener< FieldEvent >()
		{
			@Override
			public void handleEvent( FieldEvent be )
			{
				check();
			}
		});
	}

	private void check()
	{
		if ( this.getOriginalValue() != null
			&& this.getValue() != null
			&& this.getOriginalValue().equals( this.getValue() ) )
		{
			return;
		}

		checking = true;
		setReadOnly( true );

		Services.uniques().existUnique( cenarioDTO, this.getValue(),
			uniqueDomain.toString(), new AsyncCallback< Boolean >()
		{
			@Override
			public void onSuccess( Boolean exist )
			{
				error = exist;
				if ( exist )
				{
					markInvalid( uniqueInvalidMessage );
				}

				isValid( true );
				error = false;
				checking = false;
				setReadOnly( false );
			}

			@Override
			public void onFailure( Throwable caught )
			{
				error = true;
				isValid( true );
				checking = false;
				setReadOnly( false );
			}
		});
	}

	public boolean isChecking()
	{
		return checking;
	}
}
