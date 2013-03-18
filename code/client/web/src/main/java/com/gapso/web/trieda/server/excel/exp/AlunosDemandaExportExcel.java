package com.gapso.web.trieda.server.excel.exp;

import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
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

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public AlunosDemandaExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public AlunosDemandaExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super( true, ExcelInformationType.DEMANDAS_POR_ALUNO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
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
		return this.getI18nConstants().alunosDemanda();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		boolean result = false;
		
		List< AlunoDemanda > alunosDemanda
			= AlunoDemanda.findAll( this.instituicaoEnsino );

//		Set< AlunoDemanda > alunosDemanda
//			= new HashSet< AlunoDemanda >( listAlunosDemanda );

		if ( !alunosDemanda.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
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
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.DEMANDAS_POR_ALUNO.getSheetName());
		if (mapLevel2 != null && !mapLevel2.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			int nextRow = this.initialRow;
			
			Cell campusCell = getCell(nextRow,2,sheet);
			Cell turnoCell = getCell(nextRow,3,sheet);
			Cell cursoCell = getCell(nextRow,4,sheet);
			Cell curriculoCell = getCell(nextRow,5,sheet);
			Cell periodoCell = getCell(nextRow,6,sheet);
			Cell matriculaCell = getCell(nextRow,8,sheet);
			
			int lastRowNumber = sheet.getLastRowNum()+1;
			while (campusCell != null && nextRow <= lastRowNumber) {
				for (Entry<String,Map<String,String>> entry : mapLevel2.entrySet()) {
					String cellValue = entry.getKey();
					if (cellValue.equals(ExcelInformationType.RELATORIO_VISAO_ALUNO.getSheetName())) {
						String alunoKey = matriculaCell.getStringCellValue();
						String cellHyperlink = entry.getValue().get(alunoKey);
						if (cellHyperlink != null) {
							setCellWithHyperlink(nextRow,12,sheet,cellValue,cellHyperlink,true);
						}
					} else if (cellValue.equals(ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName())) {
						String visaoCursoKey = campusCell.getStringCellValue()+"-"+turnoCell.getStringCellValue()+"-"+cursoCell.getStringCellValue()+"-"+curriculoCell.getStringCellValue()+"-"+((int)periodoCell.getNumericCellValue());
						String cellHyperlink = entry.getValue().get(visaoCursoKey);
						if (cellHyperlink != null) {
							setCellWithHyperlink(nextRow,13,sheet,cellValue,cellHyperlink,true);
						}
					}
				}
				
				nextRow++;
				campusCell = getCell(nextRow,2,sheet);
				turnoCell = getCell(nextRow,3,sheet);
				cursoCell = getCell(nextRow,4,sheet);
				curriculoCell = getCell(nextRow,5,sheet);
				periodoCell = getCell(nextRow,6,sheet);
				matriculaCell = getCell(nextRow,8,sheet);
			}
			
			autoSizeColumns((short)11,(short)12,sheet);
		}		
	}

	private int writeData( AlunoDemanda alunoDemanda, int row, Sheet sheet )
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
