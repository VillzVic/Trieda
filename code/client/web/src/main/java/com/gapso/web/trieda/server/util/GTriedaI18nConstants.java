package com.gapso.web.trieda.server.util;

import java.util.Locale;

import com.gapso.gipe.common.GLanguageManager;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;

public class GTriedaI18nConstants implements TriedaI18nConstants
{
	private GLanguageManager languageManager = null;

	public GTriedaI18nConstants()
	{
		this.languageManager = new GLanguageManager(
			"TriedaI18nConstants", new Locale( "pt", "BR" ) );
	}

	@Override
	public String adicionar() {
		return this.languageManager.getText("adicionar");
	}

	@Override
	public String andar() {
		return this.languageManager.getText("andar");
	}

	@Override
	public String areaTitulacao() {
		return this.languageManager.getText("areaTitulacao");
	}

	@Override
	public String areasTitulacao() {
		return this.languageManager.getText("areasTitulacao");
	}
	
	@Override
	public String associacaoAlunoDemanda() {
		return this.languageManager.getText("associacaoAlunoDemanda");
	}

	@Override
	public String bairro() {
		return this.languageManager.getText("bairro");
	}

	@Override
	public String campi() {
		return this.languageManager.getText("campi");
	}

	@Override
	public String campiHeadingPanel() {
		return this.languageManager.getText("campiHeadingPanel");
	}

	@Override
	public String campiTrabalho() {
		return this.languageManager.getText("campiTrabalho");
	}

	@Override
	public String campus() {
		return this.languageManager.getText("campus");
	}

	@Override
	public String capacidadeInstalada() {
		return this.languageManager.getText("capacidadeInstalada");
	}
	
	@Override
	public String capacidadeMax() {
		return this.languageManager.getText("capacidadeMax");
	}

	@Override
	public String capacidadeMediaSalas() {
		return this.languageManager.getText("capacidadeMediaSalas");
	}

	@Override
	public String cargaHorariaAnterior() {
		return this.languageManager.getText("cargaHorariaAnterior");
	}

	@Override
	public String cargaHorariaMax() {
		return this.languageManager.getText("cargaHorariaMax");
	}

	@Override
	public String cargaHorariaMin() {
		return this.languageManager.getText("cargaHorariaMin");
	}
	
	@Override
	public String cargaHorariaMinutos() {
		return this.languageManager.getText("cargaHorariaMinutos");
	}
	
	@Override
	public String cargaHorariaSemanal() {
		return this.languageManager.getText("cargaHorariaSemanal");
	}

	@Override
	public String codigo() {
		return this.languageManager.getText("codigo");
	}
	
	@Override
	public String compatibilidadeEntreDisciplinas() {
		return this.languageManager.getText("compatibilidadeEntreDisciplinas");
	}

	@Override
	public String confirmacao() {
		return this.languageManager.getText("confirmacao");
	}

	@Override
	public String cpf() {
		return this.languageManager.getText("cpf");
	}

	@Override
	public String cpfProfessor() {
		return this.languageManager.getText("cpfProfessor");
	}

	@Override
	public String creditos() {
		return this.languageManager.getText("creditos");
	}
	
	@Override
	public String credito_s() {
		return this.languageManager.getText("credito_s");
	}

	@Override
	public String creditosPraticos() {
		return this.languageManager.getText("creditosPraticos");
	}

	@Override
	public String creditosTeoricos() {
		return this.languageManager.getText("creditosTeoricos");
	}

	@Override
	public String curriculos() {
		return this.languageManager.getText("curriculos");
	}

	@Override
	public String curso() {
		return this.languageManager.getText("curso");
	}

	@Override
	public String cursos() {
		return this.languageManager.getText("cursos");
	}

	@Override
	public String cursosAreasTitulacao() {
		return this.languageManager.getText("cursosAreasTitulacao");
	}

	@Override
	public String cursou() {
		return this.languageManager.getText("cursou");
	}

