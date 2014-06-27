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
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class SalasImportExcel
	extends AbstractImportExcel< SalasImportExcelBean >
{
	static public String CODIGO_COLUMN_NAME;
	static public String TIPO_COLUMN_NAME;
	static public String UNIDADE_COLUMN_NAME;
	static public String NUMERO_COLUMN_NAME;
	static public String DESCRICAO_COLUMN_NAME;
	static public String ANDAR_COLUMN_NAME;
	static public String CAPACIDADE_INSTALADA_COLUMN_NAME;
	static public String CAPACIDADE_MAX_COLUMN_NAME;
	static public String CUSTO_OPERACAO_CRED_COLUMN_NAME;
	static public String EXTERNA_COLUMN_NAME;

	private List< String > headerColumnsNames;

	public SalasImportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );
		resolveHeaderColumnNames();

		this.headerColumnsNames = new ArrayList< String >();
		this.headerColumnsNames.add( CODIGO_COLUMN_NAME );
		this.headerColumnsNames.add( TIPO_COLUMN_NAME );
		this.headerColumnsNames.add( UNIDADE_COLUMN_NAME );
		this.headerColumnsNames.add( NUMERO_COLUMN_NAME );
		this.headerColumnsNames.add( DESCRICAO_COLUMN_NAME );
		this.headerColumnsNames.add( ANDAR_COLUMN_NAME );
		this.headerColumnsNames.add( CAPACIDADE_INSTALADA_COLUMN_NAME );
		this.headerColumnsNames.add( CAPACIDADE_MAX_COLUMN_NAME );
		this.headerColumnsNames.add( CUSTO_OPERACAO_CRED_COLUMN_NAME );
		this.headerColumnsNames.add( EXTERNA_COLUMN_NAME );
	}

	@Override
	protected boolean sheetMustBeProcessed(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		String sheetName = workbook.getSheetName( sheetIndex );
		return ExcelInformationType.SALAS.getSheetName().equals( sheetName );
	}

	@Override
	protected List< String > getHeaderColumnsNames(
		int sheetIndex, Sheet sheet, Workbook workbook )
	{
		return this.headerColumnsNames;
	}

	@Override
	protected SalasImportExcelBean createExcelBean(
		Row header, Row row, int sheetIndex,
		Sheet sheet, Workbook workbook )
	{
		SalasImportExcelBean bean = new SalasImportExcelBean( row.getRowNum() + 1 );

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
					else if ( TIPO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setTipoStr( cellValue );
					}
					else if ( UNIDADE_COLUMN_NAME.equals( columnName ) )
					{
						bean.setCodigoUnidadeStr( cellValue );
					}
					else if ( NUMERO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setNumeroStr( cellValue );
					}
					else if ( DESCRICAO_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setDescricaoStr( cellValue );
					}
					else if ( ANDAR_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setAndarStr( cellValue );
					}
					else if ( CAPACIDADE_INSTALADA_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCapacidadeInstaladaStr( cellValue );
					}
					else if ( CAPACIDADE_MAX_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCapacidadeMaxStr( cellValue );
					}
					else if ( CUSTO_OPERACAO_CRED_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setCustoOperacaoCredStr( cellValue );
					}
					else if ( EXTERNA_COLUMN_NAME.endsWith( columnName ) )
					{
						bean.setExternaStr( cellValue );
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
		return ExcelInformationType.SALAS.getSheetName();
	}
	
	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected void processSheetContent(
		String sheetName, List< SalasImportExcelBean > sheetContent )
	{
		if ( doSyntacticValidation( sheetName, sheetContent )
			&& doLogicValidation( sheetName, sheetContent ) )
		{
			updateDataBase( sheetName, sheetContent );
		}
	}

	private boolean doSyntacticValidation(
		String sheetName, List< SalasImportExcelBean > sheetContent )
	{
		// Map utilizado para associar um erro
		// às linhas do arquivo onde o mesmo ocorre
		// [ ImportExcelError -> Lista de linhas onde o erro ocorre ]
		Map< ImportExcelError, List< Integer > > syntacticErrorsMap
			= new HashMap< ImportExcelError, List< Integer > >();

		for ( SalasImportExcelBean bean : sheetContent )
		{
			List< ImportExcelError > errorsBean
				= bean.checkSyntacticErrors();

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
			List< Integer > linhasComErro
				= syntacticErrorsMap.get( error );

			getErrors().add( error.getMessage(
				linhasComErro.toString(), getI18nMessages() ) );
		}

		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(
		String sheetName, List< SalasImportExcelBean > sheetContent )
	{
		// Verifica se alguma sala apareceu
		// mais de uma vez no arquivo de entrada
		checkUniqueness( sheetContent );

		// verifica se há referência a alguma unidade não cadastrada
		checkNonRegisteredUnidade( sheetContent );

		// verifica se há referência a algum tipo de sala não cadastrado
		checkNonRegisteredTipoSala( sheetContent );
		
		checkCapacidadeInstaladaMaiorCapacidadeMax( sheetContent );

		return getErrors().isEmpty();
	}

	private void checkUniqueness(
		List< SalasImportExcelBean > sheetContent )
	{
		// Map com os códigos das salas e as linhas
		// em que a mesma aparece no arquivo de entrada
		// [ CódigoSala -> Lista de Linhas do Arquivo de Entrada ]
		Map< String, List< Integer > > salaCodigoToRowsMap
			= new HashMap< String, List< Integer > >();

		for ( SalasImportExcelBean bean : sheetContent )
		{
			List< Integer > rows = salaCodigoToRowsMap.get( bean.getCodigoStr() );

			if ( rows == null )
			{
				rows = new ArrayList< Integer >();
				salaCodigoToRowsMap.put( bean.getCodigoStr(), rows );
			}

			rows.add( bean.getRow() );
		}
		
		// Verifica se alguma sala apareceu mais de uma vez no arquivo de entrada
		for ( Entry< String, List< Integer > > entry
			: salaCodigoToRowsMap.entrySet() )
		{
			if ( entry.getValue().size() > 1 )
			{
				getErrors().add( getI18nMessages().excelErroLogicoUnicidadeViolada(
					entry.getKey(), entry.getValue().toString() ) );
			}
		}
	}

	private void checkNonRegisteredUnidade(
		List< SalasImportExcelBean > sheetContent )
	{
		// [ CódigoUnidade -> Unidade ]
		Map< String, Unidade> unidadeBDMap = Unidade.buildUnidadeCodigoToUnidadeMap(
			Unidade.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( SalasImportExcelBean bean : sheetContent )
		{
			Unidade unidade = unidadeBDMap.get(
				bean.getCodigoUnidadeStr() );

			if ( unidade != null )
			{
				bean.setUnidade( unidade );
			}
			else
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(
				UNIDADE_COLUMN_NAME, rowsWithErrors.toString() ) );
		}
	}

	private void checkNonRegisteredTipoSala(
		List< SalasImportExcelBean > sheetContent )
	{
		InstituicaoEnsino instituicaoEnsino = this.getCenario().getInstituicaoEnsino(); 

		// [ NomeTipoSala -> TipoSala ]
		Map< String, TipoSala > tiposSalaBDMap
			= TipoSala.buildTipoSalaNomeToTipoSalaMap(
				TipoSala.findByCenario( instituicaoEnsino, getCenario() ) );

		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( SalasImportExcelBean bean : sheetContent )
		{
			TipoSala tipoSala = tiposSalaBDMap.get( bean.getTipoStr() );

			if ( tipoSala != null )
			{
				bean.setTipo( tipoSala );
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
	
	private void checkCapacidadeInstaladaMaiorCapacidadeMax(
		List< SalasImportExcelBean > sheetContent )
	{
		List< Integer > rowsWithErrors
			= new ArrayList< Integer >();

		for ( SalasImportExcelBean bean : sheetContent )
		{
			if ( bean.getCapacidadeInstalada() > bean.getCapacidadeMax() )
			{
				rowsWithErrors.add( bean.getRow() );
			}
		}

		if ( !rowsWithErrors.isEmpty() )
		{
			getErrors().add( getI18nMessages().excelErroLogicoCapacidadeInstaladaMaiorCapacidadeMax(
					rowsWithErrors.toString() ) );
		}
	}

	@Transactional
	private void updateDataBase( String sheetName,
		List< SalasImportExcelBean > sheetContent )
	{
		Map< String, Sala > salasBDMap = Sala.buildSalaCodigoToSalaMap(
			Sala.findByCenario( this.instituicaoEnsino, getCenario() ) );

		List<Sala> persistedSalas = new ArrayList<Sala>();
		for ( SalasImportExcelBean salaExcel : sheetContent )
		{
			Sala salaBD = salasBDMap.get( salaExcel.getCodigoStr() );

			if ( salaBD != null )
			{
				// Update
				salaBD.setNumero( salaExcel.getNumeroStr() );
				salaBD.setDescricao(salaExcel.getDescricaoStr() );
				salaBD.setAndar( salaExcel.getAndarStr() );
				salaBD.setCapacidadeInstalada( salaExcel.getCapacidadeInstalada() );
				salaBD.setCapacidadeMax( salaExcel.getCapacidadeMax() );
				salaBD.setCustoOperacaoCred( salaExcel.getCustoOperacaoCred() );
				salaBD.setTipoSala( salaExcel.getTipo() );
				salaBD.setUnidade( salaExcel.getUnidade() );
				salaBD.setExterna( salaExcel.getExterna() );

				salaBD.merge();
			}
			else
			{
				// Insert
				Sala newSala = new Sala();

				newSala.setCodigo( salaExcel.getCodigoStr() );
				newSala.setNumero( salaExcel.getNumeroStr() );
				newSala.setDescricao(salaExcel.getDescricaoStr() );
				newSala.setAndar( salaExcel.getAndarStr() );
				newSala.setCapacidadeInstalada( salaExcel.getCapacidadeInstalada() );
				newSala.setCapacidadeMax( salaExcel.getCapacidadeMax() );
				newSala.setCustoOperacaoCred( salaExcel.getCustoOperacaoCred() );
				newSala.setTipoSala( salaExcel.getTipo() );
				newSala.setUnidade( salaExcel.getUnidade() );
				newSala.setExterna( salaExcel.getExterna() );

				newSala.persist();
				persistedSalas.add(newSala);
			}
		}
		
		if (!persistedSalas.isEmpty()) {
			List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(instituicaoEnsino, getCenario());
			Sala.preencheHorariosDasSalas(persistedSalas,semanasLetivas);
		}
	}

	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoSala() );
			TIPO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().tipo() );
			UNIDADE_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoUnidade() );
			NUMERO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().numero() );
			DESCRICAO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().descricao() );
			ANDAR_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().andar() );
			CAPACIDADE_INSTALADA_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().capacidadeInstaladaAlunos() );
			CAPACIDADE_MAX_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().capacidadeMaxAlunos() );
			CUSTO_OPERACAO_CRED_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().custoOperacaoCred() );
			EXTERNA_COLUMN_NAME = HtmlUtils.htmlUnescape( "Ambiente Externo?" );
		}
	}
}
