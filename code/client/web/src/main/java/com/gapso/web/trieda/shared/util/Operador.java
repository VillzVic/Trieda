package com.gapso.web.trieda.shared.util;

public enum Operador {
		IGUAL ( "Igual a", " = " ),
	    DIFERENTE ( "Diferente de", " <> " ),
	    MENOR_QUE ( "Menor que"," < " ),
	    MENOR_IGUAL ( "Menor ou igual a"," <= " ),
	    MAIOR_QUE ( "Maior que"," > " ),
	    MAIOR_IGUAL( "Maior ou igual a"," >= " );

	    private String nome;
	    private String operadorSQL;

	    Operador( String nome, String operadorSQL )
	    {
	    	this.nome = nome;
	    	this.operadorSQL = operadorSQL;
	    }

	    public String getNome()
	    {
	    	return this.nome;
	    }

	    @Override
	    public String toString()
	    {
	    	return this.getNome();
	    }

		public String getOperadorSQL() {
			return operadorSQL;
		}
	}
