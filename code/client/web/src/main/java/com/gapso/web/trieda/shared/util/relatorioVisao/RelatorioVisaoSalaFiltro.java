package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.HashMap;
import java.util.Map;

import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.FieldEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class RelatorioVisaoSalaFiltro extends RelatorioVisaoFiltro{
	private static final long serialVersionUID = 1L;
	private String salaCodigo;
	
	public ExcelInformationType getExcelType(){
		return ExcelInformationType.RELATORIO_VISAO_SALA;
	}
	
	public Map<String, String> getMapStringIds(){
		Map<String, String> mapIds = new HashMap<String, String>();

		mapIds.put("salaCodigo", salaCodigo);
		
		return mapIds;
	}
	
	public void addSalaValueListener(ComboBox<SalaDTO> f){
		f.setFireChangeEventOnSetValue(true);
		f.addListener(Events.Change, new Listener<FieldEvent>(){
			public void handleEvent(FieldEvent fe){
				if (fe.getValue() != null){
					salaCodigo = ((SalaDTO) fe.getValue()).getCodigo();
				}
				else {
					salaCodigo = null;
				}
			}
		});
	}

	public String getSalaCodigo(){
		return this.salaCodigo;
	}

	public void setSalaCodigo(String salaCodigo){
		this.salaCodigo = salaCodigo;
	}
	
}
