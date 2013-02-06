package com.gapso.trieda.domain;


public class TriedaTrio<P, S, T> {

	private P primeiro;
	private S segundo;
	private T terceiro;
	
	static public <P,S,T> TriedaTrio<P,S,T> create(P primeiro, S segundo, T terceiro) {
		return new TriedaTrio<P,S,T>(primeiro,segundo,terceiro);
	}

	private TriedaTrio() {
		super();
	}

	private TriedaTrio(P primeiro, S segundo, T terceiro) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
		this.terceiro = terceiro;
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
	
	public T getTerceiro() {
		return terceiro;
	}

	public void setTerceiro(T terceiro) {
		this.terceiro = terceiro;
	}

	@Override
	public String toString() {
		return "("+primeiro.toString()+","+segundo.toString()+","+terceiro.toString()+")";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result
				+ ((primeiro == null) ? 0 : primeiro.hashCode());
		result = prime * result + ((segundo == null) ? 0 : segundo.hashCode());
		result = prime * result
				+ ((terceiro == null) ? 0 : terceiro.hashCode());
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
		TriedaTrio<P,S,T> other = (TriedaTrio<P,S,T>) obj;
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
		if (terceiro == null) {
			if (other.terceiro != null)
				return false;
		} else if (!terceiro.equals(other.terceiro))
			return false;
		return true;
	}
}