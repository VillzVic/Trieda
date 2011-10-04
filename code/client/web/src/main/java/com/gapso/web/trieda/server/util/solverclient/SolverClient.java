package com.gapso.web.trieda.server.util.solverclient;

import javax.ws.rs.core.MediaType;

import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.WebResource;
import com.sun.jersey.core.util.MultivaluedMapImpl;
import com.sun.jersey.multipart.BodyPart;
import com.sun.jersey.multipart.MultiPart;

public class SolverClient
	implements ISolverClient
{
	private String url;
	private String problemName;
	private Client client = Client.create();
	private WebResource webResource;

	public SolverClient( String url, String problemName )
	{
		this.url = url;
		this.problemName = problemName;
		this.client = Client.create();
		this.webResource = this.client.resource( getUrl() );
	}

	public String getUrl()
	{
		return this.url;
	}

	public String getProblemName()
	{
		return this.problemName;
	}

	@Override
	public String help()
	{
		return this.webResource.path(
			"/help" ).get( String.class );
	}

	@Override
	public String version()
	{
		return this.webResource.path( "/version" ).get( String.class );
	}

	@Override
	public boolean isFinished( Long round )
	{
		MultivaluedMapImpl queryParams = new MultivaluedMapImpl();

		queryParams.add( "round", round );
		SolverResponse sr = webResource.path(
			"/isFinished/" + getProblemName() + "/" )
			.queryParams( queryParams ).post( SolverResponse.class );

		return ( ( sr.getStatus() ) ? (Boolean) sr.getObject() : false );
	}

	@Override
	public boolean containsResult( Long round )
	{
		MultivaluedMapImpl queryParams = new MultivaluedMapImpl();

		queryParams.add( "round", round );
		SolverResponse sr = this.webResource.path(
			"/containsResult/" + getProblemName() + "/" )
			.queryParams(queryParams).post( SolverResponse.class );

		return ( ( sr.getStatus() ) ? (Boolean) sr.getObject() : false );
	}

	@Override
	public long requestOptimization( byte [] fileBytes )
	{
		this.webResource = this.webResource.path(
			"/requestOptimization/"	+ getProblemName() + "/" );

		MultiPart multiPart = new MultiPart().bodyPart(
			new BodyPart( fileBytes, MediaType.APPLICATION_OCTET_STREAM_TYPE ) );

		SolverResponse sr = this.webResource.type(
			"multipart/mixed" ).post( SolverResponse.class, multiPart );

		return ( sr.getStatus() ? (Long) sr.getObject() : -1 );
	}

	@Override
	public byte [] getContent( Long round )
	{
		MultivaluedMapImpl queryParams = new MultivaluedMapImpl();

		queryParams.add( "round", round );

		SolverResponse sr = this.webResource.path(
			"/getContent/" + getProblemName() + "/" )
			.queryParams( queryParams ).post( SolverResponse.class );

		return ( ( sr.getStatus() ) ? (byte[]) sr.getObject() : null );
	}
}
