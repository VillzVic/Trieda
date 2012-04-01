package com.gapso.web.trieda.server.excel.imp;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationImpl;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class TRIEDAImportExcel 
	extends ProgressDeclarationImpl
	implements IImportExcel
{
	protected List< String > errors;
	protected List< String > warnings;
	private Cenario cenario;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	private InstituicaoEnsino instituicaoEnsino;

	protected TRIEDAImportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		this.instituicaoEnsino = instituicaoEnsino;
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;

		this.errors = new ArrayList< String >();
		this.warnings = new ArrayList< String >();
	}

	@Override
	public boolean load(
		String fileName, HSSFWorkbook workbook )
	{
		return false;
	}

	@Override
	public boolean load(String fileName, InputStream inputStream )
	{
		boolean flag = true;
		try
		{
			POIFSFileSystem poifs = new POIFSFileSystem( inputStream );
			HSSFWorkbook workbook = new HSSFWorkbook( poifs );

			List< IImportExcel > importers = new ArrayList< IImportExcel >();

			importers.add( new CampiImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new UnidadesImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new SalasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisciplinasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new AreasTitulacaoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CursosImportExcel( this.cenario, i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CursoAreasTitulacaoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CurriculosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisciplinasSalasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new EquivalenciasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DemandasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new AlunosDemandaImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new ProfessoresImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CampiTrabalhoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new HabilitacoesProfessoresImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );

			for ( IImportExcel importer : importers )
			{
				getProgressReport().setInitNewPartial("Importando " + importer.getSheetName());
				flag = ( importer.load( fileName, workbook ) && flag );
				getProgressReport().setPartial("Etapa concluída");

				for ( String error : importer.getErrors() )
				{
					getErrors().add( importer.getSheetName() + ": " + error );
				}

				for ( String warning : importer.getWarnings() )
				{
					getErrors().add( importer.getSheetName() + ": " + warning );
				}
			}
		}
		catch ( Exception e ) {
			e.printStackTrace();
			flag = false;
			String errorMessage = TriedaUtil.extractMessage(e);
			getErrors().add(getI18nMessages().excelErroGenericoImportacao(errorMessage));
		}

		return flag;
	}

	@Override
	public List< String > getErrors()
	{
		return this.errors;
	}

	@Override
	public List< String > getWarnings()
	{
		return this.warnings;
	}

	protected Cenario getCenario()
	{
		return this.cenario;
	}

	protected TriedaI18nConstants getI18nConstants()
	{
		return this.i18nConstants;
	}

	protected TriedaI18nMessages getI18nMessages()
	{
		return this.i18nMessages;
	}

	@Override
	public String getSheetName()
	{
		return ExcelInformationType.TUDO.getSheetName();
	}

}
