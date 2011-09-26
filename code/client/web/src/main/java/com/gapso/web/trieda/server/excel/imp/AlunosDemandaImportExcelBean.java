package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Turno;

public class AlunosDemandaImportExcelBean
	extends AbstractImportExcelBean
	implements Comparable< AlunosDemandaImportExcelBean >
{
	private String codigoCampusStr;
	private String codigoTurnoStr;
	private String codigoCursoStr;
	private String codigoCurriculoStr;
	private String periodoStr;
	private String disciplinaCodigoStr;
	private String matriculaAlunoStr;
	private String cpfAlunoStr;
	private String nomeAlunoStr;
	private String prioridadeAlunoStr;

	private Campus campus;
	private Turno turno;
	private Curso curso;
	private Curriculo curriculo;
	private Integer periodo;
	private Integer prioridade;
	private Disciplina disciplina;
	private Aluno aluno;
	private AlunoDemanda alunoDemanda;

	public AlunosDemandaImportExcelBean( int row )
	{
		super( row );
	}
	
	public List< ImportExcelError > checkSyntacticErrors()
	{
		List< ImportExcelError > erros
			= new ArrayList< ImportExcelError >();

		if ( !tudoVazio() )
		{
			checkMandatoryField( codigoCampusStr, ImportExcelError.ALUNO_DEMANDA_CAMPUS_VAZIO, erros );
			checkMandatoryField( codigoTurnoStr, ImportExcelError.ALUNO_DEMANDA_TURNO_VAZIO, erros );
			checkMandatoryField( codigoCursoStr, ImportExcelError.ALUNO_DEMANDA_CURSO_VAZIO, erros );
			checkMandatoryField( codigoCurriculoStr, ImportExcelError.ALUNO_DEMANDA_CURRICULO_VAZIO, erros );
			checkMandatoryField( disciplinaCodigoStr, ImportExcelError.ALUNO_DEMANDA_DISCIPLINA_VAZIO, erros );
			checkMandatoryField( matriculaAlunoStr, ImportExcelError.ALUNO_DEMANDA_MATRICULA_ALUNO_VAZIO, erros );
			checkMandatoryField( nomeAlunoStr, ImportExcelError.ALUNO_DEMANDA_NOME_ALUNO_VAZIO, erros );

			periodo = checkNonNegativeIntegerField( periodoStr,
				ImportExcelError.ALUNO_DEMANDA_PERIODO_FORMATO_INVALIDO,
				ImportExcelError.ALUNO_DEMANDA_PERIODO_NEGATIVO, erros );

			prioridade = checkNonNegativeIntegerField( prioridadeAlunoStr,
					ImportExcelError.ALUNO_DEMANDA_PRIORIDADE_FORMATO_INVALIDO,
					ImportExcelError.ALUNO_DEMANDA_PRIORIDADE_NEGATIVO, erros );
		}
		else
		{
			erros.add( ImportExcelError.TUDO_VAZIO );
		}

		return erros;
	}

	public boolean tudoVazio()
	{
		return ( isEmptyField( codigoCampusStr )
			&& isEmptyField( codigoTurnoStr )
			&& isEmptyField( codigoCursoStr )
			&& isEmptyField( codigoCurriculoStr )
			&& isEmptyField( periodoStr )
			&& isEmptyField( disciplinaCodigoStr )
			&& isEmptyField( matriculaAlunoStr )
			&& isEmptyField( nomeAlunoStr )
			&& isEmptyField( prioridadeAlunoStr ) );
	}

	public String getNaturalKeyString()
	{
		return getCodigoCampusStr() +
			"-" + getCodigoTurnoStr() +
			"-" + getCodigoCursoStr() +
			"-" + getCodigoCurriculoStr() +
			"-" + getPeriodoStr() +
			"-" + getDisciplinaCodigoStr() +
			"-" + getMatriculaAlunoStr() +
			"-" + getNomeAlunoStr() +
			"-" + getPrioridadeAlunoStr();
	}

	public String getCodigoCampusStr()
	{
		return codigoCampusStr;
	}

	public void setCodigoCampusStr( String codigoCampusStr )
	{
		this.codigoCampusStr = codigoCampusStr;
	}

	public String getCodigoTurnoStr()
	{
		return codigoTurnoStr;
	}

	public void setCodigoTurnoStr( String codigoTurnoStr )
	{
		this.codigoTurnoStr = codigoTurnoStr;
	}

	public String getCodigoCursoStr()
	{
		return codigoCursoStr;
	}

	public void setCodigoCursoStr( String codigoCursoStr )
	{
		this.codigoCursoStr = codigoCursoStr;
	}

	public String getCodigoCurriculoStr()
	{
		return codigoCurriculoStr;
	}

	public void setCodigoCurriculoStr( String codigoCurriculoStr )
	{
		this.codigoCurriculoStr = codigoCurriculoStr;
	}

	public String getMatriculaAlunoStr()
	{
		return matriculaAlunoStr;
	}

	public void setMatriculaAlunoStr( String matriculaAlunoStr )
	{
		this.matriculaAlunoStr = matriculaAlunoStr;
	}

	public String getCpfAlunoStr()
	{
		return cpfAlunoStr;
	}

	public void setCpfAlunoStr( String cpfAlunoStr )
	{
		this.cpfAlunoStr = cpfAlunoStr;
	}

	public String getNomeAlunoStr()
	{
		return nomeAlunoStr;
	}

	public void setNomeAlunoStr( String nomeAlunoStr )
	{
		this.nomeAlunoStr = nomeAlunoStr;
	}

	public String getPrioridadeAlunoStr()
	{
		return prioridadeAlunoStr;
	}

	public void setPrioridadeAlunoStr( String prioridadeAlunoStr )
	{
		this.prioridadeAlunoStr = prioridadeAlunoStr;
	}

	public Campus getCampus()
	{
		return campus;
	}

	public void setCampus( Campus campus )
	{
		this.campus = campus;
	}

	public Turno getTurno()
	{
		return turno;
	}

	public void setTurno( Turno turno )
	{
		this.turno = turno;
	}

	public Curriculo getCurriculo()
	{
		return curriculo;
	}

	public void setCurriculo( Curriculo curriculo )
	{
		this.curriculo = curriculo;
	}

	public Aluno getAluno()
	{
		return aluno;
	}

	public void setAluno( Aluno aluno )
	{
		this.aluno = aluno;
	}

	public AlunoDemanda getAlunoDemanda()
	{
		return alunoDemanda;
	}

	public void setAlunoDemanda( AlunoDemanda alunoDemanda )
	{
		this.alunoDemanda = alunoDemanda;
	}

	public String getPeriodoStr()
	{
		return periodoStr;
	}

	public void setPeriodoStr( String periodoStr )
	{
		this.periodoStr = periodoStr;
	}

	public String getDisciplinaCodigoStr()
	{
		return disciplinaCodigoStr;
	}

	public void setDisciplinaCodigoStr( String disciplinaCodigoStr )
	{
		this.disciplinaCodigoStr = disciplinaCodigoStr;
	}

	public Curso getCurso()
	{
		return curso;
	}

	public void setCurso( Curso curso )
	{
		this.curso = curso;
	}

	public Disciplina getDisciplina()
	{
		return disciplina;
	}

	public void setDisciplina( Disciplina disciplina )
	{
		this.disciplina = disciplina;
	}

	public Integer getPeriodo()
	{
		return periodo;
	}

	public void setPeriodo( Integer periodo )
	{
		this.periodo = periodo;
	}

	public Integer getPrioridade()
	{
		return prioridade;
	}

	public void setPrioridade( Integer prioridade )
	{
		this.prioridade = prioridade;
	}

	@Override
	public int compareTo( AlunosDemandaImportExcelBean o )
	{
		int result = getCampus().compareTo( o.getCampus() );
		
		if ( result == 0 )
		{
			result = getTurno().compareTo( o.getTurno() );

			if ( result == 0 )
			{
				result = getCurso().compareTo( o.getCurso() );
				
				if ( result == 0 )
				{
					result = getCurriculo().compareTo( o.getCurriculo() );
					
					if ( result == 0 )
					{
						result = getPeriodo().compareTo( o.getPeriodo() );
						
						if ( result == 0 )
						{
							result = getDisciplina().compareTo( o.getDisciplina() );
							
							if ( result == 0 )
							{
								result = getAluno().compareTo( o.getAluno() );
							}
						}
					}
				}
			}
		}

		return result;
	}

	public static Map< String, AlunosDemandaImportExcelBean > buildNomeAlunoToImportExcelBeanMap(
		List< AlunosDemandaImportExcelBean > beans )
	{
		Map< String, AlunosDemandaImportExcelBean > beansMap
			= new HashMap< String, AlunosDemandaImportExcelBean >();

		for ( AlunosDemandaImportExcelBean bean : beans )
		{
			beansMap.put( bean.getNomeAlunoStr(), bean );
		}

		return beansMap;
	}
}
