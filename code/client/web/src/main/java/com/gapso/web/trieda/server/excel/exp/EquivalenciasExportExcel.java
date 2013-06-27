package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class EquivalenciasExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT(6,2);

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return row;
		}

		public int getCol()
		{
			return col;
		}
	}

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public EquivalenciasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public EquivalenciasExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.EQUIVALENCIAS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().equivalencias();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().equivalencias();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< Equivalencia > equivalencias
			= Equivalencia.findAll( this.instituicaoEnsino );

		if ( !equivalencias.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.getSheetName(), workbook );
			}

			Sheet sheet = workbook.getSheet(this.getSheetName());
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;
			for (Equivalencia equivalencia : equivalencias) {
				nextRow = writeData(equivalencia,nextRow,sheet);
			}
			
			return true;
		}
		
		return false;
	}
	
	private int writeData(Equivalencia equivalencia, int row, Sheet sheet) {
		if ( !equivalencia.getEquivalenciaGeral() && !equivalencia.getCursos().isEmpty() )
		{
			for ( Curso curso : equivalencia.getCursos() ) {
				// Cursou
				setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],equivalencia.getCursou().getCodigo());
				// Elimina
				setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],equivalencia.getElimina().getCodigo());
				//Curso
				setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()], curso.getCodigo() );
				
				row++;
			}
		}
		else
		{
			// Cursou
			setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],equivalencia.getCursou().getCodigo());
			// Elimina
			setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],equivalencia.getElimina().getCodigo());
			
			row++;
		}
		
		return row;
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}


