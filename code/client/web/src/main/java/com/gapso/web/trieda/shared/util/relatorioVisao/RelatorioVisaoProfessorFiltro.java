package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoProfessorFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 1L;
	private CampusDTO campusDTO;
	private TurnoDTO turnoDTO;
	private ProfessorDTO professorDTO;
	private ProfessorVirtualDTO professorVirtualDTO;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_PROFESSOR;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = super.getMapStringIds();
		
		mapIds.put("campusId", campusDTO.getId().toString());
		mapIds.put("professorId", (professorDTO != null) ? professorDTO.getId().toString() : "-1");
		mapIds.put("professorVirtualId", (professorVirtualDTO != null) ? professorVirtualDTO.getId().toString() : "-1");
		
		return mapIds;
	}

	public void addCampusValueListener(Field<CampusDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				campusDTO = (CampusDTO) fe.getValue();
			}
		});
	}
	
	public void addProfessorValueListener(Field<ProfessorDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				professorDTO = (ProfessorDTO) fe.getValue();
			}
		});
	}
	
	public void addProfessorVirtualValueListener(Field<ProfessorVirtualDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				professorVirtualDTO = (ProfessorVirtualDTO) fe.getValue();
			}
		});
	}

	public CampusDTO getCampusDTO(){
		return campusDTO;
	}

	public void setCampusDTO(CampusDTO campusDTO){
		this.campusDTO = campusDTO;
	}

	public TurnoDTO getTurnoDTO(){
		return this.turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO){
		this.turnoDTO = turnoDTO;
	}

	public ProfessorDTO getProfessorDTO(){
		return this.professorDTO;
	}

	public void setProfessorDTO(ProfessorDTO professorDTO){
		this.professorDTO = professorDTO;
	}

	public ProfessorVirtualDTO getProfessorVirtualDTO(){
		return this.professorVirtualDTO;
	}

	public void setProfessorVirtualDTO(ProfessorVirtualDTO professorVirtualDTO){
		this.professorVirtualDTO = professorVirtualDTO;
	}

}
