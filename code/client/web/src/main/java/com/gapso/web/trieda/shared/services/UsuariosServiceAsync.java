package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface UsuariosServiceAsync
{
	void getUsuario( String username, AsyncCallback< UsuarioDTO > callback );
	void save( UsuarioDTO usuarioDTO, AsyncCallback< Void > callback );
	void remove( List< UsuarioDTO > usuarioDTOList, AsyncCallback< Void > callback );
	void avoidSessionExpire( AsyncCallback< Boolean > callback  );
	void getBuscaList( String nome, String username, String email,
		PagingLoadConfig config, AsyncCallback< PagingLoadResult< UsuarioDTO > > callback );
	void getCurrentUser( AsyncCallback< UsuarioDTO > callback );
}
