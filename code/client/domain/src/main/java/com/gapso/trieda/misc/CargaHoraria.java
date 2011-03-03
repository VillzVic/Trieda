package com.gapso.trieda.misc;


public enum CargaHoraria {
	
    CONCENTRAR("Concentrar em poucos dias"),
    DISTRIBUIR("Distribuir em todos os dias"),
    INDIFERENTE("Indiferente");
    
    private String text;
    
    CargaHoraria(String text) {
    	this.text = text;
    }

	public String getText() {
		return text;
	}

	@Override
	public String toString() {
		return text;
	}
    
}
