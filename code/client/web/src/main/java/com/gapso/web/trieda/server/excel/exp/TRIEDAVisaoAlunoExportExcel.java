package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class TRIEDAVisaoAlunoExportExcel
	extends AbstractExportExcel
{
	public TRIEDAVisaoAlunoExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
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
		return getI18nConstants().trieda() + "VisaoAluno";
	}

	@Override
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< IExportExcel > exporters = new ArrayList< IExportExcel >();

		exporters.add( new DemandasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new AlunosDemandaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		exporters.add( new RelatorioVisaoAlunoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );

		Exception exception = null;
		try {
			// [sheetTarget -> [sheetOrigin -> [key -> link]]]
			Map<String,Map<String,Map<String,String>>> hyperlinksMap = new HashMap<String,Map<String,Map<String,String>>>();
			for (IExportExcel exporter : exporters) {
				getProgressReport().setInitNewPartial("Exportando " + exporter.getFileName());
				//TODO: MEDIÇÃO PERFORMANCE
				double start = System.currentTimeMillis();System.out.print(exporter.getClass().getName());
				exporter.export(workbook, templateWorkbook);
				// se necessário, colhe as informações de hyperlinks
				Map<String,Map<String,Map<String,String>>> localHyperlinksMap = exporter.getHyperlinksMap();
				if (localHyperlinksMap != null && !localHyperlinksMap.isEmpty()) {
					for (Entry<String,Map<String,Map<String,String>>> entry : localHyperlinksMap.entrySet()) {
						String sheetTarget = entry.getKey();
						
						Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(sheetTarget);
						if (mapLevel2 == null) {
							mapLevel2 = new HashMap<String,Map<String,String>>();
							hyperlinksMap.put(sheetTarget,mapLevel2);
						}
						
						for (Entry<String,Map<String,String>> entry2 : entry.getValue().entrySet()) {
							mapLevel2.put(entry2.getKey(),entry2.getValue());
						}
					}
				}
				//TODO: MEDIÇÃO PERFORMANCE
				double time = (System.currentTimeMillis() - start)/1000.0;System.out.println(" tempo = " + time + " segundos");
				getProgressReport().setPartial("Etapa concluída");
			}
			
			// escreve hyperlinks
			getProgressReport().setInitNewPartial("Escrevendo hyperlinks");
			for (IExportExcel exporter : exporters) {
				//TODO: MEDIÇÃO PERFORMANCE
				double start = System.currentTimeMillis();System.out.print(exporter.getClass().getName());
				exporter.resolveHyperlinks(hyperlinksMap,workbook);
				//TODO: MEDIÇÃO PERFORMANCE
				double time = (System.currentTimeMillis() - start)/1000.0;System.out.println(" tempo = " + time + " segundos");
			}
			getProgressReport().setPartial("Etapa concluída");
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
		workbook.removeSheetAt( workbook.getSheetIndex(
			ExcelInformationType.PALETA_CORES.getSheetName() ) );

			return true;
		}
	}
}
