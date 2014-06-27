package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class SalasExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		NUMBER_INT(6,8),
		NUMBER_DOUBLE( 6, 10 );
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

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public SalasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}
	
	public SalasExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.SALAS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return getI18nConstants().salas();
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
		return getI18nConstants().salas();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< Sala > salas = Sala.findByCenario( this.instituicaoEnsino, getCenario() );

		
		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !salas.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;

			for ( Sala s : salas )
			{
				nextRow = writeData( s, nextRow, sheet );
			}

			return true;
		}

		return false;
	}
	
	private int writeData( Sala sala, int row, Sheet sheet )
	{
		// Codigo
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getCodigo());
		// Tipo
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getTipoSala().getNome());
		// Unidade
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getUnidade().getCodigo());
		// Numero
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getNumero());
		// Descricao
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getDescricao());
		// Andar
		setCell(row,7,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getAndar());
		// Capacidade Instalada
		setCell(row,8,sheet,cellStyles[ExcelCellStyleReference.NUMBER_INT.ordinal()],sala.getCapacidadeInstalada());
		// Capacidade Maxima
		setCell(row,9,sheet,cellStyles[ExcelCellStyleReference.NUMBER_INT.ordinal()],sala.getCapacidadeMax());
		// Custo de Operacao por Credito
		setCell(row,10,sheet,cellStyles[ExcelCellStyleReference.NUMBER_DOUBLE.ordinal()],sala.getCustoOperacaoCred());
		// Ambiente Externo?
		setCell(row,11,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()], sala.getExterna() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}

