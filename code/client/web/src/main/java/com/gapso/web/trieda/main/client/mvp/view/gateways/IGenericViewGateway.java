package com.gapso.web.trieda.main.client.mvp.view.gateways;

import java.util.List;

import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;

public interface IGenericViewGateway {
	/**
	   * Displays a standard read-only message box with an OK button (comparable to
	   * the basic JavaScript alert prompt).
	   * 
	   * @param title the title bar text
	   * @param msg the message box body text
	   * @param callback listener to be called when the box is closed
	   */
	void showMessageBoxAlert(String title, String msg, Listener<MessageBoxEvent> callback);
	
	/**
	   * Displays a standard read-only message box with an OK button (comparable to
	   * the basic JavaScript alert prompt).
	   * 
	   * @param title the title bar text
	   * @param msg the message box body text
	   * @param callback listener to be called when the box is closed
	   */
	void showMessageBoxInfo(String title, String msg, Listener<MessageBoxEvent> callback);
	
	void showOtimizarMessagesView(List<String> warnings, List<String> errors);
	void showRequisicoesOtimizacaoView(boolean showEmpty);
}