package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.RootPanel;

public class ExportExcelFormSubmit
{
	private FormPanel formPanel;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private HiddenField< String > chaveField;

	public ExportExcelFormSubmit( ExcelParametros parametros,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this.instituicaoEnsinoDTO = parametros.getInstituicaoEnsinoDTO();

		this.formPanel = new FormPanel();
        this.formPanel.setMethod( Method.GET );
        this.formPanel.setAction( GWT.getModuleBaseURL() + "exportExcelServlet" );
        this.formPanel.addListener( Events.Submit,
        new ExcelFormListener( i18nConstants, i18nMessages ) );

		chaveField = new HiddenField<String>();
		chaveField.setName("chaveRegistro");
		chaveField.setValue("valorInicial");
		Services.progressReport().getNewKey(new AsyncCallback<String>(){
			@Override
			public void onSuccess(String progressKey){
				chaveField.setValue(progressKey);
			}
			@Override
			public void onFailure(Throwable t){}
		});

        this.addParameter(
        	ExcelInformationType.getInformationParameterName(),
        	parametros.getInfo().toString() );
        
        this.addParameter(
        	ExcelInformationType.getFileExtensionParameterName(),
        	parametros.getFileExtension() );

        // Deve ser setado em toda classe que realiza exportação de dados
		addParameter( "instituicaoEnsinoId",
			this.instituicaoEnsinoDTO.getId().toString() );
		
		this.formPanel.add(chaveField);
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
	
	public String getChaveRegistro()
	{
		return this.chaveField.getValue();
	}

	public void addParameter( String name, String value )
	{
		HiddenField< String > hiddenField
			= new HiddenField< String >();

		hiddenField.setName( name );
		hiddenField.setValue( value );

		this.formPanel.add( hiddenField );
	}

	public void submit()
	{
		RootPanel.get().add( this.formPanel );
		this.formPanel.submit();
	}
}
