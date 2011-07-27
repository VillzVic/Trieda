package com.gapso.web.trieda.server.excel.exp;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;

public class RelatorioVisaoSalaFiltroExcel
	implements ExportExcelFilter
{
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private SalaDTO salaDTO;
	private TurnoDTO turnoDTO;

	public RelatorioVisaoSalaFiltroExcel( Long campusId,
		Long unidadeId, Long salaId, Long turnoId )
	{
		this.campusDTO = ConvertBeans.toCampusDTO( Campus.find( campusId ) );
		this.unidadeDTO = ConvertBeans.toUnidadeDTO( Unidade.find( unidadeId ) );
		this.salaDTO = ConvertBeans.toSalaDTO( Sala.find( salaId ) );
		this.turnoDTO = ConvertBeans.toTurnoDTO( Turno.find( turnoId ) );
	}

	public CampusDTO getCampusDTO() {
		return campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO) {
		this.campusDTO = campusDTO;
	}

	public UnidadeDTO getUnidadeDTO() {
		return unidadeDTO;
	}

	public void setUnidadeDTO(UnidadeDTO unidadeDTO) {
		this.unidadeDTO = unidadeDTO;
	}

	public SalaDTO getSalaDTO() {
		return salaDTO;
	}

	public void setSalaDTO(SalaDTO salaDTO) {
		this.salaDTO = salaDTO;
	}

	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
	}

	public ExportExcelFilter getFilter(){
		return this;
	}
}
