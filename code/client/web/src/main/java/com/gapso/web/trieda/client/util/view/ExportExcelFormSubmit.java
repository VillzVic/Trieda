package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.gapso.web.trieda.shared.excel.ExportedInformationType;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.RootPanel;

public class ExportExcelFormSubmit {
	
	private FormPanel formPanel;
	private HiddenField<String> hiddenVal = new HiddenField<String>();

	public ExportExcelFormSubmit(ExportedInformationType infoToBeExported) {
		this.hiddenVal = new HiddenField<String>();
		this.hiddenVal.setName(ExportedInformationType.getParameterName());
		this.hiddenVal.setValue(infoToBeExported.toString());

        this.formPanel = new FormPanel();
        this.formPanel.add(hiddenVal);
        this.formPanel.setMethod(Method.GET);
        this.formPanel.setAction(GWT.getModuleBaseURL() + "exportExcelServlet");
	}
	
	public void submit() {
		RootPanel.get().add(formPanel);	
        formPanel.submit();
	}
}