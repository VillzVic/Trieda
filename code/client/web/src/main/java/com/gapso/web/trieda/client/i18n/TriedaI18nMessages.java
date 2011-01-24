package com.gapso.web.trieda.client.i18n;

import com.google.gwt.i18n.client.Messages;

public interface TriedaI18nMessages extends Messages {
	
	String falhaOperacaoBD(String falha);
	String sucessoRemoverDoBD(String codigoItem);
	String sucessoSalvarNoBD(String codigoItem);

  // Exemplo com data
  //@DefaultMessage("Last update: {0,date,medium} {0,time,medium}")
  //String lastUpdate(Date timestamp);
}
