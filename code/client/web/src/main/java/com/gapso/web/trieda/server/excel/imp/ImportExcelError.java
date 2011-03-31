package com.gapso.web.trieda.server.excel.imp;

import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;


public enum ImportExcelError {
	
	CAMPUS_CODIGO_VAZIO,
	CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO,
	CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO,
	CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO,
	CAMPUS_ESTADO_VALOR_INVALIDO,
	CAMPUS_NOME_VAZIO,
	
	CURRICULO_CODIGO_VAZIO,
	CURRICULO_CURSO_VAZIO,
	CURRICULO_DESCRICAO_VAZIO,
	CURRICULO_DISCIPLINA_VAZIO,
	CURRICULO_PERIODO_FORMATO_INVALIDO,
	CURRICULO_PERIODO_VALOR_NEGATIVO,
	CURRICULO_PERIODO_VAZIO,
	
	CURSO_CODIGO_VAZIO,
	CURSO_NOME_VAZIO,
	CURSO_TIPO_VAZIO,
	CURSO_MIN_DOUTOR_FORMATO_INVALIDO,
	CURSO_MIN_DOUTOR_VALOR_NEGATIVO,
	CURSO_MIN_DOUTOR_VAZIO,
	CURSO_MIN_MESTRE_FORMATO_INVALIDO,
	CURSO_MIN_MESTRE_VALOR_NEGATIVO,
	CURSO_MIN_MESTRE_VAZIO,
	CURSO_MAX_DISC_PROF_FORMATO_INVALIDO,
	CURSO_MAX_DISC_PROF_VALOR_NEGATIVO,
	CURSO_MAX_DISC_PROF_VAZIO,
	CURSO_MAIS_DE_UMA_DISC_PROF_FORMATO_INVALIDO,
	CURSO_MAIS_DE_UMA_DISC_PROF_VAZIO,
	
	DISCIPLINA_CODIGO_VAZIO,
	DISCIPLINA_CREDITOS_TEORICOS_FORMATO_INVALIDO,
	DISCIPLINA_CREDITOS_TEORICOS_VALOR_NEGATIVO,
	DISCIPLINA_CREDITOS_TEORICOS_VAZIO,
	DISCIPLINA_CREDITOS_PRATICOS_FORMATO_INVALIDO,
	DISCIPLINA_CREDITOS_PRATICOS_VALOR_NEGATIVO,
	DISCIPLINA_CREDITOS_PRATICOS_VAZIO,
	DISCIPLINA_MAX_ALUNOS_PRATICOS_FORMATO_INVALIDO,
	DISCIPLINA_MAX_ALUNOS_PRATICOS_VALOR_NEGATIVO,
	DISCIPLINA_MAX_ALUNOS_PRATICOS_VAZIO,
	DISCIPLINA_MAX_ALUNOS_TEORICOS_FORMATO_INVALIDO,
	DISCIPLINA_MAX_ALUNOS_TEORICOS_VALOR_NEGATIVO,
	DISCIPLINA_MAX_ALUNOS_TEORICOS_VAZIO,
	DISCIPLINA_NIVEL_DIFICULDADE_VALOR_INVALIDO,
	DISCIPLINA_NIVEL_DIFICULDADE_VAZIO,
	DISCIPLINA_NOME_VAZIO,
	DISCIPLINA_USA_LABORATORIO_FORMATO_INVALIDO,
	DISCIPLINA_USA_LABORATORIO_VAZIO,
	DISCIPLINA_TIPO_VAZIO,
	
	SALA_ANDAR_VAZIO,
	SALA_CAPACIDADE_FORMATO_INVALIDO,
	SALA_CAPACIDADE_VALOR_NEGATIVO,
	SALA_CAPACIDADE_VAZIO,
	SALA_CODIGO_VAZIO,
	SALA_NUMERO_VAZIO,
	SALA_TIPO_VAZIO,
	SALA_UNIDADE_VAZIO,
	
	TUDO_VAZIO,
	
	UNIDADE_CAMPUS_VAZIO,
	UNIDADE_CODIGO_VAZIO,
	UNIDADE_NOME_VAZIO;
	
