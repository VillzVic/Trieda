package com.gapso.web.trieda.shared.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.extjs.gxt.ui.client.widget.form.SimpleComboValue;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class PeriodoComboBox extends SimpleComboBox<Integer> {
	
	private CurriculoComboBox curriculoComboBox;
	private ITriedaI18nGateway i18nGateway;
	
	public PeriodoComboBox(CurriculoComboBox curriculoCB, ITriedaI18nGateway i18nGateway) {
		this.curriculoComboBox = curriculoCB;
		this.i18nGateway = i18nGateway;
		configureView();
		configureContentOfComboBox();
	}
	
	private void configureContentOfComboBox() {
		if (curriculoComboBox != null) {
			RpcProxy<ListLoadResult<Integer>> proxy = new RpcProxy<ListLoadResult<Integer>>() {
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<Integer>> callback) {
					PeriodoComboBox.this.reloadContent(curriculoComboBox.getValue());
				}
			};
			
			setStore(new ListStore<SimpleComboValue<Integer>>(new BaseListLoader<BaseListLoadResult<Integer>>(proxy)));
			
			curriculoComboBox.addSelectionChangedListener(new SelectionChangedListener<CurriculoDTO>() {
				@Override
				public void selectionChanged(SelectionChangedEvent<CurriculoDTO> se) {
					PeriodoComboBox.this.reloadContent(se.getSelectedItem());
				}
			});
		}
	}
	
	private void configureView() {
		setFieldLabel("Período");
		setEmptyText("Selecione um período");
		setEditable(false);
		setEnabled(false);
		setAutoWidth(true);
	}
	
	private void reloadContent(CurriculoDTO curriculoDTO) {
		getStore().removeAll();
		setEnabled(curriculoDTO != null);
		if(curriculoDTO != null) {
			Services.curriculos().getPeriodos(curriculoDTO,new AbstractAsyncCallbackWithDefaultOnFailure<List<Integer>>(i18nGateway) {
				@Override
				public void onSuccess(List<Integer> result) {
					PeriodoComboBox.this.add(result);
				}
			});
		}
	}
}