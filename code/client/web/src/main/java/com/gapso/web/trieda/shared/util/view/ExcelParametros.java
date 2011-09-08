package com.gapso.web.trieda.shared.util.view;

import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class ExcelParametros
{
	private ExcelInformationType info;
	private InstituicaoEnsinoDTO instituicaoEnsinoDTO;

	public ExcelParametros(
		ExcelInformationType info,
		InstituicaoEnsinoDTO instituicaoEnsinoDTO )
	{
		this.info = info;
		this.instituicaoEnsinoDTO = instituicaoEnsinoDTO;
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
}
