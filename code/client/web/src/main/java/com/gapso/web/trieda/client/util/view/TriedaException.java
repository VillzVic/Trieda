package com.gapso.web.trieda.client.util.view;

public class TriedaException extends Exception {

	private static final long serialVersionUID = 7252239658737885318L;
	
	public TriedaException( ) {}
	
	public TriedaException(Exception e) {
		super(e.getMessage(),e.getCause());
	}
}