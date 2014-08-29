package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ParametroConfiguracaoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("instituicaoEnsino")
public interface InstituicaoEnsinoService extends RemoteService
{
	InstituicaoEnsinoDTO getInstituicaoEnsino( Long id );
	ListLoadResult< InstituicaoEnsinoDTO > getList();
	ParametroConfiguracaoDTO getConfiguracoes();
	void saveConfiguracoes(ParametroConfiguracaoDTO parametroConfiguracaoDTO);
	PagingLoadResult<InstituicaoEnsinoDTO> getBuscaList(String nome, PagingLoadConfig config);
	void remove(List<InstituicaoEnsinoDTO> instituicaoEnsinoDTOList) throws TriedaException;
	void save(InstituicaoEnsinoDTO instituicaoEnsinoDTO) throws TriedaException;
}
