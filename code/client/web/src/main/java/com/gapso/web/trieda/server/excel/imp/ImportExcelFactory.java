package com.gapso.web.trieda.server.excel.imp;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ImportExcelFactory
{
	static public IImportExcel createImporter(String infoToBeImported, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		IImportExcel importer = null;
		ExcelInformationType informationToBeImported = ExcelInformationType.valueOf(infoToBeImported);

		switch (informationToBeImported)
		{
		case TUDO:
			importer = new TRIEDAImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case CAMPI:
			importer = new CampiImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case CURRICULOS:
			importer = new CurriculosImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case CURSOS:
			importer = new CursosImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case AREAS_TITULACAO:
			importer = new AreasTitulacaoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DISCIPLINAS_SALAS:
			importer = new DisciplinasSalasImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DISCIPLINAS_PRE_REQUISITOS:
			importer = new DisciplinasPreRequisitosImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DISCIPLINAS_CO_REQUISITOS:
			importer = new DisciplinasCoRequisitosImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case ALUNOS_DISCIPLINAS_CURSADAS:
			importer = new AlunosDisciplinasCursadasImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		// case PROFESSORES: importer = new ProfessoresImportExcel( cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
		case PROFESSORES:
			importer = new MultiImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[] { ProfessoresImportExcel.class, DisponibilidadesProfessoresImportExcel.class });
			break;
		// case DISCIPLINAS: importer = new DisciplinasImportExcel( cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
		case DISCIPLINAS:
			importer = new MultiImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[] { DisciplinasImportExcel.class, DisponibilidadesDisciplinasImportExcel.class });
			break;
		case EQUIVALENCIAS:
			importer = new EquivalenciasImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case CURSO_AREAS_TITULACAO:
			importer = new CursoAreasTitulacaoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case CAMPI_TRABALHO:
			importer = new CampiTrabalhoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case UNIDADES:
			importer = new UnidadesImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case SALAS:
			importer = new MultiImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[] { SalasImportExcel.class, DisponibilidadesSalasImportExcel.class });
			break;
		case OFERTAS_CURSOS_CAMPI:
			importer = new OfertasCursosCampiImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DEMANDAS:
			importer = new DemandasImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case ALUNOS:
			importer = new AlunosImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DEMANDAS_POR_ALUNO:
			importer = new AlunosDemandaImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case HABILITACAO_PROFESSORES:
			importer = new HabilitacoesProfessoresImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case CAMPI_DESLOCAMENTO:
			importer = new CampiDeslocamentoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case UNIDADES_DESLOCAMENTO:
			importer = new UnidadesDeslocamentoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case TIPOS_CURSO:
			importer = new TiposCursoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DIVISOES_CREDITO:
			importer = new DivisoesCreditoImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case DIVISOES_CREDITO_DISCIPLINA:
			importer = new DivisoesCreditoDisciplinaImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case TURNOS:
			importer = new TurnosImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
			break;
		case SEMANA_LETIVA:
			importer = new MultiImportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[] { SemanaLetivaImportExcel.class, SemanaLetivaHorariosImportExcel.class });
			break;
		}

		return importer;
	}
}
