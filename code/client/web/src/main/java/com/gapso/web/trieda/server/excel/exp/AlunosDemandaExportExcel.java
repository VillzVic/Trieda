package com.gapso.web.trieda.server.excel.exp;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

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
	private int initialRow;

	public AlunosDemandaExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public AlunosDemandaExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( true, ExcelInformationType.DEMANDAS_POR_ALUNO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().alunosDemanda();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().alunosDemanda();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		boolean result = false;
		
		List< AlunoDemanda > listAlunosDemanda
			= AlunoDemanda.findAll( this.instituicaoEnsino );

		Set< AlunoDemanda > alunosDemanda
			= new HashSet< AlunoDemanda >( listAlunosDemanda );

		if ( !alunosDemanda.isEmpty() )
		{
			HSSFSheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles( sheet );
			int nextRow = this.initialRow;

			for ( AlunoDemanda alunoDemanda : alunosDemanda )
			{
				nextRow = writeData( alunoDemanda, nextRow, sheet );
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}

	private int writeData( AlunoDemanda alunoDemanda, int row, HSSFSheet sheet )
	{
		// Codigo Campus
		setCell( row, 2, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCampus().getCodigo() );

		// Turno
		setCell( row, 3, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getTurno().getNome() );

		// Código Curso
		setCell( row, 4, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCurso().getCodigo() );

		// Matriz Curricular
		setCell( row, 5, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				alunoDemanda.getDemanda().getOferta().getCurriculo().getCodigo() );

		// Se a demanda do aluno não indicar o período,
		// procuramos o período a partir do currículo da demanda
		Integer periodo = alunoDemanda.getPeriodo();
		if ( periodo == null || periodo <= 0 )
		{
			periodo = alunoDemanda.getDemanda().getOferta().getCurriculo().getPeriodo(alunoDemanda.getDemanda().getDisciplina());
		}

		// Período
		setCell( row, 6, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], periodo );

		// Código Disciplina
		setCell( row, 7, sheet,
			this.cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getDemanda().getDisciplina().getCodigo() );

		// Matrícula do Aluno
		setCell( row, 8, sheet,
			this.cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getAluno().getMatricula() );

		// Nome do Aluno
		setCell( row, 9, sheet,
			this.cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getAluno().getNome() );

		// Prioridade
		setCell( row, 10, sheet,
			this.cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ],
				alunoDemanda.getPrioridade() );

		String alunoAtendido = HtmlUtils.htmlUnescape( "N&atilde;o" );
		if ( alunoDemanda.getAtendido() != null
			&& alunoDemanda.getAtendido() == true )
		{
			alunoAtendido = "Sim";
		}

		// Atendido
		setCell( row, 11, sheet,
			this.cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ], alunoAtendido );

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
