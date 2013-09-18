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

public class ExportExcelModal
	extends Window
{
	private Button exportarXlsBt;
	private Button exportarXlsxBt;
	private Button cancelarBt;
	private String textExportarXls = "Exportar .XLS";
	private String textExportarXlsx = "Exportar .XLSX";
	private String textCancelar = "Cancelar";

	public ExportExcelModal( String heading, ImageResource icon )
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
		if ( this.textExportarXls != null && this.textExportarXlsx != null)
		{
			this.exportarXlsBt = new Button( this.textExportarXls,
				AbstractImagePrototype.create( Resources.DEFAULTS.save16() ) );

			addButton( this.exportarXlsBt );
			
			this.exportarXlsxBt = new Button( this.textExportarXlsx,
					AbstractImagePrototype.create( Resources.DEFAULTS.save16() ) );

				addButton( this.exportarXlsxBt );
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

	public Button getExportarXlsBt()
	{
		return this.exportarXlsBt;
	}
	
	public Button getExportarXlsxBt()
	{
		return this.exportarXlsxBt;
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