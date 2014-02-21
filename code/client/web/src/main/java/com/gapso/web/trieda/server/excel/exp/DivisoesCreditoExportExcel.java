package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DivisoesCreditoExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		INT( 6, 2 );

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

	public DivisoesCreditoExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public DivisoesCreditoExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		super( true, ExcelInformationType.DIVISOES_CREDITO.getSheetName(), cenario,i18nConstants,i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().divisoesCredito();
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
		return getI18nConstants().divisoesCredito();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< DivisaoCredito > divisaoCredito = DivisaoCredito.findByCenario(instituicaoEnsino, getCenario());

		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		if (!divisaoCredito.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;
			for ( DivisaoCredito d : divisaoCredito )
			{
				nextRow = writeData( d, nextRow, sheet );
			}

			return true;
		}

		return false;
	}

	private int writeData( DivisaoCredito divisaoCredito, int row, Sheet sheet )
	{
		// Creditos
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getCreditos());
		
		// Dia 1
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia1());
		
		// Dia 2
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia2());
		
		// Dia 3
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia3());

		// Dia 4
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia4());

		// Dia 5
		setCell(row,7,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia5());
		
		// Dia 6
		setCell(row,8,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia6());
		
		// Dia 7
		setCell(row,9,sheet,cellStyles[ExcelCellStyleReference.INT.ordinal()],divisaoCredito.getDia7());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}