package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.web.trieda.server.DemandasServiceImpl;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class DemandasExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		NUMBER( 6, 6 ),
		DOUBLE_NUMBER( 6, 9 );

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return this.row;
		}

		public int getCol()
		{
			return this.col;
		}
	}

	private CellStyle[] cellStyles;
	
	private boolean removeUnusedSheets;
	private int initialRow;
	private Map< String, Boolean > mapDemandasExportadas = new HashMap< String, Boolean >();

	public DemandasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public DemandasExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.DEMANDAS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().ofertasEDemandas();
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
		return this.getI18nConstants().ofertasEDemandas();
	}

	@Override
	protected boolean fillInExcel( Workbook workbook )
	{
		boolean result = false;
		
		List< Oferta > listOfertas = Oferta.findByCenario(
				this.instituicaoEnsino, getCenario() );

		Set< Oferta > ofertas
			= new HashSet< Oferta >( listOfertas );

		if ( !ofertas.isEmpty() )
		{
			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles( sheet );
			
			DemandasServiceImpl demandasService = new DemandasServiceImpl();
			ParDTO<Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>>,Integer> pair = demandasService.calculaQuantidadeDeNaoAtendimentosPorDemanda(ofertas);
			Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>> demandaToQtdAlunosNaoAtendidosMap = pair.getPrimeiro();

			int nextRow = this.initialRow;
			for ( Oferta oferta : ofertas )
			{
				nextRow = writeData( oferta, nextRow, sheet, demandaToQtdAlunosNaoAtendidosMap);
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}

		return result;
	}
	
	@Override
	public void resolveHyperlinks(Map<String,Map<String,Map<String,String>>> hyperlinksMap, Workbook workbook) {
		Map<String,Map<String,String>> mapLevel2 = hyperlinksMap.get(ExcelInformationType.DEMANDAS.getSheetName());
		if (mapLevel2 != null && !mapLevel2.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			int nextRow = this.initialRow;
			
			Cell campusCell = getCell(nextRow,2,sheet);
			Cell turnoCell = getCell(nextRow,3,sheet);
			Cell cursoCell = getCell(nextRow,4,sheet);
			Cell curriculoCell = getCell(nextRow,5,sheet);
			Cell periodoCell = getCell(nextRow,6,sheet);
			
			int lastRowNumber = sheet.getLastRowNum()+1;
			while (campusCell != null && nextRow <= lastRowNumber) {
				String key = campusCell.getStringCellValue()+"-"+turnoCell.getStringCellValue()+"-"+cursoCell.getStringCellValue()+"-"+curriculoCell.getStringCellValue()+"-"+((int)periodoCell.getNumericCellValue());
				
				int col = 13;
				for (Entry<String,Map<String,String>> entry : mapLevel2.entrySet()) {
					String cellValue = entry.getKey();
					String cellHyperlink = entry.getValue().get(key);
					if (cellHyperlink != null) {
						setCellWithHyperlink(nextRow,col++,sheet,cellValue,cellHyperlink,true);
					}
				}
				
				nextRow++;
				campusCell = getCell(nextRow,2,sheet);
				turnoCell = getCell(nextRow,3,sheet);
				cursoCell = getCell(nextRow,4,sheet);
				curriculoCell = getCell(nextRow,5,sheet);
				periodoCell = getCell(nextRow,6,sheet);
			}
			
			autoSizeColumns((short)12,(short)12,sheet);
		}		
	}
	
	private int writeData( Oferta oferta, int row, Sheet sheet , Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>> demandaToQtdAlunosNaoAtendidosMap)
	{
		if ( oferta.getDemandas().isEmpty() )
		{
			// Campus
			setCell( row, 2, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				oferta.getCampus().getCodigo() );

			// Turno
			setCell( row, 3, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				oferta.getTurno().getNome() );

			row++;
		}
		else
		{
			// [ CodDisciplina -> Demanda ]
			Map< String, Demanda > demandasMap
				= new HashMap< String, Demanda >();

			for ( Demanda demanda : oferta.getDemandas() )
			{
				demandasMap.put( demanda.getDisciplina().getCodigo(), demanda );
			}

			Curriculo curriculo = oferta.getCurriculo();
			List< Integer > listPeriodos = null;

			if ( curriculo != null )
			{
				listPeriodos = curriculo.getPeriodos();
			}

			if ( listPeriodos != null && listPeriodos.size() > 0 )
			{
				for ( Integer periodo : listPeriodos )
				{
					List< CurriculoDisciplina > disciplinasDeUmPeriodo = curriculo.getCurriculoDisciplinasByPeriodo(periodo);

					if ( disciplinasDeUmPeriodo == null
						|| disciplinasDeUmPeriodo.size() == 0 )
					{
						continue;
					}

					for ( CurriculoDisciplina disciplinaDeUmPeriodo : disciplinasDeUmPeriodo )
					{
						Demanda demanda = demandasMap.get(
							disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

						int quantidade = ( demanda == null ? 0 : demanda.getQuantidade() );

						if ( quantidade == 0 )
						{
							continue;
						}

						// Chegando nesse ponto, temos que mais uma linha será escrita
						// na planilha de exportação. Assim, registramos essa linha
						String key = "";

						key += oferta.getCampus().getCodigo();
						key += "-" + oferta.getTurno().getNome();
						key += "-" + curriculo.getCurso().getCodigo();
						key += "-" + curriculo.getCodigo();
						key += "-" + periodo;
						key += "-" + disciplinaDeUmPeriodo.getDisciplina().getCodigo();

						if ( this.mapDemandasExportadas.containsKey( key ) )
						{
							continue;
						}
						else
						{
							this.mapDemandasExportadas.put( key, true );
						}
						
						ParDTO<Integer,Map<Disciplina,Integer>> par = demandaToQtdAlunosNaoAtendidosMap.get(demanda);
						Integer qtdNaoAtendida = par.getPrimeiro();
						Set<Disciplina> disciplinasSubstitutas = par.getSegundo().keySet();

						// Campus
						setCell( row, 2, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							oferta.getCampus().getCodigo() );

						// Turno
						setCell( row, 3, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							oferta.getTurno().getNome() );

						// Curso
						setCell( row, 4, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							curriculo.getCurso().getCodigo() );

						// Matriz Curricular
						setCell( row, 5, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							curriculo.getCodigo() );

						// Período
						setCell( row, 6, sheet, this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], periodo );

						// Disciplina
						setCell( row, 7, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
							disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

						// Demanda de Alunos
						setCell( row, 8, sheet, this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], quantidade );
						
						// Receita
						setCell( row, 9, sheet, this.cellStyles[ ExcelCellStyleReference.DOUBLE_NUMBER.ordinal() ], oferta.getReceita() );
						
						// Demanda Atendida
						setCell( row, 10, sheet, this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], (quantidade - qtdNaoAtendida) );
						
						// Demanda Não Atendida
						setCell( row, 11, sheet, this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], qtdNaoAtendida );
						
						// Disciplinas Substitutas
						String disciplinasSubstitutasStr = "";
						if (disciplinasSubstitutas != null && !disciplinasSubstitutas.isEmpty()) {
							List<String> codigosDisciplinasSubstitutas = new ArrayList<String>();
							for (Disciplina disciplinaSubistituta : disciplinasSubstitutas) {
								codigosDisciplinasSubstitutas.add(disciplinaSubistituta.getCodigo());
							}
							Collections.sort(codigosDisciplinasSubstitutas);
							StringBuffer disciplinasSubstitutasStrB = new StringBuffer("");
							for (String codigoDisciplinaSubstituta : codigosDisciplinasSubstitutas) {
								disciplinasSubstitutasStrB.append(codigoDisciplinaSubstituta + ", ");
							}
							disciplinasSubstitutasStr = disciplinasSubstitutasStrB.substring(0, disciplinasSubstitutasStrB.length() - 2);
						}
						setCell( row, 12, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], disciplinasSubstitutasStr);
						
						row++;
					}
				}
			}
		}

		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference
			: ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
