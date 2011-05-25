package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class HabilitacoesProfessoresImportExcel extends AbstractImportExcel<HabilitacoesProfessoresImportExcelBean> {
	
	static public String CPF_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;
	static public String PREFERENCIA_COLUMN_NAME;
	static public String NOTA_COLUMN_NAME;
	
	private List<String> headerColumnsNames;
	
	public HabilitacoesProfessoresImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		resolveHeaderColumnNames();
		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CPF_COLUMN_NAME);
		this.headerColumnsNames.add(NOME_COLUMN_NAME);
		this.headerColumnsNames.add(DISCIPLINA_COLUMN_NAME);
		this.headerColumnsNames.add(PREFERENCIA_COLUMN_NAME);
		this.headerColumnsNames.add(NOTA_COLUMN_NAME);
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.HABILITACAO_PROFESSORES.getSheetName().equals(sheetName);
	}
	
	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected HabilitacoesProfessoresImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		HabilitacoesProfessoresImportExcelBean bean = new HabilitacoesProfessoresImportExcelBean(row.getRowNum()+1);
        for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
            HSSFCell cell = row.getCell(cellIndex);        	
        	if (cell != null) {
        		HSSFCell headerCell = header.getCell(cell.getColumnIndex());
        		if(headerCell != null){
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);
					if (CPF_COLUMN_NAME.equals(columnName)) {
						bean.setCpfStr(cellValue);
					} else if (NOME_COLUMN_NAME.endsWith(columnName)) {
						bean.setNomeStr(cellValue);
					} else if (DISCIPLINA_COLUMN_NAME.endsWith(columnName)) {
						bean.setDisciplinaStr(cellValue);
					} else if (PREFERENCIA_COLUMN_NAME.equals(columnName)) {
						bean.setPreferenciaStr(cellValue);
					} else if (NOTA_COLUMN_NAME.endsWith(columnName)) {
						bean.setNotaStr(cellValue);
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
		return ExcelInformationType.HABILITACAO_PROFESSORES.getSheetName();
	}
	
	@Override
	protected void processSheetContent(String sheetName, List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent) && doLogicValidation(sheetName,sheetContent)) {
			updateDataBase(sheetName,sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (HabilitacoesProfessoresImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		// verifica se algum Habilitação de processor apareceu mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);
		// verifica se há referência a algum tipo de curso não cadastrado
		checkNonRegisteredDisciplina(sheetContent);
		checkNonRegisteredProfessor(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		// map com as habilitação dos professores e as linhas em que o mesmo aparece no arquivo de entrada
		// [NaturalKeyStringCurso -> Lista de Linhas do Arquivo de Entrada]
		Map<String,List<Integer>> beansToRowsMap = new HashMap<String,List<Integer>>();
		 
		for (HabilitacoesProfessoresImportExcelBean bean : sheetContent) {
			List<Integer> rows = beansToRowsMap.get(bean.getNaturalKeyString());
			if (rows == null) {
				rows = new ArrayList<Integer>();
				beansToRowsMap.put(bean.getNaturalKeyString(),rows);
			}
			rows.add(bean.getRow());
		}
		
		// verifica se alguma habilitação dos professores apareceu mais de uma vez no arquivo de entrada
		for (Entry<String,List<Integer>> entry : beansToRowsMap.entrySet()) {
			if (entry.getValue().size() > 1) {
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(),entry.getValue().toString()));
			}
		}
	}
	
	private void checkNonRegisteredDisciplina(List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		// [CodigoDisciplina -> Disciplina]
		Map<String,Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (HabilitacoesProfessoresImportExcelBean bean : sheetContent) {
			Disciplina disciplina = disciplinasBDMap.get(bean.getDisciplinaStr());
			if (disciplina != null) {
				bean.setDisciplina(disciplina);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(DISCIPLINA_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	private void checkNonRegisteredProfessor(List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		// [CpfProfessor -> Professor]
		Map<String,Professor> professoresBDMap = Professor.buildProfessorCpfToProfessorMap(Professor.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (HabilitacoesProfessoresImportExcelBean bean : sheetContent) {
			Professor professor = professoresBDMap.get(bean.getCpfStr());
			if (professor != null) {
				bean.setProfessor(professor);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CPF_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	@Transactional
	private void updateDataBase(String sheetName, List<HabilitacoesProfessoresImportExcelBean> sheetContent) {
		Map<String,ProfessorDisciplina> professoresDisciplinasBDMap = ProfessorDisciplina.buildCursoNaturalKeyToProfessorDisciplinaMap(ProfessorDisciplina.findAll());
		
		for (HabilitacoesProfessoresImportExcelBean pdExcel : sheetContent) {
			ProfessorDisciplina pdBD = professoresDisciplinasBDMap.get(pdExcel.getNaturalKeyString());
			if (pdBD != null) {
				// update
				pdBD.setNota(pdExcel.getNota());
				pdBD.setPreferencia(pdExcel.getPreferencia());
				pdBD.merge();
			} else {
				// insert
				ProfessorDisciplina newPd = new ProfessorDisciplina();
				newPd.setDisciplina(pdExcel.getDisciplina());
				newPd.setProfessor(pdExcel.getProfessor());
				newPd.setNota(pdExcel.getNota());
				newPd.setPreferencia(pdExcel.getPreferencia());
				
				newPd.persist();
			}
		}
	}
	
	private void resolveHeaderColumnNames() {
		if (CPF_COLUMN_NAME == null) {
			CPF_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().cpf());
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().nome());
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().disciplina());
			PREFERENCIA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().preferencia());
			NOTA_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().notaDesempenho());
		}
	}
}