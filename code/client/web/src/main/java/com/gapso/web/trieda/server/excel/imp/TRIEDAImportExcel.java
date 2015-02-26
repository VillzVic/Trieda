package com.gapso.web.trieda.server.excel.imp;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;

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
		String fileName, Workbook workbook )
	{
		return false;
	}

	@Override
	public boolean load(String fileName, InputStream inputStream )
	{
		boolean flag = true;
		try
		{
			Workbook workbook = WorkbookFactory.create( inputStream );
			if (checkAnyValidSheet(workbook))
			{
				this.errors.add("Não foi encontrada nenhuma aba válida para a importação");
				return false;
			}

			List< IImportExcel > importers = new ArrayList< IImportExcel >();

			importers.add( new TurnosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new SemanaLetivaImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new SemanaLetivaHorariosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CampiImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new UnidadesImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new UnidadesDeslocamentoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new SalasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisponibilidadesSalasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisciplinasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new AreasTitulacaoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new TiposCursoImportExcel( this.cenario, i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CursosImportExcel( this.cenario, i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CursoAreasTitulacaoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CurriculosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino, true ) );
			importers.add( new DisponibilidadesDisciplinasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisciplinasSalasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new EquivalenciasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new OfertasCursosCampiImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DemandasImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino, false ) );
			importers.add( new AlunosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new AlunosDemandaImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new ProfessoresImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisponibilidadesProfessoresImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new CampiTrabalhoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new HabilitacoesProfessoresImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DivisoesCreditoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DivisoesCreditoDisciplinaImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino ) );
			importers.add( new DisciplinasPreRequisitosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino) );
			importers.add( new DisciplinasCoRequisitosImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino) );
			importers.add( new AtendimentoImportExcel( this.cenario, this.i18nConstants, this.i18nMessages, this.instituicaoEnsino) );
			
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
	
	private boolean checkAnyValidSheet(Workbook workbook) {
		boolean nenhumaAbaValida = true;
		for ( int sheetIndex = 0; sheetIndex < workbook.getNumberOfSheets(); sheetIndex++ )
		{
			for (ExcelInformationType value : ExcelInformationType.values())
			{
				if (value.getSheetName().equals(workbook.getSheetName( sheetIndex )))
				{
					nenhumaAbaValida = false;
				}
			}
		}
		return nenhumaAbaValida;
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
