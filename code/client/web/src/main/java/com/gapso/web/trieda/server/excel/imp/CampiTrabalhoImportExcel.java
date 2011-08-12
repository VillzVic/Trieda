package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CampiTrabalhoImportExcel extends AbstractImportExcel<CampiTrabalhoImportExcelBean> {
	
	static public String CAMPUS_COLUMN_NAME;
	static public String CPF_COLUMN_NAME;
	static public String PROFESSOR_COLUMN_NAME;
	
	private List<String> headerColumnsNames;
	
	public CampiTrabalhoImportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
{
		super( cenario, i18nConstants, i18nMessages );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CAMPUS_COLUMN_NAME );
		this.headerColumnsNames.add( CPF_COLUMN_NAME );
		this.headerColumnsNames.add( PROFESSOR_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.CAMPI_TRABALHO.getSheetName().equals(sheetName);
	}
	
	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected CampiTrabalhoImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		CampiTrabalhoImportExcelBean bean = new CampiTrabalhoImportExcelBean(row.getRowNum()+1);
        for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
            HSSFCell cell = row.getCell(cellIndex);        	
        	if (cell != null) {
        		HSSFCell headerCell = header.getCell(cell.getColumnIndex());
        		if(headerCell != null) {
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);
					if (CAMPUS_COLUMN_NAME.equals(columnName)) {
						bean.setCampusStr(cellValue);
					} else if (CPF_COLUMN_NAME.equals(columnName)) {
						bean.setCpfStr(cellValue);
					} else if (PROFESSOR_COLUMN_NAME.equals(columnName)) {
						bean.setProfessorStr(cellValue);
					}
        		}
        	}
        }
		return bean;
	}

	@Override
	protected String getHeaderToString() {
		return this.headerColumnsNames.toString();
	}

	@Override
	public String getSheetName() {
		return ExcelInformationType.CAMPI_TRABALHO.getSheetName();
	}
	
	@Override
	protected void processSheetContent(String sheetName, List<CampiTrabalhoImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent) && doLogicValidation(sheetName,sheetContent)) {
			updateDataBase(sheetName,sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<CampiTrabalhoImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (CampiTrabalhoImportExcelBean bean : sheetContent) {
			List<ImportExcelError> errorsBean = bean.checkSyntacticErrors();
			for (ImportExcelError error : errorsBean) {
				List<Integer> rowsWithErrors = syntacticErrorsMap.get(error);
				if (rowsWithErrors == null) {
					rowsWithErrors = new ArrayList<Integer>();
					syntacticErrorsMap.put(error,rowsWithErrors);
				}
				rowsWithErrors.add(bean.getRow());
			}
		}
		
		// coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet()) {
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);
			getErrors().add(error.getMessage(linhasComErro.toString(),getI18nMessages()));
		}
		
		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(String sheetName, List<CampiTrabalhoImportExcelBean> sheetContent) {
		// verifica se há referência a algum tipo de contrato não cadastrado
		checkNonRegisteredCampus(sheetContent);
		checkNonRegisteredProfessor(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkNonRegisteredCampus(List<CampiTrabalhoImportExcelBean> sheetContent) {
		// [CodidoCampus -> Campus]
		Map<String,Campus> campiBDMap = Campus.buildCampusCodigoToCampusMap(Campus.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (CampiTrabalhoImportExcelBean bean : sheetContent) {
			Campus campus = campiBDMap.get(bean.getCampusStr());
			if (campus != null) {
				bean.setCampus(campus);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CAMPUS_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	private void checkNonRegisteredProfessor(List<CampiTrabalhoImportExcelBean> sheetContent) {
		// [CodidoCampus -> Campus]
		Map<String, Professor> professoresBDMap = Professor.buildProfessorCpfToProfessorMap(Professor.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (CampiTrabalhoImportExcelBean bean : sheetContent) {
			Professor professor = professoresBDMap.get(bean.getCpfStr());
			if (professor != null) {
				bean.setProfessor(professor);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(PROFESSOR_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	@Transactional
	private void updateDataBase(String sheetName, List<CampiTrabalhoImportExcelBean> sheetContent) {
		Set<Professor> professorMerge = new HashSet<Professor>();
		
		for (CampiTrabalhoImportExcelBean campiTrabalhoExcel : sheetContent) {
			professorMerge.add(campiTrabalhoExcel.getProfessor());
			campiTrabalhoExcel.getProfessor().getCampi().add(campiTrabalhoExcel.getCampus());
		}
		for(Professor professor : professorMerge) {
			professor.merge();
		}
		
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( CAMPUS_COLUMN_NAME == null )
		{
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCampus() );
			CPF_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().cpf() );
			PROFESSOR_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().professor() );
		}
	}
}
