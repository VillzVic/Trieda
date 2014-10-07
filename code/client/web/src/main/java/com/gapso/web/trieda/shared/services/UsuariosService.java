package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlterarSenhaDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "usuarios" )
public interface UsuariosService
	extends RemoteService
{
	UsuarioDTO getUsuario( String username );
	void save( UsuarioDTO usuarioDTO );
	void remove( List< UsuarioDTO > usuarioDTOList ) throws TriedaException;
	Boolean avoidSessionExpire();
	PagingLoadResult< UsuarioDTO > getBuscaList( String nome, String username,
		String email, PagingLoadConfig config );
	UsuarioDTO getCurrentUser();
	InstituicaoEnsinoDTO getInstituicaoEnsinoUserDTO();
	void changePassword(UsuarioDTO usuarioDTO, AlterarSenhaDTO alterarSenhaDTO) throws TriedaException;
}
