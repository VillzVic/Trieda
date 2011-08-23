package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class TRIEDAExportExcel
	extends AbstractExportExcel
{
	public TRIEDAExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor )
	{
		super( cenario, i18nConstants, i18nMessages );
		
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
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().trieda();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< IExportExcel > exporters = new ArrayList< IExportExcel >();

		exporters.add( new CampiExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new UnidadesExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new SalasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new CursosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new AreasTitulacaoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new CursoAreasTitulacaoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new DisciplinasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new DisciplinasSalasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new EquivalenciasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new CampiTrabalhoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new CurriculosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new DemandasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new ProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new HabilitacoesProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new ResumoCursoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new ResumoDisciplinaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new RelatorioVisaoSalaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );
		exporters.add( new RelatorioVisaoProfessorExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.isVisaoProfessor() ) );
		exporters.add( new RelatorioVisaoCursoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages() ) );

		for ( IExportExcel exporter : exporters )
		{
			exporter.export( workbook );
		}

		// Código relacionado à issue
		// ISSUE http://jira.gapso.com.br/browse/TRIEDA-1041
		workbook.removeSheetAt( workbook.getSheetIndex(
			ExcelInformationType.PALETA_CORES.getSheetName() ) );

		return true;
	}
}
