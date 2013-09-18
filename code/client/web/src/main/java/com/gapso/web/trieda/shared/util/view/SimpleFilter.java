package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.util.KeyNav;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.BoxLayout.BoxLayoutPack;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SimpleFilter
	extends FormPanel
{
	private FormData formDataFilter;
	private Button submitButton;
	private Button resetButton;
	private boolean hiddenResetButton;

	public SimpleFilter()
	{
		this( false );
	}

	public SimpleFilter( boolean hiddenResetButton )
	{
		super();
		this.hiddenResetButton = hiddenResetButton;
		configuration();
	}

	private void configuration()
	{
		formDataFilter = new FormData( "-5" );
		setHeadingHtml( "Filtro" );
		setWidth( 350 );
		createButtons();
	}

	public void addField( Field< ? > field )
	{
		insert( field, getItemCount() - 1, formDataFilter );
	}

	private void createButtons()
	{
		LayoutContainer lc = new LayoutContainer();
		HBoxLayout llc = new HBoxLayout();

		llc.setPadding( new Padding( 5 ) );
		llc.setPack( BoxLayoutPack.CENTER );
		lc.setLayout( llc );

		HBoxLayoutData hbld = new HBoxLayoutData(
			new Margins( 0, 5, 0, 0 ) );

		submitButton = new Button( "Filtrar",
			AbstractImagePrototype.create( Resources.DEFAULTS.filter16() ) );
	
		lc.add( submitButton, hbld );
		setDefaultButton( getSubmitButton() );

		if ( !hiddenResetButton )
		{
			resetButton = new Button( "Limpar",
				AbstractImagePrototype.create( Resources.DEFAULTS.filterClean16() ) );

			lc.add( resetButton, hbld );
		}

		add( lc );
	}

	public void setDefaultButton( final Button button )
	{
		new KeyNav< ComponentEvent >( this )
		{
			@Override
			public void onEnter( ComponentEvent ce )
			{
				super.onEnter( ce );
				button.fireEvent( Events.Select );
			}
		};
	}
	
	public Button getSubmitButton()
	{
		return submitButton;
	}

	public Button getResetButton()
	{
		return resetButton;
	}

	public void hiddenResetButton( boolean hiddenResetButton )
	{
		this.hiddenResetButton = hiddenResetButton;
	}
}
