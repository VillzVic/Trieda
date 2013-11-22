package com.gapso.web.trieda.shared.excel;

public enum ExcelInformationType
{
	TUDO( "" ),
	CAMPI( "Campi" ),
	CURRICULOS( "Curriculos"),
	CURSOS( "Cursos" ),
	AREAS_TITULACAO( "Areas de Titulacao" ),
	CURSO_AREAS_TITULACAO( "Curso - Area de Titulacao" ),
	DEMANDAS( "Ofertas e Demandas" ),
	DEMANDAS_POR_ALUNO( "Demanda por Aluno" ),
	DISCIPLINAS( "Disciplinas" ),
	EQUIVALENCIAS( "Equivalencias" ),
	DISCIPLINAS_SALAS( "Disciplinas-Salas" ),
	PALETA_CORES( "PaletaCores" ),
	PROFESSORES( "Professores" ),
	DISPONIBILIDADES_PROFESSORES( "Disponibilidades Professores" ),
	DISPONIBILIDADES_SALAS( "Disponibilidades Salas" ),
	CAMPI_TRABALHO( "Campi de Trabalho" ),
	RELATORIO_VISAO_SALA( "Relatorio Visao Sala" ),
	RELATORIO_VISAO_CURSO( "Relatorio Visao Curso" ),
	RELATORIO_VISAO_PROFESSOR( "Relatorio Visao Professor" ),
	RELATORIO_VISAO_ALUNO("Relatorio Visao Aluno"),
	SALAS( "Salas" ),
	UNIDADES( "Unidades" ),
	RESUMO_CURSO( "Resumo por curso" ),
	RESUMO_DISCIPLINA( "Resumo por disciplina" ),
	HABILITACAO_PROFESSORES( "Habilitacao dos Professores" ),
	ATENDIMENTOS_POR_ALUNO( "Atendimentos por Aluno" ),
	AULAS( "Aulas" ),
	ALUNOS( "Alunos" ),
	ATENDIMENTOS_MATRICULA("Atendimentos por Matricula"),
	ATENDIMENTOS_DISCIPLINA("Atendimentos por Disciplina"),
	ATENDIMENTOS_FAIXA_DEMANDA("Atendimentos Faixa de Demanda"),
	PERCENT_MESTRES_DOUTORES("Porcentagem Mestres e Doutores"),
	ATENDIMENTOS_CARGA_HORARIA("Atendimentos por Carga Horaria"),
	DISCIPLINAS_PRE_REQUISITOS("Disciplinas Pre-Requisitos"),
	DISCIPLINAS_CO_REQUISITOS("Disciplinas Co-Requisitos"),
	ALUNOS_DISCIPLINAS_CURSADAS("Disciplinas Cursadas por Aluno"),
	ATENDIMENTOS_FAIXA_CREDITO("Atendimentos Faixa de Credito"),
	ATENDIMENTOS_FAIXA_DISCIPLINA("Atendimentos Faixa Disciplina"),
	PROFESSORES_QUANTIDADE_JANELAS("Professores Quantidade Janelas"),
	PROFESSORES_DISCIPLINAS_HABILITADAS("Professores Disc Habilitadas"),
	PROFESSORES_DISCIPLINAS_LECIONADAS("Professores Disc Lecionadas"),
	PROFESSORES_TITULACOES("Professores Titulacoes"),
	PROFESSORES_AREAS_CONHECIMENTO("Professores Areas Conhecimento"),
	AMBIENTES_FAIXA_OCUPACAO("Ambientes Faixa Ocupacao"),
	AMBIENTES_FAIXA_UTILIZACAO("Ambientes Faixa Utilizacao"),
	TURNOS("Turnos"),
	TIPOS_CURSO("Tipos de Curso"),
	DIVISOES_CREDITO("Divisoes de Credito"),
	CAMPI_DESLOCAMENTO("Deslocamento entre Campi"),
	UNIDADES_DESLOCAMENTO("Deslocamento entre Unidades"),
	RESUMO_CENARIO("Resumo por Cenario"),
	RESUMO_CAMPI("Resumo por Campus"),
	TODAS_TABELAS( "" ),
	TODAS_GRADES_HORARIAS( "" ),
	TODAS_VISAO_ALUNO( "" );

	private String sheetName;

	ExcelInformationType( String sheetName )
	{
		this.sheetName = sheetName;
	}

	public String getSheetName()
	{
		return sheetName;
	}

	public static String getInformationParameterName()
	{
		return "excelInformationType";
	}

	public static String getFileParameterName()
	{
		return "uploadedFile";
	}

	public static String getFileExtensionParameterName()
	{
		return "fileExtension";
	}
	
	public static String prefixError()
	{
		return "@e@";
	}

	public static String prefixWarning()
	{
		return "@w@";
	}
}
