package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public abstract class RelatorioVisaoFiltro implements ExportExcelFilter, Serializable{
	private static final long serialVersionUID = -1052100767865582524L;
	private TurnoDTO turnoDTO;
	
	public ExportExcelFilter getFilter(){
		return this;
	}
	
	public abstract ExcelInformationType getExcelType();
	
	public void addTurnoValueListener(Field<TurnoDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				turnoDTO = (TurnoDTO) fe.getValue();
			}
		});
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = new HashMap<String, String>();
		
		mapIds.put("turnoId", turnoDTO.getId().toString());
		
		return mapIds;
	}
	
	public TurnoDTO getTurnoDTO(){
		return this.turnoDTO;
	}

	public void setTurnoDTO(TurnoDTO turnoDTO){
		this.turnoDTO = turnoDTO;
	}
	

}
