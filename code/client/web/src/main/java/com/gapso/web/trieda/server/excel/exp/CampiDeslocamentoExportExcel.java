package com.gapso.web.trieda.server.excel.exp;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.TriedaPar;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CampiDeslocamentoExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		INTEGER( 6, 3 );

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

	public CampiDeslocamentoExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public CampiDeslocamentoExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		super( true, ExcelInformationType.CAMPI_DESLOCAMENTO.getSheetName(), cenario,i18nConstants,i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().deslocamentoCampi();
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
		return getI18nConstants().deslocamentoCampi();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< DeslocamentoCampus > deslocamentos = DeslocamentoCampus.findAllByCampus(
		 	this.instituicaoEnsino, getCenario() );
		List<Campus> campi = Campus.findByCenario(instituicaoEnsino, getCenario());
		
		Map<TriedaPar<Integer, Integer>, Integer> deslocamentosRowColumnMap =
			criaDeslocamentoRowColumnMap(deslocamentos, campi);
		
		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		if (campi.size() > 1) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			preencheOrigensDestinos( this.initialRow, campi, sheet );
			for ( Entry<TriedaPar<Integer, Integer>, Integer> d : deslocamentosRowColumnMap.entrySet() )
			{
				writeData( d.getKey(), d.getValue(), sheet );
			}

			return true;
		}

		return false;
	}
	
	private Map<TriedaPar<Integer, Integer>, Integer> criaDeslocamentoRowColumnMap(
			List<DeslocamentoCampus> deslocamentos, List<Campus> campi)
	{
		Map<TriedaPar<Integer, Integer>, Integer> deslocamentosRowColumnMap =
				new HashMap<TriedaPar<Integer, Integer>, Integer>();
		
		int row = 6;
		int column = 3;
		int origemRow = row;
		for (Campus campusOrigem : campi)
		{
			int destinoColumn = column;
			for (Campus campusDestino : campi)
			{
				int tempoDeslocamento = 0;
				for (DeslocamentoCampus deslocamento : deslocamentos)
				{
					if (campusOrigem.getCodigo().equals(deslocamento.getOrigem().getCodigo())
							&& campusDestino.getCodigo().equals(deslocamento.getDestino().getCodigo()))
					{
						tempoDeslocamento = deslocamento.getTempo();
					}

				}
				deslocamentosRowColumnMap.put(TriedaPar.create(origemRow, destinoColumn), tempoDeslocamento);
				destinoColumn++;
			}
			origemRow++;
		}
		
		return deslocamentosRowColumnMap;
	}

	private void preencheOrigensDestinos(int row, List<Campus> campi, Sheet sheet )
	{
		int origensRow = row;
		int destinoColumn = 3;
		for (Campus campus : campi)
		{
			setCell(origensRow,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],campus.getNome());
			setCell(row-1,destinoColumn,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],campus.getNome());
			origensRow++;
			destinoColumn++;
		}
	}

	private void writeData( TriedaPar<Integer, Integer> rowColumnPar,
			Integer tempo, Sheet sheet )
	{
		int row = rowColumnPar.getPrimeiro();
		int column = rowColumnPar.getSegundo();
		// Nome
		setCell(row,column,sheet,cellStyles[ExcelCellStyleReference.INTEGER.ordinal()],tempo);
		
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}