package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.List;

import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.misc.Dificuldades;

public class DisciplinasImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< DisciplinasImportExcelBean >
{
	private String codigoStr;
	private String nomeStr;
	private String creditosTeoricosStr;
	private String creditosPraticosStr;
	private String exigeLaboratorioStr;
	private String tipoStr;
	private String nivelDificuldadeStr;
	private String maxAlunosTeoricoStr;
	private String maxAlunosPraticoStr;
	private String aulasContinuasStr;
	private String professorUnicoStr;
	private String usaSabadoStr;
	private String usaDomingoStr;

	private Integer creditosTeoricos;
	private Integer creditosPraticos;
	private Boolean exigeLaboratorio;
	private TipoDisciplina tipo;
	private Dificuldades nivelDificuldade;
	private Integer maxAlunosTeorico;
	private Integer maxAlunosPratico;
	private Boolean aulasContinuas;
	private Boolean professorUnico;
	private Boolean usaSabado;
	private Boolean usaDomingo;

	public DisciplinasImportExcelBean( int row )
	{
		super( row );
	}

	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros = new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( codigoStr, ImportExcelError.DISCIPLINA_CODIGO_VAZIO,erros );
			checkMandatoryField( nomeStr, ImportExcelError.DISCIPLINA_NOME_VAZIO,erros );
			checkMandatoryField( creditosTeoricosStr, ImportExcelError.DISCIPLINA_CREDITOS_TEORICOS_VAZIO, erros );
			checkMandatoryField( creditosPraticosStr, ImportExcelError.DISCIPLINA_CREDITOS_PRATICOS_VAZIO, erros );
			checkMandatoryField( exigeLaboratorioStr, ImportExcelError.DISCIPLINA_USA_LABORATORIO_VAZIO, erros );
			checkMandatoryField( tipoStr, ImportExcelError.DISCIPLINA_TIPO_VAZIO, erros );
			checkMandatoryField( nivelDificuldadeStr, ImportExcelError.DISCIPLINA_NIVEL_DIFICULDADE_VAZIO, erros );
			checkMandatoryField( maxAlunosTeoricoStr, ImportExcelError.DISCIPLINA_MAX_ALUNOS_TEORICOS_VAZIO, erros );
			checkMandatoryField( maxAlunosPraticoStr, ImportExcelError.DISCIPLINA_MAX_ALUNOS_PRATICOS_VAZIO, erros );
			checkMandatoryField( aulasContinuasStr, ImportExcelError.DISCIPLINA_AULAS_CONTINUAS_VAZIO, erros );
			checkMandatoryField( professorUnicoStr, ImportExcelError.DISCIPLINA_PROFESSOR_UNICO_VAZIO, erros );
			checkMandatoryField( usaSabadoStr, ImportExcelError.DISCIPLINA_USA_SABADO_VAZIO, erros );
			checkMandatoryField( usaDomingoStr, ImportExcelError.DISCIPLINA_USA_DOMINGO_VAZIO, erros );

			creditosTeoricos = checkNonNegativeIntegerField( creditosTeoricosStr,
				ImportExcelError.DISCIPLINA_CREDITOS_TEORICOS_FORMATO_INVALIDO,
				ImportExcelError.DISCIPLINA_CREDITOS_TEORICOS_VALOR_NEGATIVO, erros );

			creditosPraticos = checkNonNegativeIntegerField( creditosPraticosStr,
				ImportExcelError.DISCIPLINA_CREDITOS_PRATICOS_FORMATO_INVALIDO,
				ImportExcelError.DISCIPLINA_CREDITOS_PRATICOS_VALOR_NEGATIVO, erros );

			exigeLaboratorio = checkBooleanField( exigeLaboratorioStr,
				ImportExcelError.DISCIPLINA_USA_LABORATORIO_FORMATO_INVALIDO, erros );

			nivelDificuldade = checkEnumField( nivelDificuldadeStr, Dificuldades.class,
				ImportExcelError.DISCIPLINA_NIVEL_DIFICULDADE_VALOR_INVALIDO, erros );

			maxAlunosTeorico = checkNonNegativeIntegerField( maxAlunosTeoricoStr,
				ImportExcelError.DISCIPLINA_MAX_ALUNOS_TEORICOS_FORMATO_INVALIDO,
				ImportExcelError.DISCIPLINA_MAX_ALUNOS_TEORICOS_VALOR_NEGATIVO, erros );

			maxAlunosPratico = checkNonNegativeIntegerField( maxAlunosPraticoStr,
				ImportExcelError.DISCIPLINA_MAX_ALUNOS_PRATICOS_FORMATO_INVALIDO,
				ImportExcelError.DISCIPLINA_MAX_ALUNOS_PRATICOS_VALOR_NEGATIVO, erros );
			
			aulasContinuas = checkBooleanField( aulasContinuasStr,
					ImportExcelError.DISCIPLINA_AULAS_CONTINUAS_FORMATO_INVALIDO, erros );
			
			professorUnico = checkBooleanField( professorUnicoStr,
					ImportExcelError.DISCIPLINA_PROFESSOR_UNICO_FORMATO_INVALIDO, erros );
			
