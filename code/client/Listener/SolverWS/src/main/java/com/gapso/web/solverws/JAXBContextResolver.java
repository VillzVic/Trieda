package com.gapso.web.solverws;

import java.util.HashMap;
import java.util.Map;

import javax.ws.rs.ext.ContextResolver;
import javax.ws.rs.ext.Provider;
import javax.xml.bind.JAXBContext;

import com.sun.jersey.api.json.JSONJAXBContext;

@Provider
public class JAXBContextResolver implements ContextResolver<JAXBContext> {

	private JAXBContext context;
	@SuppressWarnings("rawtypes")
	private Class[] types = { SolverResponse.class };

	@SuppressWarnings("deprecation")
	public JAXBContextResolver() throws Exception {
		Map<String, Object> props = new HashMap<String, Object>();
		props.put(JSONJAXBContext.JSON_NOTATION, "MAPPED_JETTISON");
		props.put(JSONJAXBContext.JSON_ROOT_UNWRAPPING, Boolean.FALSE);
		this.context = new JSONJAXBContext(types, props);
	}

	public JAXBContext getContext(Class<?> objectType) {
		return (types[0].equals(objectType)) ? context : null;
	}

}
