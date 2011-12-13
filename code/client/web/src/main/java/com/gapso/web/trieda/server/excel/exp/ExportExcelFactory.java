package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter( String infoToBeExported, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino )
	{
		boolean visaoProfessor = ( cenario.getCriadoPor() == null ? true :
			cenario.getCriadoPor().getProfessor() != null && cenario.getCriadoPor().getProfessor().getId() > 0 );

		IExportExcel exporter = null;

		ExcelInformationType informationToBeExported
			= ExcelInformationType.valueOf( infoToBeExported );

		switch ( informationToBeExported )
		{
			case AREAS_TITULACAO: exporter = new AreasTitulacaoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case CAMPI: exporter = new CampiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case CAMPI_TRABALHO: exporter = new CampiTrabalhoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case CURRICULOS: exporter = new CurriculosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case CURSOS: exporter = new CursosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case CURSO_AREAS_TITULACAO: exporter = new CursoAreasTitulacaoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case DEMANDAS: exporter = new DemandasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case DISCIPLINAS: exporter = new DisciplinasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case DISCIPLINAS_SALAS: exporter = new DisciplinasSalasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case EQUIVALENCIAS: exporter = new EquivalenciasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino); break;
			case HABILITACAO_PROFESSORES: exporter = new HabilitacoesProfessoresExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case PROFESSORES: exporter = new ProfessoresExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case RELATORIO_VISAO_CURSO: exporter = new RelatorioVisaoCursoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino ); break;
			case RELATORIO_VISAO_SALA: exporter = new RelatorioVisaoSalaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino ); break;
			case RELATORIO_VISAO_PROFESSOR: exporter = new RelatorioVisaoProfessorExportExcel(cenario, i18nConstants, i18nMessages, filter, visaoProfessor, instituicaoEnsino ); break;
			case RESUMO_DISCIPLINA: exporter = new ResumoDisciplinaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino ); break;
			case RESUMO_CURSO: exporter = new ResumoCursoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino ); break;
			case SALAS: exporter = new SalasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case TUDO: exporter = new TRIEDAExportExcel(cenario, i18nConstants, i18nMessages, visaoProfessor, instituicaoEnsino ); break;
			case UNIDADES: exporter = new UnidadesExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
		}

		return exporter;
	}
}
