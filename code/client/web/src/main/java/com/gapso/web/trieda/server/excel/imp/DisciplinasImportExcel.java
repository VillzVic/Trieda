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

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisciplinasImportExcel
	extends AbstractImportExcel< DisciplinasImportExcelBean >
{
	static public String CODIGO_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String CRED_TEORICOS_COLUMN_NAME;
	static public String CRED_PRATICOS_COLUMN_NAME;
	static public String USA_LABORATORIO_COLUMN_NAME;
	static public String TIPO_COLUMN_NAME;
	static public String NIVEL_DIFICULDADE_COLUMN_NAME;
	static public String MAX_ALUNOS_TEORICOS_COLUMN_NAME;
	static public String MAX_ALUNOS_PRATICOS_COLUMN_NAME;
	static public String AULAS_CONTINUAS_COLUMN_NAME;
	static public String PROFESSOR_UNICO_COLUMN_NAME;
	static public String USA_SABADO_COLUMN_NAME;
	static public String USA_DOMINGO_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public DisciplinasImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( NOME_COLUMN_NAME );
		this.headerColumnsNames.add( CRED_TEORICOS_COLUMN_NAME );
		this.headerColumnsNames.add( CRED_PRATICOS_COLUMN_NAME );
		this.headerColumnsNames.add( USA_LABORATORIO_COLUMN_NAME );
		this.headerColumnsNames.add( TIPO_COLUMN_NAME );
		this.headerColumnsNames.add( NIVEL_DIFICULDADE_COLUMN_NAME );
		this.headerColumnsNames.add( MAX_ALUNOS_TEORICOS_COLUMN_NAME );
		this.headerColumnsNames.add( MAX_ALUNOS_PRATICOS_COLUMN_NAME );
		this.headerColumnsNames.add( AULAS_CONTINUAS_COLUMN_NAME );
		this.headerColumnsNames.add( PROFESSOR_UNICO_COLUMN_NAME );
		this.headerColumnsNames.add( USA_SABADO_COLUMN_NAME );
		this.headerColumnsNames.add( USA_DOMINGO_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.DISCIPLINAS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
		{
		return this.headerColumnsNames;
	}

	@Override
	protected DisciplinasImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		DisciplinasImportExcelBean bean
			= new DisciplinasImportExcelBean( row.getRowNum() + 1 );

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

					if ( CODIGO_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCodigoStr( cellValue );
					}
					else if ( NOME_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNomeStr( cellValue );
					}
					else if ( CRED_TEORICOS_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCreditosTeoricosStr( cellValue );
					}
					else if ( CRED_PRATICOS_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCreditosPraticosStr( cellValue );
					}
					else if ( USA_LABORATORIO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setExigeLaboratorioStr( cellValue );
					}
					else if ( TIPO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setTipoStr( cellValue );
					}
					else if ( NIVEL_DIFICULDADE_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNivelDificuldadeStr( cellValue );
					}
					else if ( MAX_ALUNOS_TEORICOS_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMaxAlunosTeoricoStr( cellValue );
					}
					else if ( MAX_ALUNOS_PRATICOS_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setMaxAlunosPraticoStr( cellValue );
					}
					else if ( AULAS_CONTINUAS_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setAulasContinuasStr( cellValue );
					}
					else if ( PROFESSOR_UNICO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setProfessorUnicoStr( cellValue );
					}
					else if ( USA_SABADO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setUsaSabadoStr( cellValue );
					}
					else if ( USA_DOMINGO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setUsaDomingoStr( cellValue );
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
		return ExcelInformationType.DISCIPLINAS.getSheetName();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< DisciplinasImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< DisciplinasImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre

		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( DisciplinasImportExcelBean bean : sheetContent )
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
		String sheetName, List< DisciplinasImportExcelBean > sheetContent )
	{
		// Verifica se alguma disciplina apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		// Verifica se há referência a
		// algum tipo de disciplina não cadastrado
		checkNonRegisteredTipoDisciplina( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniqueness(
		List< DisciplinasImportExcelBean > sheetContent )
	{
		// Map com os códigos das disciplinas e as
		// linhas em que o mesmo aparece no arquivo de entrada

		// [ CódigoDisciplina -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > disciplinaCodigoToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( DisciplinasImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = disciplinaCodigoToRowsMap.get( bean.getCodigoStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				disciplinaCodigoToRowsMap.put( bean.getCodigoStr(), rows );
			}

			rows.add( bean.getRow() );
		}

		// Verifica se alguma disciplina apareceu
		// mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry
			: disciplinaCodigoToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}

	private void checkNonRegisteredTipoDisciplina(
		List< DisciplinasImportExcelBean > sheetContent )
	{
		// [ NomeTipoDisciplina -> TipoDisciplina ]
		Map< String, TipoDisciplina > tiposDisciplinaBDMap
			= TipoDisciplina.buildTipoDisciplinaNomeToTipoDisciplinaMap(
				TipoDisciplina.findAll( this.instituicaoEnsino ) );

		List< Integer > rowsWithErrors = new ArrayList< Integer >();

		for ( DisciplinasImportExcelBean bean : sheetContent )
		{
			TipoDisciplina tipoDisciplina
				= tiposDisciplinaBDMap.get( bean.getTipoStr() );

			if ( tipoDisciplina != null )
			{
				bean.setTipo( tipoDisciplina );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				TIPO_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	@ProgressReportMethodScan(texto = "Atualizando banco de dados")
	private void updateDataBase(
		String sheetName, List< DisciplinasImportExcelBean > sheetContent )
	{
		Map< String, Disciplina > disciplinasBDMap
			= Disciplina.buildDisciplinaCodigoToDisciplinaMap(
				Disciplina.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List<Disciplina> persistedDisciplinas = new ArrayList<Disciplina>();
		int count = 0, total=sheetContent.size(); System.out.print(" "+total);
		for ( DisciplinasImportExcelBean disciplinaExcel : sheetContent )
		{
			Disciplina disciplinaBD = disciplinasBDMap.get(
				disciplinaExcel.getCodigoStr() );

			if ( disciplinaBD != null )
			{
				// Update
				disciplinaBD.setNome( disciplinaExcel.getNomeStr() );
				disciplinaBD.setCreditosPratico( disciplinaExcel.getCreditosPraticos() );
				disciplinaBD.setCreditosTeorico( disciplinaExcel.getCreditosTeoricos() );
				disciplinaBD.setLaboratorio( disciplinaExcel.getExigeLaboratorio() );
				disciplinaBD.setMaxAlunosPratico( disciplinaExcel.getMaxAlunosPratico() );
				disciplinaBD.setMaxAlunosTeorico( disciplinaExcel.getMaxAlunosTeorico() );
				disciplinaBD.setDificuldade( disciplinaExcel.getNivelDificuldade() );
				disciplinaBD.setTipoDisciplina( disciplinaExcel.getTipo() );
				disciplinaBD.setAulasContinuas( disciplinaExcel.getAulasContinuas() );
				disciplinaBD.setProfessorUnico( disciplinaExcel.getProfessorUnico() );
				disciplinaBD.setUsaSabado( disciplinaExcel.getUsaSabado() );
				disciplinaBD.setUsaDomingo( disciplinaExcel.getUsaDomingo() );
				
				disciplinaBD.merge();
				persistedDisciplinas.add(disciplinaBD);
			}
			else
			{
				// Insert
				Disciplina newDisciplina = new Disciplina();

				newDisciplina.setCenario( getCenario() );
				newDisciplina.setCodigo( disciplinaExcel.getCodigoStr() );
				newDisciplina.setNome( disciplinaExcel.getNomeStr() );
				newDisciplina.setCreditosPratico( disciplinaExcel.getCreditosPraticos() );
				newDisciplina.setCreditosTeorico( disciplinaExcel.getCreditosTeoricos() );
				newDisciplina.setLaboratorio( disciplinaExcel.getExigeLaboratorio() );
				newDisciplina.setMaxAlunosPratico( disciplinaExcel.getMaxAlunosPratico() );
				newDisciplina.setMaxAlunosTeorico( disciplinaExcel.getMaxAlunosTeorico() );
				newDisciplina.setDificuldade( disciplinaExcel.getNivelDificuldade() );
				newDisciplina.setTipoDisciplina( disciplinaExcel.getTipo() );
				newDisciplina.setAulasContinuas( disciplinaExcel.getAulasContinuas() );
				newDisciplina.setProfessorUnico( disciplinaExcel.getProfessorUnico() );
				newDisciplina.setUsaSabado( disciplinaExcel.getUsaSabado() );
				newDisciplina.setUsaDomingo( disciplinaExcel.getUsaDomingo() );

				newDisciplina.persist();
				persistedDisciplinas.add(newDisciplina);
			}
			
			count++;total--;if (count == 100) {
				System.out.println("\t   Faltam "+total+" disciplinas"); 
				count = 0;
				}
		}
		
		if (!persistedDisciplinas.isEmpty()) {
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findAll(instituicaoEnsino);
			Disciplina.preencheHorariosDasDisciplinas(persistedDisciplinas,semanasLetivas);
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoDisciplina() );
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nomeDisciplina() );
			CRED_TEORICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().creditosTeoricos() );
			CRED_PRATICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().creditosPraticos() );
			USA_LABORATORIO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().exigeLaboratorio() );
			TIPO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().tipo() );
			NIVEL_DIFICULDADE_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nivelDificuldade() );
			MAX_ALUNOS_TEORICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().maxAlunosTeorico() );
			MAX_ALUNOS_PRATICOS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().maxAlunosPratico() );
			AULAS_CONTINUAS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().aulasContinuas() );
			PROFESSOR_UNICO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().professorUnico() );
			USA_SABADO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().usaSabado() );
			USA_DOMINGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().usaDomingo() );
		}
	}
	
}
