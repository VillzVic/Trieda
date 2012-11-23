package com.gapso.web.trieda.server.excel.imp;

import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public enum ImportExcelError
{
	ALUNO_MATRICULA_VAZIO,
	ALUNO_NOME_VAZIO,
	ALUNO_FORMANDO_VAZIO,
	ALUNO_FORMANDO_FORMATO_INVALIDO,
	
	ALUNO_DEMANDA_CAMPUS_VAZIO,
	ALUNO_DEMANDA_CURSO_VAZIO,
	ALUNO_DEMANDA_CURRICULO_VAZIO,
	ALUNO_DEMANDA_TURNO_VAZIO,
	ALUNO_DEMANDA_DISCIPLINA_VAZIO,
	ALUNO_DEMANDA_NOME_ALUNO_VAZIO,
	ALUNO_DEMANDA_MATRICULA_ALUNO_VAZIO,
	ALUNO_DEMANDA_PERIODO_FORMATO_INVALIDO,
	ALUNO_DEMANDA_PERIODO_NEGATIVO,
	ALUNO_DEMANDA_PRIORIDADE_FORMATO_INVALIDO,
	ALUNO_DEMANDA_PRIORIDADE_NEGATIVO,
	
	AREA_TITULACAO_CODIGO_VAZIO,
	
	CAMPITRABALHO_CAMPUS_VAZIO,
	CAMPITRABALHO_CPF_VAZIO,
	CAMPITRABALHO_PROFESSOR_VAZIO,
	
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
	CURRICULO_SEMANA_LETIVA_VAZIA,

	CURSO_AREA_TITULACAO_CURSO_VAZIO,
	CURSO_AREA_TITULACAO_AREA_VAZIO,
	
	CURSO_CODIGO_VAZIO,
	CURSO_NOME_VAZIO,
	CURSO_TIPO_VAZIO,
	CURSO_MIN_DOUTOR_FORMATO_INVALIDO,
	CURSO_MIN_DOUTOR_VALOR_NEGATIVO,
	CURSO_MIN_DOUTOR_VAZIO,
	CURSO_MIN_MESTRE_FORMATO_INVALIDO,
	CURSO_MIN_MESTRE_VALOR_NEGATIVO,
	CURSO_MIN_MESTRE_VAZIO,
	CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_FORMATO_INVALIDO,
	CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_VALOR_NEGATIVO,
	CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_VAZIO,
	CURSO_MIN_TEMPO_INTEGRAL_FORMATO_INVALIDO,
	CURSO_MIN_TEMPO_INTEGRAL_VALOR_NEGATIVO,
	CURSO_MIN_TEMPO_INTEGRAL_VAZIO,
	CURSO_MAX_DISC_PROF_FORMATO_INVALIDO,
	CURSO_MAX_DISC_PROF_VALOR_NEGATIVO,
	CURSO_MAX_DISC_PROF_VAZIO,
	CURSO_MAIS_DE_UMA_DISC_PROF_FORMATO_INVALIDO,
	CURSO_MAIS_DE_UMA_DISC_PROF_VAZIO,
	
	DEMANDA_CAMPUS_VAZIO,
	DEMANDA_CURSO_VAZIO,
	DEMANDA_DEMANDA_FORMATO_INVALIDO,
	DEMANDA_DEMANDA_VALOR_NEGATIVO,
	DEMANDA_DEMANDA_VAZIO,
	DEMANDA_DISCIPLINA_VAZIO,
	DEMANDA_MATRIZ_CURRIULAR_VAZIO,
	DEMANDA_PERIODO_FORMATO_INVALIDO,
	DEMANDA_PERIODO_VALOR_NEGATIVO,
	DEMANDA_PERIODO_VAZIO,
	DEMANDA_RECEITA_FORMATO_INVALIDO,
	DEMANDA_RECEITA_VALOR_NEGATIVO,
	DEMANDA_TURNO_VAZIO,
	
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
	
	DISCIPLINASALA_CURSO_VAZIO,
	DISCIPLINASALA_DISCIPLINA_VAZIO,
	DISCIPLINASALA_MATRIZ_CURRIULAR_VAZIO,
	DISCIPLINASALA_PERIODO_FORMATO_INVALIDO,
	DISCIPLINASALA_PERIODO_VALOR_NEGATIVO,
	DISCIPLINASALA_PERIODO_VAZIO,
	DISCIPLINASALA_SALA_VAZIO,
	
	EQUIVALENCIA_CURSOU_VAZIO,
	EQUIVALENCIA_ELIMINA_VAZIO,
	
	HABILITACOESPROFESSORES_CODIGO_CAMPUS_VAZIO,
	HABILITACOESPROFESSORES_CPF_PROFESSOR_VAZIO,
	HABILITACOESPROFESSORES_DISCIPLINA_VAZIO,
	HABILITACOESPROFESSORES_NOTA_FORMATO_INVALIDO,
	HABILITACOESPROFESSORES_NOTA_VALOR_NEGATIVO,
	HABILITACOESPROFESSORES_NOTA_VAZIO,
	HABILITACOESPROFESSORES_PREFERENCIA_FORMATO_INVALIDO,
	HABILITACOESPROFESSORES_PREFERENCIA_VALOR_NEGATIVO,
	HABILITACOESPROFESSORES_PREFERENCIA_VAZIO,
	
	PROFESSOR_CPF_VAZIO,
	PROFESSOR_NOME_VAZIO,
	PROFESSOR_TIPO_VAZIO,
	PROFESSOR_CARGA_HORARIA_MAX_VAZIO,
	PROFESSOR_CARGA_HORARIA_MIN_VAZIO,
	PROFESSOR_TITULACAO_VAZIO,
	PROFESSOR_AREA_TITULACAO_VAZIO,
	PROFESSOR_CARGA_HORARIA_ANTERIOR_VAZIO,
	PROFESSOR_VALOR_CREDITO_VAZIO,
	PROFESSOR_CARGA_HORARIA_MAX_FORMATO_INVALIDO,
	PROFESSOR_CARGA_HORARIA_MAX_VALOR_NEGATIVO,
	PROFESSOR_CARGA_HORARIA_MIN_FORMATO_INVALIDO,
	PROFESSOR_CARGA_HORARIA_MIN_VALOR_NEGATIVO,
	PROFESSOR_CARGA_HORARIA_ANTERIOR_FORMATO_INVALIDO,
	PROFESSOR_CARGA_HORARIA_ANTERIOR_VALOR_NEGATIVO,
	PROFESSOR_VALOR_CREDITO_FORMATO_INVALIDO,
	PROFESSOR_VALOR_CREDITO_VALOR_NEGATIVO,
	
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

	public String getMessage( String param1, TriedaI18nMessages i18nMessages )
	{
		switch ( this )
		{
			case ALUNO_MATRICULA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosImportExcel.MATRICULA_COLUMN_NAME );
			case ALUNO_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosImportExcel.NOME_COLUMN_NAME );
			case ALUNO_FORMANDO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,AlunosImportExcel.FORMANDO_COLUMN_NAME);
			case ALUNO_FORMANDO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosImportExcel.FORMANDO_COLUMN_NAME );
		
			case ALUNO_DEMANDA_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.CAMPUS_COLUMN_NAME );
			case ALUNO_DEMANDA_CURSO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.CURSO_COLUMN_NAME );
			case ALUNO_DEMANDA_CURRICULO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.CURRICULO_COLUMN_NAME );
			case ALUNO_DEMANDA_TURNO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.TURNO_COLUMN_NAME );
			case ALUNO_DEMANDA_DISCIPLINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.DISCIPLINA_COLUMN_NAME );
			case ALUNO_DEMANDA_NOME_ALUNO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.NOME_ALUNO_COLUMN_NAME );
			case ALUNO_DEMANDA_MATRICULA_ALUNO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, AlunosDemandaImportExcel.MATRICULA_ALUNO_COLUMN_NAME );
			case ALUNO_DEMANDA_PERIODO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido( param1, AlunosDemandaImportExcel.PERIODO_COLUMN_NAME );
			case ALUNO_DEMANDA_PERIODO_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo( param1, AlunosDemandaImportExcel.PERIODO_COLUMN_NAME );
			case ALUNO_DEMANDA_PRIORIDADE_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido( param1, AlunosDemandaImportExcel.PRIORIDADE_COLUMN_NAME );
			case ALUNO_DEMANDA_PRIORIDADE_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo( param1, AlunosDemandaImportExcel.PRIORIDADE_COLUMN_NAME );
		
			case AREA_TITULACAO_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,AreasTitulacaoImportExcel.CODIGO_COLUMN_NAME);
			
			case CAMPITRABALHO_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiTrabalhoImportExcel.CAMPUS_COLUMN_NAME);
			case CAMPITRABALHO_CPF_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiTrabalhoImportExcel.CPF_COLUMN_NAME);
			case CAMPITRABALHO_PROFESSOR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CampiTrabalhoImportExcel.PROFESSOR_COLUMN_NAME);
		
			case CAMPUS_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, CampiImportExcel.CODIGO_COLUMN_NAME );
			case CAMPUS_CUSTO_MEDIO_CREDITO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido( param1, CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME );
			case CAMPUS_CUSTO_MEDIO_CREDITO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo( param1, CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME );
			case CAMPUS_CUSTO_MEDIO_CREDITO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, CampiImportExcel.CUSTO_CREDITO_COLUMN_NAME );
			case CAMPUS_ESTADO_VALOR_INVALIDO: return i18nMessages.excelErroSintaticoValorInvalido( param1, CampiImportExcel.ESTADO_COLUMN_NAME );
			case CAMPUS_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1, CampiImportExcel.NOME_COLUMN_NAME );

			case CURRICULO_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.CODIGO_COLUMN_NAME);
			case CURRICULO_CURSO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.CURSO_COLUMN_NAME);
			case CURRICULO_DESCRICAO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.DESCRICAO_COLUMN_NAME);
			case CURRICULO_DISCIPLINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.DISCIPLINA_COLUMN_NAME);
			case CURRICULO_PERIODO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CurriculosImportExcel.PERIODO_COLUMN_NAME);
			case CURRICULO_PERIODO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CurriculosImportExcel.PERIODO_COLUMN_NAME);
			case CURRICULO_PERIODO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CurriculosImportExcel.PERIODO_COLUMN_NAME);
			
			case CURSO_AREA_TITULACAO_CURSO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursoAreasTitulacaoImportExcel.CURSO_COLUMN_NAME);
			case CURSO_AREA_TITULACAO_AREA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursoAreasTitulacaoImportExcel.AREA_TITULACAO_COLUMN_NAME);
			
			case CURSO_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.CODIGO_COLUMN_NAME);
			case CURSO_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.NOME_COLUMN_NAME);
			case CURSO_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.TIPO_COLUMN_NAME);
			case CURSO_MIN_DOUTOR_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MIN_DOUTOR_COLUMN_NAME);
			case CURSO_MIN_DOUTOR_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MIN_DOUTOR_COLUMN_NAME);
			case CURSO_MIN_DOUTOR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MIN_DOUTOR_COLUMN_NAME);
			case CURSO_MIN_MESTRE_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MIN_MESTRE_COLUMN_NAME);
			case CURSO_MIN_MESTRE_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MIN_MESTRE_COLUMN_NAME);
			case CURSO_MIN_MESTRE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MIN_MESTRE_COLUMN_NAME);
			case CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME);
			case CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME);
			case CURSO_MIN_TEMPO_INTEGRAL_PARCIAL_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MIN_TEMPO_INTEGRAL_PARCIAL_COLUMN_NAME);
			case CURSO_MIN_TEMPO_INTEGRAL_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MIN_TEMPO_INTEGRAL_COLUMN_NAME);
			case CURSO_MIN_TEMPO_INTEGRAL_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MIN_TEMPO_INTEGRAL_COLUMN_NAME);
			case CURSO_MIN_TEMPO_INTEGRAL_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MIN_TEMPO_INTEGRAL_COLUMN_NAME);
			case CURSO_MAX_DISC_PROF_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MAX_DISC_PROF_COLUMN_NAME);
			case CURSO_MAX_DISC_PROF_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,CursosImportExcel.MAX_DISC_PROF_COLUMN_NAME);
			case CURSO_MAX_DISC_PROF_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MAX_DISC_PROF_COLUMN_NAME);
			case CURSO_MAIS_DE_UMA_DISC_PROF_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,CursosImportExcel.MAIS_DE_UMA_DISC_PROF_COLUMN_NAME);
			case CURSO_MAIS_DE_UMA_DISC_PROF_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,CursosImportExcel.MAIS_DE_UMA_DISC_PROF_COLUMN_NAME);
			
			case DEMANDA_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.CAMPUS_COLUMN_NAME);
			case DEMANDA_CURSO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.CURSO_COLUMN_NAME);
			case DEMANDA_DEMANDA_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DemandasImportExcel.DEMANDA_COLUMN_NAME);
			case DEMANDA_DEMANDA_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DemandasImportExcel.DEMANDA_COLUMN_NAME);
			case DEMANDA_DEMANDA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.DEMANDA_COLUMN_NAME);
			case DEMANDA_DISCIPLINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.DISCIPLINA_COLUMN_NAME);
			case DEMANDA_MATRIZ_CURRIULAR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.MATRIZ_CURRICULAR_COLUMN_NAME);
			case DEMANDA_PERIODO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DemandasImportExcel.PERIODO_COLUMN_NAME);
			case DEMANDA_PERIODO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DemandasImportExcel.PERIODO_COLUMN_NAME);
			case DEMANDA_PERIODO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.PERIODO_COLUMN_NAME);
			case DEMANDA_RECEITA_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DemandasImportExcel.RECEITA_COLUMN_NAME);
			case DEMANDA_RECEITA_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DemandasImportExcel.RECEITA_COLUMN_NAME);
			case DEMANDA_TURNO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DemandasImportExcel.TURNO_COLUMN_NAME);
			
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
			
			case DISCIPLINASALA_CURSO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasSalasImportExcel.CURSO_COLUMN_NAME);
			case DISCIPLINASALA_DISCIPLINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasSalasImportExcel.DISCIPLINA_COLUMN_NAME);
			case DISCIPLINASALA_MATRIZ_CURRIULAR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasSalasImportExcel.MATRIZ_CURRICULAR_COLUMN_NAME);
			case DISCIPLINASALA_PERIODO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,DisciplinasSalasImportExcel.PERIODO_COLUMN_NAME);
			case DISCIPLINASALA_PERIODO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,DisciplinasSalasImportExcel.PERIODO_COLUMN_NAME);
			case DISCIPLINASALA_PERIODO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasSalasImportExcel.PERIODO_COLUMN_NAME);
			case DISCIPLINASALA_SALA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,DisciplinasSalasImportExcel.SALA_COLUMN_NAME);
			
			case EQUIVALENCIA_CURSOU_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,EquivalenciasImportExcel.CURSOU_COLUMN_NAME);
			case EQUIVALENCIA_ELIMINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,EquivalenciasImportExcel.ELIMINA_COLUMN_NAME);
			
			case HABILITACOESPROFESSORES_CODIGO_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,HabilitacoesProfessoresImportExcel.CODIGO_CAMPUS_COLUMN_NAME);
			case HABILITACOESPROFESSORES_CPF_PROFESSOR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,HabilitacoesProfessoresImportExcel.CPF_PROFESSOR_COLUMN_NAME);
			case HABILITACOESPROFESSORES_DISCIPLINA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,HabilitacoesProfessoresImportExcel.DISCIPLINA_COLUMN_NAME);
			case HABILITACOESPROFESSORES_NOTA_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,HabilitacoesProfessoresImportExcel.NOTA_COLUMN_NAME);
			case HABILITACOESPROFESSORES_NOTA_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,HabilitacoesProfessoresImportExcel.NOTA_COLUMN_NAME);
			case HABILITACOESPROFESSORES_NOTA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,HabilitacoesProfessoresImportExcel.NOTA_COLUMN_NAME);
			case HABILITACOESPROFESSORES_PREFERENCIA_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,HabilitacoesProfessoresImportExcel.PREFERENCIA_COLUMN_NAME);
			case HABILITACOESPROFESSORES_PREFERENCIA_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,HabilitacoesProfessoresImportExcel.PREFERENCIA_COLUMN_NAME);
			case HABILITACOESPROFESSORES_PREFERENCIA_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,HabilitacoesProfessoresImportExcel.PREFERENCIA_COLUMN_NAME);
			
			case PROFESSOR_CPF_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.CPF_COLUMN_NAME);
			case PROFESSOR_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.NOME_COLUMN_NAME);
			case PROFESSOR_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.TIPO_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_MAX_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.CARGA_HORARIA_MAX_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_MIN_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.CARGA_HORARIA_MIN_COLUMN_NAME);
			case PROFESSOR_TITULACAO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.TITULACAO_COLUMN_NAME);
			case PROFESSOR_AREA_TITULACAO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.AREA_TITULACAO_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_ANTERIOR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.CARGA_HORARIA_ANTERIOR_COLUMN_NAME);
			case PROFESSOR_VALOR_CREDITO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,ProfessoresImportExcel.VALOR_CREDITO_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_MAX_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,ProfessoresImportExcel.CARGA_HORARIA_MAX_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_MIN_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,ProfessoresImportExcel.CARGA_HORARIA_MIN_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_ANTERIOR_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,ProfessoresImportExcel.CARGA_HORARIA_ANTERIOR_COLUMN_NAME);
			case PROFESSOR_VALOR_CREDITO_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,ProfessoresImportExcel.VALOR_CREDITO_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_MAX_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,ProfessoresImportExcel.CARGA_HORARIA_MAX_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_MIN_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,ProfessoresImportExcel.CARGA_HORARIA_MIN_COLUMN_NAME);
			case PROFESSOR_CARGA_HORARIA_ANTERIOR_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,ProfessoresImportExcel.CARGA_HORARIA_ANTERIOR_COLUMN_NAME);
			case PROFESSOR_VALOR_CREDITO_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,ProfessoresImportExcel.VALOR_CREDITO_COLUMN_NAME);
			
			case SALA_ANDAR_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.ANDAR_COLUMN_NAME);
			case SALA_CAPACIDADE_FORMATO_INVALIDO: return i18nMessages.excelErroSintaticoFormatoInvalido(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CAPACIDADE_VALOR_NEGATIVO: return i18nMessages.excelErroSintaticoValorNegativo(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CAPACIDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.CAPACIDADE_COLUMN_NAME);
			case SALA_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.CODIGO_COLUMN_NAME);
			case SALA_NUMERO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.NUMERO_COLUMN_NAME);
			case SALA_TIPO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.TIPO_COLUMN_NAME);
			case SALA_UNIDADE_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia(param1,SalasImportExcel.UNIDADE_COLUMN_NAME);

			case TUDO_VAZIO: return i18nMessages.excelErroSintaticoLinhaVazia( param1 );

			case UNIDADE_CAMPUS_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1,UnidadesImportExcel.CAMPUS_COLUMN_NAME );
			case UNIDADE_CODIGO_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1,UnidadesImportExcel.CODIGO_COLUMN_NAME );
			case UNIDADE_NOME_VAZIO: return i18nMessages.excelErroSintaticoColunaVazia( param1,UnidadesImportExcel.NOME_COLUMN_NAME );
		}

		return "";
	}
}
