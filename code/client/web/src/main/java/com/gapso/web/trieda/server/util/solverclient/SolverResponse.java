package com.gapso.web.trieda.server.util.solverclient;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class SolverResponse
{
	private Boolean status;
	private Object object;
	
	public SolverResponse()
	{
		super();
	}

	public SolverResponse(
		Boolean status, Object object )
	{
		this.status = status;
		this.object = object;
	}

	public Boolean getStatus()
	{
		return this.status;
	}

	public void setStatus( Boolean status )
	{
		this.status = status;
	}

	public Object getObject()
	{
		return this.object;
	}

	public void setObject( Object object )
	{
		this.object = object;
	}
}
