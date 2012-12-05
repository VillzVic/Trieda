package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SextetoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AtendimentosPorSalaExportExcel extends AbstractExportExcel {
	enum ExcelCellStyleReference {
		TEXT(7,2),
		NUMBER(7,8);

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

	public AtendimentosPorSalaExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino) {
		this(true,cenario,i18nConstants,i18nMessages,instituicaoEnsino);
	}

	public AtendimentosPorSalaExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino) {
		super(true,ExcelInformationType.ATENDIMENTOS_POR_ALUNO.getSheetName(),cenario,i18nConstants,i18nMessages,instituicaoEnsino);

		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
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
		RelatorioVisaoSalaExportExcel visaoSalaExpExcel = new RelatorioVisaoSalaExportExcel(false,getCenario(),getI18nConstants(),getI18nMessages(),this.instituicaoEnsino) {
			@Override
			protected int writeHeader(List<List<ParDTO<String,?>>> rowsHeadersPairs, int row, boolean ehTatico) {
				return row;
			}
			
			@Override
			protected int writeAulas(List<AtendimentoRelatorioDTO> aulas, int row, int mdcTemposAula, boolean ehTatico, List<String> labelsDasLinhasDaGradeHoraria) {
				// agrupa as aulas por dia da semana e coleta disciplinas
				Map<Integer,List<AtendimentoRelatorioDTO>> colunaGradeHorariaToAulasMap = new HashMap<Integer,List<AtendimentoRelatorioDTO>>();
				for(AtendimentoRelatorioDTO aula : aulas){
					List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(aula.getSemana());
					if(aulasDoDia == null){
						aulasDoDia = new ArrayList<AtendimentoRelatorioDTO>();
						colunaGradeHorariaToAulasMap.put(aula.getSemana(), aulasDoDia);
					}
					aulasDoDia.add(aula);
				}

				// para cada dia da semana, escreve as aulas no excel
				for (Integer colunaGradeHoraria : colunaGradeHorariaToAulasMap.keySet()) {
					List<AtendimentoRelatorioDTO> aulasDoDia = colunaGradeHorariaToAulasMap.get(colunaGradeHoraria);
					if(aulasDoDia == null || aulasDoDia.isEmpty()) continue;

					// para cada aula
					for (AtendimentoRelatorioDTO aula : aulasDoDia) {
						String horarioInicio = "N/A";
						String horarioFim = "N/A";
						
						// obtém a qtd de linhas que devem ser desenhadas para cada crédito da aula em questão
						int linhasDeExcelPorCreditoDaAula = aula.getDuracaoDeUmaAulaEmMinutos() / mdcTemposAula;
						
						// calcula horario de inicio e fim
						if(!ehTatico){
							AtendimentoOperacionalDTO aulaOp = (AtendimentoOperacionalDTO) aula;
							horarioInicio = aulaOp.getHorarioString();
							int index = labelsDasLinhasDaGradeHoraria.indexOf(horarioInicio);
							if (index != -1) {
								index = index + aula.getTotalCreditos() * linhasDeExcelPorCreditoDaAula;
								horarioFim = labelsDasLinhasDaGradeHoraria.get(index);
							}
						}
						
						// para cada aluno na aula
						for (AlunoDemandaDTO alunoDTO : aula.getAlunosDemandas()) {
							writeCells(row,alunoDTO,aula,horarioInicio,horarioFim);
							row++;
						}
					}
				}
				
				return row;
			}
			
			private void writeCells(int row, AlunoDemandaDTO alunoDTO, AtendimentoRelatorioDTO aula, String horarioInicio, String horarioFim) {
				int column = 2;
				// Sala
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getSalaString());
				// Dia Semana
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],Semanas.get(aula.getDiaSemana()).name());
				// Horário Início
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],horarioInicio);
				// Horário Fim
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],horarioFim);
				// Código Disciplina
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getDisciplinaString());
				// Disciplina Substituta
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getDisciplinaSubstitutaString());
				// Turma
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],aula.getTurma());
				// Professor
				String professorCPF = (aula.getProfessorId() != null) ? aula.getProfessorCPF() : aula.getProfessorVirtualCPF();
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],professorCPF);
				// Aula - Créditos Teóricos
				int credT = (aula.isTeorico() ? aula.getTotalCreditos() : 0);
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credT);
				// Aula - Créditos Práticos
				int credP = (aula.isTeorico() ? 0 : aula.getTotalCreditos());
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credP);
				// Aula - Total
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credT+credP);
				// Aula - Carga Horária Teórica
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credT*aula.getSemanaLetivaTempoAula());
				// Aula - Carga Horária Prática
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],credP*aula.getSemanaLetivaTempoAula());
				// Aula - Carga Horária Total
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.NUMBER.ordinal()],(credT+credP)*aula.getSemanaLetivaTempoAula());
				
				// Aluno - Matrícula
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],alunoDTO.getAlunoMatricula());
				// Aluno - Prioridade
				setCell(row,column++,sheet,AtendimentosPorSalaExportExcel.this.cellStyles[AtendimentosPorSalaExportExcel.ExcelCellStyleReference.TEXT.ordinal()],alunoDTO.getAlunoPrioridade());
			}
		}; 
		
		List<SextetoDTO<Integer,Integer,Integer,List<AtendimentoRelatorioDTO>,List<Integer>,List<String>>> structureReportControl = visaoSalaExpExcel.getStructureReportControl();
		
		if (visaoSalaExpExcel.getAtendimentosRelatorioDTOList(structureReportControl)) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(),workbook);
			}
			
			visaoSalaExpExcel.sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(visaoSalaExpExcel.sheet);
			visaoSalaExpExcel.buildColorPaletteCellStyles(workbook);
			visaoSalaExpExcel.initialRow = 7;
			visaoSalaExpExcel.processStructureReportControl(structureReportControl);
			
			return true;
		}
		
		return false;
	}

	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}