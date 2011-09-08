package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.form.FileUploadField;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Encoding;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.core.client.GWT;

public class ImportExcelFormView
	extends MyComposite
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private HiddenField< String > hiddenField;
	private FileUploadField fileUploadField;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;

	public ImportExcelFormView( ExcelParametros parametros,
		SimpleGrid< ? extends AbstractDTO< ? > > gridToBeUpdated )
	{
		// Deve ser setado em toda classe que realiza importação de dados
		this.instituicaoEnsinoDTO = parametros.getInstituicaoEnsinoDTO();

		initUI( parametros.getInfo(), gridToBeUpdated );
		initActions();

		addParameter( "instituicaoEnsinoId",
			this.instituicaoEnsinoDTO.getId().toString() );
	}

	public InstituicaoEnsinoDTO getInstituicaoEnsinoDTO()
	{
		return instituicaoEnsinoDTO;
	}

	public void setInstituicaoEnsinoDTO(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO )
	{
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
	}

	private void initUI( ExcelInformationType infoToBeImported,
		SimpleGrid< ? extends AbstractDTO< ? > > gridToBeUpdated )
	{
		this.simpleModal = new SimpleModal(
			"Importação de Excel", Resources.DEFAULTS.importar16() );

		createForm( infoToBeImported, gridToBeUpdated );
		this.simpleModal.setWidth( 320 );
		this.simpleModal.setHeight( 115 );
		this.simpleModal.setContent( formPanel );
	}

	private void createForm( ExcelInformationType infoToBeImported,
		SimpleGrid< ? extends AbstractDTO< ? > > gridToBeUpdated )
	{
		this.hiddenField = new HiddenField< String >();
		this.hiddenField.setName(
			ExcelInformationType.getInformationParameterName() );
		this.hiddenField.setValue( infoToBeImported.toString() );

		this.fileUploadField = new FileUploadField();  
		this.fileUploadField.setAllowBlank( false );
		this.fileUploadField.setName(
			ExcelInformationType.getFileParameterName() );  
		this.fileUploadField.setFieldLabel( "Arquivo" );

        this.formPanel = new FormPanel();
        this.formPanel.setLabelWidth( 50 );
        this.formPanel.setFrame( true );
        this.formPanel.setHeaderVisible( false );
        this.formPanel.setAction(
        	GWT.getModuleBaseURL() + "importExcelServlet" );
        this.formPanel.setEncoding( Encoding.MULTIPART ); 
        this.formPanel.setMethod( Method.POST );
        this.formPanel.setButtonAlign( HorizontalAlignment.CENTER );

        this.formPanel.add( this.hiddenField );
        this.formPanel.add( this.fileUploadField );

        this.formPanel.addListener( Events.Submit, new ExcelFormListener(
        	simpleModal, gridToBeUpdated, getI18nConstants(), getI18nMessages() ) );

		this.simpleModal.setFocusWidget( this.fileUploadField );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );
	}

	public void addParameter( String name, String value )
	{
		HiddenField< String > hiddenField = new HiddenField< String >();
		hiddenField.setName( name );
		hiddenField.setValue( value );
		this.formPanel.add( hiddenField );
	}

	private void initActions()
	{
		this.simpleModal.getSalvarBt().addSelectionListener(
			new SelectionListener< ButtonEvent >()
		{  
		      @Override  
		      public void componentSelected( ButtonEvent ce )
		      {  
		    	  if ( !formPanel.isValid() )
		    	  {
		    		  return;
		    	  }

		    	  formPanel.submit();
		      }  
		});
	}

	public void show()
	{
		this.simpleModal.show();
	}
}
