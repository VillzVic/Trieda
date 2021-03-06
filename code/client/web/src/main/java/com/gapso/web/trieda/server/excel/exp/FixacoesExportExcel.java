package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class FixacoesExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		CURRENCY( 6, 7 );

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

	public FixacoesExportExcel(Cenario cenario, 
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public FixacoesExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.FIXACOES.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().fixacoes();
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
		return getI18nConstants().fixacoes();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List<Fixacao> fixacao = Fixacao.findByCenario(this.instituicaoEnsino, getCenario());

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !fixacao.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);

			int nextRow = this.initialRow;
			for ( Fixacao c : fixacao )
			{
				nextRow = writeData(c, nextRow, sheet );
			}

			return true;
		}
		
		return false;
	}

	private int writeData(Fixacao fixacao, int row, Sheet sheet)
	{
		// Código
		setCell( row, 2, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getCodigo() );

		// Descrição
		setCell( row, 3, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getDescricao() );

		// CPF Professor
		setCell( row, 4, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getProfessor().getCpf() );

		// Código Disciplina
		setCell( row, 5, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getDisciplina().getCodigo() );

		// Código Campus
		setCell( row, 6, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getCampus().getCodigo() );

		// Código Unidade
		setCell( row, 7, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getUnidade().getCodigo() );

		// Código Sala
		setCell( row, 8, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getSala().getCodigo() );

		// Dias e Horários de Aula
		setCell( row, 9, sheet, cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], fixacao.getHorariosStr() );

		// TODO exportar horários

		row++;
		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
