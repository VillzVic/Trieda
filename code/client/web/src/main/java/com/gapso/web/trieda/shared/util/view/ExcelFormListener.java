package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.event.FormEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.mvp.presenter.OtimizarMessagesPresenter;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.mvp.view.OtimizarMessagesView;

public class ExcelFormListener
	implements Listener< FormEvent >
{
	private SimpleModal modalToBeHidden;
	private SimpleGrid< ? extends AbstractDTO< ? > > gridToBeUpdated;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;

	public ExcelFormListener( SimpleModal modalToBeHidden,
		SimpleGrid< ? extends AbstractDTO< ? > > gridToBeUpdated,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this.modalToBeHidden = modalToBeHidden;
		this.gridToBeUpdated = gridToBeUpdated;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;
	}

	public ExcelFormListener( TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages )
	{
		this( null, null, i18nConstants, i18nMessages );
	}

	@Override
	public void handleEvent(FormEvent be) {
		if (modalToBeHidden != null) {
			modalToBeHidden.hide();
		}
		
		String[] resSplit = be.getResultHtml().split("[<>\n]");
		List<String> warnings = new ArrayList<String>();
		List<String> errors = new ArrayList<String>();
		for (String str : resSplit) {
			String newStr = str.trim();
			if (newStr.startsWith(ExcelInformationType.prefixWarning())) {
				warnings.add(newStr.substring(ExcelInformationType.prefixWarning().length()));
			} else if (newStr.startsWith(ExcelInformationType.prefixError())) {
				errors.add(newStr.substring(ExcelInformationType.prefixError().length()));
			}
		}
		
		if (!warnings.isEmpty() || !errors.isEmpty()) {				
			Presenter presenter = new OtimizarMessagesPresenter(warnings,errors,new OtimizarMessagesView());
			presenter.go(null);
		} else {
			if (gridToBeUpdated != null) {
				gridToBeUpdated.updateList();
			}
			//MessageBox.info(i18nConstants.informacao(),i18nMessages.sucessoImportacaoExcel(),null);
		}
	}
}