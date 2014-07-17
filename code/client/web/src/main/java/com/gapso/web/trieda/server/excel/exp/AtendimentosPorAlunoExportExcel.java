package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.web.trieda.server.util.Atendimento;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
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

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public AtendimentosPorAlunoExportExcel(Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		this(true,cenario,i18nConstants,i18nMessages,instituicaoEnsino, fileExtension);
	}

	public AtendimentosPorAlunoExportExcel(boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
			InstituicaoEnsino instituicaoEnsino, String fileExtension)
	{
		super(true,ExcelInformationType.ATENDIMENTOS_POR_ALUNO.getSheetName(),cenario,i18nConstants,i18nMessages,instituicaoEnsino, fileExtension);

		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 7;
	}

	@Override
	public String getFileName() {
		return "Atendimentos por Aluno";
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
		return "Atendimentos por Aluno";
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook) {
		List<Oferta> ofertas = Oferta.findByCenario(instituicaoEnsino,getCenario());
		Map<Demanda,Map<Atendimento,List<Aluno>>> demandaToAlunosPorAtendimentosMap = getMapDemandaToAlunosPorAtendimento(ofertas);

		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		if (!demandaToAlunosPorAtendimentosMap.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			
			int nextRow = this.initialRow;
			for(Demanda demanda: demandaToAlunosPorAtendimentosMap.keySet()){
				Map<Atendimento,List<Aluno>> atendimentosMap = demandaToAlunosPorAtendimentosMap.get(demanda);
				for(Atendimento atendimento : atendimentosMap.keySet()){
					for(Aluno aluno : atendimentosMap.get(atendimento)){
						nextRow = writeData(demanda,atendimento,aluno,nextRow,sheet);
					}
				}
			}

			return true;
		}

		return false;
	}
	
	private Map<Demanda, Map<Atendimento, List<Aluno>>> getMapDemandaToAlunosPorAtendimento(List<Oferta> ofertas){
		Map<Demanda, Map<Atendimento, List<Aluno>>> demandaToAlunosPorAtendimentosMap = new HashMap<Demanda, Map<Atendimento, List<Aluno>>>();
		
		for(Oferta oferta: ofertas){
			Set<Demanda> demandas = oferta.getDemandas();
			for(Demanda demanda : demandas){
				Map<Atendimento, List<Aluno>> atendimentosMap = demandaToAlunosPorAtendimentosMap.get(demanda);
				if(atendimentosMap == null){
					atendimentosMap = new HashMap<Atendimento,List<Aluno>>();
					demandaToAlunosPorAtendimentosMap.put(demanda, atendimentosMap);
				}
				
				List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByDemanda(instituicaoEnsino, demanda);
				
				for(AlunoDemanda alunoDemanda : alunosDemanda){
					Set<AtendimentoTatico> atts = alunoDemanda.getAtendimentosTatico();
					List<Aluno> alunosAtendimento;
					for(AtendimentoTatico att : atts){
						Atendimento at = new Atendimento(att);
						alunosAtendimento = atendimentosMap.get(at);
						if(alunosAtendimento == null){
							alunosAtendimento = new ArrayList<Aluno>();
							atendimentosMap.put(at, alunosAtendimento);
						}
						alunosAtendimento.add(alunoDemanda.getAluno());
					}
					
					Set<AtendimentoOperacional> atops = alunoDemanda.getAtendimentosOperacional();
					for(AtendimentoOperacional atop : atops){
						Atendimento at = new Atendimento(atop);
						alunosAtendimento = atendimentosMap.get(at);
						if(alunosAtendimento == null){
							alunosAtendimento = new ArrayList<Aluno>();
							atendimentosMap.put(at, alunosAtendimento);
						}
						alunosAtendimento.add(alunoDemanda.getAluno());
					}
				}
			}
		}
		
		return demandaToAlunosPorAtendimentosMap;
	}

	private int writeData(Demanda demanda, Atendimento atendimento, Aluno aluno, int row, Sheet sheet) {
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
		setCell(row,9,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],atendimento.getCreditosPraticos());
		// Créditos Teóricos
		setCell(row,10,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],atendimento.getCreditosTeoricos());
		// Qtd Atendida
		setCell(row,11,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],atendimento.getQuantidadeAlunos());
		// Dia da Semana
		setCell(row,12,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],atendimento.getDiaSemana().name());
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

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}