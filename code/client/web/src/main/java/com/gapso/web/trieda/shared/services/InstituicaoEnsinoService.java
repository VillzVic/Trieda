package com.gapso.web.trieda.shared.services;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("instituicaoEnsino")
public interface InstituicaoEnsinoService extends RemoteService
{
	InstituicaoEnsinoDTO getInstituicaoEnsino( Long id );
	ListLoadResult< InstituicaoEnsinoDTO > getList();
	ParametroConfiguracaoDTO getConfiguracoes();
	void saveConfiguracoes(ParametroConfiguracaoDTO parametroConfiguracaoDTO);
}
