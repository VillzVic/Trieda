package com.gapso.web.trieda.shared.util.view;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class ExcelParametros
{
	private ExcelInformationType info;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;
	private CenarioDTO cenarioDTO;
	private String fileExtension;

	public ExcelParametros(
		ExcelInformationType info,
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO)
	{
		this.info = info;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
	}
	
	public ExcelParametros(
		ExcelInformationType info,
		InstituicaoEnsinoDTO instituicaoEnsinoDTO, 
		CenarioDTO cenarioDTO, String fileExtension )
	{
		this.info = info;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
		this.cenarioDTO = cenarioDTO;
		this.fileExtension = fileExtension;
	}

	public ExcelInformationType getInfo() {
		return info;
	}

	public void setInfo( ExcelInformationType info )
	{
		this.info = info;
	}

	public InstituicaoEnsinoDTO getInstituicaoEnsinoDTO() {
		return instituicaoEnsinoDTO;
	}

	public void setInstituicaoEnsinoDTO(InstituicaoEnsinoDTO instituicaoEnsinoDTO) {
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
	}
	
	public CenarioDTO getCenarioDTO() {
		return cenarioDTO;
	}
	
	public void setCenarioDTO( CenarioDTO cenarioDTO ) {
		this.cenarioDTO = cenarioDTO;
	}
	
	public void setFileExtension ( String extension ) {
		this.fileExtension = extension;
	}
	
	public String getFileExtension () {
		return fileExtension;
	}
}
