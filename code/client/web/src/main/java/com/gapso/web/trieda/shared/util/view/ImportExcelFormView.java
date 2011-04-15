package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FileUploadField;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Encoding;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.google.gwt.core.client.GWT;

public class ImportExcelFormView extends MyComposite {
	
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private HiddenField<String> hiddenField;
	private FileUploadField fileUploadField;
	private Button importBtn;
	private Button cancelBtn;

	public ImportExcelFormView(ExcelInformationType infoToBeImported, SimpleGrid<? extends AbstractDTO<?>> gridToBeUpdated) {
		initUI(infoToBeImported,gridToBeUpdated);
		initActions();
	}

	private void initUI(ExcelInformationType infoToBeImported, SimpleGrid<? extends AbstractDTO<?>> gridToBeUpdated) {
		this.simpleModal = new SimpleModal("Importação de Excel",Resources.DEFAULTS.importar16());
		createForm(infoToBeImported,gridToBeUpdated);
		this.simpleModal.setContent(formPanel);
	}

	private void createForm(ExcelInformationType infoToBeImported, SimpleGrid<? extends AbstractDTO<?>> gridToBeUpdated) {
		this.hiddenField = new HiddenField<String>();
		this.hiddenField.setName(ExcelInformationType.getInformationParameterName());
		this.hiddenField.setValue(infoToBeImported.toString());
		
		this.fileUploadField = new FileUploadField();  
		this.fileUploadField.setAllowBlank(false);
		this.fileUploadField.setName(ExcelInformationType.getFileParameterName());  
		this.fileUploadField.setFieldLabel("File");
		
		this.cancelBtn = new Button("Reset");  
		this.importBtn = new Button("Submit");  

        this.formPanel = new FormPanel();
        this.formPanel.setFrame(true);
        this.formPanel.setAction(GWT.getModuleBaseURL() + "importExcelServlet");
        this.formPanel.setEncoding(Encoding.MULTIPART); 
        this.formPanel.setMethod(Method.POST);
        this.formPanel.setButtonAlign(HorizontalAlignment.CENTER);
        
        this.formPanel.add(this.hiddenField);
        this.formPanel.add(this.fileUploadField);
        this.formPanel.addButton(this.cancelBtn);  
        this.formPanel.addButton(this.importBtn);
        
        this.formPanel.addListener(Events.Submit,new ExcelFormListener(simpleModal,gridToBeUpdated,getI18nConstants(),getI18nMessages()));
		
		this.simpleModal.setFocusWidget(this.fileUploadField);
	}
	
	private void initActions() {
		this.importBtn.addSelectionListener(new SelectionListener<ButtonEvent>() {  
		      @Override  
		      public void componentSelected(ButtonEvent ce) {  
		        if (!formPanel.isValid()) {  
		          return;  
		        }
		        formPanel.submit();
		      }  
		});
	}
	
	public void show() {
		this.simpleModal.show();
	}
}