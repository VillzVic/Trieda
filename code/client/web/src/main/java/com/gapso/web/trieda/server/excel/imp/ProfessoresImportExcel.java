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

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class ProfessoresImportExcel extends AbstractImportExcel<ProfessoresImportExcelBean> {
	
	static public String CPF_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String TIPO_COLUMN_NAME;
	static public String CARGA_HORARIA_MAX_COLUMN_NAME;
	static public String CARGA_HORARIA_MIN_COLUMN_NAME;
	static public String TITULACAO_COLUMN_NAME;
	static public String AREA_TITULACAO_COLUMN_NAME;
	static public String NOTA_DESEMPENHO_COLUMN_NAME;
	static public String CARGA_HORARIA_ANTERIOR_COLUMN_NAME;
	static public String VALOR_CREDITO_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public ProfessoresImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CPF_COLUMN_NAME );
		this.headerColumnsNames.add( NOME_COLUMN_NAME );
		this.headerColumnsNames.add( TIPO_COLUMN_NAME );
		this.headerColumnsNames.add( CARGA_HORARIA_MAX_COLUMN_NAME );
		this.headerColumnsNames.add( CARGA_HORARIA_MIN_COLUMN_NAME );
		this.headerColumnsNames.add( TITULACAO_COLUMN_NAME );
		this.headerColumnsNames.add( AREA_TITULACAO_COLUMN_NAME );
		this.headerColumnsNames.add( NOTA_DESEMPENHO_COLUMN_NAME );
		this.headerColumnsNames.add( CARGA_HORARIA_ANTERIOR_COLUMN_NAME );
		this.headerColumnsNames.add( VALOR_CREDITO_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.PROFESSORES.getSheetName().equals( sheetName );
	}

	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected ProfessoresImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		ProfessoresImportExcelBean bean = new ProfessoresImportExcelBean(row.getRowNum()+1);
        for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
            HSSFCell cell = row.getCell(cellIndex);        	
        	if (cell != null) {
        		HSSFCell headerCell = header.getCell(cell.getColumnIndex());
        		if(headerCell != null){
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);
					if (CPF_COLUMN_NAME.equals(columnName)) {
						bean.setCpfStr(cellValue);
					} else if (NOME_COLUMN_NAME.equals(columnName)) {
						bean.setNomeStr(cellValue);
					} else if (TIPO_COLUMN_NAME.equals(columnName)) {
						bean.setTipoStr(cellValue);
					} else if (CARGA_HORARIA_MAX_COLUMN_NAME.equals(columnName)) {
						bean.setCargaHorariaMaxStr(cellValue);
					} else if (CARGA_HORARIA_MIN_COLUMN_NAME.equals(columnName)) {
						bean.setCargaHorariaMinStr(cellValue);
					} else if (TITULACAO_COLUMN_NAME.equals(columnName)) {
						bean.setTitulacaoStr(cellValue);
					} else if (AREA_TITULACAO_COLUMN_NAME.equals(columnName)) {
						bean.setAreaTitulacaoStr(cellValue);
					} else if (CARGA_HORARIA_ANTERIOR_COLUMN_NAME.equals(columnName)) {
						bean.setCargaHorariaAnteriorStr(cellValue);
					} else if (VALOR_CREDITO_COLUMN_NAME.equals(columnName)) {
						bean.setValorCreditoStr(cellValue);
					}
        		}
        	}
        }
		return bean;
	}

	@Override
	public String getSheetName() {
		return ExcelInformationType.PROFESSORES.getSheetName();
	}
	
	@Override
	protected String getHeaderToString() {
		return this.headerColumnsNames.toString();
	}

	@Override
	protected void processSheetContent(String sheetName, List<ProfessoresImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent) && doLogicValidation(sheetName,sheetContent)) {
			updateDataBase(sheetName,sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<ProfessoresImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (ProfessoresImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<ProfessoresImportExcelBean> sheetContent) {
		// verifica se algum professor apareceu mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);
		// verifica se há referência a algum tipo de contrato não cadastrado
		checkNonRegisteredTipoContrato(sheetContent);
		checkNonRegisteredTitulacao(sheetContent);
		checkNonRegisteredAreaTitulacao(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<ProfessoresImportExcelBean> sheetContent) {
		// map com os códigos dos professores e as linhas em que o mesmo aparece no arquivo de entrada
		// [CpfProfessor -> Lista de Linhas do Arquivo de Entrada]
		Map<String,List<Integer>> professorCpfToRowsMap = new HashMap<String,List<Integer>>();
		 
		for (ProfessoresImportExcelBean bean : sheetContent) {
			List<Integer> rows = professorCpfToRowsMap.get(bean.getCpfStr());
			if (rows == null) {
				rows = new ArrayList<Integer>();
				professorCpfToRowsMap.put(bean.getCpfStr(),rows);
			}
			rows.add(bean.getRow());
		}
		
		// verifica se algum professor apareceu mais de uma vez no arquivo de entrada
		for (Entry<String,List<Integer>> entry : professorCpfToRowsMap.entrySet()) {
			if (entry.getValue().size() > 1) {
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(),entry.getValue().toString()));
			}
		}
	}
	
	private void checkNonRegisteredTipoContrato(List<ProfessoresImportExcelBean> sheetContent) {
		// [NomeTipoContrato -> TipoContrato]
		Map<String,TipoContrato> tiposContratoBDMap = TipoContrato.buildTipoContratoNomeToTipoContratoMap(TipoContrato.findAll());
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (ProfessoresImportExcelBean bean : sheetContent) {
			TipoContrato tipoContrato = tiposContratoBDMap.get(bean.getTipoStr());
			if (tipoContrato != null) {
				bean.setTipo(tipoContrato);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TIPO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	private void checkNonRegisteredTitulacao(List<ProfessoresImportExcelBean> sheetContent) {
		// [NomeTitulacao -> Titulacao]
		Map<String,Titulacao> titulacoesBDMap = Titulacao.buildTitulacaoNomeToTitulacaoMap(Titulacao.findAll());
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (ProfessoresImportExcelBean bean : sheetContent) {
			Titulacao titulacao = titulacoesBDMap.get(bean.getTitulacaoStr());
			if (titulacao != null) {
				bean.setTitulacao(titulacao);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(TITULACAO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}
	private void checkNonRegisteredAreaTitulacao(List<ProfessoresImportExcelBean> sheetContent) {
		// [CodigoAreaTitulacao -> AeraTitulacao]
		Map<String,AreaTitulacao> areasTitulacoesBDMap = AreaTitulacao.buildAreaTitulacaoCodigoToAreaTitulacaoMap(AreaTitulacao.findAll());
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (ProfessoresImportExcelBean bean : sheetContent) {
			AreaTitulacao areaTitulacao = areasTitulacoesBDMap.get(bean.getAreaTitulacaoStr());
			if (areaTitulacao != null) {
				bean.setAreaTitulacao(areaTitulacao);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(AREA_TITULACAO_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	@Transactional
	private void updateDataBase( String sheetName,
		List< ProfessoresImportExcelBean > sheetContent )
	{
		Map< String, Professor > professoresBDMap = Professor.buildProfessorCpfToProfessorMap(
			Professor.findByCenario( getCenario() ) );

		for ( ProfessoresImportExcelBean professorExcel : sheetContent )
		{
			Professor professorBD = professoresBDMap.get( professorExcel.getCpfStr() );

			if ( professorBD != null )
			{
				// Update
				professorBD.setNome( professorExcel.getNomeStr() );
				professorBD.setTipoContrato( professorExcel.getTipo() );
				professorBD.setCargaHorariaMax( professorExcel.getCargaHorariaMax() );
				professorBD.setCargaHorariaMin( professorExcel.getCargaHorariaMin() );
				professorBD.setTitulacao( professorExcel.getTitulacao() );
				professorBD.setAreaTitulacao( professorExcel.getAreaTitulacao() );
				professorBD.setCreditoAnterior( professorExcel.getCargaHorariaAnterior() );
				professorBD.setValorCredito( professorExcel.getValorCredito() );

				professorBD.merge();
			}
			else
			{
				// Insert
				Professor newProfessor = new Professor();

				newProfessor.setCenario( getCenario() );
				newProfessor.setCpf( professorExcel.getCpfStr() );
				newProfessor.setNome( professorExcel.getNomeStr() );
				newProfessor.setTipoContrato( professorExcel.getTipo() );
				newProfessor.setCargaHorariaMax( professorExcel.getCargaHorariaMax() );
				newProfessor.setCargaHorariaMin( professorExcel.getCargaHorariaMin() );
				newProfessor.setTitulacao( professorExcel.getTitulacao() );
				newProfessor.setAreaTitulacao( professorExcel.getAreaTitulacao() );
				newProfessor.setCreditoAnterior( professorExcel.getCargaHorariaAnterior() );
				newProfessor.setValorCredito( professorExcel.getValorCredito() );

				newProfessor.persist();
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CPF_COLUMN_NAME == null )
		{
			CPF_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().cpf() );
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nomeProfessor() );
			TIPO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().tipoContrato() );
			CARGA_HORARIA_MAX_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().cargaHorariaMax() );
			CARGA_HORARIA_MIN_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().cargaHorariaMin() );
			TITULACAO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().titulacao() );
			AREA_TITULACAO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().areaTitulacao() );
			NOTA_DESEMPENHO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().notaDesempenho() );
			CARGA_HORARIA_ANTERIOR_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().cargaHorariaAnterior() );
			VALOR_CREDITO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().valorCredito() );	
		}
	}
}
