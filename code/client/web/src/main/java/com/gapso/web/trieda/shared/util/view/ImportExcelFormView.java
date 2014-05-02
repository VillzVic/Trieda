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
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ImportExcelFormView
	extends MyComposite
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private HiddenField< String > hiddenField;
	private HiddenField< String > chaveField;
	private HiddenField< String > cenarioField;
	private FileUploadField fileUploadField;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;

	public ImportExcelFormView( ExcelParametros parametros,
		SimpleGrid< ? extends AbstractDTO< ? > > gridToBeUpdated )
	{
		// Deve ser setado em toda classe que realiza importação de dados
		this.instituicaoEnsinoDTO = parametros.getInstituicaoEnsinoDTO();
		this.cenarioDTO = parametros.getCenarioDTO();

		initUI( parametros.getInfo(), gridToBeUpdated );
		initActions();
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
		this.cenarioField = new HiddenField< String >();
		this.cenarioField.setName(
			ExcelInformationType.getCenarioParameterName() );
		this.cenarioField.setValue( cenarioDTO.getId().toString() );
		chaveField = new HiddenField<String>();
		chaveField.setName("chaveRegistro");
		chaveField.setValue(null);
		Services.progressReport().getNewKey(new AsyncCallback<String>(){
			@Override
			public void onSuccess(String progressKey){
				chaveField.setValue(progressKey);
			}
			
			@Override
			public void onFailure(Throwable t){}
		});

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
        this.formPanel.add( this.cenarioField );
        this.formPanel.add( this.chaveField );
        this.formPanel.add( this.fileUploadField );

        this.formPanel.addListener( Events.Submit, new ExcelFormListener(
        	simpleModal, gridToBeUpdated, getI18nConstants(), getI18nMessages() ) );

		this.simpleModal.setFocusWidget( this.fileUploadField );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );
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
		    	  simpleModal.getSalvarBt().mask();

		    	  formPanel.submit();
		    	  
		    	  new AcompanhamentoPanelPresenter(chaveField.getValue(), new AcompanhamentoPanelView());
		      }  
		});
	}

	public void show()
	{
		this.simpleModal.show();
	}
}
