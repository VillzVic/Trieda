package com.gapso.web.login.client;

import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.util.KeyNav;
import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Viewport;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.Method;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.CenterLayout;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.Element;
import com.google.gwt.user.client.ui.RootPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Login implements EntryPoint {
	
	private Viewport viewport;
	
	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad() {
		viewport = new Viewport();
		viewport.setLayout(new FitLayout());
		ContentPanel panel = new ContentPanel(new CenterLayout());
		
		final FormPanel form = new FormPanel() {

			@Override
			protected void onRender(Element target, int index) {
				super.onRender(target, index);
				getLayoutTarget().dom.setPropertyString("target", null);
			}
			
		};
		form.setAction("/resources/j_spring_security_check");
		form.setMethod(Method.POST);
		form.setFrame(true);  
		form.setHeading("Acesso restrito");  
		form.setWidth(350);  
		FormLayout layout = new FormLayout();  
		layout.setLabelWidth(75);  
		form.setLayout(layout);  
		
		FormData formData = new FormData("-20");  
		
		TextField<String> usernameTF = new TextField<String>();
		usernameTF.setName("j_username");
		usernameTF.setFieldLabel("Usuário");  
		form.add(usernameTF, formData);  
		
		TextField<String> passwordTF = new TextField<String>();  
		passwordTF.setName("j_password");
		passwordTF.setFieldLabel("Senha");
		passwordTF.setPassword(true);
		form.add(passwordTF, formData);
		
		Button enviarBt = new Button("Acessar");
		enviarBt.addSelectionListener(new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				form.submit();
			}
		});
		setDefaultButton(form, enviarBt);
		form.addButton(enviarBt);
		
		
		panel.add(form);
		
		viewport.add(panel);
		RootPanel.get().add(viewport);
		RootPanel.get("loading").setVisible(false);
	}
	
	public void setDefaultButton(Component comp, final Button button) {
		new KeyNav<ComponentEvent>(comp) {
			@Override
			public void onEnter(ComponentEvent ce) {
				super.onEnter(ce);
				button.fireEvent(Events.Select);
			}
		};
	}
}