			usaSabado = checkBooleanField( usaSabadoStr,
					ImportExcelError.DISCIPLINA_USA_SABADO_FORMATO_INVALIDO, erros );
			
			usaDomingo = checkBooleanField( usaDomingoStr,
					ImportExcelError.DISCIPLINA_USA_DOMINGO_FORMATO_INVALIDO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}
	
	private boolean tudoVazio()
	{
		return ( isEmptyField( codigoStr )
			&& isEmptyField( nomeStr )
			&& isEmptyField( creditosTeoricosStr )
			&& isEmptyField( creditosPraticosStr )
			&& isEmptyField( exigeLaboratorioStr )
			&& isEmptyField( tipoStr )
			&& isEmptyField( nivelDificuldadeStr )
			&& isEmptyField( maxAlunosTeoricoStr )
			&& isEmptyField( maxAlunosPraticoStr )
			&& isEmptyField( aulasContinuasStr )
			&& isEmptyField( professorUnicoStr )
			&& isEmptyField( usaSabadoStr )
			&& isEmptyField( usaDomingoStr )	);
	}

	public String getCodigoStr()
	{
		return codigoStr;
	}

	public void setCodigoStr( String codigoStr )
	{
		this.codigoStr = codigoStr;
	}

	public String getNomeStr()
	{
		return nomeStr;
	}

	public void setNomeStr( String nomeStr )
	{
		this.nomeStr = nomeStr;
	}

	public String getCreditosTeoricosStr()
	{
		return creditosTeoricosStr;
	}

	public void setCreditosTeoricosStr( String creditosTeoricosStr )
	{
		this.creditosTeoricosStr = creditosTeoricosStr;
	}

	public String getCreditosPraticosStr()
	{
		return creditosPraticosStr;
	}

	public void setCreditosPraticosStr( String creditosPraticosStr )
	{
		this.creditosPraticosStr = creditosPraticosStr;
	}

	public String getExigeLaboratorioStr()
	{
		return exigeLaboratorioStr;
	}

	public void setExigeLaboratorioStr( String exigeLaboratorioStr )
	{
		this.exigeLaboratorioStr = exigeLaboratorioStr;
	}

	public String getTipoStr()
	{
		return tipoStr;
	}

	public void setTipoStr( String tipoStr )
	{
		this.tipoStr = tipoStr;
	}

	public String getNivelDificuldadeStr()
	{
		return nivelDificuldadeStr;
	}

	public void setNivelDificuldadeStr( String nivelDificuldadeStr )
	{
		this.nivelDificuldadeStr = nivelDificuldadeStr;
	}

	public String getMaxAlunosTeoricoStr()
	{
		return maxAlunosTeoricoStr;
	}

	public void setMaxAlunosTeoricoStr( String maxAlunosTeoricoStr )
	{
		this.maxAlunosTeoricoStr = maxAlunosTeoricoStr;
	}

	public String getMaxAlunosPraticoStr()
	{
		return maxAlunosPraticoStr;
	}

	public void setMaxAlunosPraticoStr( String maxAlunosPraticoStr )
	{
		this.maxAlunosPraticoStr = maxAlunosPraticoStr;
	}
	
	public String getUsaSabadoStr()
	{
		return usaSabadoStr;
	}

	public void setUsaSabadoStr( String usaSabadoStr )
	{
		this.usaSabadoStr = usaSabadoStr;
	}

	public String getUsaDomingoStr()
	{
		return usaDomingoStr;
	}

	public void setUsaDomingoStr( String usaDomingoStr )
	{
		this.usaDomingoStr = usaDomingoStr;
	}
	
	public String getAulasContinuasStr()
	{
		return aulasContinuasStr;
	}

	public void setAulasContinuasStr( String aulasContinuasStr )
	{
		this.aulasContinuasStr = aulasContinuasStr;
	}
	
	public String getProfessorUnicoStr()
	{
		return professorUnicoStr;
	}

	public void setProfessorUnicoStr( String professorUnicoStr )
	{
		this.professorUnicoStr = professorUnicoStr;
	}
	
	public TipoDisciplina getTipo()
	{
		return tipo;
	}

	public void setTipo( TipoDisciplina tipo )
	{
		this.tipo = tipo;
	}

	public Integer getCreditosTeoricos()
	{
		return creditosTeoricos;
	}

	public Integer getCreditosPraticos()
	{
		return creditosPraticos;
	}

	public Boolean getExigeLaboratorio()
	{
		return exigeLaboratorio;
	}

	public Dificuldades getNivelDificuldade()
	{
		return nivelDificuldade;
	}

	public Integer getMaxAlunosTeorico()
	{
		return maxAlunosTeorico;
	}

	public Integer getMaxAlunosPratico()
	{
		return maxAlunosPratico;
	}
	
	public Boolean getUsaSabado()
	{
		return usaSabado;
	}
	
	public Boolean getUsaDomingo()
	{
		return usaDomingo;
	}
	
	public Boolean getAulasContinuas()
	{
		return aulasContinuas;
	}
	
	public Boolean getProfessorUnico()
	{
		return professorUnico;
	}

	@Override
	public int compareTo( DisciplinasImportExcelBean o )
	{
		return getCodigoStr().compareTo( o.getCodigoStr() );
	}
}
