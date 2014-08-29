package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
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
	void getBuscaList(String nome, PagingLoadConfig config,
			AsyncCallback<PagingLoadResult<InstituicaoEnsinoDTO>> callback);
	void remove(List<InstituicaoEnsinoDTO> instituicaoEnsinoDTOList,
			AsyncCallback<Void> callback);
	void save(InstituicaoEnsinoDTO instituicaoEnsinoDTO,
			AsyncCallback<Void> callback);
}
