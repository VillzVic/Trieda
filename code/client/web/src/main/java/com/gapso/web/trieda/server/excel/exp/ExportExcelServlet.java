package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.teklabs.gwt.i18n.client.LocaleFactory;
import com.teklabs.gwt.i18n.server.LocaleProxy;

public class ExportExcelServlet extends HttpServlet {
	
	private static final long serialVersionUID = -7987228694777660184L;
	
	private static TriedaI18nMessages i18nMessages = null;
	private static TriedaI18nConstants i18nConstants = null;
	private static Cenario cenario = null;

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		LocaleProxy.initialize();
		i18nMessages = LocaleFactory.get(TriedaI18nMessages.class);
		i18nConstants = LocaleFactory.get(TriedaI18nConstants.class);
		cenario = Cenario.findMasterData();
		
		// Obtém os parâmetros
		String informationToBeExported = request.getParameter(ExcelInformationType.getInformationParameterName());
		if (!informationToBeExported.isEmpty()) {
			// Get Excel Data
			IExportExcel exporter = ExportExcelFactory.createExporter(informationToBeExported,cenario,i18nConstants,i18nMessages);			
			HSSFWorkbook workbook = exporter.export();
			
			if (exporter.getErrors().isEmpty()) {
				// Write data on response output stream
				writeExcelToHttpResponse(exporter.getFileName(),workbook,response);
			} else {
				response.setContentType("text/html");
				for (String msg : exporter.getWarnings()) {
					response.getWriter().println(ExcelInformationType.prefixWarning()+msg);
				}
				for (String msg : exporter.getErrors()) {
					response.getWriter().println(ExcelInformationType.prefixError()+msg);
				}
				response.getWriter().flush();
			}
		}
	}
	
	private void writeExcelToHttpResponse(String excelFileName, HSSFWorkbook excel, HttpServletResponse response) throws IOException {
		response.setContentType("application/vnd.ms-excel");  
		response.setHeader("Content-disposition","attachment; filename="+excelFileName+".xls");
		ServletOutputStream out = null;
		try {
			out = response.getOutputStream();
			excel.write(out);
			out.flush();
		} catch (IOException e) {
			throw e;
		} finally {
			if (out != null) {
		        out.close();
			}
		}
	}
}