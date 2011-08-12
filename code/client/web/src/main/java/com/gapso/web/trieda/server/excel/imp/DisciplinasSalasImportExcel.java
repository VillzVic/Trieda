package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class DisciplinasSalasImportExcel
	extends AbstractImportExcel< DisciplinasSalasImportExcelBean >
{
	static public String SALA_COLUMN_NAME;
	static public String CURSO_COLUMN_NAME;
	static public String MATRIZ_CURRICULAR_COLUMN_NAME;
	static public String PERIODO_COLUMN_NAME;
	static public String DISCIPLINA_COLUMN_NAME;

	private List< String > headerColumnsNames;
	
	public DisciplinasSalasImportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( SALA_COLUMN_NAME );
		this.headerColumnsNames.add( CURSO_COLUMN_NAME );
		this.headerColumnsNames.add( MATRIZ_CURRICULAR_COLUMN_NAME );
		this.headerColumnsNames.add( PERIODO_COLUMN_NAME );
		this.headerColumnsNames.add( DISCIPLINA_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.DISCIPLINAS_SALAS.getSheetName().equals(sheetName);
	}
	
	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected DisciplinasSalasImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		DisciplinasSalasImportExcelBean bean = new DisciplinasSalasImportExcelBean(row.getRowNum()+1);
        for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
            HSSFCell cell = row.getCell(cellIndex);        	
        	if (cell != null) {
        		HSSFCell headerCell = header.getCell(cell.getColumnIndex());
        		if(headerCell != null){
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);
					if (SALA_COLUMN_NAME.equals(columnName)) {
						bean.setSalaStr(cellValue);
					} else if (CURSO_COLUMN_NAME.equals(columnName)) {
						bean.setCursoStr(cellValue);
					} else if (MATRIZ_CURRICULAR_COLUMN_NAME.equals(columnName)) {
						bean.setMatrizCurricularStr(cellValue);
					} else if (PERIODO_COLUMN_NAME.equals(columnName)) {
						bean.setPeriodoStr(cellValue);
					} else if (DISCIPLINA_COLUMN_NAME.equals(columnName)) {
						bean.setDisciplinaStr(cellValue);
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
		return ExcelInformationType.DISCIPLINAS_SALAS.getSheetName();
	}
	
	@Override
	protected void processSheetContent(String sheetName, List<DisciplinasSalasImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent) && doLogicValidation(sheetName,sheetContent)) {
			updateDataBase(sheetName,sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<DisciplinasSalasImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (DisciplinasSalasImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<DisciplinasSalasImportExcelBean> sheetContent) {
		checkNonRegisteredSala(sheetContent);
		checkNonRegisteredCurso(sheetContent);
		checkNonRegisteredDisciplina(sheetContent);
		checkNonRegisteredCurriculo(sheetContent);
		checkNonRegisteredDisciplinaEmCurricular(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkNonRegisteredSala(List<DisciplinasSalasImportExcelBean> sheetContent) {
		// [CodidoSala -> Sala]
		Map<String,Sala> salasBDMap = Sala.buildSalaCodigoToSalaMap(Sala.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisciplinasSalasImportExcelBean bean : sheetContent) {
			Sala sala = salasBDMap.get(bean.getSalaStr());
			if (sala != null) {
				bean.setSala(sala);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(SALA_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	private void checkNonRegisteredCurso(List<DisciplinasSalasImportExcelBean> sheetContent) {
		// [CodigoCurso -> Curso]
		Map<String,Curso> cursosBDMap = Curso.buildCursoCodigoToCursoMap(Curso.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisciplinasSalasImportExcelBean bean : sheetContent) {
			Curso curso = cursosBDMap.get(bean.getCursoStr());
			if (curso != null) {
				bean.setCurso(curso);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CURSO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	private void checkNonRegisteredDisciplina(List<DisciplinasSalasImportExcelBean> sheetContent) {
		// [CodigoDisciplina -> Disciplina]
		Map<String,Disciplina> disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(Disciplina.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisciplinasSalasImportExcelBean bean : sheetContent) {
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
	private void checkNonRegisteredCurriculo(List<DisciplinasSalasImportExcelBean> sheetContent) {
		// [CodigoCurriculo -> Curriculo]
		Map<String,Curriculo> curriculosBDMap = Curriculo.buildCurriculoCodigoToCurriculoMap(Curriculo.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (DisciplinasSalasImportExcelBean bean : sheetContent) {
			Curriculo curriculo = curriculosBDMap.get(bean.getMatrizCurricularStr());
			if (curriculo != null) {
				bean.setMatrizCurricular(curriculo);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(MATRIZ_CURRICULAR_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	private void checkNonRegisteredDisciplinaEmCurricular(
		List< DisciplinasSalasImportExcelBean > sheetContent )
	{
		// [ ChaveNaturalCurriculo -> Curriculo ]
		Map<String, CurriculoDisciplina> curriculosDisciplinasBDMap
			= CurriculoDisciplina.buildNaturalKeyToCurriculoDisciplinaMap(
				CurriculoDisciplina.findByCenario( getCenario() ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();
		for ( DisciplinasSalasImportExcelBean bean : sheetContent )
		{
			CurriculoDisciplina curriculoDisciplina = curriculosDisciplinasBDMap.get(
				getNaturalKeyStringDeCurriculoDisciplina( bean ) );

			if ( curriculoDisciplina == null )
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoDisciplinaEmMatrizCurricular(
				DISCIPLINA_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase( String sheetName, List< DisciplinasSalasImportExcelBean > sheetContent )
	{
		Map< String,Sala > salasBDMap = Sala.buildSalaCodigoToSalaMap(
			Sala.findByCenario( getCenario() ) );

		Map< String, CurriculoDisciplina > curriculoDisciplinaBDMap
			= CurriculoDisciplina.buildNaturalKeyToCurriculoDisciplinaMap(
				CurriculoDisciplina.findByCenario( getCenario() ) );

		for ( DisciplinasSalasImportExcelBean disciplinasSalasExcel : sheetContent )
		{
			Sala salaBD = salasBDMap.get( disciplinasSalasExcel.getSalaStr() );
			CurriculoDisciplina curriculoDisciplinaBD = curriculoDisciplinaBDMap.get(
				getNaturalKeyStringDeCurriculoDisciplina( disciplinasSalasExcel ) );

			curriculoDisciplinaBD.getSalas().add( salaBD );
		}

		for ( CurriculoDisciplina curriculoDisciplina : curriculoDisciplinaBDMap.values() )
		{
			curriculoDisciplina.merge();
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( SALA_COLUMN_NAME == null )
		{
			SALA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoSala() );
			CURSO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCurso() );
			MATRIZ_CURRICULAR_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoMatrizCurricular() );
			PERIODO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().periodo() );
			DISCIPLINA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
		}
	}

	private String getNaturalKeyStringDeCurriculoDisciplina(
		DisciplinasSalasImportExcelBean bean )
	{
		return bean.getCursoStr()
			+ "-" + bean.getMatrizCurricularStr()
			+ "-" + bean.getPeriodo()
			+ "-" + bean.getDisciplinaStr();
	}
}