package com.gapso.web.trieda.shared.i18n;

import com.google.gwt.i18n.client.Messages;

public interface TriedaI18nMessages
	extends Messages
{
	String confirmacaoButton();
	String erroExclusaoAreaTitulacao();
	String erroExclusaoAreaTitulacaoTitle();
	String excelErroArquivoInvalido( String nomeArquivo, String motivo );
	String excelErroBD( String nomeArquivo, String motivo );
	String excelErroGenericoExportacao( String motivo );
	String excelErroImportadorNulo( String infoASerImportada );
	String excelErroLogicoEntidadesNaoCadastradas( String nomeColuna, String linhasComErro );
	String excelErroLogicoDisciplinaEmMatrizCurricular( String nomeColuna, String linhasComErro );
	String excelErroLogicoUnicidadeViolada( String valorRepetido, String linhasComErro );
	String excelErroLogicoUnicidadeVioladaCurriculoPorCurso( String valorRepetido, String linhasComErro );
	String excelErroLogicoUnicidadeVioladaCurriculoPorDescricao( String valorRepetido, String linhasComErro );
	String excelErroLogicoUnicidadeVioladaDisciplinaCurriculo( String disciplina, String periodo, String curriculo, String linhasComErro );
	String excelErroObterExcelTemplate( String nomeTemplate, String nomeRelatorio, String motivo );
	String excelErroSintaticoCabecalhoAusente( String cabecalho, String nomeArquivo );
	String excelErroSintaticoColunaVazia( String linhasComErro, String nomeColuna );
	String excelErroSintaticoFormatoInvalido( String linhasComErro, String nomeColuna );
	String excelErroSintaticoLinhasInvalidas( String linhasComErro, String nomeArquivo );
	String excelErroSintaticoLinhaVazia( String linhasComErro );
	String excelErroSintaticoValorInvalido( String linhasComErro, String nomeColuna );
	String excelErroSintaticoValorNegativo( String linhasComErro, String nomeColuna );
	String falhaOperacao();
	String loading();
	String ofertasNaoCadastradas( String campus );
	String sucessoImportacaoExcel();
	String sucessoRemoverDoBD( String codigoItem );
	String sucessoSalvarNoBD( String codigoItem );

  // Exemplo com data
  // @DefaultMessage( "Last update: { 0, date, medium } { 0, time, medium }" )
  // String lastUpdate( Date timestamp );
}
