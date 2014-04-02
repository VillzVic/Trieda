package com.gapso.web.trieda.server.excel.exp;

import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class OfertasCursosCampiExportExcel  extends AbstractExportExcel{
	
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		DOUBLE_NUMBER( 6, 6 );

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return this.row;
		}

		public int getCol()
		{
			return this.col;
		}
	}
	
	private CellStyle[] cellStyles;
	
	private boolean removeUnusedSheets;
	private int initialRow;
	
	public OfertasCursosCampiExportExcel( Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
		{
			this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
		}
	
	public OfertasCursosCampiExportExcel(
			boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
		{
			super( true, ExcelInformationType.OFERTAS_CURSOS_CAMPI.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

			this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
			this.removeUnusedSheets = removeUnusedSheets;
			this.initialRow = 6;
		}


	@Override
	public String getFileName() {
		return this.getI18nConstants().ofertasCursosCampi();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().ofertasCursosCampi();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		boolean result = false;
		
		List< Oferta > listOfertas = Oferta.findByCenario(
				this.instituicaoEnsino, getCenario() );

		Set< Oferta > ofertas
			= new HashSet< Oferta >( listOfertas );

		if ( !ofertas.isEmpty() )
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
			for ( Oferta oferta : ofertas )
			{
				nextRow = writeData( oferta, nextRow, sheet);
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.OFERTAS_CURSOS_CAMPI.getSheetName());
		if (mapLevel2 != null && !mapLevel2.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			int nextRow = this.initialRow;
			
			Cell campusCell = getCell(nextRow,2,sheet);
			Cell turnoCell = getCell(nextRow,3,sheet);
			Cell cursoCell = getCell(nextRow,4,sheet);
			Cell curriculoCell = getCell(nextRow,5,sheet);
			
			int lastRowNumber = sheet.getLastRowNum()+1;
			while (campusCell != null && nextRow <= lastRowNumber) {
				String key = campusCell.getStringCellValue()+"-"+turnoCell.getStringCellValue()+"-"+cursoCell.getStringCellValue()+"-"+curriculoCell.getStringCellValue();
				
				int col = 13;
				for (Entry<String,Map<String,String>> entry : mapLevel2.entrySet()) {
					String cellValue = entry.getKey();
					String cellHyperlink = entry.getValue().get(key);
					if (cellHyperlink != null) {
						setCellWithHyperlink(nextRow,col++,sheet,cellValue,cellHyperlink,true);
					}
				}
				
				nextRow++;
				campusCell = getCell(nextRow,2,sheet);
				turnoCell = getCell(nextRow,3,sheet);
				cursoCell = getCell(nextRow,4,sheet);
				curriculoCell = getCell(nextRow,5,sheet);
			}
			
			autoSizeColumns((short)12,(short)12,sheet);
		}		
	}
	
	private int writeData( Oferta oferta, int row, Sheet sheet)
	{
		Curriculo curriculo = oferta.getCurriculo();

		// Campus
		setCell( row, 2, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			oferta.getCampus().getCodigo() );

		// Turno
		setCell( row, 3, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			oferta.getTurno().getNome() );

		// Curso
		setCell( row, 4, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			curriculo.getCurso().getCodigo() );

		// Matriz Curricular
		setCell( row, 5, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			curriculo.getCodigo() );

		// Receita
		setCell( row, 6, sheet, this.cellStyles[ ExcelCellStyleReference.DOUBLE_NUMBER.ordinal() ], oferta.getReceita() );
		
		row++;

		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

}
