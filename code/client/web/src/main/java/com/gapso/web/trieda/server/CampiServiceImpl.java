package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.web.trieda.client.mvc.model.CampusModel;
import com.gapso.web.trieda.client.services.CampiService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class CampiServiceImpl extends RemoteServiceServlet implements CampiService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		
		WebApplicationContext ctx = WebApplicationContextUtils.getRequiredWebApplicationContext(getServletContext());
		Campus bean = ctx.getBean("campusBean", Campus.class);
		
		List<ModelData> list = new ArrayList<ModelData>();
		list.add(new CampusModel("bean " + bean, "********"));
		for(int i = 1; i <= 10; i++) {
			list.add(new CampusModel("Codigo "+i, "Nome "+i));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
