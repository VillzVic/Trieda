package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;

public class ProfessoresImportExcelBean extends AbstractImportExcelBean implements Comparable<ProfessoresImportExcelBean> {
	
	private String cpfStr;
	private String nomeStr;
	private String tipoStr;
	private String cargaHorariaMaxStr;
	private String cargaHorariaMinStr;
	private String titulacaoStr;
	private String areaTitulacaoStr;
	private String cargaHorariaAnteriorStr;
	private String valorCreditoStr;
	
	private TipoContrato tipo;
	private Integer cargaHorariaMax;
	private Integer cargaHorariaMin;
	private Titulacao titulacao;
	private AreaTitulacao areaTitulacao;
	private Integer cargaHorariaAnterior;
	private Double valorCredito;
	
	public ProfessoresImportExcelBean(int row) {
		super(row);
	}
	
	public List<ImportExcelError> checkSyntacticErrors() {
		List<ImportExcelError> erros = new ArrayList<ImportExcelError>();
		if (!tudoVazio()) {
			checkMandatoryField(cpfStr,ImportExcelError.PROFESSOR_CPF_VAZIO,erros);
			checkMandatoryField(nomeStr,ImportExcelError.PROFESSOR_NOME_VAZIO,erros);
			checkMandatoryField(tipoStr,ImportExcelError.PROFESSOR_TIPO_VAZIO,erros);
			checkMandatoryField(cargaHorariaMaxStr,ImportExcelError.PROFESSOR_CARGA_HORARIA_MAX_VAZIO,erros);
			checkMandatoryField(cargaHorariaMinStr,ImportExcelError.PROFESSOR_CARGA_HORARIA_MIN_VAZIO,erros);
			checkMandatoryField(titulacaoStr,ImportExcelError.PROFESSOR_TITULACAO_VAZIO,erros);
			checkMandatoryField(areaTitulacaoStr,ImportExcelError.PROFESSOR_AREA_TITULACAO_VAZIO,erros);
			checkMandatoryField(cargaHorariaAnteriorStr,ImportExcelError.PROFESSOR_CARGA_HORARIA_ANTERIOR_VAZIO,erros);
			checkMandatoryField(valorCreditoStr,ImportExcelError.PROFESSOR_VALOR_CREDITO_VAZIO,erros);
			
			cargaHorariaMax = checkNonNegativeIntegerField(cargaHorariaMaxStr,ImportExcelError.PROFESSOR_CARGA_HORARIA_MAX_FORMATO_INVALIDO,ImportExcelError.PROFESSOR_CARGA_HORARIA_MAX_VALOR_NEGATIVO,erros);
			cargaHorariaMin = checkNonNegativeIntegerField(cargaHorariaMinStr,ImportExcelError.PROFESSOR_CARGA_HORARIA_MIN_FORMATO_INVALIDO,ImportExcelError.PROFESSOR_CARGA_HORARIA_MIN_VALOR_NEGATIVO,erros);
			cargaHorariaAnterior = checkNonNegativeIntegerField(cargaHorariaAnteriorStr,ImportExcelError.PROFESSOR_CARGA_HORARIA_ANTERIOR_FORMATO_INVALIDO,ImportExcelError.PROFESSOR_CARGA_HORARIA_ANTERIOR_VALOR_NEGATIVO,erros);
			valorCredito = checkNonNegativeDoubleField(valorCreditoStr,ImportExcelError.PROFESSOR_VALOR_CREDITO_FORMATO_INVALIDO,ImportExcelError.PROFESSOR_VALOR_CREDITO_VALOR_NEGATIVO,erros);
		} else {
			erros.add(ImportExcelError.TUDO_VAZIO);
		}
		return erros;
	}
	
	private boolean tudoVazio() {
		return isEmptyField(cpfStr) &&
			isEmptyField(nomeStr) &&
			isEmptyField(tipoStr) &&
			isEmptyField(cargaHorariaMaxStr) &&
			isEmptyField(cargaHorariaMinStr) &&
			isEmptyField(titulacaoStr) &&
			isEmptyField(areaTitulacaoStr) &&
			isEmptyField(cargaHorariaAnteriorStr) &&
			isEmptyField(valorCreditoStr);
	}

	public String getCpfStr() {
		return cpfStr;
	}
	public void setCpfStr(String cpfStr) {
		this.cpfStr = cpfStr;
	}

	public String getNomeStr() {
		return nomeStr;
	}
	public void setNomeStr(String nomeStr) {
		this.nomeStr = nomeStr;
	}

	public String getTipoStr() {
		return tipoStr;
	}
	public void setTipoStr(String tipoStr) {
		this.tipoStr = tipoStr;
	}

	public String getCargaHorariaMaxStr() {
		return cargaHorariaMaxStr;
	}
	public void setCargaHorariaMaxStr(String cargaHorariaMaxStr) {
		this.cargaHorariaMaxStr = cargaHorariaMaxStr;
	}

	public String getCargaHorariaMinStr() {
		return cargaHorariaMinStr;
	}
	public void setCargaHorariaMinStr(String cargaHorariaMinStr) {
		this.cargaHorariaMinStr = cargaHorariaMinStr;
	}

	public String getTitulacaoStr() {
		return titulacaoStr;
	}
	public void setTitulacaoStr(String titulacaoStr) {
		this.titulacaoStr = titulacaoStr;
	}

	public String getAreaTitulacaoStr() {
		return areaTitulacaoStr;
	}
	public void setAreaTitulacaoStr(String areaTitulacaoStr) {
		this.areaTitulacaoStr = areaTitulacaoStr;
	}

	public String getCargaHorariaAnteriorStr() {
		return cargaHorariaAnteriorStr;
	}
	public void setCargaHorariaAnteriorStr(String cargaHorariaAnteriorStr) {
		this.cargaHorariaAnteriorStr = cargaHorariaAnteriorStr;
	}

	public String getValorCreditoStr() {
		return valorCreditoStr;
	}
	public void setValorCreditoStr(String valorCreditoStr) {
		this.valorCreditoStr = valorCreditoStr;
	}
	
	public TipoContrato getTipo() {
		return tipo;
	}

	public Integer getCargaHorariaMax() {
		return cargaHorariaMax;
	}

	public Integer getCargaHorariaMin() {
		return cargaHorariaMin;
	}

	public Titulacao getTitulacao() {
		return titulacao;
	}

	public AreaTitulacao getAreaTitulacao() {
		return areaTitulacao;
	}

	public Integer getCargaHorariaAnterior() {
		return cargaHorariaAnterior;
	}

	public Double getValorCredito() {
		return valorCredito;
	}
	
	public void setTipo(TipoContrato tipo) {
		this.tipo = tipo;
	}

	public void setTitulacao(Titulacao titulacao) {
		this.titulacao = titulacao;
	}

	public void setAreaTitulacao(AreaTitulacao areaTitulacao) {
		this.areaTitulacao = areaTitulacao;
	}

	@Override
	public int compareTo(ProfessoresImportExcelBean o) {
		return getCpfStr().compareTo(o.getCpfStr());
	}
}
