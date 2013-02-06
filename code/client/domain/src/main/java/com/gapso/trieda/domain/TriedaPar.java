package com.gapso.trieda.domain;


public class TriedaPar<P, S> {

	private P primeiro;
	private S segundo;
	
	static public <P,S> TriedaPar<P,S> create(P primeiro, S segundo) {
		return new TriedaPar<P,S>(primeiro,segundo);
	}

	private TriedaPar() {
		super();
	}

	private TriedaPar(P primeiro, S segundo) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
	}

	public P getPrimeiro() {
		return primeiro;
	}

	public void setPrimeiro(P primeiro) {
		this.primeiro = primeiro;
	}

	public S getSegundo() {
		return segundo;
	}

	public void setSegundo(S segundo) {
		this.segundo = segundo;
	}

	@Override
	public String toString() {
		return "("+primeiro.toString()+","+segundo.toString()+")";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result
				+ ((primeiro == null) ? 0 : primeiro.hashCode());
		result = prime * result + ((segundo == null) ? 0 : segundo.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		TriedaPar<P, S> other = (TriedaPar<P, S>) obj;
		if (primeiro == null) {
			if (other.primeiro != null)
				return false;
		} else if (!primeiro.equals(other.primeiro))
			return false;
		if (segundo == null) {
			if (other.segundo != null)
				return false;
		} else if (!segundo.equals(other.segundo))
			return false;
		return true;
	}
}