	@Override
	public String custoDocente() {
		return this.languageManager.getText("custoDocente");
	}

	@Override
	public String custoMedioCredito() {
		return this.languageManager.getText("custoMedioCredito");
	}

	@Override
	public String custoMedioCreditoExcel() {
		return this.languageManager.getText("custoMedioCreditoExcel");

	}
	
	@Override
	public String dbVersion() {
		return this.languageManager.getText("dbVersion");
	}

	
	@Override
	public String demanda() {
		return this.languageManager.getText("demanda");
	}

	@Override
	public String demandaDeAlunos() {
		return this.languageManager.getText("demandaDeAlunos");
	}

	@Override
	public String descricao() {
		return this.languageManager.getText("descricao");
	}

	@Override
	public String deslocamentoUnidades() {
		return this.languageManager.getText("deslocamentoUnidades");
	}

	@Override
	public String deslocamentoUnidadesCampus() {
		return this.languageManager.getText("deslocamentoUnidadesCampus");
	}
	
	@Override
	public String dia() {
		return this.languageManager.getText("dia");
	}
	
	@Override
	public String diaDaSemana() {
		return this.languageManager.getText("diaDaSemana");
	}
	
	@Override
	public String diasHorariosAula() {
		return this.languageManager.getText("diasHorariosAula");
	}

	@Override
	public String disciplina() {
		return this.languageManager.getText("disciplina");
	}

	@Override
	public String disciplinas() {
		return this.languageManager.getText("disciplinas");
	}

	@Override
	public String disciplinasAssociadas() {
		return this.languageManager.getText("disciplinasAssociadas");
	}
	
	@Override
	public String disciplinaAula() {
		return this.languageManager.getText("disciplinaAula");
	}
	
	@Override
	public String disciplinasSalas() {
		return this.languageManager.getText("disciplinasSalas");
	}
	
	@Override
	public String disponibilidadesDisciplinas() {
		return this.languageManager.getText("disponibilidadesDisciplinas");
	}
	
	@Override
	public String disponibilidadesProfessores() {
		return this.languageManager.getText("disponibilidadesProfessores");
	}

	@Override
	public String disponibilidadesSemanaLetiva() {
		return this.languageManager.getText("disponibilidadesSemanaLetiva");
	}

	@Override
	public String edicaoDe() {
		return this.languageManager.getText("edicaoDe");
	}

	@Override
	public String editar() {
		return this.languageManager.getText("editar");
	}

	@Override
	public String elimina() {
		return this.languageManager.getText("elimina");
	}

	@Override
	public String equivalencias() {
		return this.languageManager.getText("equivalencias");
	}

	@Override
	public String estado() {
		return this.languageManager.getText("estado");
	}

	@Override
	public String exportarExcel() {
		return this.languageManager.getText("exportarExcel");
	}
	
	@Override
	public String fim() {
		return this.languageManager.getText("fim");
	}

	@Override
	public String habilitacaoProfessores() {
		return this.languageManager.getText("habilitacaoProfessores");
	}

	@Override
	public String gruposSalas() {
		return this.languageManager.getText("gruposSalas");
	}
	
	@Override
	public String horarios() {
		return this.languageManager.getText("horarios");
	}
	
	@Override
	public String horarioInicial() {
		return this.languageManager.getText("horarioInicial");
	}

	@Override
	public String horarioFinal() {
		return this.languageManager.getText("horarioFinal");
	}

	@Override
	public String importarExcel() {
		return this.languageManager.getText("importarExcel");
	}

	@Override
	public String informacao() {
		return this.languageManager.getText("informacao");
	}
	
	@Override
	public String inicio() {
		return this.languageManager.getText("inicio");
	}

	@Override
	public String insercaoDe() {
		return this.languageManager.getText("insercaoDe");
	}

	@Override
	public String maisDeUmaDisciplinaProfessor() {
		return this.languageManager.getText("maisDeUmaDisciplinaProfessor");
	}

	@Override
	public String margem() {
		return this.languageManager.getText("margem");
	}

