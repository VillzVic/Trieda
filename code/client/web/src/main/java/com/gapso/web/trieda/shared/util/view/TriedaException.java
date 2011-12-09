package com.gapso.web.trieda.shared.util.view;

import java.io.Serializable;

public class TriedaException
	extends Exception
	implements Serializable
{
	private static final long serialVersionUID = 7252239658737885318L;
	private String completeMessage;

	public TriedaException( )
	{
		super();
		this.completeMessage = "";
	}

	public TriedaException( Exception e )
	{
		super( e.getCause() );
		this.completeMessage = extractMessage(e);
	}
	
	public String getCompleteMessage() {
		return completeMessage;
	}
	
	private String extractMessage(Throwable caught) {
		String caughtMessage = "";
		if (caught != null) {
			caughtMessage = "Message: " + caught.getMessage();
			Throwable throwable = caught.getCause();
			while (throwable != null) {
				caughtMessage += "\nCause: " + throwable.getMessage();
				throwable = throwable.getCause();
			}
		}
		return caughtMessage;
	}
}