	public String getMessage(String param1, TriedaI18nMessages i18nMessages) {
		
		switch (this) {
			case CAMPUS_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiImportExcel.CODIGO_COLUMN_NAME);
			case CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME);
			case CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME);
			case CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME);
			case CAMPUS_ESTADO_VALOR_INVALIDO: return i18nMessages.excelErroSintaticoValorInvalido(param1,CampiImportExcel.ESTADO_COLUMN_NAME);
			case CAMPUS_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiImportExcel.NOME_COLUMN_NAME);

			case CURRICULO_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.CODIGO_COLUMN_NAME);
			case CURRICULO_CURSO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.CURSO_COLUMN_NAME);
			case CURRICULO_DESCRICAO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.DESCRICAO_COLUMN_NAME);
			case CURRICULO_DISCIPLINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.DISCIPLINA_COLUMN_NAME);
			case CURRICULO_PERIODO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CurriculosImportExcel.PERIODO_COLUMN_NAME);
			case CURRICULO_PERIODO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CurriculosImportExcel.PERIODO_COLUMN_NAME);
			case CURRICULO_PERIODO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.PERIODO_COLUMN_NAME);
			
			case CURSO_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.CODIGO_COLUMN_NAME);
			case CURSO_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.NOME_COLUMN_NAME);
			case CURSO_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.TIPO_COLUMN_NAME);
			case CURSO_MIN_DOUTOR_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MIN_DOUTOR_COLUMN_NAME);
			case CURSO_MIN_DOUTOR_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MIN_DOUTOR_COLUMN_NAME);
			case CURSO_MIN_DOUTOR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MIN_DOUTOR_COLUMN_NAME);
			case CURSO_MIN_MESTRE_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MIN_MESTRE_COLUMN_NAME);
			case CURSO_MIN_MESTRE_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MIN_MESTRE_COLUMN_NAME);
			case CURSO_MIN_MESTRE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MIN_MESTRE_COLUMN_NAME);
			case CURSO_MAX_DISC_PROF_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MAX_DISC_PROF_COLUMN_NAME);
			case CURSO_MAX_DISC_PROF_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MAX_DISC_PROF_COLUMN_NAME);
			case CURSO_MAX_DISC_PROF_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MAX_DISC_PROF_COLUMN_NAME);
			case CURSO_MAIS_DE_UMA_DISC_PROF_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MAIS_DE_UMA_DISC_PROF_COLUMN_NAME);
			case CURSO_MAIS_DE_UMA_DISC_PROF_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MAIS_DE_UMA_DISC_PROF_COLUMN_NAME);
			
			case DISCIPLINA_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.CODIGO_COLUMN_NAME);
			case DISCIPLINA_CREDITOS_TEORICOS_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DisciplinasImportExcel.CRED_TEORICOS_COLUMN_NAME);
			case DISCIPLINA_CREDITOS_TEORICOS_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DisciplinasImportExcel.CRED_TEORICOS_COLUMN_NAME);
			case DISCIPLINA_CREDITOS_TEORICOS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.CRED_TEORICOS_COLUMN_NAME);
			case DISCIPLINA_CREDITOS_PRATICOS_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DisciplinasImportExcel.CRED_PRATICOS_COLUMN_NAME);
			case DISCIPLINA_CREDITOS_PRATICOS_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DisciplinasImportExcel.CRED_PRATICOS_COLUMN_NAME);
			case DISCIPLINA_CREDITOS_PRATICOS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.CRED_PRATICOS_COLUMN_NAME);
			case DISCIPLINA_MAX_ALUNOS_PRATICOS_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DisciplinasImportExcel.MAX_ALUNOS_PRATICOS_COLUMN_NAME);
			case DISCIPLINA_MAX_ALUNOS_PRATICOS_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DisciplinasImportExcel.MAX_ALUNOS_PRATICOS_COLUMN_NAME);
			case DISCIPLINA_MAX_ALUNOS_PRATICOS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.MAX_ALUNOS_PRATICOS_COLUMN_NAME);
			case DISCIPLINA_MAX_ALUNOS_TEORICOS_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DisciplinasImportExcel.MAX_ALUNOS_TEORICOS_COLUMN_NAME);
			case DISCIPLINA_MAX_ALUNOS_TEORICOS_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DisciplinasImportExcel.MAX_ALUNOS_TEORICOS_COLUMN_NAME);
			case DISCIPLINA_MAX_ALUNOS_TEORICOS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.MAX_ALUNOS_TEORICOS_COLUMN_NAME);
			case DISCIPLINA_NIVEL_DIFICULDADE_VALOR_INVALIDO: return i18nMessages.excelErroSintaticoValorInvalido(param1,DisciplinasImportExcel.NIVEL_DIFICULDADE_COLUMN_NAME);
			case DISCIPLINA_NIVEL_DIFICULDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.NIVEL_DIFICULDADE_COLUMN_NAME);
			case DISCIPLINA_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.NOME_COLUMN_NAME);
			case DISCIPLINA_USA_LABORATORIO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DisciplinasImportExcel.USA_LABORATORIO_COLUMN_NAME);
			case DISCIPLINA_USA_LABORATORIO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.USA_LABORATORIO_COLUMN_NAME);
			case DISCIPLINA_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasImportExcel.TIPO_COLUMN_NAME);
			
			case SALA_ANDAR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.ANDAR_COLUMN_NAME);
			case SALA_CAPACIDADE_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CAPACIDADE_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CAPACIDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.CODIGO_COLUMN_NAME);
			case SALA_NUMERO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.NUMERO_COLUMN_NAME);
			case SALA_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.TIPO_COLUMN_NAME);
			case SALA_UNIDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.UNIDADE_COLUMN_NAME);
			
			case TUDO_VAZIO: return i18nMessages.excelErroSintaticoLinhaVazia(param1);
			
			case UNIDADE_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,UnidadesImportExcel.CAMPUS_COLUMN_NAME);
			case UNIDADE_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,UnidadesImportExcel.CODIGO_COLUMN_NAME);
			case UNIDADE_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,UnidadesImportExcel.NOME_COLUMN_NAME);
		}
		return "";
	}
}