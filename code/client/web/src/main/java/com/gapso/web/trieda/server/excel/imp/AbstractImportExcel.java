package com.gapso.web.trieda.server.excel.imp;

import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationImpl;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public abstract class AbstractImportExcel<ExcelBeanType> extends ProgressDeclarationImpl implements IImportExcel
{
	protected List<String> errors;
	protected List<String> warnings;

	protected Cenario cenario;
	protected InstituicaoEnsino instituicaoEnsino;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;

	protected AbstractImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		this.instituicaoEnsino = instituicaoEnsino;
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;

		this.errors = new ArrayList<String>();
		this.warnings = new ArrayList<String>();
	}

	protected abstract List<String> getHeaderColumnsNames();
	protected abstract ExcelBeanType createExcelBean(Row header, Row row);
	protected abstract String getHeaderToString();
	
	protected abstract boolean doSyntacticValidation(List<ExcelBeanType> sheetContent);
	protected abstract boolean doLogicValidation(List<ExcelBeanType> sheetContent);
	protected abstract void updateDataBase(List<ExcelBeanType> sheetContent);

	// @Transactional
	@Override
	public boolean load(String fileName, Workbook workbook, boolean obrigatorio)
	{
		errors.clear();
		warnings.clear();

		List<ExcelBeanType> excelBeansList = readWorkbook(fileName, workbook, obrigatorio);

		if (errors.isEmpty())
			try
			{
				processSheetContent(excelBeansList);
			}
			catch (Exception e)
			{
				e.printStackTrace();
				errors.add(getI18nMessages().excelErroBD(fileName, TriedaUtil.extractMessage(e)));
			}

		return errors.isEmpty();
	}

	@Override
	public boolean load(String fileName, InputStream inputStream)
	{
		try
		{
			Workbook workbook = WorkbookFactory.create(inputStream);
			return load(fileName, workbook, true);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			this.errors.add(getI18nMessages().excelErroArquivoInvalido(fileName, TriedaUtil.extractMessage(e)));
			return false;
		}
	}
		
	@Override
	public boolean load(String fileName, File file)
	{
		try
		{
			Workbook workbook = WorkbookFactory.create(file);
			return load(fileName, workbook, true);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			this.errors.add(getI18nMessages().excelErroArquivoInvalido(fileName, TriedaUtil.extractMessage(e)));
			return false;
		}
	}

	@Override
	public List<String> getErrors()
	{
		return this.errors;
	}

	@Override
	public List<String> getWarnings()
	{
		return this.warnings;
	}

	protected List<ExcelBeanType> readWorkbook(String fileName, Workbook workbook, boolean obrigatorio)
	{
		List<ExcelBeanType> excelBeansList = new ArrayList<ExcelBeanType>();
		Sheet sheet = workbook.getSheet(getSheetName());
		
		if (sheet == null)
		{
			if (obrigatorio) errors.add("Não foi encontrada a aba " + getSheetName() + " para a importação");
			return excelBeansList;
		}
		
		// Procura cabeçalho
		List<String> headerColumnsNames = getHeaderColumnsNames();

		int rowIndex = sheet.getFirstRowNum();
		Row header = sheet.getRow(rowIndex++);

		boolean validHeader = isHeaderValid(header, headerColumnsNames);

		while ((rowIndex < sheet.getLastRowNum()) && !validHeader)
		{
			header = sheet.getRow(rowIndex++);

			validHeader = isHeaderValid(header, headerColumnsNames);
		}

		if (validHeader)
		{
			List<Integer> nullRows = new ArrayList<Integer>();

			// Efetua a leitura dos dados do arquivo
			for (; rowIndex <= sheet.getLastRowNum(); rowIndex++)
			{
				Row row = sheet.getRow(rowIndex);

				if (row != null)
				{
					excelBeansList.add(createExcelBean(header, row));
				}
				else
				{
					nullRows.add(rowIndex);
				}
			}

			// Verifica se existem linhas nulas
			if (!nullRows.isEmpty())
			{
				this.errors.add(getI18nMessages().excelErroSintaticoLinhasInvalidas(nullRows.toString(), fileName));
			}
		}
		else
		{
			this.errors.add(getI18nMessages().excelErroSintaticoCabecalhoAusente(getHeaderToString(), fileName));
		}

		return excelBeansList;
	}

	protected boolean isHeaderValid(Row candidateHeader, List<String> headerColumnsNames)
	{
		if (candidateHeader != null)
		{
			boolean[] columnStatus = new boolean[headerColumnsNames.size()];

			// Para cada coluna da linha a ser verificada
			for (int cellIndex = candidateHeader.getFirstCellNum(); cellIndex <= candidateHeader.getLastCellNum(); cellIndex++)
			{
				Cell cell = candidateHeader.getCell(cellIndex);

				if (cell != null && cell.getCellType() == Cell.CELL_TYPE_STRING)
				{
					String columnName = cell.getRichStringCellValue().getString();

					// Para cada coluna no header
					for (int headerColumnIndex = 0; headerColumnIndex < headerColumnsNames.size(); headerColumnIndex++)
					{
						String str1 = (headerColumnsNames.get(headerColumnIndex) == null ? "" : headerColumnsNames.get(headerColumnIndex).trim());
						String str2 = (columnName == null ? "" : columnName.trim());

						if (str1.equals(str2))
							columnStatus[headerColumnIndex] = true;
					}
				}
			}

			// Verifica se todas as colunas necessárias foram encontradas no header
			boolean test = true;
			for (int i = 0; i < columnStatus.length; i++)
				test = (test && columnStatus[i]);
			return test;
		}

		return false;
	}
	
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(List<ExcelBeanType> sheetContent)
	{
		if (doSyntacticValidation(sheetContent) && doLogicValidation(sheetContent))
		{
			ProgressReportWriter prw = this.getProgressReport();

			if (prw != null) {
				prw.setInitNewPartial("Atualizando banco de dados");
			}

			updateDataBase(sheetContent);

			if (prw != null) {
				prw.setPartial("Fim de Atualizando banco de dados");	
			}
		}
	}

	protected String getCellValue(Cell cell)
	{
		switch (cell.getCellType())
		{
		case Cell.CELL_TYPE_STRING: return getCellStringValue(cell);
		case Cell.CELL_TYPE_NUMERIC: return getCellNumericValue(cell);
		case Cell.CELL_TYPE_FORMULA: errors.add(getI18nMessages().excelErroSintaticoFormula(cell.getCellFormula(), Integer.toString(cell.getColumnIndex())));
		}

		return null;
	}

	protected String getCellStringValue(Cell cell)
	{
		return cell.getRichStringCellValue().getString().trim();
	}

	protected String getCellNumericValue(Cell cell)
	{
		if ((int) cell.getNumericCellValue() == cell.getNumericCellValue())
		{
			return Integer.toString((int) cell.getNumericCellValue());
		}
		return Double.toString(cell.getNumericCellValue());
	}

	protected Cenario getCenario()
	{
		return this.cenario;
	}

	protected TriedaI18nConstants getI18nConstants()
	{
		return this.i18nConstants;
	}

	protected TriedaI18nMessages getI18nMessages()
	{
		return this.i18nMessages;
	}

}
