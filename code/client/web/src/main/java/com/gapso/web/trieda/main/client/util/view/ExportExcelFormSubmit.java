package com.gapso.web.trieda.main.client.util.view;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.HiddenField;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.RootPanel;

public class ExportExcelFormSubmit {
	
	private FormPanel formPanel;
	private HiddenField<String> hiddenField;

	public ExportExcelFormSubmit(ExcelInformationType infoToBeExported, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		this.hiddenField = new HiddenField<String>();
		this.hiddenField.setName(ExcelInformationType.getInformationParameterName());
		this.hiddenField.setValue(infoToBeExported.toString());

        this.formPanel = new FormPanel();
        this.formPanel.add(this.hiddenField);
        this.formPanel.setMethod(Method.GET);
        this.formPanel.setAction(GWT.getModuleBaseURL() + "exportExcelServlet");
        this.formPanel.addListener(Events.Submit,new ExcelFormListener(i18nConstants,i18nMessages));
	}
	
	public void submit() {
		RootPanel.get().add(this.formPanel);	
		this.formPanel.submit();
	}
}