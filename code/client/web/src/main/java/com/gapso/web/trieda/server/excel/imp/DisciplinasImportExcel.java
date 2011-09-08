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
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class DisciplinasImportExcel
	extends AbstractImportExcel< DisciplinasImportExcelBean >
{
	static public String CODIGO_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String CRED_TEORICOS_COLUMN_NAME;
	static public String CRED_PRATICOS_COLUMN_NAME;
	static public String USA_LABORATORIO_COLUMN_NAME;
	static public String TIPO_COLUMN_NAME;
	static public String NIVEL_DIFICULDADE_COLUMN_NAME;
	static public String MAX_ALUNOS_TEORICOS_COLUMN_NAME;
	static public String MAX_ALUNOS_PRATICOS_COLUMN_NAME;
	
	private List<String> headerColumnsNames;
	private InstituicaoEnsino instituicaoEnsino;
	
	public DisciplinasImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );
		resolveHeaderColumnNames();

		this.instituicaoEnsino
			= cenario.getSemanaLetiva().getInstituicaoEnsino();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( NOME_COLUMN_NAME );
		this.headerColumnsNames.add( CRED_TEORICOS_COLUMN_NAME );
		this.headerColumnsNames.add( CRED_PRATICOS_COLUMN_NAME );
		this.headerColumnsNames.add( USA_LABORATORIO_COLUMN_NAME );
		this.headerColumnsNames.add( TIPO_COLUMN_NAME );
		this.headerColumnsNames.add( NIVEL_DIFICULDADE_COLUMN_NAME );
		this.headerColumnsNames.add( MAX_ALUNOS_TEORICOS_COLUMN_NAME );
		this.headerColumnsNames.add( MAX_ALUNOS_PRATICOS_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.DISCIPLINAS.getSheetName().equals(sheetName);
	}
	
	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected DisciplinasImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		DisciplinasImportExcelBean bean = new DisciplinasImportExcelBean(row.getRowNum()+1);
        for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
            HSSFCell cell = row.getCell(cellIndex);        	
        	if (cell != null) {
        		HSSFCell headerCell = header.getCell(cell.getColumnIndex());
        		if (headerCell != null) {
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);
					if (CODIGO_COLUMN_NAME.equals(columnName)) {
						bean.setCodigoStr(cellValue);
					} else if (NOME_COLUMN_NAME.endsWith(columnName)) {
						bean.setNomeStr(cellValue);
					} else if (CRED_TEORICOS_COLUMN_NAME.equals(columnName)) {
						bean.setCreditosTeoricosStr(cellValue);
					} else if (CRED_PRATICOS_COLUMN_NAME.endsWith(columnName)) {
						bean.setCreditosPraticosStr(cellValue);
					} else if (USA_LABORATORIO_COLUMN_NAME.endsWith(columnName)) {
						bean.setUsaLaboratorioStr(cellValue);
					} else if (TIPO_COLUMN_NAME.endsWith(columnName)) {
						bean.setTipoStr(cellValue);
					} else if (NIVEL_DIFICULDADE_COLUMN_NAME.endsWith(columnName)) {
						bean.setNivelDificuldadeStr(cellValue);
					} else if (MAX_ALUNOS_TEORICOS_COLUMN_NAME.endsWith(columnName)) {
						bean.setMaxAlunosTeoricoStr(cellValue);
					} else if (MAX_ALUNOS_PRATICOS_COLUMN_NAME.endsWith(columnName)) {
						bean.setMaxAlunosPraticoStr(cellValue);
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
		return ExcelInformationType.DISCIPLINAS.getSheetName();
	}
	
	@Override
	protected void processSheetContent(String sheetName, List<DisciplinasImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent) && doLogicValidation(sheetName,sheetContent)) {
			updateDataBase(sheetName,sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<DisciplinasImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (DisciplinasImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<DisciplinasImportExcelBean> sheetContent) {
		// verifica se alguma disciplina apareceu mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);
		// verifica se há referência a algum tipo de disciplina não cadastrado
		checkNonRegisteredTipoDisciplina(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<DisciplinasImportExcelBean> sheetContent) {
		// map com os códigos das disciplinas e as linhas em que o mesmo aparece no arquivo de entrada
		// [CódigoDisciplina -> Lista de Linhas do Arquivo de Entrada]
		Map<String,List<Integer>> disciplinaCodigoToRowsMap = new HashMap<String,List<Integer>>();
		 
		for (DisciplinasImportExcelBean bean : sheetContent) {
			List<Integer> rows = disciplinaCodigoToRowsMap.get(bean.getCodigoStr());
			if (rows == null) {
				rows = new ArrayList<Integer>();
				disciplinaCodigoToRowsMap.put(bean.getCodigoStr(),rows);
			}
			rows.add(bean.getRow());
		}
		
		// verifica se alguma disciplina apareceu mais de uma vez no arquivo de entrada
		for (Entry<String,List<Integer>> entry : disciplinaCodigoToRowsMap.entrySet()) {
			if (entry.getValue().size() > 1) {
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(),entry.getValue().toString()));
			}
		}
	}
	
	private void checkNonRegisteredTipoDisciplina(
		List< DisciplinasImportExcelBean > sheetContent )
	{
		// [ NomeTipoDisciplina -> TipoDisciplina ]
		Map< String, TipoDisciplina > tiposDisciplinaBDMap
			= TipoDisciplina.buildTipoDisciplinaNomeToTipoDisciplinaMap(
				TipoDisciplina.findAll( this.instituicaoEnsino ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( DisciplinasImportExcelBean bean : sheetContent )
		{
			TipoDisciplina tipoDisciplina
				= tiposDisciplinaBDMap.get( bean.getTipoStr() );

			if ( tipoDisciplina != null )
			{
				bean.setTipo( tipoDisciplina );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				TIPO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase(
		String sheetName, List< DisciplinasImportExcelBean > sheetContent )
	{
		Map< String, Disciplina > disciplinasBDMap = Disciplina.buildDisciplinaCodigoToDisciplinaMap(
			Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		for (DisciplinasImportExcelBean disciplinaExcel : sheetContent) {
			Disciplina disciplinaBD = disciplinasBDMap.get(disciplinaExcel.getCodigoStr());
			if (disciplinaBD != null) {
				// update
				disciplinaBD.setNome(disciplinaExcel.getNomeStr());
				disciplinaBD.setCreditosPratico(disciplinaExcel.getCreditosPraticos());
				disciplinaBD.setCreditosTeorico(disciplinaExcel.getCreditosTeoricos());
				disciplinaBD.setLaboratorio(disciplinaExcel.getUsaLaboratorio());
				disciplinaBD.setMaxAlunosPratico(disciplinaExcel.getMaxAlunosPratico());
				disciplinaBD.setMaxAlunosTeorico(disciplinaExcel.getMaxAlunosTeorico());
				disciplinaBD.setDificuldade(disciplinaExcel.getNivelDificuldade());
				disciplinaBD.setTipoDisciplina(disciplinaExcel.getTipo());
				
				disciplinaBD.merge();
			} else {
				// insert
				Disciplina newDisciplina = new Disciplina();
				newDisciplina.setCenario(getCenario());
				newDisciplina.setCodigo(disciplinaExcel.getCodigoStr());
				newDisciplina.setNome(disciplinaExcel.getNomeStr());
				newDisciplina.setCreditosPratico(disciplinaExcel.getCreditosPraticos());
				newDisciplina.setCreditosTeorico(disciplinaExcel.getCreditosTeoricos());
				newDisciplina.setLaboratorio(disciplinaExcel.getUsaLaboratorio());
				newDisciplina.setMaxAlunosPratico(disciplinaExcel.getMaxAlunosPratico());
				newDisciplina.setMaxAlunosTeorico(disciplinaExcel.getMaxAlunosTeorico());
				newDisciplina.setDificuldade(disciplinaExcel.getNivelDificuldade());
				newDisciplina.setTipoDisciplina(disciplinaExcel.getTipo());
				
				newDisciplina.persist();
			}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nomeDisciplina() );
			CRED_TEORICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().creditosTeoricos() );
			CRED_PRATICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().creditosPraticos() );
			USA_LABORATORIO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().usaLaboratorio() );
			TIPO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().tipo() );
			NIVEL_DIFICULDADE_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nivelDificuldade() );
			MAX_ALUNOS_TEORICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().maxAlunosTeorico() );
			MAX_ALUNOS_PRATICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().maxAlunosPratico() );
		}
	}
}
