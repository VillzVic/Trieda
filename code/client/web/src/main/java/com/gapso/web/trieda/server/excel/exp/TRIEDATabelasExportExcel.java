package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class TRIEDATabelasExportExcel
	extends AbstractExportExcel
{
	public TRIEDATabelasExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super(false, "", cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
		this.isVisaoProfessor = isVisaoProfessor;
	}

	private boolean isVisaoProfessor;

	public boolean isVisaoProfessor()
	{
		return isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().trieda();
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
		return getI18nConstants().trieda() + "Tabelas";
	}

	@Override
	protected boolean fillInExcel( Workbook workbook )
	{
		List< IExportExcel > exporters = new ArrayList< IExportExcel >();

		exporters.add( new CampiExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new UnidadesExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new SalasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new CursosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new AreasTitulacaoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new CursoAreasTitulacaoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new DisciplinasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new DisciplinasSalasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new EquivalenciasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new CampiTrabalhoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new CurriculosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new DemandasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new AlunosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new AlunosDemandaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new ProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new HabilitacoesProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new ResumoCursoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new ResumoDisciplinaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new AtendimentosPorSalaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new AulasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );

		Exception exception = null;
		try {
			for (IExportExcel exporter : exporters) {
				getProgressReport().setInitNewPartial("Exportando " + exporter.getFileName());
				//TODO: MEDIÇÃO PERFORMANCE
				double start = System.currentTimeMillis();System.out.print(exporter.getClass().getName());
				exporter.export(workbook);
				//TODO: MEDIÇÃO PERFORMANCE
				double time = (System.currentTimeMillis() - start)/1000.0;System.out.println(" tempo = " + time + " segundos");
				getProgressReport().setPartial("Etapa concluída");
			}
		} catch (Exception e) {
			e.printStackTrace();
			exception = e;
		}
		
		if (exception != null) {
			this.errors.add(getI18nMessages().excelErroGenericoExportacao(exception.toString()));
			return false;
		} else {
			// Código relacionado à issue
			// ISSUE http://jira.gapso.com.br/browse/TRIEDA-1041
			workbook.removeSheetAt( workbook.getSheetIndex(ExcelInformationType.PALETA_CORES.getSheetName() ) );
			return true;
		}
	}
}
