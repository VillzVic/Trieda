package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.dev.util.collect.HashSet;

@ProgressDeclarationAnnotation
public class DisciplinasSalasImportExcel extends AbstractImportExcel<DisciplinasSalasImportExcelBean>
{
	static public String SALA_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public DisciplinasSalasImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(SALA_COLUMN_NAME);
		this.headerColumnsNames.add(DISCIPLINA_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected DisciplinasSalasImportExcelBean createExcelBean(Row header, Row row)
	{
		DisciplinasSalasImportExcelBean bean = new DisciplinasSalasImportExcelBean(row.getRowNum() + 1);

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

					if (SALA_COLUMN_NAME.equals(columnName))
					{
						bean.setSalaStr(cellValue);
					}
					else if (DISCIPLINA_COLUMN_NAME.equals(columnName))
					{
						bean.setDisciplinaStr(cellValue);
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
		return ExcelInformationType.DISCIPLINAS_SALAS.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<DisciplinasSalasImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (DisciplinasSalasImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<DisciplinasSalasImportExcelBean> sheetContent)
	{
		checkNonRegisteredSala(sheetContent);
		checkNonRegisteredDisciplina(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkNonRegisteredSala(List<DisciplinasSalasImportExcelBean> sheetContent)
	{
		// [ CodidoSala -> Sala ]
		Map<String, Sala> salasBDMap = Sala.buildSalaCodigoToSalaMap(Sala.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (DisciplinasSalasImportExcelBean bean : sheetContent)
		{
			Sala sala = salasBDMap.get(bean.getSalaStr());

			if (sala != null)
			{
				bean.setSala(sala);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(SALA_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredDisciplina(List<DisciplinasSalasImportExcelBean> sheetContent)
	{
		// [ CodigoDisciplina -> Disciplina ]
		Map<String, Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (DisciplinasSalasImportExcelBean bean : sheetContent)
		{
			Disciplina disciplina = disciplinasBDMap.get(bean.getDisciplinaStr());

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

	@Transactional
	@Override
	protected void updateDataBase(List<DisciplinasSalasImportExcelBean> sheetContent)
	{
		Map<String, Sala> salasBDMap = Sala.buildSalaCodigoToSalaMap(Sala.findByCenario(this.instituicaoEnsino, getCenario()));
		Map<String, Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(instituicaoEnsino, getCenario()));

		Set<CurriculoDisciplina> curriculoDisciplinaAlterados = new HashSet<CurriculoDisciplina>();
		int count = 0, total = sheetContent.size();
		System.out.print(" " + total);
		for (DisciplinasSalasImportExcelBean bean : sheetContent)
		{
			Sala salaBD = salasBDMap.get(bean.getSalaStr());
			Disciplina disciplinaBD = disciplinasBDMap.get(bean.getDisciplinaStr());

			disciplinaBD.getSalas().add(salaBD);

			count++;
			total--;
			if (count == 100)
			{
				System.out.println("   Faltam " + total + " DisciplinasSalasBeans");
				count = 0;
			}
		}

		count = 0;
		total = curriculoDisciplinaAlterados.size();
		System.out.println(" " + total);
		for (CurriculoDisciplina curriculoDisciplina : curriculoDisciplinaAlterados)
		{
			curriculoDisciplina.mergeWithoutFlush();
			count++;
			total--;
			if (count == 100)
			{
				System.out.println("   Faltam " + total + " CurriculoDisciplina");
				count = 0;
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if (SALA_COLUMN_NAME == null)
		{
			SALA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoSala());
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoDisciplina());
		}
	}
}
