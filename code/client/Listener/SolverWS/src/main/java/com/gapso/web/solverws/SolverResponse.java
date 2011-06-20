package com.gapso.web.solverws;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class SolverResponse {

	private Boolean status;
	private Object object;
	
	public SolverResponse(){
		
	}
	
	public SolverResponse(Boolean status, Object object){
		this.status = status;
		this.object = object;
	}

	public Boolean getStatus() {
		return status;
	}

	public void setStatus(Boolean status) {
		this.status = status;
	}

	public Object getObject() {
		return object;
	}

	public void setObject(Object object) {
		this.object = object;
	}

}