	@Override
	public String margemPercente() {
		return this.languageManager.getText("margemPercente");
	}

	@Override
	public String matrizCurricular() {
		return this.languageManager.getText("matrizCurricular");
	}

	@Override
	public String maxAlunosPratico() {
		return this.languageManager.getText("maxAlunosPratico");
	}

	@Override
	public String maxAlunosTeorico() {
		return this.languageManager.getText("maxAlunosTeorico");
	}

	@Override
	public String maxDisciplinasProfessor() {
		return this.languageManager.getText("maxDisciplinasProfessor");
	}

	@Override
	public String mensagemErro() {
		return this.languageManager.getText("mensagemErro");
	}

	@Override
	public String minPercentualDoutor() {
		return this.languageManager.getText("minPercentualDoutor");
	}

	@Override
	public String minPercentualMestre() {
		return this.languageManager.getText("minPercentualMestre");
	}

	@Override
	public String minTempoIntegralParcial() {
		return this.languageManager.getText("minTempoIntegralParcial");
	}

	@Override
	public String minTempoIntegral() {
		return this.languageManager.getText("minTempoIntegral");
	}

	@Override
	public String municipio() {
		return this.languageManager.getText("municipio");
	}

	@Override
	public String nao() {
		return this.languageManager.getText("nao");
	}

	@Override
	public String nivelDificuldade() {
		return this.languageManager.getText("nivelDificuldade");
	}

	@Override
	public String nome() {
		return this.languageManager.getText("nome");
	}

	@Override
	public String notaDesempenho() {
		return this.languageManager.getText("notaDesempenho");
	}

	@Override
	public String numero() {
		return this.languageManager.getText("numero");
	}
	
	@Override
	public String ofertasCursosCampi() {
		return this.languageManager.getText("ofertasCursosCampi");
	}

	@Override
	public String ofertasEDemandas() {
		return this.languageManager.getText("ofertasEDemandas");
	}

	@Override
	public String otimizado() {
		return this.languageManager.getText("otimizado");
	}

	@Override
	public String periodo() {
		return this.languageManager.getText("periodo");
	}

	@Override
	public String prat() {
		return this.languageManager.getText("prat");
	}
	
	@Override
	public String pratico() {
		return this.languageManager.getText("pratico");
	}
	
	@Override
	public String pratico_s() {
		return this.languageManager.getText("pratico_s");
	}

	@Override
	public String preenchaA() {
		return this.languageManager.getText("preenchaA");
	}

	@Override
	public String preenchaO() {
		return this.languageManager.getText("preenchaO");
	}

	@Override
	public String preferencia() {
		return this.languageManager.getText("preferencia");
	}

	@Override
	public String professor() {
		return this.languageManager.getText("professor");
	}

	@Override
	public String professores() {
		return this.languageManager.getText("professores");
	}

	@Override
	public String publicado() {
		return this.languageManager.getText("publicado");
	}

	@Override
	public String quantidadeAlunos() {
		return this.languageManager.getText("quantidadeAlunos");
	}

	@Override
	public String rateio() {
		return this.languageManager.getText("rateio");
	}

	@Override
	public String receita() {
		return this.languageManager.getText("receita");
	}

	@Override
	public String relatorioVisaoSala() {
		return this.languageManager.getText("relatorioVisaoSala");
	}

	@Override
	public String remover() {
		return this.languageManager.getText("remover");
	}
	
	@Override
	public String removerTodos() {
		return this.languageManager.getText("removerTodos");
	}

	@Override
	public String resumoDisciplina() {
		return this.languageManager.getText("resumoDisciplina");
	}

	@Override
	public String resumoCurso() {
		return this.languageManager.getText("resumoCurso");
	}

	@Override
	public String sala() {
		return this.languageManager.getText("sala");
	}

	@Override
	public String salas() {
		return this.languageManager.getText("salas");
	}

	@Override
	public String salasHeadingPanel() {
		return this.languageManager.getText("salasHeadingPanel");
	}

