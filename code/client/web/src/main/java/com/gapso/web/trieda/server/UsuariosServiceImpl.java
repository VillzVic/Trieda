package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Usuario;
import com.gapso.web.trieda.client.services.UsuariosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class UsuariosServiceImpl extends RemoteServiceServlet implements UsuariosService {

	private static final long serialVersionUID = 5672570072070386404L;

	@Override
	public UsuarioDTO getUsuario(String username) {
		return ConvertBeans.toUsuarioDTO(Usuario.find(username));
	}
	
	@Override
	public PagingLoadResult<UsuarioDTO> getBuscaList(String nome, String username, String email, PagingLoadConfig config) {
		List<UsuarioDTO> list = new ArrayList<UsuarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		List<Usuario> usuarios = Usuario.findAllBy(nome, username, email, config.getOffset(), config.getLimit(), orderBy);
		for(Usuario usuario : usuarios) {
			list.add(ConvertBeans.toUsuarioDTO(usuario));
		}
		BasePagingLoadResult<UsuarioDTO> result = new BasePagingLoadResult<UsuarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Usuario.count(nome, username, email));
		return result;
	}

	@Override
	public void save(UsuarioDTO usuarioDTO) {
		Usuario usuario = ConvertBeans.toUsuario(usuarioDTO);
		if(usuario.getVersion() != null) {
			usuario.merge();
		} else {
			usuario.persist();
		}
	}
	
	@Override
	public void remove(List<UsuarioDTO> usuarioDTOList) {
		for(UsuarioDTO usuarioDTO : usuarioDTOList) {
			ConvertBeans.toUsuario(usuarioDTO).remove();
		}
	}

}
