package com.gapso.trieda.misc;


public enum Dificuldades {
	
    MEDIO, BAIXO, ALTO;
    
    public static Dificuldades get(String name) {
    	for(Dificuldades s : Dificuldades.values()) {
    		if(s.name().equals(name)) return s;
    	}
    	return null;
    }
}
