package com.gapso.web.trieda.server.excel;

import java.io.IOException;
import java.io.InputStream;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;

public class ExportExcelServlet extends HttpServlet {
	
	public static enum ExportedInformationType {
		UNIDADES;
		
		public static String getParameterName() {
			return "exportedInformationType";
		}
	}

	private static final long serialVersionUID = -7987228694777660184L;

	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		boolean success = false;
		
		// Obtém os parâmetros
		String exportedInformationType = request.getParameter(ExportedInformationType.getParameterName());
		
		if (!exportedInformationType.isEmpty()) {
			// Get Excel Data
			HSSFWorkbook excel = generateExcel();// = new ExportService().generateExcelReport();

			// Write data on response output stream
			writeExcelToHttpResponse("arq_teste",excel,response);
			
			success = true;
		} else {
			success = false;
		}
		
		if (!success) {
			response.setContentType("text/plain");
			response.getWriter().print("An error has occured");// TODO: incluir mensagem de erro
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
	
	private HSSFWorkbook generateExcel() throws IOException {
		return getExcelTemplate("/templateTagHorimetro.xls");
	}
	
	private HSSFWorkbook getExcelTemplate(String caminhoExcelTemplate) throws IOException {
		final InputStream inTemplate = ExportExcelServlet.class.getResourceAsStream(caminhoExcelTemplate);
		HSSFWorkbook workBook = null;
		try {
			workBook = new HSSFWorkbook(inTemplate);
		} catch (IOException e) {
			throw e;
		} finally {
			if (inTemplate != null) {
				inTemplate.close();
			}
		}
		return workBook;
	}
}