package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AlunosDemandaExportExcel
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

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;

	public AlunosDemandaExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public AlunosDemandaExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.ALUNOS_DEMANDA.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().alunosDemanda();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().alunosDemanda();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< AlunoDemanda > alunosDemanda
			= AlunoDemanda.findAll( this.instituicaoEnsino );

		if ( !alunosDemanda.isEmpty() )
		{
			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );
			int nextRow = this.initialRow;

			for ( AlunoDemanda alunoDemanda : alunosDemanda )
			{
				nextRow = writeData( alunoDemanda, nextRow, sheet );
			}

			return true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.sheetName, workbook );
		}

		return false;
	}

	private int writeData( AlunoDemanda alunoDemanda, int row, HSSFSheet sheet )
	{
		// Codigo Campus
		setCell( row, 2, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCampus().getCodigo() );

		// Turno
		setCell( row, 3, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getTurno().getNome() );

		// Código Curso
		setCell( row, 4, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCurriculo().getCodigo() );

		// Matriz Curricular
		setCell( row, 5, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCurriculo().getCodigo() );

		// Período
		setCell( row, 6, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCurriculo().getPeriodo(
					this.instituicaoEnsino, alunoDemanda.getDemanda().getDisciplina() ) );

		// Código Disciplina
		setCell( row, 7, sheet,
			cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getDemanda().getDisciplina().getCodigo() );

		// Matrícula do Aluno
		setCell( row, 8, sheet,
			cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getAluno().getMatricula() );

		// Nome do Aluno
		setCell( row, 9, sheet,
			cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getAluno().getNome() );

		// Prioridade
		setCell( row, 10, sheet,
			cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getPrioridade() );

		String alunoAtendido = HtmlUtils.htmlUnescape( "N&atilde;o" );
		if ( alunoDemanda.getAtendido() != null
			&& alunoDemanda.getAtendido() == true )
		{
			alunoAtendido = "Sim";
		}

		// Atendido
		setCell( row, 11, sheet,
			cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ], alunoAtendido );

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
