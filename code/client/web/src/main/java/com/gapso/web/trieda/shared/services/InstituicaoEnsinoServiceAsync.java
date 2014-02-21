package com.gapso.web.trieda.shared.services;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface InstituicaoEnsinoServiceAsync
{
	void getInstituicaoEnsino( Long id, AsyncCallback< InstituicaoEnsinoDTO > callback );
	void getList( AsyncCallback< ListLoadResult< InstituicaoEnsinoDTO > > callback );
	void getConfiguracoes(AsyncCallback<ParametroConfiguracaoDTO> callback);
	void saveConfiguracoes(ParametroConfiguracaoDTO parametroConfiguracaoDTO,
			AsyncCallback<Void> callback);
}
