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
import com.gapso.web.trieda.shared.excel.PlanilhasExportExcel;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class TRIEDAExportExcel
	extends AbstractExportExcel
{
	public TRIEDAExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino, String fileExtension,
		Map< String, Boolean > planilhasExportExcel )
	{
		super(false, "", cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
		this.isVisaoProfessor = isVisaoProfessor;
		this.planilhasExportExcel = planilhasExportExcel;
	}

	private boolean isVisaoProfessor;
	private Map<String, Boolean> planilhasExportExcel;

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
		return getI18nConstants().trieda();
	}

	@Override
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< IExportExcel > exporters = new ArrayList< IExportExcel >();

		if( planilhasExportExcel.get(PlanilhasExportExcel.CAMPI) )
			exporters.add( new CampiExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.UNIDADES) )
			exporters.add( new UnidadesExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.SALAS) )
			exporters.add( new SalasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.CURSOS) )
			exporters.add( new CursosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.AREAS_TITULACAO) )
			exporters.add( new AreasTitulacaoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.CURSO_AREAS_TITULACAO) )
			exporters.add( new CursoAreasTitulacaoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.DISCIPLINAS) )
			exporters.add( new DisciplinasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.DISCIPLINAS_SALAS) )
			exporters.add( new DisciplinasSalasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.EQUIVALENCIAS) )
			exporters.add( new EquivalenciasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.CAMPI_TRABALHO) )
			exporters.add( new CampiTrabalhoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.CURRICULOS) )
			exporters.add( new CurriculosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.DEMANDAS) )
			exporters.add( new DemandasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.ALUNOS) )
			exporters.add( new AlunosExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.DEMANDAS_POR_ALUNO) )
			exporters.add( new AlunosDemandaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.PROFESSORES) )
			exporters.add( new ProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.DISPONIBILIDADES_PROFESSORES) )
			exporters.add( new DisponibilidadesProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.HABILITACAO_PROFESSORES) )
			exporters.add( new HabilitacoesProfessoresExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.RESUMO_CURSO) )
			exporters.add( new ResumoCursoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.RESUMO_DISCIPLINA) )
			exporters.add( new ResumoDisciplinaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.ATENDIMENTOS_MATRICULA) )
			exporters.add( new AtendimentosMatriculaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.ATENDIMENTOS_DISCIPLINA) )
			exporters.add( new AtendimentosDisciplinaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.ATENDIMENTOS_FAIXA_DEMANDA) )
			exporters.add( new AtendimentosFaixaDemandaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.RELATORIO_VISAO_SALA) )
			exporters.add( new RelatorioVisaoSalaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.RELATORIO_VISAO_PROFESSOR) )
			exporters.add( new RelatorioVisaoProfessorExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.isVisaoProfessor(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.RELATORIO_VISAO_CURSO) )
			exporters.add( new RelatorioVisaoCursoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.RELATORIO_VISAO_ALUNO) )
			exporters.add( new RelatorioVisaoAlunoExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.ATENDIMENTOS_POR_ALUNO) )
			exporters.add( new AtendimentosPorSalaExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );
		if( planilhasExportExcel.get(PlanilhasExportExcel.AULAS) )
			exporters.add( new AulasExportExcel( false, getCenario(), getI18nConstants(), getI18nMessages(), this.instituicaoEnsino, fileExtension ) );

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
		
		//Removendo planilhas nao utilizadas
		List< String > nomesPlanilhas = new ArrayList< String >();
		for(IExportExcel i : exporters) {
			nomesPlanilhas.add( ((AbstractExportExcel) i).getSheetName() );
		}
		removeUnusedSheets(nomesPlanilhas, workbook);

			return true;
		}
	}
}
