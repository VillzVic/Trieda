package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.Dialog;
import com.extjs.gxt.ui.client.widget.Html;
import com.extjs.gxt.ui.client.widget.Label;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.Widget;

/**
 * Classe utilitária para gerar caixas de mensagens com informações de detalhamento
 */
public class TriedaDetailMessageBox {
	
	private static TriedaI18nConstants I18N_CONSTANTS = GWT.create(TriedaI18nConstants.class);
	
	/**
	 * Mostra uma caixa de mensagem padrão com um botão OK.
	 * 
	 * @param title
	 *            o texto da barra de título
	 * @param msg
	 *            o texto da mensagem principal
	 * @param caught
	 *            a exceção que será mostrada como detalhamento da mensagem
	 */
	public static void alert(String title, String msg, Throwable caught) {
		alert(title,msg,TriedaUtil.extractMessage(caught));
	}
	
	/**
	 * Mostra uma caixa de mensagem padrão com um botão OK.
	 * 
	 * @param title
	 *            o texto da barra de título
	 * @param msg
	 *            o texto da mensagem principal
	 * @param detailMsg
	 *            o texto do detalhamento sobre a mensagem principal
	 */
	public static void alert(String title, String msg, String detailMsg) {
		// North Panel Layout:
		// --------------------------------------
		// | ICON_COMPONENT | MESSAGE_COMPONENT |
		// --------------------------------------
		ContentPanel northPanel = getNorthPanel();
		northPanel.setLayout(new BorderLayout());
		northPanel.add(getIconComponent(MessageBox.WARNING),new BorderLayoutData(LayoutRegion.WEST,32));
		northPanel.add(getMessageComponent(msg),new BorderLayoutData(LayoutRegion.CENTER));

		// Dialog Layout:
		// ----------------------------
		// |       NORTH_PANEL        |
		// ----------------------------
		// | DETAIL_MESSAGE_COMPONENT |
		// ----------------------------
		Dialog dialog = getDialog(title);
		dialog.setLayout(new RowLayout(Orientation.VERTICAL));
		dialog.add(northPanel);
		dialog.add(getDetailMessageComponent(detailMsg));
		
		dialog.show();
	}
	
	private static Widget getIconComponent(String messageBoxIcon) {
		Html html = new Html("<div class=\"" + messageBoxIcon + "\" style=\"width: 31px; height: 32px\"></div>");
		html.setStyleName("x-window-dlg");
		return html;
	}
	
	private static Widget getMessageComponent(String msg) {
		return new Label(msg);
	}
	
	private static Widget getDetailMessageComponent(String detailMsg) {
		FieldSet fieldSet = new FieldSet();
		fieldSet.setHeadingHtml(I18N_CONSTANTS.triedaDetailMessageHeadingText());
		fieldSet.setCollapsible(true);
		fieldSet.setExpanded(false);
		fieldSet.addText(detailMsg);
		return fieldSet;
	}
	
	private static ContentPanel getNorthPanel() {
		ContentPanel northPanel = new ContentPanel();
		northPanel.setBorders(false);
		northPanel.setBodyBorder(false);
		northPanel.setHeaderVisible(false);
		northPanel.setHeight(33);
		northPanel.setBodyStyle("background-color: transparent;");
		return northPanel;
	}
	
	private static Dialog getDialog(String title) {
		Dialog dialog = new Dialog();
		
		dialog.setHeadingHtml(title);
		dialog.setConstrain(true);
		dialog.setMinimizable(false);
		dialog.setMaximizable(false);
		dialog.setClosable(false);
		dialog.setModal(true);
		dialog.setPlain(true);
		dialog.setFooter(true);
		dialog.setBodyBorder(false);
		dialog.setMinWidth(500);
		dialog.setMinHeight(100);
		dialog.setButtons(Dialog.OK);
		dialog.setButtonAlign(HorizontalAlignment.CENTER);
		dialog.setHideOnButtonClick(true);
		
		return dialog;
	}
}