package com.gapso.web.trieda.server.excel.exp;

import java.util.Map;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class ExportExcelFactory {
	
	static public IExportExcel createExporter( String infoToBeExported, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino, String fileExtension,
		Map< String, Boolean > planilhasExportExcel, String nomeArquivo)
	{
		boolean visaoProfessor = ( cenario.getCriadoPor() == null ? true :
			cenario.getCriadoPor().getProfessor() != null && cenario.getCriadoPor().getProfessor().getId() > 0 );

		IExportExcel exporter = null;

		ExcelInformationType informationToBeExported
			= ExcelInformationType.valueOf( infoToBeExported );

		switch ( informationToBeExported )
		{
			case ALUNOS: exporter = new AlunosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case ALUNOS_DISCIPLINAS_CURSADAS: exporter = new AlunosDisciplinasCursadasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case AMBIENTES_FAIXA_UTILIZACAO: exporter = new AmbientesFaixaUtilizacaoHorariosExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case AMBIENTES_FAIXA_OCUPACAO: exporter = new AmbientesFaixaOcupacaoCapacidadeExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break; 
			case AMBIENTES_FAIXA_OCUPACAO_SEMANA: exporter = new AmbientesFaixaOcupacaoDiaSemanaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break; 
			case AMBIENTES_FAIXA_UTILIZACAO_SEMANA: exporter = new AmbientesFaixaUtilizacaoHorariosDiaSemanaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break; 
			case AREAS_TITULACAO: exporter = new AreasTitulacaoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case ATENDIMENTOS_MATRICULA: exporter = new AtendimentosMatriculaExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension); break;
			case ATENDIMENTOS_DISCIPLINA: exporter = new AtendimentosDisciplinaExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension); break;
			case ATENDIMENTOS_FAIXA_CREDITO: exporter = new AtendimentosFaixaCreditoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case ATENDIMENTOS_FAIXA_DISCIPLINA: exporter = new AtendimentosFaixaDisciplinaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case ATENDIMENTOS_FAIXA_DEMANDA: exporter = new AtendimentosFaixaDemandaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case CAMPI: exporter = new CampiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case CAMPI_TRABALHO: exporter = new CampiTrabalhoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case CURRICULOS: exporter = new CurriculosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case CURSOS: exporter = new CursosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case CURSO_AREAS_TITULACAO: exporter = new CursoAreasTitulacaoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case OFERTAS_CURSOS_CAMPI: exporter = new OfertasCursosCampiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DEMANDAS: exporter = new DemandasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DEMANDAS_POR_ALUNO: exporter = new AlunosDemandaExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case CAMPI_DESLOCAMENTO: exporter = new CampiDeslocamentoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case UNIDADES_DESLOCAMENTO: exporter = new UnidadesDeslocamentosExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case DISCIPLINAS: exporter = new MultiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[]{DisciplinasExportExcel.class,DisponibilidadesDisciplinasExportExcel.class}, fileExtension, ExcelInformationType.DISCIPLINAS.getSheetName() ); break;
			//case DISCIPLINAS: exporter = new DisciplinasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DISCIPLINAS_SALAS: exporter = new DisciplinasSalasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DISCIPLINAS_PRE_REQUISITOS: exporter = new DisciplinasPreRequisitosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DISCIPLINAS_CO_REQUISITOS: exporter = new DisciplinasCoRequisitosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DIVISOES_CREDITO: exporter = new DivisoesCreditoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case DIVISOES_CREDITO_DISCIPLINA: exporter = new DivisoesCreditoDisciplinaExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case EQUIVALENCIAS: exporter = new EquivalenciasExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case FIXACOES: exporter = new FixacoesExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension); break;
			case HABILITACAO_PROFESSORES: exporter = new HabilitacoesProfessoresExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case PERCENT_MESTRES_DOUTORES: exporter = new PercentMestresDoutoresExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			//case PROFESSORES: exporter = new ProfessoresExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino ); break;
			case PROFESSORES: exporter = new MultiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[]{ProfessoresExportExcel.class,DisponibilidadesProfessoresExportExcel.class}, fileExtension, ExcelInformationType.PROFESSORES.getSheetName() ); break;
			case PROFESSORES_QUANTIDADE_JANELAS: exporter = new ProfessoresJanelasGradeExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_DISCIPLINAS_HABILITADAS: exporter = new ProfessoresDisciplinasHabilitadasExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_DISCIPLINAS_LECIONADAS: exporter = new ProfessoresDisciplinasLecionadasExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_TITULACOES: exporter = new ProfessoresTitulacoesExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_AREAS_CONHECIMENTO: exporter = new ProfessoresAreasConhecimentoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case RELATORIO_VISAO_CURSO: exporter = new RelatorioVisaoCursoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case RELATORIO_VISAO_SALA: exporter = new RelatorioVisaoSalaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case RELATORIO_VISAO_PROFESSOR: exporter = new RelatorioVisaoProfessorExportExcel(cenario, i18nConstants, i18nMessages, filter, visaoProfessor, instituicaoEnsino, fileExtension ); break;
			case RELATORIO_VISAO_ALUNO: exporter = new RelatorioVisaoAlunoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case RESUMO_CAMPI: exporter = new ResumoCampiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case RESUMO_CENARIO: exporter = new ResumoCenarioExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case RESUMO_DISCIPLINA: exporter = new ResumoDisciplinaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case RESUMO_CURSO: exporter = new ResumoCursoExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case SALAS: exporter = new MultiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[]{SalasExportExcel.class,DisponibilidadesSalasExportExcel.class}, fileExtension, ExcelInformationType.SALAS.getSheetName() ); break;
			case TUDO: exporter = new TRIEDAExportExcel(cenario, i18nConstants, i18nMessages, visaoProfessor, instituicaoEnsino, fileExtension, planilhasExportExcel, nomeArquivo ); break;
			case UNIDADES: exporter = new UnidadesExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case TIPOS_CURSO: exporter = new TiposCursoExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case TODAS_TABELAS:  exporter = new TRIEDATabelasExportExcel(cenario, i18nConstants, i18nMessages, visaoProfessor, instituicaoEnsino, fileExtension ); break;
			case TODAS_GRADES_HORARIAS:  exporter = new TRIEDAGradesHorariasExportExcel(cenario, i18nConstants, i18nMessages, visaoProfessor, instituicaoEnsino, fileExtension ); break;
			case TODAS_VISAO_ALUNO:  exporter = new TRIEDAVisaoAlunoExportExcel(cenario, i18nConstants, i18nMessages, visaoProfessor, instituicaoEnsino, fileExtension ); break;
			case TURNOS:  exporter = new TurnosExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension ); break;
			case SEMANA_LETIVA:  exporter = new MultiExportExcel(cenario, i18nConstants, i18nMessages, instituicaoEnsino, new Class[]{SemanaLetivaExportExcel.class,SemanaLetivaHorariosExportExcel.class}, fileExtension, ExcelInformationType.SEMANA_LETIVA.getSheetName() ); break;
			case PROFESSORES_UTILIZADOS: exporter = new ProfessoresUtilizadosExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_GRADE_CHEIA: exporter = new ProfessoresGradeCheiaExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_BEM_ALOCADOS: exporter = new ProfessoresBemAlocadosExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_MAL_ALOCADOS: exporter = new ProfessoresMalAlocadosExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_JANELAS: exporter = new ProfessoresJanelaGradeExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_DESLOCAMENTO_UNIDADES: exporter = new ProfessoresDeslocamentoUnidadesExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case PROFESSORES_DESLOCAMENTO_CAMPI: exporter = new ProfessoresDeslocamentoCampiExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
			case COMPARAR_CENARIOS: exporter = new CompararCenariosExportExcel(cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension ); break;
		default:
			break;
		}

		return exporter;
	}
}
