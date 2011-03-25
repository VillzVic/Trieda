package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FormEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FileUploadField;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Encoding;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.gapso.web.trieda.client.mvp.presenter.OtimizarMessagesPresenter;
import com.gapso.web.trieda.client.mvp.presenter.Presenter;
import com.gapso.web.trieda.client.mvp.view.MyComposite;
import com.gapso.web.trieda.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.google.gwt.core.client.GWT;

public class ImportExcelFormView extends MyComposite {
	
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private HiddenField<String> hiddenField;
	private FileUploadField fileUploadField;
	private Button importBtn;
	private Button cancelBtn;

	public ImportExcelFormView(ExcelInformationType infoToBeImported) {
		initUI(infoToBeImported);
		initActions();
	}

	private void initUI(ExcelInformationType infoToBeImported) {
		this.simpleModal = new SimpleModal("Importação de Excel",Resources.DEFAULTS.importar16());
		createForm(infoToBeImported);
		this.simpleModal.setContent(formPanel);
	}

	private void createForm(ExcelInformationType infoToBeImported) {
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
        
        this.formPanel.addListener(Events.Submit, new Listener<FormEvent>() {
			@Override
			public void handleEvent(FormEvent be) {
				simpleModal.hide();
				String[] resSplit = be.getResultHtml().split("[<>\n]");
				List<String> warnings = new ArrayList<String>();
				List<String> errors = new ArrayList<String>();
				for (String str : resSplit) {
					String newStr = str.trim();
					if (newStr.startsWith("@w@")) {
						warnings.add(newStr.substring(3));
					} else if (newStr.startsWith("@e@")) {
						errors.add(newStr.substring(3));
					}
				}
				
				if (!warnings.isEmpty() || !errors.isEmpty()) {				
					Presenter presenter = new OtimizarMessagesPresenter(warnings,errors,new OtimizarMessagesView());
					presenter.go(null);
				} else {
					MessageBox.info(getI18nConstants().informacao(),getI18nMessages().sucessoImportacaoExcel(),null);
				}
			}});
		
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
		        //simpleModal.hide();
		      }  
		});
	}
	
	public void show() {
		this.simpleModal.show();
	}
}