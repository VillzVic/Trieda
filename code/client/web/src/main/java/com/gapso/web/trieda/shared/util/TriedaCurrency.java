package com.gapso.web.trieda.shared.util;

import com.google.gwt.user.client.rpc.IsSerializable;

/*
 * Observação : Ao usar essa classe em objetos do tipo 'DTO', não se pode
 * setar diretamente o objeto com o método 'set' da classe 'BaseModel',
 * mas sim setar o string correspondente ao objeto 'TriedaCurrency'
 * */
public class TriedaCurrency implements IsSerializable {
	private static final long serialVersionUID = -4919776123579824061L;
	private Double value;

	public TriedaCurrency() {
		this.setValue(0.0);
	}

	public TriedaCurrency(Double v) {
		this.setValue(v);
	}

	@Override
	public int hashCode() {
		return super.hashCode();
	}

	@Override
	public boolean equals(Object obj) {
		return super.equals(obj);
	}

	@Override
	public String toString() {
		long y = (long) (this.value * 100);
		Double d = ((double) y / 100);

		Double resto = d * 100;
		Integer resto2 = resto.intValue() % 100; // Casas decimais
		String str1 = (resto == 0 ? "00" : ((resto >= 10) ? resto2.toString()
				: "0" + resto2.toString()));

		Integer i = this.value.intValue();
		String str2 = i.toString(); // Parte inteira do número

		String result = (str2 + "," + str1);
		return result;
	}

	public Double getDoubleValue() {
		return this.value;
	}

	public void setDoubleValue(Double v) {
		this.setValue(v);
	}

	private void setValue(Double v) {
		this.value = (v == null ? 0.0 : v);
	}
}