	@Override
	public String selecioneA() {
		return this.languageManager.getText("selecioneA");
	}

	@Override
	public String selecioneO() {
		return this.languageManager.getText("selecioneO");
	}

	@Override
	public String sim() {
		return this.languageManager.getText("sim");
	}

	@Override
	public String teo() {
		return this.languageManager.getText("teo");
	}
	
	@Override
	public String teorico() {
		return this.languageManager.getText("teorico");
	}
	
	@Override
	public String teorico_s() {
		return this.languageManager.getText("teorico_s");
	}

	@Override
	public String tipo() {
		return this.languageManager.getText("tipo");
	}

	@Override
	public String tipoContrato() {
		return this.languageManager.getText("tipoContrato");
	}

	@Override
	public String TipoCredito() {
		return this.languageManager.getText("TipoCredito");
	}

	@Override
	public String titulacao() {
		return this.languageManager.getText("titulacao");
	}

	@Override
	public String totalDecreditos() {
		return this.languageManager.getText("totalDecreditos");
	}

	@Override
	public String trieda() {
		return this.languageManager.getText("trieda");

	}
	
	@Override
	public String triedaVersion() {
		return this.languageManager.getText("triedaVersion");

	}

	@Override
	public String triedaDetailMessageHeadingText()
	{
		return this.languageManager.getText("triedaDetailMessageHeadingText");
	}

	@Override
	public String turma()
	{
		return this.languageManager.getText("turma");
	}

	@Override
	public String turno()
	{
		return this.languageManager.getText("turno");
	}

	@Override
	public String unidade()
	{
		return this.languageManager.getText("unidade");
	}

	@Override
	public String unidades()
	{
		return this.languageManager.getText("unidades");
	}

	@Override
	public String unidadesHeadingPanel()
	{
		return this.languageManager.getText("unidadesHeadingPanel");
	}

	@Override
	public String exigeLaboratorio()
	{
		return this.languageManager.getText("exigeLaboratorio");
	}
	
	@Override
	public String usaSabado()
	{
		return this.languageManager.getText("usaSabado");
	}
	
	@Override
	public String usaDomingo()
	{
		return this.languageManager.getText("usaDomingo");
	}

	@Override
	public String valorCredito()
	{
		return this.languageManager.getText("valorCredito");
	}

	@Override
	public String otimizadoTatico()
	{
		return this.languageManager.getText( "otimizadoTatico" );
	}

	@Override
	public String otimizadoOperacional()
	{
		return this.languageManager.getText( "otimizadoOperacional" );
	}

	@Override
	public String relatorioVisaoCurso()
	{
		return this.languageManager.getText( "relatorioVisaoCurso" );
	}

	@Override
	public String capacidadeInstaladaAlunos()
	{
		return this.languageManager.getText( "capacidadeInstaladaAlunos" );
	}
	
	@Override
	public String capacidadeMaxAlunos()
	{
		return this.languageManager.getText( "capacidadeMaxAlunos" );
	}
	
	@Override
	public String custoOperacaoCred()
	{
		return this.languageManager.getText( "custoOperacaoCred" );
	}

	@Override
	public String codigoAreaTitulacao()
	{
		return this.languageManager.getText( "codigoAreaTitulacao" );
	}

	@Override
	public String codigoCampus()
	{
		return this.languageManager.getText( "codigoCampus" );
	}

	@Override
	public String codigoCurriculo()
	{
		return this.languageManager.getText( "codigoCurriculo" );
	}

	@Override
	public String codigoCurso()
	{
		return this.languageManager.getText( "codigoCurso" );
	}

	@Override
	public String codigoDisciplina()
	{
		return this.languageManager.getText( "codigoDisciplina" );
	}

	@Override
	public String codigoMatrizCurricular()
	{
		return this.languageManager.getText( "codigoMatrizCurricular" );
	}

	@Override
	public String codigoSala()
	{
		return this.languageManager.getText( "codigoSala" );
	}

