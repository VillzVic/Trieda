package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("usuarios")
public interface UsuariosService extends RemoteService {

	UsuarioDTO getUsuario(String username);
	void save(UsuarioDTO usuarioDTO);
	void remove(List<UsuarioDTO> usuarioDTOList);
	PagingLoadResult<UsuarioDTO> getBuscaList(String nome, String username,
			String email, PagingLoadConfig config);
	
}
