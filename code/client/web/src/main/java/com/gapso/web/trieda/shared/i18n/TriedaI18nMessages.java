package com.gapso.web.trieda.shared.i18n;

import com.google.gwt.i18n.client.Messages;

public interface TriedaI18nMessages extends Messages {
	
	String excelErroArquivoInvalido(String nomeArquivo, String motivo);
	String excelErroBD(String nomeArquivo, String motivo);
	String excelErroImportadorNulo(String infoASerImportada);
	String excelErroLogicoEntidadesNaoCadastradas(String nomeColuna, String linhasComErro);
	String excelErroLogicoUnicidadeViolada(String valorRepetido, String linhasComErro);
	String excelErroObterExcelTemplate(String nomeTemplate, String nomeRelatorio, String motivo);
	String excelErroSintaticoCabecalhoAusente(String cabecalho, String nomeArquivo);
	String excelErroSintaticoColunaVazia(String linhasComErro, String nomeColuna);
	String excelErroSintaticoFormatoInvalido(String linhasComErro, String nomeColuna);
	String excelErroSintaticoLinhasInvalidas(String linhasComErro, String nomeArquivo);
	String excelErroSintaticoLinhaVazia(String linhasComErro);
	String excelErroSintaticoValorInvalido(String linhasComErro, String nomeColuna);
	String excelErroSintaticoValorNegativo(String linhasComErro, String nomeColuna);
	String falhaOperacao();
	String sucessoImportacaoExcel();
	String sucessoRemoverDoBD(String codigoItem);
	String sucessoSalvarNoBD(String codigoItem);
	String confirmacaoButton();
	String loading();

  // Exemplo com data
  //@DefaultMessage("Last update: {0,date,medium} {0,time,medium}")
  //String lastUpdate(Date timestamp);
}