package com.gapso.web.trieda.server.util;

import java.util.Locale;

import com.gapso.gipe.common.GLanguageManager;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class GTriedaI18nMessages
	implements TriedaI18nMessages
{
	private GLanguageManager languageManager = null;

	public GTriedaI18nMessages()
	{
		this.languageManager = new GLanguageManager(
			"TriedaI18nMessages", new Locale( "pt", "BR" ) );
	}

	@Override
	public String confirmacaoButton()
	{
		return this.languageManager.getText( "confirmacaoButton" );
	}
	
	@Override
	public String erroAoSalvar(String entidade) {
		return this.languageManager.getText( "erroAoSalvar" );
	}
	
	@Override
	public String erroAoRemover(String entidade) {
		return this.languageManager.getText( "erroAoRemover" );
	}

	@Override
	public String excelErroArquivoInvalido(
		String nomeArquivo, String motivo )
	{
		String [] params = { nomeArquivo, motivo };

		return this.languageManager.getFormattedText(
			"excelErroArquivoInvalido", params );
	}

	@Override
	public String excelErroBD( String nomeArquivo, String motivo )
	{
		String [] params = { nomeArquivo, motivo };

		return this.languageManager.getFormattedText( "excelErroBD", params );
	}
	
	@Override
	public String excelErroGenericoExportacao( String motivo )
	{
		String [] params = { motivo };

		return this.languageManager.getFormattedText( "excelErroGenericoExportacao", params );
	}
	
	@Override
	public String excelErroGenericoImportacao(String motivo) {
		String [] params = {motivo};
		return this.languageManager.getFormattedText("excelErroGenericoImportacao",params);
	}

	@Override
	public String excelErroImportadorNulo( String infoASerImportada )
	{
		String [] params = { infoASerImportada };

		return this.languageManager.getFormattedText(
			"excelErroImportadorNulo", params );
	}

	@Override
	public String excelErroLogicoEntidadesNaoCadastradas(
		String nomeColuna, String linhasComErro )
	{
		String [] params = { nomeColuna, linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroLogicoEntidadesNaoCadastradas", params );
	}

	@Override
	public String excelErroLogicoDisciplinaEmMatrizCurricular(
		String nomeColuna, String linhasComErro )
	{
		String [] params = { nomeColuna, linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroLogicoDisciplinaEmMatrizCurricular", params );
	}
	
	@Override
	public String excelErroLogicoDisciplinaSemCurriculo( String linhasComErro ) {
		String [] params = { linhasComErro };
		return this.languageManager.getFormattedText("excelErroLogicoDisciplinaSemCurriculo",params);
	}

	@Override
	public String excelErroLogicoUnicidadeViolada(
		String valorRepetido, String linhasComErro )
	{
		String [] params = { valorRepetido, linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroLogicoUnicidadeViolada", params );
	}

	@Override
	public String excelErroLogicoUnicidadeVioladaCurriculoPorCurso(
		String valorRepetido, String linhasComErro )
	{
		String [] params = { valorRepetido, linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroLogicoUnicidadeVioladaCurriculoPorCurso", params );
	}

	@Override
	public String excelErroLogicoUnicidadeVioladaCurriculoPorDescricao(
		String valorRepetido, String linhasComErro )
	{
		String [] params = { valorRepetido, linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroLogicoUnicidadeVioladaCurriculoPorDescricao", params );
	}

	@Override
	public String excelErroLogicoUnicidadeVioladaDisciplinaCurriculo(
		String disciplina, String periodo, String curriculo, String linhasComErro )
	{
		String [] params = { disciplina, periodo, curriculo, linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroLogicoUnicidadeVioladaDisciplinaCurriculo", params );
	}

	@Override
	public String excelErroObterExcelTemplate(
		String nomeTemplate, String nomeRelatorio, String motivo )
	{
		String [] params = { nomeTemplate, nomeRelatorio, motivo };

		return this.languageManager.getFormattedText(
			"excelErroObterExcelTemplate", params );
	}

	@Override
	public String excelErroSintaticoCabecalhoAusente(
		String cabecalho, String nomeArquivo )
	{
		String [] params = { cabecalho, nomeArquivo };

		return this.languageManager.getFormattedText(
			"excelErroSintaticoCabecalhoAusente", params );
	}

	@Override
	public String excelErroSintaticoColunaVazia(
		String linhasComErro, String nomeColuna )
	{
		String [] params = { linhasComErro, nomeColuna };

		return languageManager.getFormattedText(
			"excelErroSintaticoColunaVazia", params );
	}

	@Override
	public String excelErroSintaticoFormatoInvalido(
		String linhasComErro, String nomeColuna )
	{
		String [] params = { linhasComErro, nomeColuna };

		return this.languageManager.getFormattedText(
			"excelErroSintaticoFormatoInvalido", params );
	}

	@Override
	public String excelErroSintaticoLinhasInvalidas(
		String linhasComErro, String nomeArquivo )
	{
		String [] params = { linhasComErro, nomeArquivo };

		return this.languageManager.getFormattedText(
			"excelErroSintaticoLinhasInvalidas", params );
	}

	@Override
	public String excelErroSintaticoLinhaVazia( String linhasComErro )
	{
		String [] params = { linhasComErro };

		return this.languageManager.getFormattedText(
			"excelErroSintaticoLinhaVazia", params );
	}

	@Override
	public String excelErroSintaticoValorInvalido(
		String linhasComErro, String nomeColuna )
	{
		String [] params = { linhasComErro, nomeColuna };

		return this.languageManager.getFormattedText(
			"excelErroSintaticoValorInvalido", params );
	}

	@Override
	public String excelErroSintaticoValorNegativo(
		String linhasComErro, String nomeColuna )
	{
		String [] params = { linhasComErro, nomeColuna };

		return this.languageManager.getFormattedText(
			"excelErroSintaticoValorNegativo", params );
	}

	@Override
	public String falhaOperacao()
	{
		return this.languageManager.getText( "falhaOperacao" );
	}

	@Override
	public String loading()
	{
		return this.languageManager.getText( "loading" );
	}

	@Override
	public String sucessoImportacaoExcel()
	{
		return this.languageManager.getText(
			"sucessoImportacaoExcel" );
	}

	@Override
	public String sucessoRemoverDoBD( String codigoItem )
	{
		String [] params = { codigoItem };

		return this.languageManager.getFormattedText(
			"sucessoRemoverDoBD", params );
	}

	@Override
	public String sucessoSalvarNoBD( String codigoItem )
	{
		String [] params = { codigoItem };

		return this.languageManager.getFormattedText(
			"sucessoSalvarNoBD", params );
	}

	@Override
	public String erroExclusaoAreaTitulacao()
	{
		return this.languageManager.getText(
			"erroExclusaoAreaTitulacao" );
	}

	@Override
	public String erroExclusaoAreaTitulacaoTitle()
	{
		return this.languageManager.getText(
			"erroExclusaoAreaTitulacaoTitle" );
	}

	@Override
	public String ofertasNaoCadastradas( String campus )
	{
		String [] params = { campus };

		return this.languageManager.getFormattedText(
			"ofertasNaoCadastradas", params );
	}
}
