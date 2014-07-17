package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.AlunosDemandaServiceImpl;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.dtos.AtendimentoCargaHorariaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.relatorioVisao.ExportExcelFilter;

public class AtendimentosCargaHorariaExportExcel
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
	
	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;
	
	public AtendimentosCargaHorariaExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino, fileExtension );
	}
	
	public AtendimentosCargaHorariaExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino, fileExtension );
	}

	public AtendimentosCargaHorariaExportExcel( boolean removeUnusedSheets,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
			InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.ATENDIMENTOS_CARGA_HORARIA.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName()
	{
		return getI18nConstants().atendimentosCargaHoraria();
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
		return getI18nConstants().atendimentosCargaHoraria();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		boolean result = false;
		
		List< AtendimentoCargaHorariaDTO > atendimentosCargaHoraria
			= new ArrayList< AtendimentoCargaHorariaDTO >();
		AlunosDemandaServiceImpl alunosDemandaServiceImpl = new AlunosDemandaServiceImpl();
		atendimentosCargaHoraria.addAll(
			alunosDemandaServiceImpl.getAtendimentoCargaHoraria(getCenario()) );
		
		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !atendimentosCargaHoraria.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for ( AtendimentoCargaHorariaDTO AtendimentoCargaHorariaDTO1 : atendimentosCargaHoraria )
			{
				nextRow = writeData( AtendimentoCargaHorariaDTO1, nextRow, sheet );
			}

			result = true;
		}

		return result;
	}

	private int writeData( AtendimentoCargaHorariaDTO atendimentoCargaHorariaDTO, int row, Sheet sheet )
	{
		int i = 2;
		
		// Matricula
		setCell( row, i++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				atendimentoCargaHorariaDTO.getAlunoMatricula() );
		
		// Segunda
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia2() );

		// Terca
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia3() );

		// Quarta
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia4() );

		// Quinta
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia5() );

		// Sexta
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia6() );
			
		// Sabado
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia7() );
			
		// Domingo
		setCell( row, i++, sheet,
			cellStyles[ ExcelCellStyleReference.INTEGER.ordinal() ],
			atendimentoCargaHorariaDTO.getDia1() );


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
