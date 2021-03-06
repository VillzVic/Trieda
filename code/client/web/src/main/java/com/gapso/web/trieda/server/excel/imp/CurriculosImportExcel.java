package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.services.CurriculoDomainService;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.dev.util.Pair;

@ProgressDeclarationAnnotation
public class CurriculosImportExcel extends AbstractImportExcel<CurriculosImportExcelBean>
{
	static public String CURSO_COLUMN_NAME;
	static public String CODIGO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String SEMANA_LETIVA_COLUMN_NAME;

	private List<String> headerColumnsNames;
	private boolean updateDisciplinaHorario;

	private CurriculoDomainService curriculoDomainService = new CurriculoDomainService();

	public CurriculosImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino, boolean updateDisciplinaHorario)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.updateDisciplinaHorario = updateDisciplinaHorario;
		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CURSO_COLUMN_NAME);
		this.headerColumnsNames.add(CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(DESCRICAO_COLUMN_NAME);
		this.headerColumnsNames.add(PERIODO_COLUMN_NAME);
		this.headerColumnsNames.add(DISCIPLINA_COLUMN_NAME);
		this.headerColumnsNames.add(SEMANA_LETIVA_COLUMN_NAME);
	}

	public CurriculosImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		this(cenario, i18nConstants, i18nMessages, instituicaoEnsino, false);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected CurriculosImportExcelBean createExcelBean(Row header, Row row)
	{
		CurriculosImportExcelBean bean = new CurriculosImportExcelBean(row.getRowNum() + 1);

		for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++)
		{
			Cell cell = row.getCell(cellIndex);

			if (cell != null)
			{
				Cell headerCell = header.getCell(cell.getColumnIndex());

				if (headerCell != null)
				{
					String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);

					if (CURSO_COLUMN_NAME.endsWith(columnName))
					{
						bean.setCursoCodigoStr(cellValue);
					}
					else if (CODIGO_COLUMN_NAME.equals(columnName))
					{
						bean.setCodigoStr(cellValue);
					}
					else if (DESCRICAO_COLUMN_NAME.endsWith(columnName))
					{
						bean.setDescricaoStr(cellValue);
					}
					else if (DISCIPLINA_COLUMN_NAME.endsWith(columnName))
					{
						bean.setDisciplinaCodigoStr(cellValue);
					}
					else if (PERIODO_COLUMN_NAME.endsWith(columnName))
					{
						bean.setPeriodoStr(cellValue);
					}
					else if (SEMANA_LETIVA_COLUMN_NAME.endsWith(columnName))
					{
						bean.setSemanaLetivaCodigoStr(cellValue);
					}
				}
			}
		}

		return bean;
	}

	@Override
	protected String getHeaderToString()
	{
		return this.headerColumnsNames.toString();
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.CURRICULOS.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<CurriculosImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (CurriculosImportExcelBean bean : sheetContent)
		{
			List<ImportExcelError> errorsBean = bean.checkSyntacticErrors();

			for (ImportExcelError error : errorsBean)
			{
				List<Integer> rowsWithErrors = syntacticErrorsMap.get(error);

				if (rowsWithErrors == null)
				{
					rowsWithErrors = new ArrayList<Integer>();
					syntacticErrorsMap.put(error, rowsWithErrors);
				}

				rowsWithErrors.add(bean.getRow());
			}
		}

		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet())
		{
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);

			getErrors().add(error.getMessage(linhasComErro.toString(), getI18nMessages()));
		}

		return syntacticErrorsMap.isEmpty();
	}

	@Override
	protected boolean doLogicValidation(List<CurriculosImportExcelBean> sheetContent)
	{
		// Verifica se alguma matriz curricular
		// apareceu mais de uma vez no arquivo de entrada
		checkUniquenessCurriculo(sheetContent);

		// Verifica se alguma disciplina
		// apareceu mais de uma vez para um par
		// ( Curriculo, Período ) no arquivo de entrada
		checkUniquenessDisciplina(sheetContent);

		// Verifica se há referência a algum curso não cadastrado
		checkNonRegisteredCurso(sheetContent);

		// Verifica se há referência a alguma disciplina não cadastrada
		checkNonRegisteredDisciplina(sheetContent);

		// Verifica se há referência a alguma semana letiva não cadastrada
		checkNonRegisteredSemanaLetiva(sheetContent);

		// TODO: Para a UNIT e a UniRitter a linha abaixo deve ser comentada
		// a verificação tem que ser alterada para não permitir mais de uma semana letiva com durações diferentes
		// checkDisciplinasAssociadasAMaisDeUmaSemanaLetiva(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkDisciplinasAssociadasAMaisDeUmaSemanaLetiva(List<CurriculosImportExcelBean> sheetContent)
	{
		// [CodDisciplina -> Pair<List<CodSemanaLetiva>,Map<CodSemanaLetiva,List<CurriculosImportExcelBean>>>]
		Map<String, Pair<Set<String>, Map<String, List<CurriculosImportExcelBean>>>> map = new HashMap<String, Pair<Set<String>, Map<String, List<CurriculosImportExcelBean>>>>();

		// preenche um Map que será utilizado para detectar se há disciplinas associadas a mais de uma semana letiva
		for (CurriculosImportExcelBean bean : sheetContent)
		{
			Pair<Set<String>, Map<String, List<CurriculosImportExcelBean>>> pair = map.get(bean.getDisciplinaCodigoStr());
			if (pair == null)
			{
				// primeira vez que a disciplina aparece

				// cria conjunto com a semana letiva da disciplina
				Set<String> semanasLetivasAssociadasComADisciplina = new HashSet<String>();
				semanasLetivasAssociadasComADisciplina.add(bean.getSemanaLetivaCodigoStr());

				// cria um map que associa os beans com as semanas letivas
				Map<String, List<CurriculosImportExcelBean>> semanaLetivaToBeansMap = new HashMap<String, List<CurriculosImportExcelBean>>();
				List<CurriculosImportExcelBean> beans = new ArrayList<CurriculosImportExcelBean>();
				beans.add(bean);
				semanaLetivaToBeansMap.put(bean.getSemanaLetivaCodigoStr(), beans);

				// cria o par e atualiza o map
				pair = Pair.create(semanasLetivasAssociadasComADisciplina, semanaLetivaToBeansMap);
				map.put(bean.getDisciplinaCodigoStr(), pair);
			}
			else
			{
				// atualiza conjunto de semanas letivas associadas com a disciplina
				Set<String> semanasLetivasAssociadasComADisciplina = pair.getLeft();
				semanasLetivasAssociadasComADisciplina.add(bean.getSemanaLetivaCodigoStr());

				// atualiza map que associa os beans com as semanas letivas
				Map<String, List<CurriculosImportExcelBean>> semanaLetivaToBeansMap = pair.getRight();
				List<CurriculosImportExcelBean> beans = semanaLetivaToBeansMap.get(bean.getSemanaLetivaCodigoStr());
				if (beans == null)
				{
					beans = new ArrayList<CurriculosImportExcelBean>();
					semanaLetivaToBeansMap.put(bean.getSemanaLetivaCodigoStr(), beans);
				}
				beans.add(bean);
			}
		}

		// Percorre um Map para detectar se há disciplinas associadas a mais de uma semana letiva
		for (Entry<String, Pair<Set<String>, Map<String, List<CurriculosImportExcelBean>>>> entryMap : map.entrySet())
		{
			String disciplina = entryMap.getKey();
			Pair<Set<String>, Map<String, List<CurriculosImportExcelBean>>> pair = entryMap.getValue();
			Set<String> semanasLetivasAssociadasComADisciplina = pair.getLeft();
			Map<String, List<CurriculosImportExcelBean>> semanaLetivaToBeansMap = pair.getRight();

			// verifica se a disciplina tem relação com mais de uma semana letiva
			if (semanasLetivasAssociadasComADisciplina.size() > 1)
			{
				String errorMsg = "A disciplina [" + disciplina + "] está associada com " + semanasLetivasAssociadasComADisciplina.size()
								+ " semanas letivas quando deveria estar associada com apenas uma. ";
				for (Entry<String, List<CurriculosImportExcelBean>> entrySemanaLetivaToBeansMap : semanaLetivaToBeansMap.entrySet())
				{
					String semanaLetiva = entrySemanaLetivaToBeansMap.getKey();
					List<CurriculosImportExcelBean> beans = entrySemanaLetivaToBeansMap.getValue();
					errorMsg += " Casos da semana letiva [" + semanaLetiva + "]: ";
					for (CurriculosImportExcelBean bean : beans)
					{
						errorMsg += "(" + bean.getCursoCodigoStr() + "," + bean.getCodigoStr() + "," + bean.getPeriodo() + "); ";
					}
				}

				getErrors().add(errorMsg);
			}
		}
	}

	private void checkUniquenessCurriculo(List<CurriculosImportExcelBean> sheetContent)
	{
		checkUniquenessCurriculoByCurso(sheetContent);
		checkUniquenessCurriculoByDescricao(sheetContent);
	}

	private void checkUniquenessCurriculoByCurso(List<CurriculosImportExcelBean> sheetContent)
	{
		// Map com o CodCurriculo e o conjunto de
		// pares ( CodCurriculo - CodCurso ) em que o mesmo aparece

		// [ CodCurriculo -> Conjunto de pares ( CodCurriculo - CodCurso ) ]
		Map<String, Set<String>> curriculoToParesCurriculoCursoMap = new HashMap<String, Set<String>>();

		// Map com o par ( CodCurriculo, CodCurso ) e as
		// linhas em que o mesmo aparece no arquivo de entrada

		// [ CodCurriculo - CodCurso -> Lista de Linhas do Arquivo de Entrada ]
		Map<String, List<Integer>> parCurriculoCursoToRowsMap = new HashMap<String, List<Integer>>();

		// Preenche os maps
		for (CurriculosImportExcelBean bean : sheetContent)
		{
			// Preenche o map parCurriculoCursoToRowsMap
			String par = bean.getCodigoStr() + "-" + bean.getCursoCodigoStr();
			List<Integer> rows = parCurriculoCursoToRowsMap.get(par);

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				parCurriculoCursoToRowsMap.put(par, rows);
			}

			rows.add(bean.getRow());

			// Preenche o map curriculoToParesCurriculoCursoMap
			Set<String> pares = curriculoToParesCurriculoCursoMap.get(bean.getCodigoStr());

			if (pares == null)
			{
				pares = new HashSet<String>();
				curriculoToParesCurriculoCursoMap.put(bean.getCodigoStr(), pares);
			}

			pares.add(par);
		}

		// Verifica se alguma matriz curricular
		// apareceu mais de uma vez no arquivo de entrada
		for (Entry<String, Set<String>> entry : curriculoToParesCurriculoCursoMap.entrySet())
		{
			String codCurriculo = entry.getKey();
			Set<String> pares = entry.getValue();

			// Verifica se para um curriculo
			// existe mais de um par ( CodCurriculo, CodCurso )

			if (pares.size() > 1)
			{
				// Para cada par ( CodCurriculo, CodCurso ), coleta
				// as linhas em que os mesmos aparecem no arquivo de entrada
				List<Integer> allRows = new ArrayList<Integer>();

				for (String par : pares)
				{
					List<Integer> rows = parCurriculoCursoToRowsMap.get(par);

					if (rows != null)
					{
						allRows.addAll(rows);
					}
				}

				Collections.sort(allRows);

				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeVioladaCurriculoPorCurso(codCurriculo, allRows.toString()));
			}
		}
	}

	private void checkUniquenessCurriculoByDescricao(List<CurriculosImportExcelBean> sheetContent)
	{
		// Map com o CodCurriculo e o conjunto de
		// pares ( CodCurriculo - Descricao ) em que o mesmo aparece

		// [ CodCurriculo -> Conjunto de pares ( CodCurriculo - Descricao ) ]
		Map<String, Set<String>> curriculoToParesCurriculoDescricaoMap = new HashMap<String, Set<String>>();

		// Map com o par ( CodCurriculo, Descricao ) e as
		// linhas em que o mesmo aparece no arquivo de entrada

		// [ CodCurriculo - Descricao -> Lista de Linhas do Arquivo de Entrada ]
		Map<String, List<Integer>> parCurriculoDescricaoToRowsMap = new HashMap<String, List<Integer>>();

		// Preenche os maps
		for (CurriculosImportExcelBean bean : sheetContent)
		{
			// Preenche o map parCurriculoDescricaoToRowsMap
			String par = bean.getCodigoStr() + "-" + bean.getDescricaoStr();
			List<Integer> rows = parCurriculoDescricaoToRowsMap.get(par);

			if (rows == null)
			{
				rows = new ArrayList<Integer>();
				parCurriculoDescricaoToRowsMap.put(par, rows);
			}

			rows.add(bean.getRow());

			// Preenche o map curriculoToParesCurriculoDescricaoMap
			Set<String> pares = curriculoToParesCurriculoDescricaoMap.get(bean.getCodigoStr());

			if (pares == null)
			{
				pares = new HashSet<String>();
				curriculoToParesCurriculoDescricaoMap.put(bean.getCodigoStr(), pares);
			}

			pares.add(par);
		}

		// Verifica se alguma matriz curricular
		// apareceu mais de uma vez no arquivo de entrada
		for (Entry<String, Set<String>> entry : curriculoToParesCurriculoDescricaoMap.entrySet())
		{
			String codCurriculo = entry.getKey();
			Set<String> pares = entry.getValue();

			// Verifica se para um curriculo
			// existe mais de um par ( CodCurriculo, Descricao )
			if (pares.size() > 1)
			{
				// Para cada par ( CodCurriculo, Descricao ), coleta
				// as linhas em que os mesmos aparecem no arquivo de entrada
				List<Integer> allRows = new ArrayList<Integer>();

				for (String par : pares)
				{
					List<Integer> rows = parCurriculoDescricaoToRowsMap.get(par);

					if (rows != null)
					{
						allRows.addAll(rows);
					}
				}

				Collections.sort(allRows);

				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeVioladaCurriculoPorDescricao(codCurriculo, allRows.toString()));
			}
		}
	}

	private void checkUniquenessDisciplina(List<CurriculosImportExcelBean> sheetContent)
	{
		// Codigo referente á issue http://jira.gapso.com.br/browse/TRIEDA-791

		// Map com o trio ( CodCurriculo, Periodo, CodDisciplina )
		// e os beans em que tal trio aparece no arquivo de entrada

		// [ CodCurriculo - Periodo - CodDisciplina -> Lista de Beans ]
		Map<String, List<CurriculosImportExcelBean>> trioToBeansMap = new HashMap<String, List<CurriculosImportExcelBean>>();

		for (CurriculosImportExcelBean bean : sheetContent)
		{
			String key = bean.getCodigoStr() + "-" + bean.getPeriodoStr() + "-" + bean.getDisciplinaCodigoStr();

			List<CurriculosImportExcelBean> beans = trioToBeansMap.get(key);

			if (beans == null)
			{
				beans = new ArrayList<CurriculosImportExcelBean>();
				trioToBeansMap.put(key, beans);
			}

			beans.add(bean);
		}

		// Verifica se algum trio ( CodCurriculo, Periodo, CodDisciplina )
		// apareceu mais de uma vez no arquivo de entrada
		for (Entry<String, List<CurriculosImportExcelBean>> entry : trioToBeansMap.entrySet())
		{
			if (entry.getValue().size() > 1)
			{
				CurriculosImportExcelBean firstBean = entry.getValue().get(0);

				List<Integer> rows = new ArrayList<Integer>();

				for (CurriculosImportExcelBean bean : entry.getValue())
				{
					rows.add(bean.getRow());
				}

				Collections.sort(rows);

				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeVioladaDisciplinaCurriculo(firstBean.getDisciplinaCodigoStr(), firstBean.getPeriodo().toString(), firstBean.getCodigoStr(),
								rows.toString()));
			}
		}
	}

	private void checkNonRegisteredCurso(List<CurriculosImportExcelBean> sheetContent)
	{
		// [ CodigoCurso -> Curso ]
		Map<String, Curso> cursosBDMap = Curso.buildCursoCodigoToCursoMap(Curso.findByCenario(this.instituicaoEnsino, getCenario()));

		if (cursosBDMap == null || cursosBDMap.size() == 0)
		{
			return;
		}

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (CurriculosImportExcelBean bean : sheetContent)
		{
			Curso curso = cursosBDMap.get(bean.getCursoCodigoStr());

			if (curso != null)
			{
				bean.setCurso(curso);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CURSO_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredDisciplina(List<CurriculosImportExcelBean> sheetContent)
	{
		// Codigo referente á issue http://jira.gapso.com.br/browse/TRIEDA-791

		// [ CodigoDisciplina -> Disciplina ]
		Map<String, Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (CurriculosImportExcelBean bean : sheetContent)
		{
			Disciplina disciplina = disciplinasBDMap.get(bean.getDisciplinaCodigoStr());
			if (disciplina != null)
			{
				bean.setDisciplina(disciplina);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(DISCIPLINA_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredSemanaLetiva(List<CurriculosImportExcelBean> sheetContent)
	{
		// [ CodigoSemanaLetiva -> Semana Letiva ]
		Map<String, SemanaLetiva> semanasLetivasBDMap = SemanaLetiva.buildSemanaLetivaCodigoToSemanaLetivaMap(SemanaLetiva.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (CurriculosImportExcelBean bean : sheetContent)
		{
			SemanaLetiva semanaLetiva = semanasLetivasBDMap.get(bean.getSemanaLetivaCodigoStr());

			if (semanaLetiva != null)
			{
				bean.setSemanaLetiva(semanaLetiva);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(SEMANA_LETIVA_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	protected void updateDataBase(List<CurriculosImportExcelBean> sheetContent)
	{
		// [ CodCurriculo -> Curriculo ]
		Map<String, Curriculo> curriculosBDMap = Curriculo.buildCurriculoCodigoToCurriculoMap(Curriculo.findByCenario(this.instituicaoEnsino, getCenario()));
		// [ CodCurriculo -> CurriculosImportExcelBean ]
		Map<String, CurriculosImportExcelBean> curriculosExcelMap = CurriculosImportExcelBean.buildCurriculoCodigoToImportExcelBeanMap(sheetContent);

		Set<Curriculo> curriculosParaAtualizar = new HashSet<Curriculo>();
		Set<Curriculo> curriculosParaInserir = new HashSet<Curriculo>();
		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);
		for (String codigoCurriculo : curriculosExcelMap.keySet())
		{
			Curriculo curriculoBD = curriculosBDMap.get(codigoCurriculo);
			CurriculosImportExcelBean curriculoExcel = curriculosExcelMap.get(codigoCurriculo);
			if (curriculoBD != null)
			{
				// Update
				curriculoBD.setDescricao(curriculoExcel.getDescricaoStr());
				curriculoBD.setCurso(curriculoExcel.getCurso());
				curriculosParaAtualizar.add(curriculoBD);
			}
			else
			{
				// Insert
				Curriculo newCurriculo = new Curriculo();
				newCurriculo.setCenario(getCenario());
				newCurriculo.setCodigo(curriculoExcel.getCodigoStr());
				newCurriculo.setDescricao(curriculoExcel.getDescricaoStr());
				newCurriculo.setCurso(curriculoExcel.getCurso());
				newCurriculo.setSemanaLetiva(curriculoExcel.getSemanaLetiva());
				curriculosParaInserir.add(newCurriculo);
				curriculosBDMap.put(newCurriculo.getCodigo(), newCurriculo);
			}
			count++;
			total--;
			if (count == 100)
			{
				System.out.println("   Faltam " + total + " curriculos");
				count = 0;
			}
		}

		// ATUALIZA CURRICULOS-DISCIPLINAS
		// Codigo referente á issue http://jira.gapso.com.br/browse/TRIEDA-791
		// [ CodCurso - CodCurriculo - Periodo - CodDisciplina -> CurriculoDisciplina ]
		Set<CurriculoDisciplina> curriculosDiscParaInserir = new HashSet<CurriculoDisciplina>();
		Map<String, CurriculoDisciplina> curriculosDisciplinasBDMap = CurriculoDisciplina.buildNaturalKeyToCurriculoDisciplinaMap(CurriculoDisciplina.findByCenario(this.instituicaoEnsino,
						getCenario()));
		count = 0;
		total = sheetContent.size();
		System.out.println("CurriculoDisciplina " + total);
		for (CurriculosImportExcelBean curriculoExcel : sheetContent)
		{
			CurriculoDisciplina curriculoDisciplinaBD = curriculosDisciplinasBDMap.get(curriculoExcel.getNaturalKeyString());
			if (curriculoDisciplinaBD == null)
			{
				// Insert
				CurriculoDisciplina newCurriculoDisciplina = new CurriculoDisciplina();
				newCurriculoDisciplina.setPeriodo(curriculoExcel.getPeriodo());
				newCurriculoDisciplina.setDisciplina(curriculoExcel.getDisciplina());
				newCurriculoDisciplina.setCurriculo(curriculosBDMap.get(curriculoExcel.getCodigoStr()));
				if (newCurriculoDisciplina.getPeriodo() != null && newCurriculoDisciplina.getDisciplina() != null && newCurriculoDisciplina.getCurriculo() != null)
				{
					curriculosDiscParaInserir.add(newCurriculoDisciplina);
				}
			}
			count++;
			total--;
			if (count == 100)
			{
				System.out.println("   Faltam " + total + " CurriculosDisciplina");
				count = 0;
			}
		}

		curriculoDomainService.updateDataBaseFromExcel(curriculosParaAtualizar, curriculosParaInserir, curriculosDiscParaInserir, updateDisciplinaHorario, cenario);
	}

	private void resolveHeaderColumnNames()
	{
		if (CODIGO_COLUMN_NAME == null)
		{
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCurso());
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCurriculo());
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().descricaoCurriculo());
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().periodo());
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().disciplina());
			SEMANA_LETIVA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().semanaLetiva());
		}
	}
}
