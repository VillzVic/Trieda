package com.gapso.trieda.misc;


public enum Semanas {

    SEG, TER, QUA, QUI, SEX, SAB, DOM;
    
    public static Semanas get(String name) {
    	for(Semanas s : Semanas.values()) {
    		if(s.name().equals(name)) return s;
    	}
    	return null;
    }
    
    public static Semanas get(int intValue) {
    	switch (intValue) {
			case 1: return DOM;
			case 2: return SEG;
			case 3: return TER;
			case 4: return QUA;
			case 5: return QUI;
			case 6: return SEX;
			case 7: return SAB;
    	}
    	return null;
    }
    
    public static int toInt(Semanas semana) {
    	switch (semana) {
			case DOM: return 1;
			case SEG: return 2;
			case TER: return 3;
			case QUA: return 4;
			case QUI: return 5;
			case SEX: return 6;
			case SAB: return 7;
		}
    	return -1;
    }
}