	@Override
	public String codigoUnidade()
	{
		return this.languageManager.getText( "codigoUnidade" );
	}

	@Override
	public String descricaoCurriculo()
	{
		return this.languageManager.getText( "descricaoCurriculo" );
	}

	@Override
	public String nomeCurso()
	{
		return this.languageManager.getText( "nomeCurso" );
	}

	@Override
	public String nomeDisciplina()
	{
		return this.languageManager.getText( "nomeDisciplina" );
	}

	@Override
	public String nomeProfessor()
	{
		return this.languageManager.getText( "nomeProfessor" );
	}

	@Override
	public String tipoCurso()
	{
		return this.languageManager.getText( "tipoCurso" );
	}

	@Override
	public String tipoDisciplina()
	{
		return this.languageManager.getText( "tipoDisciplina" );
	}

	@Override
	public String relatorioVisaoProfessor()
	{
		return this.languageManager.getText( "relatorioVisaoProfessor" );
	}

	@Override
	public String mensagemAlerta()
	{
		return this.languageManager.getText( "mensagemAlerta" );
	}

	@Override
	public String periodos()
	{
		return this.languageManager.getText( "periodos" );
	}

	@Override
	public String semanaLetiva()
	{
		return this.languageManager.getText( "semanaLetiva" );
	}

	@Override
	public String matriculaAluno()
	{
		return this.languageManager.getText( "matriculaAluno" );
	}

	@Override
	public String nomeAluno()
	{
		return this.languageManager.getText( "nomeAluno" );
	}
	
	@Override
	public String prioridade()
	{
		return this.languageManager.getText( "prioridade" );
	}

	@Override
	public String prioridadeAlunoDemanda()
	{
		return this.languageManager.getText( "prioridadeAlunoDemanda" );
	}

	@Override
	public String alunosDemanda()
	{
		return this.languageManager.getText( "alunosDemanda" );
	}

	@Override
	public String professorVirtual()
	{
		return this.languageManager.getText( "professorVirtual" );
	}
	
	@Override
	public String relatorioVisaoAluno(){
		return this.languageManager.getText("relatorioVisaoAluno");
	}

	@Override
	public String atendimentosDisciplina() {
		return this.languageManager.getText("atendimentosDisciplina");
	}

	@Override
	public String atendimentosMatricula() {
		return this.languageManager.getText("atendimentosMatricula");
	}

	@Override
	public String atendimentosFaixaDemanda() {
		return this.languageManager.getText("atendimentosFaixaDemanda");
	}
	
	public String percentMestresDoutores() {
		return this.languageManager.getText("percentMestresDoutores");
	}
	
	@Override
	public String permiteIntervaloAula(){
		return this.languageManager.getText("permiteIntervaloAula");
	}

	@Override
	public String atendimentosCargaHoraria() {
		return this.languageManager.getText("atendimentosCargaHoraria");
	}
	
	public String aulasContinuas() {
		return this.languageManager.getText("aulasContinuas");
	}

	@Override
	public String professorUnico() {
		return this.languageManager.getText("professorUnico");
	}

	@Override
	public String maxDiasSemana() {
		return this.languageManager.getText("maxDiasSemana");
	}

	@Override
	public String minCreditosDia() {
		return this.languageManager.getText("minCreditosDia");
	}

	@Override
	public String disponibilidadesSalas() {
		return this.languageManager.getText("disponibilidadesSalas");
	}

	@Override
	public String cargaHoraria() {
		return this.languageManager.getText("cargaHoraria");
	}

	@Override
	public String disciplinasPreRequisitos() {
		return this.languageManager.getText("disciplinasPreRequisitos");
	}

	@Override
	public String codigoDisciplinaPreRequisito() {
		return this.languageManager.getText("codigoDisciplinaPreRequisito");
	}

	@Override
	public String disciplinasCoRequisitos() {
		return this.languageManager.getText("disciplinasCoRequisitos");
	}

