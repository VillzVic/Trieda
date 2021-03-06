package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

@ProgressDeclarationAnnotation
public class CampiTrabalhoImportExcel extends AbstractImportExcel<CampiTrabalhoImportExcelBean>
{
	static public String CAMPUS_COLUMN_NAME;
	static public String CPF_COLUMN_NAME;
	static public String PROFESSOR_COLUMN_NAME;

	private List<String> headerColumnsNames;

	public CampiTrabalhoImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino)
	{
		super(cenario, i18nConstants, i18nMessages, instituicaoEnsino);
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CAMPUS_COLUMN_NAME);
		this.headerColumnsNames.add(CPF_COLUMN_NAME);
		this.headerColumnsNames.add(PROFESSOR_COLUMN_NAME);
	}

	@Override
	protected List<String> getHeaderColumnsNames()
	{
		return this.headerColumnsNames;
	}

	@Override
	protected CampiTrabalhoImportExcelBean createExcelBean(Row header, Row row)
	{
		CampiTrabalhoImportExcelBean bean = new CampiTrabalhoImportExcelBean(row.getRowNum() + 1);

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

					if (CAMPUS_COLUMN_NAME.equals(columnName))
					{
						bean.setCampusStr(cellValue);
					}
					else if (CPF_COLUMN_NAME.equals(columnName))
					{
						cell.setCellType(Cell.CELL_TYPE_STRING);

						cellValue = TriedaUtil.formatStringCPF(cell.getRichStringCellValue().getString().trim());

						bean.setCpfStr(cellValue);
					}
					else if (PROFESSOR_COLUMN_NAME.equals(columnName))
					{
						bean.setProfessorStr(cellValue);
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
		return ExcelInformationType.CAMPI_TRABALHO.getSheetName();
	}

	@Override
	protected boolean doSyntacticValidation(List<CampiTrabalhoImportExcelBean> sheetContent)
	{
		// Map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError, List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError, List<Integer>>();

		for (CampiTrabalhoImportExcelBean bean : sheetContent)
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
	protected boolean doLogicValidation(List<CampiTrabalhoImportExcelBean> sheetContent)
	{
		// Verifica se há referência a algum tipo de contrato não cadastrado
		checkNonRegisteredCampus(sheetContent);
		checkNonRegisteredProfessor(sheetContent);

		return getErrors().isEmpty();
	}

	private void checkNonRegisteredCampus(List<CampiTrabalhoImportExcelBean> sheetContent)
	{
		// [ CódidoCampus -> Campus ]
		Map<String, Campus> campiBDMap = Campus.buildCampusCodigoToCampusMap(Campus.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (CampiTrabalhoImportExcelBean bean : sheetContent)
		{
			Campus campus = campiBDMap.get(bean.getCampusStr());
			if (campus != null)
			{
				bean.setCampus(campus);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CAMPUS_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredProfessor(List<CampiTrabalhoImportExcelBean> sheetContent)
	{
		// [ CodidoCampus -> Campus ]
		Map<String, Professor> professoresBDMap = Professor.buildProfessorCpfToProfessorMap(Professor.findByCenario(this.instituicaoEnsino, getCenario()));

		List<Integer> rowsWithErrors = new ArrayList<Integer>();

		for (CampiTrabalhoImportExcelBean bean : sheetContent)
		{
			Professor professor = professoresBDMap.get(bean.getCpfStr());

			if (professor != null)
			{
				bean.setProfessor(professor);
			}
			else
			{
				rowsWithErrors.add(bean.getRow());
			}
		}

		if (!rowsWithErrors.isEmpty())
		{
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(PROFESSOR_COLUMN_NAME, rowsWithErrors.toString()));
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	@Override
	protected void updateDataBase(List<CampiTrabalhoImportExcelBean> sheetContent)
	{
		Set<Professor> professorMerge = new HashSet<Professor>();

		for (CampiTrabalhoImportExcelBean campiTrabalhoExcel : sheetContent)
		{
			professorMerge.add(campiTrabalhoExcel.getProfessor());
			campiTrabalhoExcel.getProfessor().getCampi().add(campiTrabalhoExcel.getCampus());
		}

		int count = 0, total = professorMerge.size();
		System.out.print(" " + total);
		for (Professor professor : professorMerge)
		{
			professor.mergeWithoutFlush();
			count++;
			total--;
			if (count == 100)
			{
				System.out.println("   Faltam " + total + " professores");
				count = 0;
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if (CAMPUS_COLUMN_NAME == null)
		{
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigoCampus());
			CPF_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cpfProfessor());
			PROFESSOR_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().professor());
		}
	}
}
