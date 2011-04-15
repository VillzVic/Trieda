package com.gapso.web.trieda.server;

import org.springframework.security.core.context.SecurityContext;
import org.springframework.security.core.context.SecurityContextHolder;

import com.gapso.trieda.domain.Usuario;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

public class RemoteService extends RemoteServiceServlet {

	private static final long serialVersionUID = 5939530994258762775L;

	protected Usuario getUsuario() {
		SecurityContext context = SecurityContextHolder.getContext();
		String username = context.getAuthentication().getName();
		return Usuario.find(username);
	}
	
	protected boolean isProfessor() {
		return getUsuario().getProfessor() != null;
	}
	protected void onlyProfessor() {
		if(!isProfessor()) {
			throw new RuntimeException("Permissão negada, usuário \""+getUsuario().getUsername()+"\" não é de Professor");
		}
	}
	
	protected boolean isAdministrador() {
		return getUsuario().getProfessor() == null;
	}
	protected void onlyAdministrador() {
		if(isAdministrador()) {
			throw new RuntimeException("Permissão negada, usuário \""+getUsuario().getUsername()+"\" não é de Administrador");
		}
	}
	
	
}
