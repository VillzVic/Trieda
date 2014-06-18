package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class AlunosImportExcel
	extends AbstractImportExcel< AlunosImportExcelBean >
{
	static public String MATRICULA_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String FORMANDO_COLUMN_NAME;
	static public String VIRTUAL_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public AlunosImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( MATRICULA_COLUMN_NAME );
		this.headerColumnsNames.add( NOME_COLUMN_NAME );
		this.headerColumnsNames.add( FORMANDO_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.ALUNOS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
		{
		return this.headerColumnsNames;
	}

	@Override
	protected AlunosImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		AlunosImportExcelBean bean
			= new AlunosImportExcelBean( row.getRowNum() + 1 );

        for ( int cellIndex = row.getFirstCellNum();
        	  cellIndex <= row.getLastCellNum(); cellIndex++ )
        {
            Cell cell = row.getCell( cellIndex );

        	if ( cell != null )
        	{
        		Cell headerCell = header.getCell( cell.getColumnIndex() );

        		if ( headerCell != null )
        		{
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue( cell );

					if ( MATRICULA_COLUMN_NAME.equals( columnName ) )
					{
						bean.setMatriculaStr( cellValue );
					}
					else if ( NOME_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNomeStr( cellValue );
					}
					else if ( FORMANDO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setFormandoStr( cellValue );
					}
					else if ( VIRTUAL_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setVirtualStr( cellValue );
					}
        		}
        	}
        }

		return bean;
	}

	@Override
	protected String getHeaderToString()
	{
		return this.headerColumnsNames.toString();
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.ALUNOS.getSheetName();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< AlunosImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			getProgressReport().setInitNewPartial("Atualizando banco de dados");
			updateDataBase( sheetName, sheetContent );
			getProgressReport().setPartial("Fim de Atualizando banco de dados");
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< AlunosImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre

		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( AlunosImportExcelBean bean : sheetContent )
		{
			List< ImportExcelError > errorsBean = bean.checkSyntacticErrors();

			for ( ImportExcelError error : errorsBean )
			{
				List< Integer > rowsWithErrors
					= syntacticErrorsMap.get( error );

				if ( rowsWithErrors == null )
				{
					rowsWithErrors = new ArrayList< Integer >();
					syntacticErrorsMap.put( error, rowsWithErrors );
				}

				rowsWithErrors.add( bean.getRow() );
			}
		}

		// Coleta os erros e adiciona os mesmos na lista de mensagens
		for ( ImportExcelError error : syntacticErrorsMap.keySet() )
		{
			List< Integer > linhasComErro = syntacticErrorsMap.get( error );
			getErrors().add( error.getMessage(
				linhasComErro.toString(), getI18nMessages() ) );
		}

		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(
		String sheetName, List< AlunosImportExcelBean > sheetContent )
	{
		// Verifica se alguma disciplina apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniqueness(
		List< AlunosImportExcelBean > sheetContent )
	{
		// Map com as matrículas dos alunos e as
		// linhas em que o mesmo aparece no arquivo de entrada

		// [ MatrículaAluno -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > alunoMatriculaToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( AlunosImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = alunoMatriculaToRowsMap.get( bean.getMatriculaStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				alunoMatriculaToRowsMap.put( bean.getMatriculaStr(), rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se alguma aluno apareceu
		// mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry
			: alunoMatriculaToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	protected void updateDataBase(
		String sheetName, List< AlunosImportExcelBean > sheetContent )
	{
		Map< String, Aluno > alunosBDMap
			= Aluno.buildMatriculaAlunoToAlunoMap(
				Aluno.findByCenario( this.instituicaoEnsino, getCenario() ) );

		int count = 0, total=sheetContent.size(); System.out.print(" "+total);
		for ( AlunosImportExcelBean alunoExcel : sheetContent )
		{
			Aluno alunoBD = alunosBDMap.get(
				alunoExcel.getMatriculaStr() );

			if ( alunoBD != null )
			{
				// Update
				alunoBD.setNome( alunoExcel.getNomeStr() );
				alunoBD.setFormando( alunoExcel.getFormando() );
				alunoBD.setVirtual( alunoExcel.getVirtual() );
				alunoBD.setCriadoTrieda(false);
				
				alunoBD.merge();
			}
			else
			{
				// Insert
				Aluno newAluno = new Aluno();

				newAluno.setCenario( getCenario() );
				newAluno.setInstituicaoEnsino(instituicaoEnsino);
				newAluno.setMatricula( alunoExcel.getMatriculaStr() );
				newAluno.setNome( alunoExcel.getNomeStr() );
				newAluno.setFormando( alunoExcel.getFormando() );
				newAluno.setVirtual( alunoExcel.getVirtual() );
				newAluno.setCriadoTrieda(false);
				
				newAluno.persist();
			}
			
			count++;total--;if (count == 100) {
				System.out.println("\t   Faltam "+total+" alunos"); 
				count = 0;
				}
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( MATRICULA_COLUMN_NAME == null )
		{
			MATRICULA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().matriculaAluno() );
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nomeAluno() );
			FORMANDO_COLUMN_NAME = HtmlUtils.htmlUnescape( "Formando?" );
			VIRTUAL_COLUMN_NAME = HtmlUtils.htmlUnescape( "Virtual?" );
		}
	}
}