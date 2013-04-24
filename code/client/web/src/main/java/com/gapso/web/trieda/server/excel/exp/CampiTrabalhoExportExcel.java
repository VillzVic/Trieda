package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class CampiTrabalhoExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 );

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

	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public CampiTrabalhoExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public CampiTrabalhoExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.CAMPI_TRABALHO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().campiTrabalho();
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
		return getI18nConstants().campiTrabalho();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< Campus > campi = Campus.findByCenario(
			this.instituicaoEnsino, getCenario() );

		boolean flag = false;

		for ( Campus campus : campi )
		{
			if ( !campus.getProfessores().isEmpty() )
			{
				flag = true;
				break;
			}
		}

		if ( flag )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.getSheetName(), workbook );
			}

			Sheet sheet = workbook.getSheet( this.getSheetName() );
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;

			for ( Campus campus : campi )
			{
				for ( Professor professor : campus.getProfessores() )
				{
					nextRow = writeData( campus.getCodigo(), professor.getCpf(),
						professor.getNome(), nextRow, sheet );
				}
			}
		}

		return flag;
	}

	private int writeData( String campusCodigo, String cpf, String professorNome, int row, Sheet sheet )
	{
		// Campus Código
		setCell( row, 2, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], campusCodigo );

		// CPF
		setCell( row, 3, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], cpf );

		// Nome
		setCell( row, 4, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], professorNome );

		row++;
		return row;
	}
	
	private void fillInCellStyles( Sheet sheet )
	{ 
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
