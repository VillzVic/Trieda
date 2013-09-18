package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.Window;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.button.ToolButton;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.AbstractImagePrototype;
import com.google.gwt.user.client.ui.Widget;

public class SimpleModal
	extends Window
{
	private Button salvarBt;
	private Button cancelarBt;
	private String textSalvar = "Salvar";
	private String textCancelar = "Cancelar";

	public SimpleModal( String textSalvar, String textCancelar,
		String heading, ImageResource icon )
	{
		this.textSalvar = textSalvar;
		this.textCancelar = textCancelar;

		setHeadingHtml( heading );
		setIcon( AbstractImagePrototype.create( icon ) );
		configuration();
	}

	public SimpleModal( String heading, ImageResource icon )
	{
		setHeadingHtml( heading );
		setIcon( AbstractImagePrototype.create( icon ) );
		configuration();
	}

	private void configuration()
	{
		setModal( true );
		setLayout( new FitLayout() );
		setBodyBorder( false );
		addButtons();
		setResizable( false );
	}

	public void setContent( Widget widget )
	{
		add( widget );
	}

	private void addButtons()
	{
		if ( this.textSalvar != null )
		{
			this.salvarBt = new Button( this.textSalvar,
				AbstractImagePrototype.create( Resources.DEFAULTS.save16() ) );

			addButton( this.salvarBt );
		}

		if ( this.textCancelar != null )
		{
			this.cancelarBt = new Button( this.textCancelar,
				AbstractImagePrototype.create( Resources.DEFAULTS.cancel16() ) );

			this.cancelarBt.addSelectionListener( new SelectionListener< ButtonEvent >()
			{
				@Override
				public void componentSelected( ButtonEvent ce )
				{
					hide();
				}
			});

			addButton( this.cancelarBt );
		}
	}

	public Button getSalvarBt()
	{
		return this.salvarBt;
	}

	public Button getCancelarBt()
	{
		return this.cancelarBt;
	}

	public ToolButton getCloseBt()
	{
		return this.closeBtn;
	}
}
