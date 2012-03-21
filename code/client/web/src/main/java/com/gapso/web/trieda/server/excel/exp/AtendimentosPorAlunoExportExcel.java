package com.gapso.web.trieda.server.excel.exp;

import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.web.trieda.server.DemandasServiceImpl;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AtendimentosPorAlunoExportExcel extends AbstractExportExcel {
	enum ExcelCellStyleReference {
		TEXT(7,2),
		NUMBER(7,6);

		private int row;
		private int col;

		private ExcelCellStyleReference(int row, int col) {
			this.row = row;
			this.col = col;
		}

		public int getRow() {
			return row;
		}

		public int getCol() {
			return col;
		}
	}

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public AtendimentosPorAlunoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino) {
		this(true,cenario,i18nConstants,i18nMessages,instituicaoEnsino);
	}

	public AtendimentosPorAlunoExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino) {
		super(true,ExcelInformationType.ATENDIMENTOS_POR_ALUNO.getSheetName(),cenario,i18nConstants,i18nMessages,instituicaoEnsino);

		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 7;
	}

	@Override
	public String getFileName() {
		return "Atendimentos por Aluno";
	}

	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return "Atendimentos por Aluno";
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		List<Oferta> ofertas = Oferta.findByCenario(instituicaoEnsino,getCenario());
		DemandasServiceImpl service = new DemandasServiceImpl();
		Map<Demanda,Map<AtendimentoTatico,List<Aluno>>> demandaToAlunosPorAtendimentosMap = service.alocaAlunosNosAtendimentos(ofertas);

		if (!demandaToAlunosPorAtendimentosMap.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(),workbook);
			}

			HSSFSheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);

			int nextRow = this.initialRow;
			for (Entry<Demanda,Map<AtendimentoTatico,List<Aluno>>> entryDemanda : demandaToAlunosPorAtendimentosMap.entrySet()) {
				Demanda demanda = entryDemanda.getKey();
				for (Entry<AtendimentoTatico,List<Aluno>> entryAtendimento : entryDemanda.getValue().entrySet()) {
					AtendimentoTatico atendimento = entryAtendimento.getKey();
					for (Aluno aluno : entryAtendimento.getValue()) {
						nextRow = writeData(demanda,atendimento,aluno,nextRow,sheet);
					}
				}
			}

			return true;
		}

		return false;
	}

	private int writeData(Demanda demanda, AtendimentoTatico atendimento, Aluno aluno, int row, HSSFSheet sheet) {
		Oferta oferta = demanda.getOferta();
		Curriculo curriculo = oferta.getCurriculo();
		
		// Código Campus
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],oferta.getCampus().getCodigo());
		// Turno
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],oferta.getTurno().getNome());
		// Código Curso
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],oferta.getCurso().getCodigo());
		// Matriz Curricular
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curriculo.getCodigo());
		// Período
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],curriculo.getPeriodo(demanda.getDisciplina()));
		// Código Disciplina
		setCell(row,7,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],demanda.getDisciplina().getCodigo());
		// Demanda de Alunos
		setCell(row,8,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],demanda.getQuantidade());
		
		// Créditos Práticos
		setCell(row,9,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],atendimento.getCreditosPratico());
		// Créditos Teóricos
		setCell(row,10,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],atendimento.getCreditosTeorico());
		// Qtd Atendida
		setCell(row,11,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],atendimento.getQuantidadeAlunos());
		// Dia da Semana
		setCell(row,12,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],atendimento.getSemana().name());
		// Turma
		setCell(row,13,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],atendimento.getTurma());
		// Sala
		setCell(row,14,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],atendimento.getSala().getCodigo());
		// Disciplina Substituta
		setCell(row,15,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],(atendimento.getDisciplinaSubstituta() != null ? atendimento.getDisciplinaSubstituta().getCodigo() : ""));
		
		// Matrícula
		setCell(row,16,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],aluno.getMatricula());
		// Nome
		setCell(row,17,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],aluno.getNome());
		
		row++;
		return row;
	}

	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}