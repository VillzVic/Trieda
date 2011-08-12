package com.gapso.web.trieda.server.util;

import java.util.Locale;

import com.gapso.gipe.common.GLanguageManager;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class GTriedaI18nMessages implements TriedaI18nMessages {
	private GLanguageManager languageManager = null;

	public GTriedaI18nMessages() {
		this.languageManager = new GLanguageManager("TriedaI18nMessages",
				new Locale("pt", "BR"));
	}

	@Override
	public String confirmacaoButton() {
		return languageManager.getText("confirmacaoButton");
	}

	@Override
	public String excelErroArquivoInvalido(String nomeArquivo, String motivo) {
		String[] params = { nomeArquivo, motivo };
		return languageManager.getFormattedText("excelErroArquivoInvalido",
				params);
	}

	@Override
	public String excelErroBD(String nomeArquivo, String motivo) {
		String[] params = { nomeArquivo, motivo };
		return languageManager.getFormattedText("excelErroBD", params);
	}

	@Override
	public String excelErroImportadorNulo(String infoASerImportada) {
		String[] params = { infoASerImportada };
		return languageManager.getFormattedText("excelErroImportadorNulo",
				params);
	}

	@Override
	public String excelErroLogicoEntidadesNaoCadastradas(String nomeColuna,
			String linhasComErro) {
		String[] params = { nomeColuna, linhasComErro };
		return languageManager.getFormattedText(
				"excelErroLogicoEntidadesNaoCadastradas", params);
	}

	@Override
	public String excelErroLogicoDisciplinaEmMatrizCurricular(
			String nomeColuna, String linhasComErro) {
		String[] params = { nomeColuna, linhasComErro };
		return languageManager.getFormattedText(
				"excelErroLogicoDisciplinaEmMatrizCurricular", params);
	}

	@Override
	public String excelErroLogicoUnicidadeViolada(String valorRepetido,
			String linhasComErro) {
		String[] params = { valorRepetido, linhasComErro };
		return languageManager.getFormattedText(
				"excelErroLogicoUnicidadeViolada", params);
	}

	@Override
	public String excelErroLogicoUnicidadeVioladaCurriculoPorCurso(
			String valorRepetido, String linhasComErro) {
		String[] params = { valorRepetido, linhasComErro };
		return languageManager.getFormattedText(
				"excelErroLogicoUnicidadeVioladaCurriculoPorCurso", params);
	}

	@Override
	public String excelErroLogicoUnicidadeVioladaCurriculoPorDescricao(
			String valorRepetido, String linhasComErro) {
		String[] params = { valorRepetido, linhasComErro };
		return languageManager.getFormattedText(
				"excelErroLogicoUnicidadeVioladaCurriculoPorDescricao", params);
	}

	@Override
	public String excelErroLogicoUnicidadeVioladaDisciplinaCurriculo(
			String disciplina, String periodo, String curriculo,
			String linhasComErro) {
		String[] params = { disciplina, periodo, curriculo, linhasComErro };
		return languageManager.getFormattedText(
				"excelErroLogicoUnicidadeVioladaDisciplinaCurriculo", params);
	}

	@Override
	public String excelErroObterExcelTemplate(String nomeTemplate,
			String nomeRelatorio, String motivo) {
		String[] params = { nomeTemplate, nomeRelatorio, motivo };
		return languageManager.getFormattedText("excelErroObterExcelTemplate",
				params);
	}

	@Override
	public String excelErroSintaticoCabecalhoAusente(String cabecalho,
			String nomeArquivo) {
		String[] params = { cabecalho, nomeArquivo };
		return languageManager.getFormattedText(
				"excelErroSintaticoCabecalhoAusente", params);
	}

	@Override
	public String excelErroSintaticoColunaVazia(String linhasComErro,
			String nomeColuna) {
		String[] params = { linhasComErro, nomeColuna };
		return languageManager.getFormattedText(
				"excelErroSintaticoColunaVazia", params);
	}

	@Override
	public String excelErroSintaticoFormatoInvalido(String linhasComErro,
			String nomeColuna) {
		String[] params = { linhasComErro, nomeColuna };
		return languageManager.getFormattedText(
				"excelErroSintaticoFormatoInvalido", params);
	}

	@Override
	public String excelErroSintaticoLinhasInvalidas(String linhasComErro,
			String nomeArquivo) {
		String[] params = { linhasComErro, nomeArquivo };
		return languageManager.getFormattedText(
				"excelErroSintaticoLinhasInvalidas", params);
	}

	@Override
	public String excelErroSintaticoLinhaVazia(String linhasComErro) {
		String[] params = { linhasComErro };
		return languageManager.getFormattedText("excelErroSintaticoLinhaVazia",
				params);
	}

	@Override
	public String excelErroSintaticoValorInvalido(String linhasComErro,
			String nomeColuna) {
		String[] params = { linhasComErro, nomeColuna };
		return languageManager.getFormattedText(
				"excelErroSintaticoValorInvalido", params);
	}

	@Override
	public String excelErroSintaticoValorNegativo(String linhasComErro,
			String nomeColuna) {
		String[] params = { linhasComErro, nomeColuna };
		return languageManager.getFormattedText(
				"excelErroSintaticoValorNegativo", params);
	}

	@Override
	public String falhaOperacao() {
		return languageManager.getText("falhaOperacao");
	}

	@Override
	public String loading() {
		return languageManager.getText("loading");
	}

	@Override
	public String sucessoImportacaoExcel() {
		return languageManager.getText("sucessoImportacaoExcel");
	}

	@Override
	public String sucessoRemoverDoBD(String codigoItem) {
		String[] params = { codigoItem };
		return languageManager.getFormattedText("sucessoRemoverDoBD", params);
	}

	@Override
	public String sucessoSalvarNoBD(String codigoItem) {
		String[] params = { codigoItem };
		return languageManager.getFormattedText("sucessoSalvarNoBD", params);
	}

	@Override
	public String erroExclusaoAreaTitulacao() {
		return languageManager.getText("erroExclusaoAreaTitulacao");
	}

	@Override
	public String erroExclusaoAreaTitulacaoTitle() {
		return languageManager.getText("erroExclusaoAreaTitulacaoTitle");
	}

	@Override
	public String ofertasNaoCadastradas(String campus) {
		String[] params = { campus };
		return languageManager.getFormattedText("ofertasNaoCadastradas", params);
	}
}
