package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.RootPanel;

public class ExportExcelFormSubmit {
	
	private FormPanel formPanel;
	private HiddenField<String> hiddenField;

	public ExportExcelFormSubmit(ExcelInformationType infoToBeExported) {
		this.hiddenField = new HiddenField<String>();
		this.hiddenField.setName(ExcelInformationType.getInformationParameterName());
		this.hiddenField.setValue(infoToBeExported.toString());

        this.formPanel = new FormPanel();
        this.formPanel.add(this.hiddenField);
        this.formPanel.setMethod(Method.GET);
        this.formPanel.setAction(GWT.getModuleBaseURL() + "exportExcelServlet");
	}
	
	public void submit() {
		RootPanel.get().add(this.formPanel);	
		this.formPanel.submit();
	}
}