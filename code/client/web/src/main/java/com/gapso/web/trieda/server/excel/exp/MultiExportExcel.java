package com.gapso.web.trieda.server.excel.exp;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class MultiExportExcel extends AbstractExportExcel {
	
	private Class<? extends IExportExcel>[] arrayExporters;
	
	public MultiExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino, 
		Class<? extends IExportExcel>[] arrayExporters, String fileExtension) {
		super(false, "", cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension);
		this.arrayExporters = arrayExporters;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().trieda();
	}

	@Override
	protected String getPathExcelTemplate() {
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().trieda();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook) {
		List<IExportExcel> exporters = new ArrayList<IExportExcel>();
		for (Class<? extends IExportExcel> c : this.arrayExporters) {
			Constructor<? extends IExportExcel> constructor;
			try {
				constructor = c.getConstructor(new Class[]{boolean.class,Cenario.class,TriedaI18nConstants.class,TriedaI18nMessages.class,InstituicaoEnsino.class});
				exporters.add((IExportExcel)constructor.newInstance(false,getCenario(),getI18nConstants(),getI18nMessages(),this.instituicaoEnsino));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		Exception exception = null;
		try {
			// [sheetTarget -> [sheetOrigin -> [key -> link]]]
			Map<String, Map<String, Map<String, String>>> hyperlinksMap = new HashMap<String, Map<String, Map<String, String>>>();
			for (IExportExcel exporter : exporters) {
				// TODO: MEDIÇÃO PERFORMANCE
				double start = System.currentTimeMillis();
				System.out.print(exporter.getClass().getName());
				exporter.export(workbook);
				// se necessário, colhe as informações de hyperlinks
				Map<String, Map<String, Map<String, String>>> localHyperlinksMap = exporter
						.getHyperlinksMap();
				if (localHyperlinksMap != null && !localHyperlinksMap.isEmpty()) {
					for (Entry<String, Map<String, Map<String, String>>> entry : localHyperlinksMap
							.entrySet()) {
						String sheetTarget = entry.getKey();

						Map<String, Map<String, String>> mapLevel2 = hyperlinksMap
								.get(sheetTarget);
						if (mapLevel2 == null) {
							mapLevel2 = new HashMap<String, Map<String, String>>();
							hyperlinksMap.put(sheetTarget, mapLevel2);
						}

						for (Entry<String, Map<String, String>> entry2 : entry
								.getValue().entrySet()) {
							mapLevel2.put(entry2.getKey(), entry2.getValue());
						}
					}
				}
				// TODO: MEDIÇÃO PERFORMANCE
				double time = (System.currentTimeMillis() - start) / 1000.0;
				System.out.println(" tempo = " + time + " segundos");
			}

			// escreve hyperlinks
			for (IExportExcel exporter : exporters) {
				// TODO: MEDIÇÃO PERFORMANCE
				double start = System.currentTimeMillis();
				System.out.print(exporter.getClass().getName());
				exporter.resolveHyperlinks(hyperlinksMap, workbook);
				// TODO: MEDIÇÃO PERFORMANCE
				double time = (System.currentTimeMillis() - start) / 1000.0;
				System.out.println(" tempo = " + time + " segundos");
			}
		} catch (Exception e) {
			e.printStackTrace();
			exception = e;
		}

		if (exception != null) {

			this.errors.add(getI18nMessages().excelErroGenericoExportacao(
					exception.toString()));
			return false;
		} else {
			// Código relacionado à issue
			// ISSUE http://jira.gapso.com.br/browse/TRIEDA-1041
			workbook.removeSheetAt(workbook
					.getSheetIndex(ExcelInformationType.PALETA_CORES
							.getSheetName()));

			return true;
		}
	}
}
