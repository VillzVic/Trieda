package com.gapso.web.trieda.server.util;

import java.util.Locale;

import com.gapso.gipe.common.GLanguageManager;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;

public class GTriedaI18nConstants
	implements TriedaI18nConstants
{
	private GLanguageManager languageManager = null;

	public GTriedaI18nConstants()
	{
		this.languageManager = new GLanguageManager(
			"TriedaI18nConstants", new Locale( "pt", "BR" ) );
	}

	@Override
	public String adicionar() {
		return languageManager.getText("adicionar");
	}

	@Override
	public String andar() {
		return languageManager.getText("andar");
	}

	@Override
	public String areaTitulacao() {
		return languageManager.getText("areaTitulacao");
	}

	@Override
	public String areasTitulacao() {
		return languageManager.getText("areasTitulacao");
	}

	@Override
	public String bairro() {
		return languageManager.getText("bairro");
	}

	@Override
	public String campi() {
		return languageManager.getText("campi");
	}

	@Override
	public String campiHeadingPanel() {
		return languageManager.getText("campiHeadingPanel");
	}

	@Override
	public String campiTrabalho() {
		return languageManager.getText("campiTrabalho");
	}

	@Override
	public String campus() {
		return languageManager.getText("campus");
	}

	@Override
	public String capacidade() {
		return languageManager.getText("capacidade");
	}

	@Override
	public String capacidadeMediaSalas() {
		return languageManager.getText("capacidadeMediaSalas");
	}

	@Override
	public String cargaHorariaAnterior() {
		return languageManager.getText("cargaHorariaAnterior");
	}

	@Override
	public String cargaHorariaMax() {
		return languageManager.getText("cargaHorariaMax");
	}

	@Override
	public String cargaHorariaMin() {
		return languageManager.getText("cargaHorariaMin");
	}

	@Override
	public String codigo() {
		return languageManager.getText("codigo");
	}

	@Override
	public String confirmacao() {
		return languageManager.getText("confirmacao");
	}

	@Override
	public String cpf() {
		return languageManager.getText("cpf");
	}

	@Override
	public String cpfProfessor() {
		return languageManager.getText("cpfProfessor");
	}

	@Override
	public String creditos() {
		return languageManager.getText("creditos");
	}

	@Override
	public String creditosPraticos() {
		return languageManager.getText("creditosPraticos");
	}

	@Override
	public String creditosTeoricos() {
		return languageManager.getText("creditosTeoricos");
	}

	@Override
	public String curriculos() {
		return languageManager.getText("curriculos");
	}

	@Override
	public String curso() {
		return languageManager.getText("curso");
	}

	@Override
	public String cursos() {
		return languageManager.getText("cursos");
	}

	@Override
	public String cursosAreasTitulacao() {
		return languageManager.getText("cursosAreasTitulacao");
	}

	@Override
	public String cursou() {
		return languageManager.getText("cursou");
	}

	@Override
	public String custoDocente() {
		return languageManager.getText("custoDocente");
	}

	@Override
	public String custoMedioCredito() {
		return languageManager.getText("custoMedioCredito");
	}

	@Override
	public String custoMedioCreditoExcel() {
		return languageManager.getText("custoMedioCreditoExcel");

	}

	@Override
	public String demandaDeAlunos() {
		return languageManager.getText("demandaDeAlunos");
	}

	@Override
	public String descricao() {
		return languageManager.getText("descricao");
	}

	@Override
	public String deslocamentoUnidades() {
		return languageManager.getText("deslocamentoUnidades");
	}

	@Override
	public String deslocamentoUnidadesCampus() {
		return languageManager.getText("deslocamentoUnidadesCampus");
	}

	@Override
	public String disciplina() {
		return languageManager.getText("disciplina");
	}

	@Override
	public String disciplinas() {
		return languageManager.getText("disciplinas");
	}

	@Override
	public String disciplinasAssociadas() {
		return languageManager.getText("disciplinasAssociadas");
	}

	@Override
	public String disciplinasSalas() {
		return languageManager.getText("disciplinasSalas");
	}

	@Override
	public String disponibilidadesSemanaLetiva() {
		return languageManager.getText("disponibilidadesSemanaLetiva");
	}

	@Override
	public String edicaoDe() {
		return languageManager.getText("edicaoDe");
	}

	@Override
	public String editar() {
		return languageManager.getText("editar");
	}

	@Override
	public String elimina() {
		return languageManager.getText("elimina");
	}

	@Override
	public String equivalencias() {
		return languageManager.getText("equivalencias");
	}

	@Override
	public String estado() {
		return languageManager.getText("estado");
	}

	@Override
	public String exportarExcel() {
		return languageManager.getText("exportarExcel");
	}

	@Override
	public String habilitacaoProfessores() {
		return languageManager.getText("habilitacaoProfessores");
	}

	@Override
	public String gruposSalas() {
		return languageManager.getText("gruposSalas");
	}

	@Override
	public String importarExcel() {
		return languageManager.getText("importarExcel");
	}

	@Override
	public String informacao() {
		return languageManager.getText("informacao");
	}

	@Override
	public String insercaoDe() {
		return languageManager.getText("insercaoDe");
	}

	@Override
	public String maisDeUmaDisciplinaProfessor() {
		return languageManager.getText("maisDeUmaDisciplinaProfessor");
	}

	@Override
	public String margem() {
		return languageManager.getText("margem");
	}

	@Override
	public String margemPercente() {
		return languageManager.getText("margemPercente");
	}

	@Override
	public String matrizCurricular() {
		return languageManager.getText("matrizCurricular");
	}

	@Override
	public String maxAlunosPratico() {
		return languageManager.getText("maxAlunosPratico");
	}

	@Override
	public String maxAlunosTeorico() {
		return languageManager.getText("maxAlunosTeorico");
	}

	@Override
	public String maxDisciplinasProfessor() {
		return languageManager.getText("maxDisciplinasProfessor");
	}

	@Override
	public String mensagemErro() {
		return languageManager.getText("mensagemErro");
	}

	@Override
	public String minPercentualDoutor() {
		return languageManager.getText("minPercentualDoutor");
	}

	@Override
	public String minPercentualMestre() {
		return languageManager.getText("minPercentualMestre");
	}

	@Override
	public String minTempoIntegralParcial() {
		return languageManager.getText("minTempoIntegralParcial");
	}

	@Override
	public String minTempoIntegral() {
		return languageManager.getText("minTempoIntegral");
	}

	@Override
	public String municipio() {
		return languageManager.getText("municipio");
	}

	@Override
	public String nao() {
		return languageManager.getText("nao");
	}

	@Override
	public String nivelDificuldade() {
		return languageManager.getText("nivelDificuldade");
	}

	@Override
	public String nome() {
		return languageManager.getText("nome");
	}

	@Override
	public String notaDesempenho() {
		return languageManager.getText("notaDesempenho");
	}

	@Override
	public String numero() {
		return languageManager.getText("numero");
	}

	@Override
	public String ofertasEDemandas() {
		return languageManager.getText("ofertasEDemandas");
	}

	@Override
	public String otimizado() {
		return languageManager.getText("otimizado");
	}

	@Override
	public String periodo() {
		return languageManager.getText("periodo");
	}

	@Override
	public String pratico() {
		return languageManager.getText("pratico");
	}

	@Override
	public String preenchaA() {
		return languageManager.getText("preenchaA");
	}

	@Override
	public String preenchaO() {
		return languageManager.getText("preenchaO");
	}

	@Override
	public String preferencia() {
		return languageManager.getText("preferencia");
	}

	@Override
	public String professor() {
		return languageManager.getText("professor");
	}

	@Override
	public String professores() {
		return languageManager.getText("professores");
	}

	@Override
	public String publicado() {
		return languageManager.getText("publicado");
	}

	@Override
	public String quantidadeAlunos() {
		return languageManager.getText("quantidadeAlunos");
	}

	@Override
	public String rateio() {
		return languageManager.getText("rateio");
	}

	@Override
	public String receita() {
		return languageManager.getText("receita");
	}

	@Override
	public String relatorioVisaoSala() {
		return languageManager.getText("relatorioVisaoSala");
	}

	@Override
	public String remover() {
		return languageManager.getText("remover");
	}

	@Override
	public String resumoDisciplina() {
		return languageManager.getText("resumoDisciplina");
	}

	@Override
	public String resumoCurso() {
		return languageManager.getText("resumoCurso");
	}

	@Override
	public String sala() {
		return languageManager.getText("sala");
	}

	@Override
	public String salas() {
		return languageManager.getText("salas");
	}

	@Override
	public String salasHeadingPanel() {
		return languageManager.getText("salasHeadingPanel");
	}

	@Override
	public String selecioneA() {
		return languageManager.getText("selecioneA");
	}

	@Override
	public String selecioneO() {
		return languageManager.getText("selecioneO");
	}

	@Override
	public String sim() {
		return languageManager.getText("sim");
	}

	@Override
	public String teorico() {
		return languageManager.getText("teorico");
	}

	@Override
	public String tipo() {
		return languageManager.getText("tipo");
	}

	@Override
	public String tipoContrato() {
		return languageManager.getText("tipoContrato");
	}

	@Override
	public String TipoCredito() {
		return languageManager.getText("TipoCredito");
	}

	@Override
	public String titulacao() {
		return languageManager.getText("titulacao");
	}

	@Override
	public String totalDecreditos() {
		return languageManager.getText("totalDecreditos");
	}

	@Override
	public String trieda() {
		return languageManager.getText("trieda");

	}

	@Override
	public String triedaDetailMessageHeadingText() {
		return languageManager.getText("triedaDetailMessageHeadingText");
	}

	@Override
	public String turma() {
		return languageManager.getText("turma");
	}

	@Override
	public String turno() {
		return languageManager.getText("turno");
	}

	@Override
	public String unidade() {
		return languageManager.getText("unidade");
	}

	@Override
	public String unidades() {
		return languageManager.getText("unidades");
	}

	@Override
	public String unidadesHeadingPanel() {
		return languageManager.getText("unidadesHeadingPanel");
	}

	@Override
	public String usaLaboratorio() {
		return languageManager.getText("usaLaboratorio");
	}

	@Override
	public String valorCredito() {
		return languageManager.getText("valorCredito");
	}

	@Override
	public String otimizadoTatico()
	{
		return languageManager.getText( "otimizadoTatico" );
	}

	@Override
	public String otimizadoOperacional()
	{
		return languageManager.getText( "otimizadoOperacional" );
	}

	@Override
	public String relatorioVisaoCurso()
	{
		return languageManager.getText( "relatorioVisaoCurso" );
	}

	@Override
	public String capacidadeAlunos()
	{
		return languageManager.getText( "capacidadeAlunos" );
	}

	@Override
	public String codigoAreaTitulacao()
	{
		return languageManager.getText( "codigoAreaTitulacao" );
	}

	@Override
	public String codigoCampus()
	{
		return languageManager.getText( "codigoCampus" );
	}

	@Override
	public String codigoCurriculo()
	{
		return languageManager.getText( "codigoCurriculo" );
	}

	@Override
	public String codigoCurso()
	{
		return languageManager.getText( "codigoCurso" );
	}

	@Override
	public String codigoDisciplina()
	{
		return languageManager.getText( "codigoDisciplina" );
	}

	@Override
	public String codigoMatrizCurricular()
	{
		return languageManager.getText( "codigoMatrizCurricular" );
	}

	@Override
	public String codigoSala()
	{
		return languageManager.getText( "codigoSala" );
	}

	@Override
	public String codigoUnidade()
	{
		return languageManager.getText( "codigoUnidade" );
	}

	@Override
	public String descricaoCurriculo()
	{
		return languageManager.getText( "descricaoCurriculo" );
	}

	@Override
	public String nomeCurso()
	{
		return languageManager.getText( "nomeCurso" );
	}

	@Override
	public String nomeDisciplina()
	{
		return languageManager.getText( "nomeDisciplina" );
	}

	@Override
	public String nomeProfessor()
	{
		return languageManager.getText( "nomeProfessor" );
	}

	@Override
	public String tipoCurso()
	{
		return languageManager.getText( "tipoCurso" );
	}

	@Override
	public String tipoDisciplina()
	{
		return languageManager.getText( "tipoDisciplina" );
	}

	@Override
	public String relatorioVisaoProfessor()
	{
		return languageManager.getText( "relatorioVisaoProfessor" );
	}

	@Override
	public String mensagemAlerta()
	{
		return languageManager.getText( "mensagemAlerta" );
	}

	@Override
	public String periodos()
	{
		return languageManager.getText( "periodos" );
	}

	@Override
	public String semanaLetiva()
	{
		return languageManager.getText( "semanaLetiva" );
	}

	@Override
	public String matriculaAluno()
	{
		return languageManager.getText( "matriculaAluno" );
	}

	@Override
	public String nomeAluno()
	{
		return languageManager.getText( "nomeAluno" );
	}

	@Override
	public String prioridadeAlunoDemanda()
	{
		return languageManager.getText( "prioridadeAlunoDemanda" );
	}

	@Override
	public String alunosDemanda()
	{
		return languageManager.getText( "alunosDemanda" );
	}
}
