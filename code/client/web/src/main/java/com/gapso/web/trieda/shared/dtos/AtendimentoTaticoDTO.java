package com.gapso.web.trieda.shared.dtos;

import java.util.ArrayList;
import java.util.List;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public class AtendimentoTaticoDTO
	extends AbstractAtendimentoRelatorioDTO< String >
	implements AtendimentoRelatorioDTO, Comparable< AtendimentoTaticoDTO >
{
	private static final long serialVersionUID = -2870302894382757778L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_UNIDADE_ID = "unidadeId";
	public static final String PROPERTY_UNIDADE_STRING = "unidadeString";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_DIA_SEMANA = "semana";
	public static final String PROPERTY_DIA_DA_SEMANA = "diaSemana";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_CURSO_NOME = "cursoNome";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_PERIODO_STRING = "periodoString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_DISCIPLINA_NOME = "disciplinaNome";
	public static final String PROPERTY_DISCIPLINA_USA_LABORATORIO = "disciplinaUsaLaboratorio";
	public static final String PROPERTY_DISCIPLINA_SUBSTITUTA_ID = "disciplinaSubstitutaId";
	public static final String PROPERTY_DISCIPLINA_SUBSTITUTA_STRING = "disciplinaSubstitutaString";
	public static final String PROPERTY_DISCIPLINA_SUBSTITUTA_NOME = "disciplinaSubstitutaNome";
	public static final String PROPERTY_DISCIPLINA_SUBSTITUTA_SEMANA_LETIVA_ID = "disciplinaSubstitutaSemanaLetivaId";
	public static final String PROPERTY_DISCIPLINA_SUBSTITUTA_SEMANA_LETIVA_TEMPO_AULA = "disciplinaSubstitutaSemanaLetivaTempoAula";
	public static final String PROPERTY_TOTAL_CRETIDOS_TEORICOS_DISCIPLINA = "totalCreditosTeoricosDisciplina";
	public static final String PROPERTY_TOTAL_CRETIDOS_PRATICOS_DISCIPLINA = "totalCreditosPraticosDisciplina";
	public static final String PROPERTY_TOTAL_CRETIDOS_DISCIPLINA = "totalCreditoDisciplina";
	public static final String PROPERTY_TOTAL_CRETIDOS_DISCIPLINA_SUBSTITUTA = "totalCreditoDisciplinaSubstituta";
	public static final String PROPERTY_OFERTA_ID = "ofertaId";
	public static final String PROPERTY_TURMA = "turma";
	public static final String PROPERTY_TURNO_ID = "turmaId";
	public static final String PROPERTY_TURNO_STRING = "turmaString";
	public static final String PROPERTY_QUANTIDADE_ALUNOS = "quantidadeAlunos";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_STRING = "quantidadeAlunosString";
	public static final String PROPERTY_NOMES_ALUNOS = "nomesAlunos";
	public static final String PROPERTY_CREDITOS_TEORICOS = "creditosTeorico";
	public static final String PROPERTY_CREDITOS_PRATICOS = "creditosPratico";
	public static final String PROPERTY_COMPARTILHAMENTO_CURSOS = "compartilhamentoCursos";
	public static final String PROPERTY_SEMANA_LETIVA_ID = "semanaLetivaId";
	public static final String PROPERTY_SEMANA_LETIVA_TEMPO_AULA = "semanaLetivaTempoAula";
	public static final String PROPERTY_PROFESSOR_CUSTO_CREDITO_SEMANAL = "professorCustoCreditoSemanal";
	public static final String PROPERTY_QTD_DEMANDA_ALUNOS_P1 = "qtdDemandaAlunosP1";
	public static final String PROPERTY_QTD_DEMANDA_ALUNOS_P2 = "qtdDemandaAlunosP2";
	public static final String PROPERTY_QTD_DEMANDA_ALUNOS_TOTAL = "qtdDemandaAlunosTotal";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_P1 = "quantidadeAlunosP1";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_P2 = "quantidadeAlunosP2";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCPF";
	public static final String PROPERTY_PROFESSOR_VIRTUAL_CPF = "professorVirtualCPF";

	public AtendimentoTaticoDTO() {
		super();
	}

	public AtendimentoTaticoDTO(AtendimentoTaticoDTO other) {
		this.copy(other);
	}

	private void copy( AtendimentoTaticoDTO other ) {
		this.setId( other.getId() );
		this.setVersion( other.getVersion() );
		this.setCenarioId( other.getCenarioId() );
		this.setCampusId( other.getCampusId() );
		this.setCampusString( other.getCampusString() ); 
		this.setUnidadeId( other.getUnidadeId() );
		this.setUnidadeString( other.getUnidadeString() );
		this.setSalaId( other.getSalaId() );
		this.setSalaString( other.getSalaString() );
		this.setSemana( other.getSemana() );
		this.setDiaSemana( other.getDiaSemana() );
		this.setCursoString( other.getCursoString() );
		this.setCursoNome( other.getCursoNome() );
		this.setCursoId( other.getCursoId() );
		this.setCurricularId( other.getCurriculoId() );
		this.setCurricularString( other.getCurriculoString() );
		this.setPeriodo( other.getPeriodo() );
		this.setPeriodoString( other.getPeriodoString() );
		this.setDisciplinaId( other.getDisciplinaId() );
		this.setDisciplinaString( other.getDisciplinaString() );
		this.setDisciplinaNome( other.getDisciplinaNome() );
		this.setDisciplinaSubstitutaId( other.getDisciplinaSubstitutaId() );
		this.setDisciplinaSubstitutaString( other.getDisciplinaSubstitutaString() );
		this.setDisciplinaSubstitutaNome( other.getDisciplinaSubstitutaNome() );
		this.setDisciplinaSubstitutaSemanaLetivaId( other.getDisciplinaSubstitutaSemanaLetivaId() );
		this.setDisciplinaSubstitutaSemanaLetivaTempoAula( other.getDisciplinaSubstitutaSemanaLetivaTempoAula() );
		this.setTurnoId( other.getTurnoId() );
		this.setTurnoString( other.getTurnoString() );
		this.setTotalCreditoDisciplina( other.getTotalCreditoDisciplina() );
		this.setTotalCreditosPraticosDisciplina( other.getTotalCreditosPraticosDisciplina() );
		this.setTotalCreditosTeoricosDisciplina( other.getTotalCreditosTeoricosDisciplina() );
		this.setTotalCreditoDisciplinaSubstituta( other.getTotalCreditoDisciplinaSubstituta() );
		this.setOfertaId( other.getOfertaId() );
		this.setTurma( other.getTurma() );
		this.setQuantidadeAlunos( other.getQuantidadeAlunos() );
		this.setQuantidadeAlunosString( other.getQuantidadeAlunosString() );
		this.setCreditosTeorico( other.getCreditosTeorico() );
		this.setCreditosPratico( other.getCreditosPratico() );
		this.setCompartilhamentoCursosString( other.getCompartilhamentoCursosString() );
		this.setProfessorCustoCreditoSemanal( other.getProfessorCustoCreditoSemanal() );
		this.setQtdDemandaAlunosP1(other.getQtdDemandaAlunosP1());
		this.setQtdDemandaAlunosP2(other.getQtdDemandaAlunosP2());
		this.setQtdDemandaAlunosTotal(other.getQtdDemandaAlunosTotal());
		this.setQuantidadeAlunosP1(other.getQuantidadeAlunosP1());
		this.setQuantidadeAlunosP2(other.getQuantidadeAlunosP2());
		this.setProfessorCPF(other.getProfessorCPF());
		this.setProfessorVirtualCPF(other.getProfessorVirtualCPF());
	}
	
	public void setDiaSemana( Integer value )
	{
		set( PROPERTY_DIA_DA_SEMANA, value );
	}

	public Integer getDiaSemana()
	{
		return get( PROPERTY_DIA_DA_SEMANA );
	}
	
	public void setProfessorCPF( String value )
	{
		set( PROPERTY_PROFESSOR_CPF, value );
	}

	public String getProfessorCPF()
	{
		return get( PROPERTY_PROFESSOR_CPF );
	}
	
	public void setProfessorVirtualCPF( String value )
	{
		set( PROPERTY_PROFESSOR_VIRTUAL_CPF, value );
	}

	public String getProfessorVirtualCPF()
	{
		return get( PROPERTY_PROFESSOR_VIRTUAL_CPF );
	}

	public void setId( Long value )
	{
		set( PROPERTY_ID, value );
	}

	public Long getId()
	{
		return get( PROPERTY_ID );
	}

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}
	
	public void setCampusId( Long value )
	{
		set( PROPERTY_CAMPUS_ID, value );
	}

	public Long getCampusId()
	{
		return get( PROPERTY_CAMPUS_ID );
	}
	
	public String getCampusString()
	{
		return get( PROPERTY_CAMPUS_STRING );
	}

	public void setCampusString( String value )
	{
		set( PROPERTY_CAMPUS_STRING, value );
	}
	
	public void setUnidadeId( Long value )
	{
		set( PROPERTY_UNIDADE_ID, value );
	}

	public Long getUnidadeId()
	{
		return get( PROPERTY_UNIDADE_ID );
	}
	
	public String getUnidadeString()
	{
		return get( PROPERTY_UNIDADE_STRING );
	}

	public void setUnidadeString( String value )
	{
		set( PROPERTY_UNIDADE_STRING, value );
	}
	
	public void setSalaId( Long value )
	{
		set( PROPERTY_SALA_ID, value );
	}

	public Long getSalaId()
	{
		return get( PROPERTY_SALA_ID );
	}
	
	public String getSalaString()
	{
		return get( PROPERTY_SALA_STRING );
	}

	public void setSalaString( String value )
	{
		set( PROPERTY_SALA_STRING, value );
	}

	public void setSemana( Integer value )
	{
		set( PROPERTY_DIA_SEMANA, value );
	}

	public Integer getSemana()
	{
		return get( PROPERTY_DIA_SEMANA );
	}
	
	public void setCursoString( String value )
	{
		set( PROPERTY_CURSO_STRING, value );
	}

	public String getCursoString()
	{
		return get( PROPERTY_CURSO_STRING );
	}
	
	public void setCursoNome( String value )
	{
		set( PROPERTY_CURSO_NOME, value );
	}

	public String getCursoNome()
	{
		return get( PROPERTY_CURSO_NOME );
	}
	
	public void setCursoId( Long value )
	{
		set( PROPERTY_CURSO_ID, value );
	}

	public Long getCursoId()
	{
		return get( PROPERTY_CURSO_ID );
	}
	
	public void setCurricularId( Long value )
	{
		set( PROPERTY_CURRICULO_ID, value );
	}

	public Long getCurriculoId()
	{
		return get( PROPERTY_CURRICULO_ID );
	}

	public void setCurricularString( String value )
	{
		set( PROPERTY_CURRICULO_STRING, value );
	}

	public String getCurriculoString()
	{
		return get( PROPERTY_CURRICULO_STRING );
	}

	public void setPeriodo( Integer value )
	{
		set( PROPERTY_PERIODO, value );
	}

	public Integer getPeriodo()
	{
		return get( PROPERTY_PERIODO );
	}
	
	public void setPeriodoString( String value )
	{
		set( PROPERTY_PERIODO_STRING, value );
	}

	public String getPeriodoString()
	{
		return get( PROPERTY_PERIODO_STRING );
	}
	
	public void setDisciplinaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_ID, value );
	}

	public Long getDisciplinaId()
	{
		return get( PROPERTY_DISCIPLINA_ID );
	}

	public String getDisciplinaString()
	{
		return get( PROPERTY_DISCIPLINA_STRING );
	}

	public void setDisciplinaString( String value )
	{
		set( PROPERTY_DISCIPLINA_STRING, value );
	}
	
	public String getDisciplinaNome()
	{
		return get( PROPERTY_DISCIPLINA_NOME );
	}
	
	public void setDisciplinaNome( String value )
	{
		set( PROPERTY_DISCIPLINA_NOME, value );
	}
	
	public Boolean getDisciplinaUsaLaboratorio()
	{
		return get( PROPERTY_DISCIPLINA_USA_LABORATORIO );
	}

	public void setDisciplinaUsaLaboratorio( Boolean value )
	{
		set( PROPERTY_DISCIPLINA_USA_LABORATORIO, value );
	}
	
	public void setDisciplinaSubstitutaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_SUBSTITUTA_ID, value );
	}

	public Long getDisciplinaSubstitutaId()
	{
		return get( PROPERTY_DISCIPLINA_SUBSTITUTA_ID );
	}

	public String getDisciplinaSubstitutaString()
	{
		return get( PROPERTY_DISCIPLINA_SUBSTITUTA_STRING );
	}

	public void setDisciplinaSubstitutaString( String value )
	{
		set( PROPERTY_DISCIPLINA_SUBSTITUTA_STRING, value );
	}
	
	public String getDisciplinaSubstitutaNome()
	{
		return get( PROPERTY_DISCIPLINA_SUBSTITUTA_NOME );
	}
	
	public void setDisciplinaSubstitutaNome( String value )
	{
		set( PROPERTY_DISCIPLINA_SUBSTITUTA_NOME, value );
	}
	
	public void setDisciplinaSubstitutaSemanaLetivaId( Long value )
	{
		set( PROPERTY_DISCIPLINA_SUBSTITUTA_SEMANA_LETIVA_ID, value );
	}

	public Long getDisciplinaSubstitutaSemanaLetivaId()
	{
		return get( PROPERTY_DISCIPLINA_SUBSTITUTA_SEMANA_LETIVA_ID );
	}
	
	public void setDisciplinaSubstitutaSemanaLetivaTempoAula( Integer value )
	{
		set( PROPERTY_DISCIPLINA_SUBSTITUTA_SEMANA_LETIVA_TEMPO_AULA, value );
	}

	public Integer getDisciplinaSubstitutaSemanaLetivaTempoAula()
	{
		return get( PROPERTY_DISCIPLINA_SUBSTITUTA_SEMANA_LETIVA_TEMPO_AULA );
	}
	
	public void setTurnoId( Long value )
	{
		set( PROPERTY_TURNO_ID, value );
	}

	public Long getTurnoId()
	{
		return get( PROPERTY_TURNO_ID );
	}

	public String getTurnoString()
	{
		return get( PROPERTY_TURNO_STRING );
	}
	public void setTurnoString( String value )
	{
		set( PROPERTY_TURNO_STRING, value );
	}

	public void setTotalCreditoDisciplina( Integer value )
	{
		set( PROPERTY_TOTAL_CRETIDOS_DISCIPLINA, value );
	}

	public Integer getTotalCreditoDisciplina()
	{
		return get( PROPERTY_TOTAL_CRETIDOS_DISCIPLINA );
	}
	
	public void setTotalCreditosTeoricosDisciplina( Integer value )
	{
		set( PROPERTY_TOTAL_CRETIDOS_TEORICOS_DISCIPLINA, value );
	}

	@Override
	public Integer getTotalCreditosTeoricosDisciplina()
	{
		return get(PROPERTY_TOTAL_CRETIDOS_TEORICOS_DISCIPLINA);
	}
	
	public void setTotalCreditosPraticosDisciplina( Integer value )
	{
		set( PROPERTY_TOTAL_CRETIDOS_PRATICOS_DISCIPLINA, value );
	}

	@Override
	public Integer getTotalCreditosPraticosDisciplina()
	{
		return get(PROPERTY_TOTAL_CRETIDOS_PRATICOS_DISCIPLINA);
	}
	
	public void setTotalCreditoDisciplinaSubstituta( Integer value )
	{
		set( PROPERTY_TOTAL_CRETIDOS_DISCIPLINA_SUBSTITUTA, value );
	}

	public Integer getTotalCreditoDisciplinaSubstituta()
	{
		return get( PROPERTY_TOTAL_CRETIDOS_DISCIPLINA_SUBSTITUTA );
	}
	
	public void setOfertaId( Long value )
	{
		set( PROPERTY_OFERTA_ID, value );
	}

	public Long getOfertaId()
	{
		return get( PROPERTY_OFERTA_ID );
	}

	public String getTurma()
	{
		return get( PROPERTY_TURMA );
	}

	public void setTurma( String value )
	{
		set( PROPERTY_TURMA, value );
	}

	public void setQuantidadeAlunos( Integer value )
	{
		set( PROPERTY_QUANTIDADE_ALUNOS, value );
	}

	public Integer getQuantidadeAlunos()
	{
		return get( PROPERTY_QUANTIDADE_ALUNOS );
	}

	public void setQuantidadeAlunosString( String value )
	{
		set( PROPERTY_QUANTIDADE_ALUNOS_STRING, value );
	}

	public String getQuantidadeAlunosString()
	{
		return get( PROPERTY_QUANTIDADE_ALUNOS_STRING );
	}
	
	public void setNomesAlunos( String value )
	{
		set( PROPERTY_NOMES_ALUNOS, value );
	}

	public String getNomesAlunos()
	{
		return get( PROPERTY_NOMES_ALUNOS );
	}
	
	public void setCreditosTeorico( Integer value )
	{
		set( PROPERTY_CREDITOS_TEORICOS, value );
	}

	public Integer getCreditosTeorico()
	{
		return get( PROPERTY_CREDITOS_TEORICOS );
	}

	public void setCreditosPratico( Integer value )
	{
		set( PROPERTY_CREDITOS_PRATICOS, value );
	}

	public Integer getCreditosPratico()
	{
		return get( PROPERTY_CREDITOS_PRATICOS );
	}

	@Override
	public String getCompartilhamentoCursosString()
	{
		return get(PROPERTY_COMPARTILHAMENTO_CURSOS );
	}

	@Override
	public void setCompartilhamentoCursosString( String s )
	{
		set( PROPERTY_COMPARTILHAMENTO_CURSOS, s );
	}

	public boolean isTeorico()
	{
		return ( getCreditosTeorico() > 0 );
	}

	public Integer getTotalCreditos()
	{
		return ( getCreditosTeorico() + getCreditosPratico() );
	}
	
	private String getContentToolTipVisao(ReportType reportType){
		String BG = TriedaUtil.beginBold(reportType);
		String ED = TriedaUtil.endBold(reportType);
		String BR = TriedaUtil.newLine(reportType);
		
		String creditosDisciplinaInfo = "";
		if (getDisciplinaSubstitutaId() != null) {
			creditosDisciplinaInfo = getTotalCreditoDisciplinaSubstituta().toString();
		} else {
			creditosDisciplinaInfo = getTotalCreditoDisciplina().toString();
		}
		
		//String nomesAlunos = getNomesAlunos().replaceAll(", ",BR);
		
		return BG + "Disciplina: " + ED + getDisciplinaString() + " - " + getDisciplinaNome() + BR
			 + ((getDisciplinaSubstitutaId() != null) ? (BG + "Substituta: " + ED + getDisciplinaSubstitutaString() + " - " + getDisciplinaSubstitutaNome() + BR) : "")
		     + BG + "Turma: " + ED + getTurma() + BR
			 + BG + "Cr&eacute;dito(s) " + ( ( isTeorico() ) ? "Te&oacute;rico(s)" : "Pr&aacute;tico(s)" ) + ": " + ED + getTotalCreditos() + " de " + creditosDisciplinaInfo + BR
			 + BG + "Curso(s): " + ED + getCursoNome() + BR
			 + BG + "Matriz(es) Curricular(es): " + ED + getCurriculoString() + BR
			 + BG + "Per&iacute;odo(s): " + ED + getPeriodoString() + BR
			 + BG + "Sala: " + ED + getSalaString() + BR
			 + BG + getQuantidadeAlunosString() + " = " + getQuantidadeAlunos() + " aluno(s)" + ED;// + BR
			 //+ nomesAlunos;
	}

	public String getContentToolTipVisaoSala(ReportType reportType){
		return getContentToolTipVisao(reportType);
	}

	public String getContentToolTipVisaoCurso(ReportType reportType) {
		return getContentToolTipVisao(reportType);
	}
	
	public String getContentToolTipVisaoAluno(ReportType reportType){
		return getContentToolTipVisao(reportType);
	}

	@Override
	public String getNaturalKey()
	{
		return getCampusString() + "-" + getUnidadeString()
			+ "-" + getSalaString() + "-" + getSemana()
			+ "-" + getCursoString() + "-" + getCurriculoString()
			+ "-" + getPeriodo() + "-" + getDisciplinaString() + "-" + getTurma();
	}

	@Override
	public int compareTo( AtendimentoTaticoDTO o )
	{
		return getNaturalKey().compareTo( o.getNaturalKey() );
	}

	@Override
	public String toString()
	{
		return ( getDisciplinaString() + "@" + getTurma()
			+ "@" + getSalaString() + "@" + getSemana() );
	}

	static public boolean podemOcorrerEmParaleloAbordagem1(AtendimentoTaticoDTO dto1, AtendimentoTaticoDTO dto2) {
		return dto1.getDisciplinaId().equals( dto2.getDisciplinaId() )
			&& !dto1.getSalaId().equals( dto2.getSalaId() )
			&& !dto1.getTurma().equals( dto2.getTurma() )
			&& dto1.getTotalCreditos().equals( dto2.getTotalCreditos() )
			&& dto1.getSemana().equals( dto2.getSemana() );
	}

	static public boolean podemOcorrerEmParaleloAbordagem2(AtendimentoTaticoDTO dto1, AtendimentoTaticoDTO dto2) {
		return !dto1.getDisciplinaId().equals( dto2.getDisciplinaId() )
			&& !dto1.getSalaId().equals( dto2.getSalaId() )
			&& !dto1.getTurma().equals( dto2.getTurma() )
			&& dto1.getTotalCreditos().equals( dto2.getTotalCreditos() )
			&& dto1.getSemana().equals( dto2.getSemana() );
	}
	
	static public int calculaTotalDeCreditos(List<AtendimentoTaticoDTO> aulas) {
		int count = 0;
		for (AtendimentoTaticoDTO aula : aulas) {
			count += aula.getTotalCreditos();
		}

		return count;
	}
	
	static public int calculaTotalDeCreditosDasAulas(
		List< List< AtendimentoTaticoDTO > > listListDTOs )
	{
		int count = 0;
		for ( List< AtendimentoTaticoDTO > listDTOs : listListDTOs )
		{
			count += listDTOs.get( 0 ).getTotalCreditos();
		}

		return count;
	}

	@Override
	public boolean isTatico()
	{
		return true;
	}

	@Override
	public boolean isProfessorVirtual()
	{
		return false;
	}

	@Override
	public String getProfessorString() {
		return "";
	}
	
	@Override
	public String getProfessorVirtualString() {
		return "";
	}
	
	@Override
	public Long getProfessorId() {
		return null;
	}
	
	@Override
	public Long getProfessorVirtualId() {
		return null;
	}

	@Override
	public Long getSemanaLetivaId()
	{
		return get( PROPERTY_SEMANA_LETIVA_ID );
	}

	public void setSemanaLetivaId( Long value )
	{
		set( PROPERTY_SEMANA_LETIVA_ID, value );
	}
	
	@Override
	public Integer getSemanaLetivaTempoAula() {
		return get( PROPERTY_SEMANA_LETIVA_TEMPO_AULA );
	}

	public void setSemanaLetivaTempoAula( Integer value ) {
		set( PROPERTY_SEMANA_LETIVA_TEMPO_AULA, value );
	}
	
	@Override
	public Double getProfessorCustoCreditoSemanal() {
		return get( PROPERTY_PROFESSOR_CUSTO_CREDITO_SEMANAL );
	}
	
	public void setProfessorCustoCreditoSemanal( Double value ) {
		set( PROPERTY_PROFESSOR_CUSTO_CREDITO_SEMANAL, value );
	}
	
	public void setQtdDemandaAlunosP1(Integer value) {
		set( PROPERTY_QTD_DEMANDA_ALUNOS_P1, value );
	}
	public Integer getQtdDemandaAlunosP1() {
		return get( PROPERTY_QTD_DEMANDA_ALUNOS_P1 );
	}
	public void setQtdDemandaAlunosP2(Integer value) {
		set( PROPERTY_QTD_DEMANDA_ALUNOS_P2, value );
	}
	public Integer getQtdDemandaAlunosP2() {
		return get( PROPERTY_QTD_DEMANDA_ALUNOS_P2 );
	}
	public void setQtdDemandaAlunosTotal(Integer value) {
		set( PROPERTY_QTD_DEMANDA_ALUNOS_TOTAL, value );
	}
	public Integer getQtdDemandaAlunosTotal() {
		return get( PROPERTY_QTD_DEMANDA_ALUNOS_TOTAL );
	}
	
	public void setQuantidadeAlunosP1(Integer value) {
		set( PROPERTY_QUANTIDADE_ALUNOS_P1, value );
	}
	public Integer getQuantidadeAlunosP1() {
		return get( PROPERTY_QUANTIDADE_ALUNOS_P1 );
	}
	public void setQuantidadeAlunosP2(Integer value) {
		set( PROPERTY_QUANTIDADE_ALUNOS_P2, value );
	}
	public Integer getQuantidadeAlunosP2() {
		return get( PROPERTY_QUANTIDADE_ALUNOS_P2 );
	}
	
	private List<AlunoDemandaDTO> alunosDemandasDTO = new ArrayList<AlunoDemandaDTO>(); 
	public List<AlunoDemandaDTO> getAlunosDemandas() {
		return alunosDemandasDTO;
	}
}
