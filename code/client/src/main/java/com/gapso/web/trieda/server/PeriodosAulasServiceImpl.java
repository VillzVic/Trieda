package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.widget.form.Time;
import com.gapso.web.trieda.client.controller.mvc.model.PeriodoAulaModel;
import com.gapso.web.trieda.client.services.PeriodosDeAulasService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class PeriodosAulasServiceImpl extends RemoteServiceServlet implements PeriodosDeAulasService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(12, 00), new Time(12, 50)));
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(13, 00), new Time(13, 50)));
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(14, 00), new Time(14, 50)));
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(15, 00), new Time(15, 50)));
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(16, 00), new Time(16, 50)));
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(17, 00), new Time(17, 50)));
		list.add(new PeriodoAulaModel("Padrão", "Tarde", new Time(18, 00), new Time(18, 50)));
		
		return new BasePagingLoadResult<ModelData>(list);
	}

}
