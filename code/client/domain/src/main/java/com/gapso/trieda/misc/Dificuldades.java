package com.gapso.trieda.misc;


public enum Dificuldades {
	
    MEDIO, BAIXO, ALTO;
    
    public static Dificuldades get(String name) {
    	for(Dificuldades s : Dificuldades.values()) {
    		if(s.name().equals(name)) return s;
    	}
    	return null;
    }
    
    public static int toInt(Dificuldades dificuldade) {
    	switch (dificuldade) {
    		case BAIXO: return 1;
			case MEDIO: return 2;
			case ALTO:  return 3;
		}
    	return -1;
    }
}