	@Override
	public String codigoDisciplinaCoRequisito() {
		return this.languageManager.getText("codigoDisciplinaCoRequisito");
	}

	@Override
	public String alunosDisciplinasCursadas() {
		return this.languageManager.getText("alunosDisciplinasCursadas");
	}

	@Override
	public String atendimentosFaixaCredito() {
		return this.languageManager.getText("atendimentosFaixaCredito");
	}

	@Override
	public String atendimentosFaixaDisciplina() {
		return this.languageManager.getText("atendimentosFaixaDisciplina");
	}

	@Override
	public String professoresQuantidadeJanelas() {
		return this.languageManager.getText("professoresQuantidadeJanelas");
	}

	@Override
	public String professoresDisciplinasHabilitadas() {
		return this.languageManager.getText("professoresDisciplinasHabilitadas");
	}

	@Override
	public String professoresDisciplinasLecionadas() {
		return this.languageManager.getText("professoresDisciplinasLecionadas");
	}

	@Override
	public String professoresTitulacoes() {
		return this.languageManager.getText("professoresTitulacoes");
	}

	@Override
	public String professoresAreasConhecimento() {
		return this.languageManager.getText("professoresAreasConhecimento");
	}

	@Override
	public String ambientesFaixaOcupacao() {
		return this.languageManager.getText("ambientesFaixaOcupacao");
	}

	@Override
	public String ambientesFaixaUsoCapacidade() {
		return this.languageManager.getText("ambientesFaixaUsoCapacidade");
	}
	
	@Override
	public String ambientesFaixaOcupacaoSemana() {
		return this.languageManager.getText("ambientesFaixaOcupacaoSemana");
	}

	@Override
	public String ambientesFaixaUtilizacaoSemana() {
		return this.languageManager.getText("ambientesFaixaUtilizacaoSemana");
	}

	@Override
	public String divisoesCredito() {
		return this.languageManager.getText("divisoesCredito");
	}

	@Override
	public String tiposCurso() {
		return this.languageManager.getText("tipoCurso");
	}

	@Override
	public String turnos() {
		return this.languageManager.getText("turnos");
	}

	@Override
	public String deslocamentoCampi() {
		return this.languageManager.getText("deslocamentoCampi");
	}

	@Override
	public String resumoCenario() {
		return this.languageManager.getText("resumoCenario");
	}

	@Override
	public String resumoCampi() {
		return this.languageManager.getText("resumoCampi");
	}
	
	@Override
	public String compararCenarios() {
		return this.languageManager.getText("compararCenarios");
	}

	@Override
	public String totalCreditosSemanais() {
		return this.languageManager.getText("totalCreditosSemanais");
	}

	@Override
	public String demandaDeAlunosReal() {
		return this.languageManager.getText("demandaDeAlunosReal");
	}

	@Override
	public String demandaDeAlunosVirtual() {
		return this.languageManager.getText("demandaDeAlunosVirtual");
	}

	@Override
	public String idAmbiente() {
		return this.languageManager.getText("idAmbiente");
	}

	@Override
	public String nomeAmbiente() {
		return this.languageManager.getText("nomeAmbiente");
	}

	@Override
	public String formando() {
		return this.languageManager.getText("formando");
	}

	@Override
	public String criadoPeloTrieda() {
		return this.languageManager.getText("criadoPeloTrieda");
	}

	@Override
	public String virtual() {
		return this.languageManager.getText("virtual");
	}

	@Override
	public String divisoesCreditoDisciplina() {
		return this.languageManager.getText("divisoesCreditoDisciplina");
	}

	@Override
	public String associacaoDisciplinas() {
		return this.languageManager.getText("associacaoDisciplinas");
	}

	@Override
	public String exigeEquivalenciaForcada() {
		return this.languageManager.getText("exigeEquivalenciaForcada");
	}

	@Override
	public String prioridadeAlunos() {
		return this.languageManager.getText("prioridadeAlunos");
	}

	@Override
	public String fixacoes() {
		return this.languageManager.getText("fixacoes");
	}
}
