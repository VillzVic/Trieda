package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CurriculosExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		NUMBER(6,5);
		private int row;
		private int col;
		private ExcelCellStyleReference(int row, int col) {
			this.row = row;
			this.col = col;
		}
		public int getRow() {
			return row;
		}
		public int getCol() {
			return col;
		}
	}
	private HSSFCellStyle[] cellStyles;
	
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;
	
	public CurriculosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public CurriculosExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.CURRICULOS.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().curriculos();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().curriculos();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Curriculo > curriculos
			= Curriculo.findByCenario(
				this.instituicaoEnsino, getCenario() );

		if ( curriculos != null && !curriculos.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.sheetName, workbook );
			}

			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );
			
			int nextRow = this.initialRow;
			for ( Curriculo c : curriculos )
			{
				nextRow = writeData( c, nextRow, sheet );
			}

			return true;
		}

		return false;
	}
	
	private int writeData( Curriculo curriculo, int row, HSSFSheet sheet )
	{
		List< Integer > listPeriodos
			= curriculo.getPeriodos( this.instituicaoEnsino );

		if ( listPeriodos == null || listPeriodos.size() == 0 )
		{
			return row;
		}

		for ( Integer periodo : listPeriodos )
		{
			List< CurriculoDisciplina > disciplinasDeUmPeriodo
				= curriculo.getCurriculoDisciplinasByPeriodo( this.instituicaoEnsino, periodo );

			if ( disciplinasDeUmPeriodo == null || disciplinasDeUmPeriodo.size() == 0 )
			{
				continue;
			}

			for ( CurriculoDisciplina disciplinaDeUmPeriodo : disciplinasDeUmPeriodo )
			{
				// Curso
				setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curriculo.getCurso().getCodigo());
				// Código
				setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curriculo.getCodigo());
				// Descrição
				setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curriculo.getDescricao());
				// Período
				setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],periodo);
				// Disciplina
				setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],disciplinaDeUmPeriodo.getDisciplina().getCodigo());
				row++;
			}
		}
				
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}