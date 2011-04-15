package com.gapso.web.trieda.shared.mvp.view;

import java.util.Iterator;

import com.extjs.gxt.ui.client.widget.Composite;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.HasWidgets;
import com.google.gwt.user.client.ui.Widget;

public class MyComposite extends Composite implements HasWidgets, ITriedaI18nGateway {
	
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	
	public MyComposite() {
		i18nConstants = GWT.create(TriedaI18nConstants.class);
		i18nMessages = GWT.create(TriedaI18nMessages.class);
	}
	
	public TriedaI18nConstants getI18nConstants() {
		return i18nConstants;
	}
	
	public TriedaI18nMessages getI18nMessages() {
		return i18nMessages;
	}

	@Override
	public void add(Widget w) {
	}
	
	@Override
	public void clear() {
	}
	
	@Override
	public Iterator<Widget> iterator() {
		return null;
	}

	@Override
	public boolean remove(Widget w) {
		// TODO Auto-generated method stub
		return false;
	}
}