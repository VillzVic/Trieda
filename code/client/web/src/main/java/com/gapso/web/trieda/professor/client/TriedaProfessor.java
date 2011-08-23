package com.gapso.web.trieda.professor.client;

import com.gapso.web.trieda.professor.client.mvp.presenter.AppPresenter;
import com.gapso.web.trieda.professor.client.mvp.view.AppView;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.RootPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class TriedaProfessor
	implements EntryPoint
{
	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad()
	{
	    Presenter appPresenter = new AppPresenter( new AppView() );
	    appPresenter.go( RootPanel.get() );
	}
}
