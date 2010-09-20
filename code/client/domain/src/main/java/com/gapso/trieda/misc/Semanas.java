package com.gapso.trieda.misc;


public enum Semanas {

    SEG, TER, QUA, QUI, SEX, SAB, DOM;
    
    public static Semanas get(String name) {
    	for(Semanas s : Semanas.values()) {
    		if(s.name().equals(name)) return s;
    	}
    	return null;
    }
}
