package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.TabItem;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GTabItem
	extends TabItem
{
	private ContentPanel panel;
	private String idTabItem;

	public GTabItem()
	{
		this.configuration();
	}

	public GTabItem( String text )
	{
		this( text, null );
	}
	public GTabItem( String text, ImageResource icon )
	{
		super( text );

		if ( icon != null )
		{
			this.setIcon( AbstractImagePrototype.create( icon ) );
		}

		this.configuration();
	}
	
	private void configuration()
	{
		this.setClosable( true );
		this.setLayout( new FitLayout() );

		this.panel = new ContentPanel( new BorderLayout() );
		this.panel.setHeaderVisible( false );
		this.panel.setBodyBorder( false );

		this.add( this.panel );
	}

	public void setTitle( String s )
	{
		super.setTitle( s );

		this.panel.setHeadingHtml( s );
		this.panel.setHeaderVisible( s != null );
	}

	public void setContent( Component widget )
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );

		this.panel.add( widget, bld );
	}

	public void setContent( Component widget, Margins margins )
	{
		BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
		bld.setMargins( margins );

		this.panel.add( widget, bld );
	}

	public String getIdTabItem()
	{
		return this.idTabItem;
	}

	public void setIdTabItem( String idTabItem )
	{
		this.idTabItem = idTabItem;
	}
}
